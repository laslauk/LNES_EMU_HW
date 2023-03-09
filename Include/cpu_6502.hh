
#ifndef CPU6502_HH
#define CPU6502_HH

#include <icpu.hh>
#include <cstdint>
#include "stack.hh"
#include <string>
#include <map>
#include "opcodes.hh"
#include <iostream>
#include "memory.hh"
#include <fstream>
#include "LNES_utilities.hh"

#define I(mnemonic_name) uint8_t mnemonic_name(uint16_t &addr, const Opcode &opcode);
#define Iargs PC,op


class LNES_Renderer;
class IBus;



class CPU_6502 : public ICPU
{

private:
    Stack _stack;

public:
    // Registers

  
    std::string DebugAddressingMode(AddressingMode mode);
    std::ofstream myfile;
 


  
  void Log(Opcode op);
    uint16_t PC = 0;
    uint8_t SP = 0xFD, A = 0, X = 0, Y = 0;

    // Flags
    bool N = 0, V = 0, B = 0, D = 0, I = 0, Z = 0, C = 0; // Processor status registers
    int cycles = 0;

    /* iCpu interface implementation */
    uint8_t readMem(uint16_t addr) override;
    void writeMem(uint16_t addr, uint8_t data) override;
    bool setBus(IBus *bus) override;

    void run();

    CPU_6502();
 
    int getLastCycle() { return 0; }
    int stepCPU();
uint16_t getOperandAddress(uint16_t &addr, const Opcode &op, int &extraCycles);
uint16_t getOperandAddress(uint16_t &addr, const Opcode &op );

   
/* OFFICIAL INSTRUCTIONS */
I(ORA) I(PHP) I(ASL) I(BCC) I(BEQ) I(BCS) I(BIT) I(CLV) I(ADC)
I(BMI) I(BNE) I(BPL) I(AND) I(BRK) I(BVC) I(BVS) I(CLC) I(CLD)
I(CMP) I(CPX) I(CPY) I(DEC) I(DEX) I(DEY) I(EOR) I(INC) I(INX)
I(INY) I(JMP) I(JSR) I(RTI) I(RTS) I(SBC) I(SEC) I(SED) I(SEI)
I(STA) I(STX) I(STY) I(TAX) I(TAY) I(TSX) I(TXA) I(TXS) I(TYA)
I(CLI) I(LDA) I(LDX) I(LDY) I(LSR) I(NOP) I(PHA) I(PLA) I(ROL)
I(ROR) I(PLP)

/* ILLEGAL INSTRUCTIONS */
I(ASO) I(RLA) I(RRA) I(AXS) I(LAS)
I(LAX) I(DCM) I(INS) I(ALR) I(ARR)
I(XAA) I(OAL) I(SAX) I(SKB) I(SKW) I(LSE)
I(HLT) I(TAS) I(SAY) I(XAS) I(AXA) I(SBC_IL)



std::pair<uint8_t, uint8_t> split16int(uint16_t val) {
    uint8_t hi =((val >> 8) & 0xFF);
    uint8_t lo = val & 0xFF;
    return std::pair<uint8_t, uint8_t> (lo, hi);
}

uint16_t concatMSB_LSB(uint8_t low, uint8_t hi)
{
    return ((hi << 8) | (low & 0xFF));
}

    void setCarry(bool val) { C = val; }
    void clearCarry() { C = false; }

    void setOverflow() { V = true; }
    void clearOVerflow() { V = false; }

    void setNegative() { N = true; }
    void clearNegative() { N = false; }

    void clearDecimalMode() { D = false; }

    void setZero() { Z = 1; }
    void clearZero() { Z = 0; }

    void checkNegative(uint8_t val) { (val & (1 << 7)) ? setNegative() : clearNegative(); };
    void checkZero(uint8_t val) { (val == 0) ? setZero() : clearZero(); };

    uint8_t stack_pop(){
        // std::cout << std::hex << "STack pop to stack addr 0x" << (unsigned)SP << std::endl;
     
        SP++;
        uint8_t M = readMem(0x100+SP);
         //   printStack();
        return M;
    }

    void stack_push(uint8_t data) {
      //  std::cout << std::hex << "pushed to stack addr 0x" << (unsigned)SP << " value " << (unsigned)data << std::endl;
     
        writeMem(0x100+SP,data);
        SP--;
            // printStack();
    }

    void setOverflowVal(bool val) {
       /*if(val > 0) { std::cout << "OVERFLOW SET at PC: " << std::hex << (unsigned)PC-2 << std::endl;} else {
         std::cout << "no overflow  at PC: " << std::hex << (unsigned)PC-2 << std::endl;
       }
       */
        V = val;
    }


    void printStack(){
     
       
        std::cout << std::hex<<"STACK AT: "<< 0x100 + (unsigned)SP <<  std::endl;
        int values = (0x1FF)-(0x100+SP+1);
        for(int it = 1; it < values; it++) {
            
            std::cout << std::hex << (unsigned)readMem(0x100+SP+it) << ", ";
        }
     
         std::cout << "END STACK: " << std::endl;
    };

std::vector<uint8_t> getInstructionArgs(uint16_t addr, const Opcode &op);
 bool Interrupt(HW::EInterrupt inter);

 bool HandleInterrupts();



    LNES_Renderer* renderer = nullptr;
private:
    long long _cpuCycles = 0;


    IBus *_bus;
    HW::EInterrupt currentInterrupt = HW::EInterrupt::None;
};

#endif
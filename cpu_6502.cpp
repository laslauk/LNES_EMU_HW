#include "cpu_6502.hh"
#include "ibus.hh"
#include "NES_MemoryMap.hh"
#include "memory.hh"
#include <fstream>
#include <string>
#include "LNES_utilities.hh"
#include "lnesrenderer.hh"
#include <iomanip>  
#include "lnesrenderer.hh"

#define RET_CYCLES  return opcode.cycles;

std::string CPU_6502::DebugAddressingMode(AddressingMode mode) {
    switch(mode) {
case 0: return "Immediate";
case 1: return "None";
case 2: return "ZeroPage";
case 3: return "ZeroPage_X";
case 4: return "ZeroPage_Y";
case 5: return "Absolute";
case 6: return "Absolute_X";
case 7: return "Absolute_Y";
case 8: return "Indirect_Y";
case 9: return "Indirect_X";
case 10: return "Indirect";
case 11: return "Accumulator";
case 12: return "Relative";
case 13: return "Implied";
    default: return "----";
    }
}

CPU_6502::CPU_6502() {
  
}

uint8_t CPU_6502::readMem(uint16_t addr)
{
/*
    if(addr == 0xFFFA || addr == 0xFFFB) {

        std::cout << "NMI vector" << std::endl;
        return 0;
    }

    else if(addr == 0xFFFC || addr == 0xFFFD) {
     std::cout << "Reset vector" << std::endl;
        return 0;

    }


    else if(addr == 0xFFFE || addr == 0xFFFF) {

     std::cout << "IRQ/BRK vector" << std::endl;
        return 0;
    }
    */

   //CPU interrupts
        if(!_bus) {

            std::cout << "BUS NOT SET TO CPU" << std::endl;
            throw;
        }

        return _bus->read_byte(addr);


    
}

void CPU_6502::writeMem(uint16_t addr, uint8_t data)
{
    /*
   if(addr == 0xFFFA || addr == 0xFFFB) {

        std::cout << "NMI vector" << std::endl;
    }

    else if(addr == 0xFFFC || addr == 0xFFFD) {
     std::cout << "Reset vector" << std::endl;
    

    }

    else if(addr == 0xFFFE || addr == 0xFFFF) {

     std::cout << "IRQ/BRK vector" << std::endl;
      
    }
    */

  
         _bus->write_byte(addr, data);
    
        
 
}

bool CPU_6502::setBus(IBus *bus)
{

    _bus = bus;
    return true;
}

void CPU_6502::run()
{
    std::cout << "start CPU run" << std::endl;
    PC = (readMem(0xFFFD)*256)+readMem(0xFFFC) ;
    std::cout << "start at " << (unsigned)PC << std::endl;


    myfile.open("log.log");
    int cycles = 0;
    renderer->initPicture();
    renderer->drawTick(NULL);
    while (true)
    {
        HandleInterrupts();
        cycles = stepCPU();
        _cpuCycles += cycles;
        _bus->tick(cycles);
         myfile << std::endl;

    }
    myfile.close();
}



void CPU_6502::Log(Opcode op) {


    myfile << std::dec<< "c" <<(unsigned)_cpuCycles << "      "<< std::hex ;
    myfile << std::uppercase << std::hex << "A:"<<std::setw(2) << std::setfill('0') <<(unsigned)A  << " ";
    myfile <<  "X:"<<std::setw(2)<<(unsigned)X << " ";
    myfile <<  "Y:"<<std::setw(2)<<(unsigned)Y << " ";
    myfile <<  "S:"<<std::setw(2)<<(unsigned)SP << " ";
    myfile <<  "P:";//<<std::setw(2)<<(unsigned)SP << " ";

        N ? myfile << "N" : myfile << "n";
        V ? myfile << "V" : myfile << "v";
        B ? myfile << "U" : myfile << "u";
        B ? myfile << "B" : myfile << "b";
        D ? myfile << "D" : myfile << "d";
        I ? myfile << "I" : myfile << "i";
        Z ? myfile << "Z" : myfile << "z";
        C ? myfile << "C" : myfile << "c";

   myfile <<  " $";


 std::vector<uint8_t>args = getInstructionArgs(PC,op);
    uint16_t pcold = PC-1;
         int tabs = 0xFF-SP;
    myfile << std::uppercase << std::setw(tabs+1) <<std::setfill(' ') << (unsigned)pcold <<  ":  " << std::setw(2)<<(unsigned)op.opcode << " ";

            if(!args.empty()) {
                for(auto it = args.begin(); it != args.end(); it++){
                    myfile << std::hex << std::setw(2) << std::setfill('0') << (unsigned)(*it) <<std::setfill('0') << std::setw(1) << " ";
                }
                  if(args.size() == 1) {
                     myfile << "   ";
                }
            }

            if(args.empty()) {
               
                  myfile << "      ";
            }
    
   



    myfile << std::hex  << " " << std::setw(3)<< op.mnemonic;

/*

    if(op.mnemonic =="LDA" && op.opcode == 0xAD) {
         myfile << std::hex  << " $";
         if(args.size() == 2)  {
         myfile <<   std::setw(2) 
         << std::setfill('0') << (unsigned)args[1] 
         <<  std::setw(2) 
         << std::setfill('0') 
         << (unsigned)args[0] 
         << " = #$" 
           <<  std::setw(2) 
         << std::setfill('0') <<(unsigned)A;
        
    }
 
 */
    if(op.mnemonic =="STA" && op.opcode == 0x91) {

        uint8_t arg_low = readMem((readMem(PC))%256);
        uint8_t arg_hi =  readMem((readMem(PC)+1)%256);
         uint16_t result = concatMSB_LSB(arg_low,arg_hi)+Y;


         myfile << std::hex  << " ($";
         myfile <<   std::setw(2) 
         << std::setfill('0') 
         << (unsigned)args[0] 
         << "),Y @ $"
         <<  std::setw(4) 
         << std::setfill('0') 
         << (unsigned)result 
         << " = #$" 
           <<  std::setw(2) 
         << std::setfill('0') <<(unsigned)readMem(result);
   
        
    }



 
    return;

 // std::vector<uint8_t>args = getInstructionArgs(PC,op);
//    uint16_t pcold = PC-1;
  //  std::cout << std::hex << PC << ": " << (int)op.opcode <<" "<< op.mnemonic << "\t" << DebugAddressingMode(op.mode) << std::endl;
    myfile << std::uppercase << std::hex << std::setw(4) << (unsigned)pcold <<  "  " << std::setw(2)<<(unsigned)op.opcode << " ";

        if(!args.empty()) {
                for(auto it = args.begin(); it != args.end(); it++){
                    myfile << std::hex << std::setw(2) << (unsigned)(*it) <<std::setfill('0') << std::setw(1) << " ";
                }
                  if(args.size() == 1) {
                     myfile << "   ";
                }
            }

            if(args.empty()) {
               
                  myfile << "      ";
            }
    
    myfile << std::hex  << " " << std::setw(3)<< op.mnemonic;
    myfile << std::hex  << std::uppercase <<"                    A:"<<std::setw(2)<<(unsigned)A << std::setfill('0')
     <<" X:"<<std::setw(2)  << (unsigned)X << std::setfill('0')<< 
     " Y:"<<(unsigned)Y << std::setfill('0')<< " P:26 SP:"<<(unsigned)SP <<" PPU:  0, 54 CYC:18"<< std::endl; 







    if(false) {
    std::vector<uint8_t>args = getInstructionArgs(PC,op);
    uint16_t pcold = PC-1;
  //  std::cout << std::hex << PC << ": " << (int)op.opcode <<" "<< op.mnemonic << "\t" << DebugAddressingMode(op.mode) << std::endl;
    myfile << std::uppercase << std::hex << std::setw(4) << (unsigned)pcold <<  "  " << std::setw(2)<<(unsigned)op.opcode << " ";

        if(!args.empty()) {
                for(auto it = args.begin(); it != args.end(); it++){
                    myfile << std::hex << std::setw(2) << (unsigned)(*it) <<std::setfill('0') << std::setw(1) << " ";
                }
                  if(args.size() == 1) {
                     myfile << "   ";
                }
            }

            if(args.empty()) {
               
                  myfile << "      ";
            }
    
    myfile << std::hex  << " " << std::setw(3)<< op.mnemonic;
    myfile << std::hex  << std::uppercase <<"                    A:"<<std::setw(2)<<(unsigned)A << std::setfill('0')
     <<" X:"<<std::setw(2)  << (unsigned)X << std::setfill('0')<< 
     " Y:"<<(unsigned)Y << std::setfill('0')<< " P:26 SP:"<<(unsigned)SP <<" PPU:  0, 54 CYC:18"<< std::endl; 

}
     //(int)A << " X:00 Y: 00 P:24 SP:" << (int)SP << " PPU" << std::endl; 
}

bool CPU_6502::HandleInterrupts(){
   // std::cout << "handle interrupts "<< std::endl;

    if(currentInterrupt != HW::EInterrupt::None) {
    
        stack_push(PC);
        uint8_t status = N << 7 | V << 6 | 1 << 5 | 0 << 4 | D << 3 | 1 << 2 | Z << 1 | C;
        stack_push(status);
          I = 1;
        //disable interrupts
    }
    
    switch(currentInterrupt) {

        case  HW::EInterrupt::None:{
            break;
        }
        case HW::EInterrupt::NMI: {
            
            std::cout << "NMI   CPU" << std::endl;
            uint16_t NMIHandlerAddr = concatMSB_LSB(readMem(0xFFFA), readMem(0xFFFB));

            std::cout << "NMIHandlerAddr:" << std::hex << (unsigned)NMIHandlerAddr << std::endl;
            PC = NMIHandlerAddr;
            _bus->tick(2);
            renderer->draw();
            currentInterrupt = HW::EInterrupt::None;
            
      
                        

       
         

            break;
        }
        default: break;
        
    }
 // std::cout << "END handle interrupts "<< std::endl;
    return true;

}

bool CPU_6502::Interrupt(HW::EInterrupt interrupt) {
    //NMI Handler address from FFFA-FFFB
    currentInterrupt = interrupt;
    return true;

}

int CPU_6502::stepCPU()
{

   // HandleInterrupts();
    int cycles = 0;
  //  std::cout << "START: " << (unsigned)PC << ", ";
    uint8_t opcode = (unsigned)readMem(PC);
    Opcode op;


 //   std::cout << "PC: " << std::hex << (unsigned)PC << std::endl;
    try {
   //         std::cout << "ADDR: " << std::hex << (unsigned)PC << " value: " << (unsigned)readMem(PC) << "opcode:" << (unsigned)opcode   << std::endl;;
     op = opcodes.at(opcode);

    }
    catch(...) {
        std::cout << "Virhe op reaidng" << std::hex << "PC: " << (unsigned)PC << ", opcode: " << opcode<<std::endl;
        throw;
    }

    std::cout << "PC: " << std::hex << (unsigned)PC << ", ";
    PC++;
    Log(op);
   


    /*TOIMINTA:
        readMem lukee PC:n joka on käsky,
        PC++ ona rgumentti

        NES CPu uses little endian addressing:
        real address 0x8000
        little endian packae 00 80
        to read data from memory cell 0x8000, register would be
        LDA %8000 <=> ad 00 80

        PROGRTAM COUNTERISSA: Seuraavan käskyn osoite muistissa, PC:n value on osoite
        muistissa. PCn jälkeen usein argumentit. Absolute on 2 seuraavaa
        PC: 0x02            8bit[STA==0x18]
                            8bit[0x14]
                            8bit[0x18]
        jolloin assolute ottaa PC+1 ja PC+2, jolloin uusi instrunction at PC+3
    */

    switch (opcode)
    {

     case 0x69: case 0x65: case 0x75: case 0x6D: case 0x7D:  case 0x79: case 0x61: case 0x71:
        cycles = ADC(Iargs); break;

    case 0x29: case 0x25: case 0x35:  case 0x2D:  case 0x3D: case 0x39: case 0x21: case 0x31: 
        cycles = AND(Iargs); break;

    case 0x0A: case 0x06: case 0x16: case 0x0E: case 0x1E: 
        cycles = ASL(Iargs); break;

    case 0x90: 
        cycles = BCC(Iargs); break;

    case 0xB0: 
        cycles = BCS(Iargs); break;

    case 0xF0: 
        cycles = BEQ(Iargs); break;
    
    case 0x24: case 0x2C:
        cycles = BIT(Iargs); break;
    
    case 0x30:
        cycles = BMI(Iargs); break;

    case 0xD0:

        cycles = BNE(Iargs); break;

    case 0x10:
        cycles = BPL(Iargs); break;


    case 0x00:
        cycles = BRK(Iargs); break;
    
    case 0x50:
        cycles = BVC(Iargs); break;
    

    case 0x70:
        cycles = BVS(Iargs); break;
        
    case 0x18:
        cycles = CLC(Iargs); break;
    case 0xD8:
        cycles = CLD(Iargs); break;
    case 0x58:
        cycles = CLI(Iargs); break;
    case 0xB8:
        cycles = CLV(Iargs); break;

    case 0xc9: case 0xC5: case 0xD5: case 0xCD: case 0xDD: case 0xD9: case 0xC1: case 0xD1:
        cycles = CMP(Iargs); break;

    case 0xE0: case 0xE4: case 0xEC:
        cycles = CPX(Iargs); break;

    case 0xC0: case 0xC4: case 0xCC:
        cycles = CPY(Iargs); break;

    case 0xC6: case 0xD6: case 0xCE: case 0xDE:
        cycles =  DEC(Iargs); break;


        case 0xCA:
        cycles = DEX(Iargs); break;
        case 0x88:
        cycles = DEY(Iargs); break;
        case 0x49: case 0x45: case 0x55: case 0x4D: case 0x5D: case 0x59: case 0x41: case 0x51:
        cycles = EOR(Iargs); break;
        case 0xE6: case 0xF6: case 0xEE: case 0xFE:
        cycles = INC(Iargs); break;
        case 0xE8:
        cycles = INX(Iargs); break;

        case 0xC8:
        cycles = INY(Iargs); break;

        case 0x4C: case 0x6C:
        cycles = JMP(Iargs); break;

        case 0x20:
        cycles = JSR(Iargs); break;

        case 0xA9: case 0xA5:case 0xB5: case 0xAD: case 0xBD: case 0xB9: case 0xA1: case 0xB1:
        cycles = LDA(Iargs); break;

        case 0xA2: case 0xA6: case 0xB6: case 0xAE: case 0xBE:
        cycles = LDX(Iargs); break;

        case 0xA0: case 0xA4: case 0xB4: case 0xAC: case 0xBC:
        cycles = LDY(Iargs); break;
    

        case 0x4A: case 0x46: case 0x56: case 0x4E: case 0x5E:
        cycles = LSR(Iargs); break;

        case 0xEA:
        cycles = NOP(Iargs); break;

        case 0x09: case 0x05: case 0x15: case 0x0D: case 0x1D: case 0x19: case 0x01: case 0x11:
        cycles = ORA(Iargs); break;

        case 0x48:
        cycles = PHA(Iargs); break;

        case 0x08:
        cycles = PHP(Iargs); break;

        case 0x68:
        cycles = PLA(Iargs); break;

        case 0x28:
        cycles = PLP(Iargs); break;

        case 0x2A: case 0x26: case 0x36: case 0x2E: case 0x3E:
        cycles = ROL(Iargs); break;

        case 0x6A: case 0x66: case 0x76: case 0x6E: case 0x7E:
        cycles = ROR(Iargs); break;
        
        case 0x40:
        cycles = RTI(Iargs); break;

        case 0x60:
        cycles = RTS(Iargs); break;

        case 0xE9: case 0xe5: case 0xF5: case 0xED: case 0xFD: case 0xF9: case 0xE1: case 0xF1:
        cycles = SBC(Iargs); break;

        case 0x38:
        cycles = SEC(Iargs); break;

        case 0xF8:
        cycles = SED(Iargs); break;

        case 0x78:
        cycles = SEI(Iargs); break;

        case 0x85: case 0x95: case 0x8D: case 0x9D: case 0x99: case 0x81: case 0x91:
        cycles = STA(Iargs); break;

        case 0x86: case 0x96: case 0x8E:
        cycles = STX(Iargs); break;

        case 0x84: case 0x94: case 0x8C:
        cycles = STY(Iargs); break;

        case 0xAA:
        cycles = TAX(Iargs); break;

        case 0xA8:
        cycles = TAY(Iargs); break;

        case 0xBA:
        cycles = TSX(Iargs); break;

        case 0x8A:
        cycles = TXA(Iargs); break;

        case 0x9A:
        cycles = TXS(Iargs); break;

        case 0xEB:
        cycles = SBC_IL(Iargs); break;

        case 0x4F: case 0x5F: case 0x5B: case 0x47: case 0x57: case 0x43: case 0x53:
        cycles = LSE(Iargs);break;


        case 0x98:
        cycles = TYA(Iargs); break;

        /* ======== ILLEGAL OPCODES ======== */

        case 0x0F: case 0x1F: case 0x1B: case 0x07: case 0x17: case 0x03: case 0x13:
        cycles = ASO(Iargs); break;

        case 0x2F: case 0x3F: case 0x3B: case 0x27: case 0x37: case 0x23: case 0x33:
        cycles = RLA(Iargs); break;

        case 0x6F: case 0x7F: case 0x7B: case 0x67: case 0x77: case 0x63: case 0x73:
        cycles = RRA(Iargs); break;

        case 0x8F: case 0x87: case 0x97: case 0x83:
        cycles = AXS(Iargs); break;

        case 0xAF: case 0xBF: case 0xA7: case 0xB7: case 0xA3: case 0xB3:
        cycles = LAX(Iargs); break; 

        case 0xCF: case 0xDF: case 0xDB: case 0xC7: case 0xD7: case 0xC3: case 0xD3:
        cycles = DCM(Iargs); break;

        case 0xEF: case 0xFF: case 0xFB: case 0xE7: case 0xF7: case 0xE3: case 0xF3:
        cycles = INS(Iargs); break;

        case 0x4B:
        cycles = ALR(Iargs);break;

        case 0x6B: 
        cycles = ARR(Iargs); break;

        case 0x8B:
        cycles = XAA(Iargs); break;

        case 0xAB:
        cycles = OAL(Iargs); break;

        case 0xCB:
        cycles = SAX(Iargs); break;

        case 0x1A: case 0x3A: case 0x5A: case 0x7A: case 0xDA: case 0xFA:
        cycles = NOP(Iargs); break;

        case 0x80: case 0x82: case 0xC2: case 0xE2: case 0x04: case 0x14: case 0x34: case 0x44: case 0x54:
        case 0x64: case 0x74: case 0xD4: case 0xF4:
        cycles = SKB(Iargs); break;

        case 0x0C: case 0x1C: case 0x3C: case 0x5C: case 0x7C: case 0xDC: case 0xFC:
        cycles = SKW(Iargs); break;

        
        case 0x02: case 0x12: case 0x22: case 0x32: case 0x42: case 0x52:
        case 0x62: case 0x72: case 0x92: case 0xB2: case 0xD2: case 0xF2:
        cycles = HLT(Iargs); break;

        case 0x9B: 
        cycles = TAS(Iargs); break;

        case 0x9C: 
        cycles = SAY(Iargs); break;

        case 0x9E: 
        cycles = XAS(Iargs); break;

        case 0x9F: case 0x93: 
        cycles = AXA(Iargs);break;




    default:
    {
        cycles =  0;
        break;
    }
    }
    //NOTE, EI TARVI PALAUTTA VÄLTSIS CYCLES PER INSTRUCTN FUNTION
    return cycles;
}

std::vector<uint8_t> CPU_6502::getInstructionArgs(uint16_t addr, const Opcode &op) {

    std::vector<uint8_t> results = {};
      switch (op.mode)
    {
    case AddressingMode::Absolute:
    {
        results.push_back( readMem(addr));
        results.push_back( readMem(addr+1));
        break;
    }

    case AddressingMode::Absolute_X:
    {
        results.push_back( readMem(addr));
        results.push_back( readMem(addr+1));
        break;
    }

    case AddressingMode::Absolute_Y:
    {
        results.push_back( readMem(addr));
        results.push_back( readMem(addr+1));
        break;
    }

    case AddressingMode::Immediate:
    {
         results.push_back( readMem(addr));
     
        break;
    }

    case AddressingMode::Indirect_X: // indexed indirect
    {
        
        results.push_back( readMem(addr));
        break;
    }

    case AddressingMode::Indirect:
    {
         results.push_back( readMem(addr));
        results.push_back( readMem(addr+1));

        break;
    }

    case AddressingMode::Indirect_Y: // Indirect indexed
    {
         results.push_back( readMem(addr));
    
        break;
    }

    case AddressingMode::None:
    {

        break;
    }

    case AddressingMode::ZeroPage:
    {

     results.push_back( readMem(addr));

        break;
    }

     case AddressingMode::Relative:
    {
         results.push_back( readMem(addr));
        break;
    }

    case AddressingMode::ZeroPage_X:
    {

        results.push_back( readMem(addr));

      
        break;
    }
    case AddressingMode::ZeroPage_Y:
    {
        results.push_back( readMem(addr));
        break;
    }

    default:
    {
        break;
    }
    }
    return results;
}


uint16_t CPU_6502::getOperandAddress(uint16_t &addr, const Opcode &op){
int i = 0;
    return getOperandAddress( addr, op, i );

}


uint16_t CPU_6502::getOperandAddress(uint16_t &addr, const Opcode &op, int &extraCycles)
{

    /*Absolute: Full memory location is used as argument to instruction. example
        STA $c000, value of Accumulator stored at memory c000; */
    uint16_t result = 0;
    switch (op.mode)
    {
    case AddressingMode::Absolute:
    {
     
        
        // Little endian, so low byte comes first
        result = concatMSB_LSB(
            readMem(addr),
            readMem(addr + 1));
        addr = addr + 2;

        //std::cout << std::hex << "Operand Address ABSOLUTE: " << (unsigned)result << std::endl;
        
        break;
    }

    case AddressingMode::Absolute_X:
    {
        result = concatMSB_LSB(readMem(addr), readMem(addr+1));
                if(result & 0xFF00 != (result+X) &0xFF00) extraCycles = 1;
        result = result + X;




        addr = addr + 2;
        break;
    }

    case AddressingMode::Absolute_Y:
    {
        result = concatMSB_LSB(readMem(addr), readMem(addr+1));
                 if(result & 0xFF00 != (result+Y) &0xFF00) extraCycles = 1;
        result = result + Y;

        addr = addr + 2;
        break;
    }

    case AddressingMode::Immediate:
    {
        std::cout << "Get addressing by mode: >Immediate " << std::endl;
        result = addr;
        std::cout << "Immediate value at 0x" << std::hex << (int)addr-1 << " is: 0x" << (int)result << std::endl;
    
        PC++;

        break;
    }

    case AddressingMode::Indirect_X: // indexed indirect
    {
        uint8_t arg_low = readMem((readMem(addr)+X)%256);
        uint8_t arg_hi =  readMem((readMem(addr)+X+1)%256);
        std::cout << "Get addressing by mode: >Indirect X / Indexed indirect " << std::hex << (unsigned)readMem(addr)<< std::endl;
     std::cout << std::hex << (readMem(addr)+X+1)%256 <<"_"<<(unsigned)(readMem(addr)+X)%256 << std::endl;
        result = concatMSB_LSB(arg_low,arg_hi);
        std::cout << std::hex << (unsigned)result << " result addr >" <<std::endl;;
        addr = addr+1;
        break;
    }

    case AddressingMode::Indirect:
    {
        uint16_t lowbyte = concatMSB_LSB(readMem(addr), readMem(addr+1));

        uint16_t highbyte = 0;
        if(readMem(addr) == 0xFF) {

            highbyte = (readMem(addr+1) << 8 ) & 0xFF00;
        } else {

            highbyte = lowbyte+1;
        }

         std::cout << std::hex << "Indirect addr argument: " << (unsigned)lowbyte << std::endl;
        
        std::cout << std::hex << "memory values " << (unsigned)readMem(lowbyte) <<"," <<(unsigned)readMem(highbyte) << std::endl;
        uint8_t arg_low = readMem(lowbyte);
        uint8_t arg_hi =  readMem(highbyte);
        result = concatMSB_LSB(arg_low, arg_hi);
   
        std::cout << std::hex << "PC:  " << (unsigned)addr-1 << "Indirect addr operand: " << (unsigned)result << std::endl;
    
        addr = addr + 2;
        break;
    }

    case AddressingMode::Indirect_Y: // Indirect indexed
    {
        uint8_t arg_low = readMem((readMem(addr))%256);
        uint8_t arg_hi =  readMem((readMem(addr)+1)%256);

            if(concatMSB_LSB(arg_low,arg_hi) & 0xFF00 != (concatMSB_LSB(arg_low,arg_hi)+Y) &0xFF00) extraCycles = 1;

         result = concatMSB_LSB(arg_low,arg_hi)+Y;



        addr = addr + 1;
        break;
    }

    case AddressingMode::None:
    {

        break;
    }

    case AddressingMode::ZeroPage:
    {

        uint8_t hi = 0;
        result = concatMSB_LSB((readMem(addr)),hi)%256;
        addr = addr + 1;

        break;
    }

    case AddressingMode::ZeroPage_X:
    {
        result = (readMem(addr)+X) % 256;
        std::cout << std::hex << "Zeropage_x result: " << (unsigned)result << std::endl;

        addr = addr + 1;
        break;
    }
    case AddressingMode::ZeroPage_Y:
    {
        result = (readMem(addr)+Y) % 256;
        addr = addr + 1;
        break;
    }

    default:
    {
        break;
    }
    }

    return result;
}

uint8_t CPU_6502::ADC(uint16_t &addr, const Opcode &opcode)
{
    // source - target
    //   A(accumulator) + M(memory) + C(carry) ->(save to) A(accumulator).
    // Reads contents of memory location to accumulator with carry bit.
    // if overflow,, carry set.

    /* A,Z,C,N = A+M+C -> A */

    uint16_t operandAddress = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operandAddress);

 //   std::cout << "ADC: [0x" << std::hex << (unsigned)operandAddress << "] = " << (unsigned)M << std::endl;

    uint16_t res = (A + M + C); // select 8 first bits

   // std::cout << std::dec << "ADC Result" << (unsigned)A << " + " << (unsigned)M << " + " << unsigned(C) << " = " << (unsigned)res << std::endl;

   
    // flags
    checkNegative(res);
   // (M >> 7) ^ (A >> 7) ? setOverflow() : clearOVerflow();
   // (A == 0) ? setZero() : clearZero();
    checkZero(res);
    // integer mode
    setCarry( res > 0xff);
    //V = ((A^M)&(A^res)&0x80) >0 ;

    bool isOverflow = (~(A^M)&(A^res)&0x0080)>0;
    setOverflowVal((~(A^M)&(A^res)&0x0080)>0);
    //setOverflowVal(((A ^res) & (M^res) > 0x80));
  
    A = res & 0x00FF;
    if(isOverflow) std::cout << "V flag set ADC!" << std::endl;

    RET_CYCLES
}

uint8_t CPU_6502::ORA(uint16_t &addr, const Opcode &opcode)
{
    uint16_t opAdd = getOperandAddress(addr,opcode);
    uint8_t M = readMem(opAdd);
  //  std::cout << std::dec << "PC: " << (unsigned)(addr-2)<<" ORA operand addr" << (unsigned)opAdd << " mem data: " << (unsigned)M << std::endl;

    A = A|M;
    checkZero(A);
    checkNegative(A);

    RET_CYCLES
}

uint8_t CPU_6502::PHP(uint16_t &addr, const Opcode &opcode)
{
    uint8_t S = 0;

        S = ((S & ~(1 << 7)) | (N << 7));
        S = ((S & ~(1 << 6)) | (V << 6));
        S = ((S & ~(1 << 5)) | (1 << 5)); // PHP sets bits 5 and 4
        S = ((S & ~(1 << 4 ))| (1 << 4)); // PHP sets bits 5 and 4
        S = ((S & ~(1 << 3)) | (D << 3));
        S = ((S & ~(1 << 2)) | (I << 2));
        S = ((S & ~(1 << 1)) | (Z << 1));
        S = ((S & ~(1 << 0)) | (C << 0));

    std::cout << std::hex << "PC: "<< (unsigned)(addr-1) <<"PHP Push values: " << (unsigned)S << std::endl;

        stack_push(S);
     
/*

    S = S & (1 << 0);
    S = S & (1 << 1);
    S = S & (1 << 2);
    S = S & (1 << 3);
    S = S & (1 << 5);
    S = S & (1 << 6);
    S = S & (1 << 7); */

    RET_CYCLES
}

uint8_t CPU_6502::ASL(uint16_t &addr, const Opcode &opcode)
{

    uint8_t M = 0;
    // Suoraan accumulatorista
    if (opcode.mode == AddressingMode::Accumulator)
    {
       
        C = (A >> 7); // set 7th bit (last) to carry
        A = (A << 1);
        checkZero(A);
        checkNegative(A);

    }

    else
    {
        uint16_t operandAddress = getOperandAddress(addr, opcode);
        std::cout << std::hex << "ASL: " << (unsigned)operandAddress << std::endl;
        M = readMem(operandAddress);
   
        C = (M >> 7); // set 7th bit (last) to carry
        M = (M << 1);
        writeMem(operandAddress,M);
        checkZero(M);
        checkNegative(M);
     }

  RET_CYCLES
}


uint8_t CPU_6502::LSR(uint16_t &addr, const Opcode &opcode)
{
  

   if (opcode.mode == AddressingMode::Accumulator)
    {
        bool c = 0;
        c = A & (1 << 0);
        setCarry(c);
        
        A = A >> 1;
        checkZero(A);
        checkNegative(A);
      
    } else {
        uint16_t operandAddress = getOperandAddress(addr, opcode);
        uint8_t M = readMem(operandAddress);
   
        bool c = 0;
        c = M & (1 << 0);
        M = M >> 1;

        setCarry(c);
        checkZero(M);
        writeMem(operandAddress, M);
        checkNegative(M);

    }
RET_CYCLES

 
    
}


uint8_t CPU_6502::LDX(uint16_t &addr, const Opcode &opcode)
{


        int cycles = opcode.cycles;
        int extraCycles = 0;
   std::cout << std::hex << "LDX addr: 0x" << (unsigned)addr-2 << "BEOFRE, Zero and Neg: " <<(unsigned)Z << "- " << (unsigned)N << " overflow: " <<(unsigned)V  <<std::endl; 
     uint16_t operandAddress = getOperandAddress(addr, opcode,extraCycles);
     X = readMem(operandAddress);
     checkZero(X);
     checkNegative(X);
     myfile <<  std::hex << " $" << (unsigned)operandAddress << " = #$" << (unsigned)X;

    std::cout << std::hex << "LDX addr: 0x" << (unsigned)addr-2 << "after, Zero and Neg: " <<(unsigned)Z << "- " << (unsigned)N << " overflow: " <<(unsigned)V  <<std::endl; 
    return (cycles+extraCycles);
}

uint8_t CPU_6502::PLP(uint16_t &addr, const Opcode &opcode)
{
    uint8_t S = stack_pop();

    N = S & (1 << 7);
    V = S & (1 << 6);
   // B = S & (0 << 5);
    D = S & (1 << 3);
    I = S & (1 << 2);
    Z = S & (1 << 1);
    C = S & (1 << 0);
 RET_CYCLES
 //   uint8_t statusReg = N << 7 | V << 6 | 1 << 5 | 1 << 4 | D << 3 | I << 2 | Z << 1 | C;
    
    
}



uint8_t CPU_6502::LDY(uint16_t &addr, const Opcode &opcode)
{
     uint16_t operandAddress = getOperandAddress(addr, opcode);
     Y = readMem(operandAddress);
     checkZero(Y);
     checkNegative(Y);
      RET_CYCLES
    
}






uint8_t CPU_6502::BCC(uint16_t &addr, const Opcode &opcode)
{

    uint8_t M = readMem(addr);


    if (C == 0)
    {
        PC = PC -1+opcode.len_bytes+ M;
     
    }
    else
    {
        PC++;
    }
           

 RET_CYCLES
}

uint8_t CPU_6502::BEQ(uint16_t &addr, const Opcode &opcode)
{
    int8_t offset = readMem(addr);
    int cycles = opcode.cycles;

 


    std::cout << "BEQ OFFSET: " << (int)offset << std::endl;
    if (Z == 1)
    {

        cycles += 1; //+1 branch succeed
        uint16_t oldPC = PC-1+opcode.len_bytes;
       

        std::cout << "BEQ OLD PC: " << (unsigned)PC << std::endl;
        PC = PC-1+opcode.len_bytes + offset;
        std::cout << "BEQ NEW PC: "  << (unsigned)PC << std::endl;

         if((oldPC & 0xFF00) != (PC & 0xFF00)) cycles = cycles + 2;
         
       
    }
    else
    {
        PC++;
    }

    myfile << " $" << std::hex << std::uppercase << PC;
   return cycles;
}

uint8_t CPU_6502::BCS(uint16_t &addr, const Opcode &opcode)
{
    //address calculated by Nykynen osote + instr len + offset
    uint8_t M = readMem(addr);
   
    if (C == 1)
    {
        PC = PC-1 +opcode.len_bytes + M;
    }

    else
    {
        PC++;
    }
    //std::cout << "BCC CP end Value: " << std::hex << (int)PC << std::endl;
 RET_CYCLES
}

uint8_t CPU_6502::BIT(uint16_t &addr, const Opcode &opcode)
{
    /* RIVIT 38-40 onglema bit test */
    uint16_t operandAddress = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operandAddress);
    uint8_t bitMask = A & M;
    Z = (bitMask == 0);
    V = M & (1 << 6);
    N = M & (1 << 7);

   // std::cout << std::hex << "operand addr"<<(unsigned)operandAddress << ", BIT TEST, mask: "<< (int)bitMask << ", V: " << (int)V << ", N: " << (int)V << "mem value: " << (unsigned)M <<std::endl;
  
   RET_CYCLES
}

uint8_t CPU_6502::BMI(uint16_t &addr, const Opcode &opcode)
{
    int8_t offset = readMem(addr);
    if (N == 1)
    {
        PC = PC -1 + opcode.cycles + offset;
    }
    else
    {
        PC++;
    }
    return 0;
}

uint8_t CPU_6502::BNE(uint16_t &addr, const Opcode &opcode)
{
   
    int8_t offset = readMem(addr);
    uint16_t oldPC = addr-1;

    std::cout << std::hex << "old PC  Branch: from " << (unsigned)PC << " with offset: " << (unsigned)offset <<std::endl;
    if (Z == 0)
    {
        int8_t low_old = addr-1 & 0x00FF;
        uint16_t hi = ((addr-1 & 0xFF00) >> 8);
        int8_t sum_lows = ( low_old+ offset) % 256;
        std::cout << std::hex << "LOW BYTE: " << (unsigned)low_old  << " - high byte: " << (unsigned)hi 
        << " low add +" << (unsigned)low_old << " sum : " 
        << (unsigned)sum_lows << "new address with high and low: " << (unsigned)concatMSB_LSB(sum_lows, hi) << std::endl;


        PC = concatMSB_LSB(sum_lows, hi) +opcode.len_bytes ; // concatMSB_LSB(low,hi) -1 + opcode.len_bytes; //
     
    }
    else
    {
        PC++;
    }
   // std::cout << std::hex << "new PC to Branch: " << (unsigned)PC << std::endl;
    // if(oldPC== 0xC72A) throw;
RET_CYCLES
}

uint8_t CPU_6502::BPL(uint16_t &addr, const Opcode &opcode)
{
    int8_t offset = readMem(addr);
    if (N == 0)
    {
        PC = PC -1+opcode.len_bytes+ offset;
    }
    else
    {
        PC++;
    }
RET_CYCLES
}

uint8_t CPU_6502::AND(uint16_t &addr, const Opcode &opcode)
{   
    int extraCycles = 0;
    uint16_t operandAddress = getOperandAddress(addr, opcode,extraCycles);
    int cycles = opcode.cycles +extraCycles;
    uint8_t M = readMem(operandAddress);
    uint8_t Aold = A;
  std::cout << std::hex << "M: " << (unsigned) M << " - A:" << (unsigned)A << std::endl; 
    uint8_t res = ( A & M);
    std::cout << "AND OPERATION:" << std::hex << (unsigned)A << " & " << (unsigned) M << ", " << (unsigned)res << std::endl;
    A = res;
    checkNegative(A);
    checkZero(A);
    std::cout << "and result check zero res:" << (unsigned)Z << std::endl;

    myfile << " #$" << std::hex << (unsigned)M;


 return cycles;
}

uint8_t CPU_6502::BRK(uint16_t &addr, const Opcode &opcode)
{
    
    stack_push(PC); // second byte after BRK ios stored to the stack.

    //RK bits 5 ja 4 on 11
    this->I = 1;
    uint8_t status = N << 7 | V << 6 | 1 << 5 | 1 << 4 | D << 3 | 1 << 2 | Z << 1 | C;
   
    stack_push(status);
 
    PC = concatMSB_LSB(readMem(0xFFFE), readMem(0xFFFF));
  
  RET_CYCLES
}

uint8_t CPU_6502::BVC(uint16_t &addr, const Opcode &opcode)
{   uint16_t addr2 = addr;
    int8_t offset = readMem(addr);
    if (V == 0)
    {
        PC = PC -1+opcode.len_bytes + offset;

        std::cout << std::hex << "V is cleared, BVC jump to " << std::hex << PC << " from " << (unsigned)(addr2)  << std::endl;
    }
    else
    {
        PC++;
    }

   

 RET_CYCLES
}
/* VÄÄRIN F968 */

uint8_t CPU_6502::BVS(uint16_t &addr, const Opcode &opcode)
{
    int8_t offset = readMem(addr);
    if (V == 1)
    {
        PC = PC -1+opcode.len_bytes+ offset;
    }
    else
    {
        PC++;
    }

  RET_CYCLES
}

uint8_t CPU_6502::CLC(uint16_t &addr, const Opcode &opcode)
{
    clearCarry();
   RET_CYCLES
}

uint8_t CPU_6502::CLD(uint16_t &addr, const Opcode &opcode)
{
    clearDecimalMode();
    RET_CYCLES
}

uint8_t CPU_6502::PHA(uint16_t &addr, const Opcode &opcode)
{
    std::cout << "PHA:" << std::hex <<(unsigned)addr-1 << std::endl;
    stack_push(A);
    //printStack();
    RET_CYCLES
}


uint8_t CPU_6502::PLA(uint16_t &addr, const Opcode &opcode)
{
std::cout << "Before PLA at  " << std::hex << (unsigned)addr-1 << std::endl;
  //  printStack();

    A = stack_pop();
    std::cout << "PLA: at" << std::hex << (unsigned)addr-1 << " - " << (unsigned)A << " at SP:" << (unsigned)SP << std::endl;
    checkNegative(A);
    checkZero(A);
   
    RET_CYCLES
}


uint8_t CPU_6502::ROR(uint16_t &addr, const Opcode &opcode)
{


   if (opcode.mode == AddressingMode::Accumulator)
    {
        bool c = 0;
        c = A & (1 << 0);
        A = A >> 1;
        A |= C << 7;
        setCarry(c);
        checkZero(A);
        checkNegative(A);



    } else {
        uint16_t operandAddress = getOperandAddress(addr, opcode);
        uint8_t M = readMem(operandAddress);
   
        bool c = 0;
        c = M & (1 << 0);
        M = M >> 1;
        M |= C << 7;
        setCarry(c);
        checkZero(M);
        checkNegative(M);
        writeMem(operandAddress, M);
    }
    
 
  RET_CYCLES
}


uint8_t CPU_6502::ROL(uint16_t &addr, const Opcode &opcode)
{


   if (opcode.mode == AddressingMode::Accumulator)
    {
        bool c = 0;
        c = A & (1 << 7);
        A = A << 1;
        A |= C << 0;
        setCarry(c);
        checkZero(A);
        checkNegative(A);
     


    } else {
        uint16_t operandAddress = getOperandAddress(addr, opcode);
        uint8_t M = readMem(operandAddress);
   
        bool c = 0;
        c = M & (1 << 7);
        M = M << 1;
        M |= C << 0;
        setCarry(c);
        checkZero(M);
        checkNegative(M);
        writeMem(operandAddress, M);
    }
    
 
   RET_CYCLES
}






uint8_t CPU_6502::CLV(uint16_t &addr, const Opcode &opcode)
{
    clearOVerflow();
    RET_CYCLES
}


uint8_t CPU_6502::NOP(uint16_t &addr, const Opcode &opcode)
{
  RET_CYCLES
}



uint8_t CPU_6502::CMP(uint16_t &addr, const Opcode &opcode)
{

    uint16_t operandAddr = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operandAddr);

    uint8_t res = (A - M);
   // std::cout << std::hex << "PC: " << (int)PC << "CMP: " << (int)A << " - " << (int)M << " = " << (int)res << std::endl;
    C = (A >= M);
    Z = (A == M);
    checkNegative(res);
   // std::cout << "ARE EQUALS PC? Z : " << std::hex << (int)Z<<  "--- is neg?: "<< (int)N <<std::endl;
  
    
  RET_CYCLES
}

uint8_t CPU_6502::CPX(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operanAddr = getOperandAddress(addr, opcode);
    uint16_t M = readMem(operanAddr) & 0x00FF;
    uint16_t res = (X - M);

    C = (X >= M);
    Z = (X == M);
    checkNegative(res);

 RET_CYCLES
}

uint8_t CPU_6502::CPY(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operanAddr = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operanAddr);
    uint8_t res = (Y - M);

    C = (Y >= M);
    Z = (Y == M);
    checkNegative(res);

 RET_CYCLES
}

uint8_t CPU_6502::DEC(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operanAddr = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operanAddr);

    uint8_t res = M - 1;
    Z = (res == 0);
    checkNegative(res);
    writeMem(operanAddr, res);

    RET_CYCLES
}

uint8_t CPU_6502::DEX(uint16_t &addr, const Opcode &opcode)
{
    X = X - 1;
    Z = (X == 0);
    checkNegative(X);
    return 0;
}

uint8_t CPU_6502::DEY(uint16_t &addr, const Opcode &opcode)
{
    Y = Y - 1;
    Z = (Y == 0);
    checkNegative(Y);
 RET_CYCLES
}

uint8_t CPU_6502::EOR(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operanAddr = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operanAddr);

    A = (A ^ M);
    checkNegative(A);
    Z = (A == 0);

   RET_CYCLES
}

uint8_t CPU_6502::INC(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operanAddr = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operanAddr);

    uint8_t res = M + 1;
    Z = (res == 0);
    checkNegative(res);
    writeMem(operanAddr, res);
 RET_CYCLES
}

uint8_t CPU_6502::INX(uint16_t &addr, const Opcode &opcode)
{
    X = X + 1;
    Z = (X == 0);
    checkNegative(Y);
RET_CYCLES
}

uint8_t CPU_6502::INY(uint16_t &addr, const Opcode &opcode)
{
    Y = Y + 1;
    Z = (Y == 0);
    checkNegative(Y);
RET_CYCLES
}

uint8_t CPU_6502::JMP(uint16_t &addr, const Opcode &opcode)
{
   
    uint16_t operanAddr = getOperandAddress(addr, opcode);
     std::cout << std::hex << "JMP, addr PC: " << (unsigned)PC-2 << " jump to -->" << operanAddr <<std::endl;
 
    //if(PC-3 == 0xDBB5) throw;
    PC = operanAddr;
   
 RET_CYCLES
}

uint8_t CPU_6502::JSR(uint16_t &addr, const Opcode &opcode)
{

  
    std::pair<uint8_t, uint8_t> vals = split16int(addr+1);

    std::cout << "SAVING JSR RETURN ADDR" << std::hex << ": " << (unsigned)addr+1 << std::endl;
    std::cout << std::hex << "JSR at addr PC: " << (unsigned)PC-1 << " - 0x"<<(unsigned)vals.second << (unsigned)vals.first<<std::endl;
    stack_push(vals.second);
    stack_push(vals.first);
   


    uint16_t operanAddr = getOperandAddress(addr, opcode);
    PC = operanAddr;
   RET_CYCLES
}


uint8_t CPU_6502::CLI(uint16_t &addr, const Opcode &opcode)
{
    I = 0;
   RET_CYCLES
}


uint8_t CPU_6502::LDA(uint16_t &addr, const Opcode &opcode)
{
    int extraCycles = 0;
    uint16_t operanAddr = getOperandAddress(addr, opcode,extraCycles);
    uint8_t M = readMem(operanAddr);

    int cycles = opcode.cycles + extraCycles;

   // std::cout << std::hex << "LDA addr: 0x" << (unsigned)addr-2 << std::endl; 
  //  std::cout << "Value at 0x" << std::hex << (int)operanAddr << " - " << (unsigned)M << std::endl;

    A = M;
    checkZero(A);
    checkNegative(A);
    std::cout << std::hex << (int) A << std::endl;

        uint8_t S = 0;

        S = ((S & ~(1 << 7)) | (N << 7));
        S = ((S & ~(1 << 6)) | (V << 6));
        S = ((S & ~(1 << 5)) | (1 << 5)); // PHP sets bits 5 and 4
        S = ((S & ~(1 << 4 ))| (1 << 4)); // PHP sets bits 5 and 4
        S = ((S & ~(1 << 3)) | (D << 3));
        S = ((S & ~(1 << 2)) | (I << 2));
        S = ((S & ~(1 << 1)) | (Z << 1));
        S = ((S & ~(1 << 0)) | (C << 0));

        myfile << std::hex << " $"<< (unsigned)operanAddr << " = #$" <<(unsigned)M;


   return cycles;
}



uint8_t CPU_6502::RTI(uint16_t &addr, const Opcode &opcode)
{
    uint8_t S = stack_pop(); // status reg

    C = S & (1 << 0);
    Z = S & (1 << 1);
    I = S & (1 << 2);
    D = S & (1 << 3);
  //  B = S & (1 << 5);
    V = S & (1 << 6);
    N = S & (1 << 7);

    // uint8_t status = N << 7 | V << 6 | 1 << 5 | 1 << 4 | D << 3 | I << 2 | Z << 1 | C;

    RET_CYCLES
}

uint8_t CPU_6502::RTS(uint16_t &addr, const Opcode &opcode)
{
   // _stack.DebugPrintStackVals();
 //   std::cout << "STACK VALUE RTS: Vanha PC: " << std::hex << PC << std::endl;;
    
    uint8_t hig = stack_pop();
   //  std::cout << "Low: " << std::hex << (int)hig << std::endl;
    uint8_t lo = stack_pop(); 
   // std::cout << "high: " << std::hex << (int)lo << std::endl;
    
    uint16_t ad = concatMSB_LSB(hig,lo);
    PC = ad+1;
    //std::cout << std::hex << "new PC: " <<  PC << std::endl;
  RET_CYCLES
}

uint8_t CPU_6502::SBC(uint16_t &addr, const Opcode &opcode)
{
   
    uint16_t operandAddress = getOperandAddress(addr, opcode);
    uint16_t fetched = readMem(operandAddress);
    uint16_t M = (fetched ^ 0x00FF);
   // std::cout << "ADC: [0x" << std::hex << (unsigned)operandAddress << "] = " << (unsigned)M << std::endl;

    uint16_t res = (A + M  +C); // select 8 first bits
 // std::cout << std::dec << "SBC Result" << (unsigned)A << " - " << (unsigned)M << " - (1-" << unsigned(C) << ") = " << (unsigned)res << std::endl;

    // flags
    checkNegative(res);

   // (M >> 7) ^ (A >> 7) ? setOverflow() : clearOVerflow();
   // (A == 0) ? setZero() : clearZero();
    checkZero(res);
    // integer mode
    C = res > 0xff;
  //  V = (A^res) & (~M ^res) & 0x80;

    setOverflowVal((res^A)&(res^M)&0x0080);
    
    
    A = res & 0x00FF;

  RET_CYCLES
}

uint8_t CPU_6502::SEC(uint16_t &addr, const Opcode &opcode)
{
    setCarry(true);
RET_CYCLES
}

uint8_t CPU_6502::SED(uint16_t &addr, const Opcode &opcode)
{
    D = 1;
    RET_CYCLES
}

uint8_t CPU_6502::SEI(uint16_t &addr, const Opcode &opcode)
{
    I = 1;
RET_CYCLES
}

uint8_t CPU_6502::STA(uint16_t &addr, const Opcode &opcode)
{   
    
    int cycles = opcode.cycles;
    uint16_t operanAddr = getOperandAddress(addr, opcode);
  
    writeMem(operanAddr, A);
   // std::cout << "STA PC: " << std::hex << PC << " - " << (int)operanAddr << " value:" << (int)A << " mem: " << (int)readMem(operanAddr) << std::endl;
 return cycles;
}

uint8_t CPU_6502::STX(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operanAddr = getOperandAddress(addr, opcode);
    writeMem(operanAddr, X);
RET_CYCLES
}

uint8_t CPU_6502::STY(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operanAddr = getOperandAddress(addr, opcode);
    writeMem(operanAddr, Y);
 RET_CYCLES
}

uint8_t CPU_6502::TAX(uint16_t &addr, const Opcode &opcode)
{
    X = A;
    Z = (X == 0);
    checkNegative(X);

RET_CYCLES
}

uint8_t CPU_6502::TAY(uint16_t &addr, const Opcode &opcode)
{
    Y = A;
    Z = (Y == 0);
    checkNegative(Y);
RET_CYCLES
}

uint8_t CPU_6502::TSX(uint16_t &addr, const Opcode &opcode)
{
    X = SP;
    Z = (X == 0);
    checkNegative(X);

RET_CYCLES
}

uint8_t CPU_6502::TXA(uint16_t &addr, const Opcode &opcode)
{
    A = X;
    Z = (A == 0);
    checkNegative(A);

RET_CYCLES
}

uint8_t CPU_6502::TXS(uint16_t &addr, const Opcode &opcode)
{
    SP = X;
 RET_CYCLES
}

uint8_t CPU_6502::TYA(uint16_t &addr, const Opcode &opcode)
{
    A = Y;
    Z = (A == 0);
    checkNegative(A);
    RET_CYCLES
}


/* ILLEGAL ISNTRUCITONS */

uint8_t CPU_6502::ASO(uint16_t &addr, const Opcode &opcode)
{

    uint16_t operandAddr = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operandAddr);


   
        C = (M >> 7); // set 7th bit (last) to carry
        M = (M << 1);
        writeMem(operandAddr,M);


        A = A|M;
        checkZero(A);
        checkNegative(A);


  RET_CYCLES
}

uint8_t CPU_6502::RLA(uint16_t &addr, const Opcode &opcode)
{
  uint16_t operandAddr = getOperandAddress(addr, opcode);
   uint8_t M = readMem(operandAddr);

    bool c = 0;
    c = M & (1 << 7);
    M = M << 1;
    M |= C << 0;
    writeMem(operandAddr, M);

 
    uint8_t res = ( A & M);
    A = res;

        setCarry(c);
        checkZero(A);
        checkNegative(A);
 

  RET_CYCLES
}

uint8_t CPU_6502::RRA(uint16_t &addr, const Opcode &opcode)
{
       
    uint16_t operandAddress = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operandAddress);

   
        bool c = 0;
        c = M & (1 << 0);
        M = M >> 1;
        M |= C << 7;
        setCarry(c);
  
        writeMem(operandAddress, M);

    uint16_t res = (A + M + C); // select 8 first bits
   
    // flags
    checkNegative(res);
    checkZero(res);
    setCarry( res > 0xff);
    setOverflowVal((~(A^M)&(A^res)&0x0080)>0);
    A = res & 0x00FF;


RET_CYCLES
}

uint8_t CPU_6502::AXS(uint16_t &addr, const Opcode &opcode)
{
   uint16_t operandAddr = getOperandAddress(addr, opcode);
   uint8_t anded = (A & X);
    writeMem(operandAddr,anded);
RET_CYCLES

}

uint8_t CPU_6502::LAX(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operandAddr = getOperandAddress(addr, opcode);
   // std::cout << "LAX: " << std::hex << "reading memory: 0x" << (unsigned)operandAddr;
    uint8_t M = readMem(operandAddr);
    //   std::cout << " mem val: " << (unsigned)M << std::endl;
    X = M;
    A = M;
    checkNegative(M);
    checkZero(M);
   // std::cout << "LAX: " << (unsigned)addr << " addr mode " << DebugAddressingMode(opcode.mode) << std::hex << std::endl;

   RET_CYCLES
}

uint8_t CPU_6502::DCM(uint16_t &addr, const Opcode &opcode)
{
    uint16_t opAddr = getOperandAddress(addr ,opcode);
    uint8_t M = readMem(opAddr);

    M = M - 1;
    uint8_t res = (A - M);
    writeMem(opAddr, M);
 
    C = (A >= M);
    Z = (A == M);
    checkNegative(res);

RET_CYCLES
}

uint8_t CPU_6502::INS(uint16_t &addr, const Opcode &opcode)
{
    uint16_t opAddr = getOperandAddress(addr ,opcode);
    uint8_t M = readMem(opAddr);

    M = M +1;
    writeMem(opAddr, M);

    M = (M ^ 0x00FF);
    uint16_t res = (A + M  +C); // select 8 first bits
    // flags
    checkNegative(res);
    checkZero(res);
    C = res > 0xff;
    setOverflowVal((res^A)&(res^M)&0x0080);
    A = res;

RET_CYCLES
}

uint8_t CPU_6502::ALR(uint16_t &addr, const Opcode &opcode)
{
RET_CYCLES
}

uint8_t CPU_6502::ARR(uint16_t &addr, const Opcode &opcode)
{
  RET_CYCLES
}

uint8_t CPU_6502::XAA(uint16_t &addr, const Opcode &opcode)
{
   RET_CYCLES
}

uint8_t CPU_6502::OAL(uint16_t &addr, const Opcode &opcode)
{
 RET_CYCLES
}

uint8_t CPU_6502::SAX(uint16_t &addr, const Opcode &opcode)
{
 RET_CYCLES
}

uint8_t CPU_6502::SKB(uint16_t &addr, const Opcode &opcode)
{
    PC++;
   RET_CYCLES
}

uint8_t CPU_6502::SKW(uint16_t &addr, const Opcode &opcode)
{
    PC = PC+2;
RET_CYCLES
}

uint8_t CPU_6502::HLT(uint16_t &addr, const Opcode &opcode)
{
  RET_CYCLES
}


uint8_t CPU_6502::TAS(uint16_t &addr, const Opcode &opcode)
{
  RET_CYCLES
}

uint8_t CPU_6502::SAY(uint16_t &addr, const Opcode &opcode)
{
  RET_CYCLES
}

uint8_t CPU_6502::XAS(uint16_t &addr, const Opcode &opcode)
{
   RET_CYCLES
}

uint8_t CPU_6502::AXA(uint16_t &addr, const Opcode &opcode)
{
   RET_CYCLES
}

uint8_t CPU_6502::LSE(uint16_t &addr, const Opcode &opcode)
{
        uint16_t operandAddress = getOperandAddress(addr, opcode);
        uint8_t M = readMem(operandAddress);
   
        bool c = 0;
        c = M & (1 << 0);
        M = M >> 1;

    writeMem(operandAddress, M);
        setCarry(c);

    A = (A ^ M);


        checkZero(A);
        checkNegative(A);
   RET_CYCLES
}



uint8_t CPU_6502::SBC_IL(uint16_t &addr, const Opcode &opcode)
{
    SBC(addr, opcode);
    RET_CYCLES
}



uint8_t CPU_6502::LAS(uint16_t &addr, const Opcode &opcode)
{
    uint16_t operandAddr = getOperandAddress(addr, opcode);
    uint8_t M = readMem(operandAddr);
    uint8_t anded = (M & SP);
    A = anded;
    X = anded;
    SP = anded;
    checkNegative(anded);
    checkZero(anded);


 RET_CYCLES
}

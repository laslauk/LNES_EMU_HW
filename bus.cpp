#include "bus.hh"
#include "icpu.hh"
#include "imemory.hh"
#include "PPU.hh"
#include "cartridge.hh"

uint8_t Bus::read_byte(uint16_t addr) {
    //  std::cout << "reading bus mem" << (unsigned)addr << std::endl;

    //0x200 == 2KiB in desimal
    if(addr >= 0x0000 && addr < 0x2000) {
        //Writing to RAM, mirror three times
        uint16_t mirrored_addr = addr & 0x7FF; //== 0b0000_0111_1111_1111; Rams uses only 11 bits of the address
          // std::cout << " Reading W_RAM addr: -" << std::hex << (unsigned)addr << std::endl;

        return _RAM->read_byte(mirrored_addr);
    }

    // PPU Read

    
    if 
    (
        addr == NS_PPU::EPPU_RegisterAddress::Status ||
        addr == NS_PPU::EPPU_RegisterAddress::OAM_Data ||
        addr == NS_PPU::EPPU_RegisterAddress::Data
    ) {
          // std::cout << "READ PPU: -" << std::hex << (unsigned)addr << std::endl;
             return _ppu->read_PPU(addr);

    }
      

      //mirrors 2007-2007
    if(addr >= 0x2008 && addr <= 0x3FFF) {
         // std::cout << "Mirrored PPU registers" << (unsigned)addr<< std::endl;
      //  uint16_t mirrored_addr = (addr & 0xF)+0x2000;
      std::cout << "Mirrored PPU " << std::endl;
               //   std::cout << "Mirrored PPU registers" << (unsigned)mirrored_addr<< std::endl;
          return _ppu->read_PPU(addr);
    }

    //TODO: MIRROR 0x4000 - 0xFFFF RANGE OF PPU 
    if(addr == 0xFFFA || addr == 0xFFFB) {
 //std::cout << "IRQ BRK NMI READ" << std::endl;
    }

    if(addr == 0xFFFC || addr == 0xFFFD) {
 //std::cout << "IRQ BRK RESET READ" << std::endl;
    }

    if(addr == 0xFFFE || addr == 0xFFFF) {
   //     std::cout << "IRQ BRK VECTOR READ" << std::endl;
    }

    //Cartridge PRG ROM
    if(addr >= 0x8000 && addr <= 0xFFFF) {
        uint16_t mappedAddr = addr-0x8000;

   // std::cout << " Reading CARTRIDGE ADDR addr: " << std::hex << (unsigned)addr <<", mapped addr: " << (unsigned)mappedAddr<< std::endl; 


        if(_cartridge->prg_rom_len() == 0x4000 && mappedAddr >= 0x4000) {
            mappedAddr = mappedAddr % 0x4000;
        }
   // std::cout << " After remapping " << std::hex << (unsigned)addr <<", mapped addr: " << (unsigned)mappedAddr<< std::endl; 

        uint8_t data = _cartridge->readByte_PRG(mappedAddr);
      //  std::cout << "data from cartridge: " << (unsigned) data << std::endl;
        
        return _cartridge->readByte_PRG(mappedAddr);

    }

    if(addr >= 0x4000 && addr <= 4017) {
            // std::cout <<  "Reading NES APU/IO regs" << std::endl;
             return 0;
    }

        if(addr >= 4018 && addr <= 0x401F) {
           //  std::cout <<  "APU and IO functionality, normally disabled" << std::endl;
             return 0;
    }

    if(addr >= 0x4020 || addr <= 0xFFFF ) {
    std::cout << "READ: Cartridge space: "<< std::hex << (unsigned)addr << std::endl;
      uint16_t mappedAddr = addr-0x4020;
         if(_cartridge->prg_rom_len() == 0x4000 && mappedAddr >= 0x4000) {
            mappedAddr = mappedAddr % 0x4000;
             return _cartridge->readByte_PRG(mappedAddr);
        }
      
    }
    std::cout << "READ: UNKNOWN MEMORY MAPPING: "<< std::hex << (unsigned)addr << std::endl;
    return _mem->read_byte(addr);
}



void Bus::write_byte(uint16_t addr,  uint8_t data) {

   
    if(addr >= 0x0000 && addr < 0x2000) {

/*
        if(addr >= 0x0100 && addr <= 0x1FF)  {
         std::cout << "WRITING to: STACK" << std::endl;
                  
    } else {}
         */

        //Writing to RAM, mirror three times

        uint16_t mirrored_addr = addr & 0x7FF; //== 0b0000_0111_1111_1111; Rams uses only 11 bits of the address
   

        return _RAM->write_byte(mirrored_addr, data);
          
    }

    //PPU Write
    if
    (
        addr == NS_PPU::EPPU_RegisterAddress::Scroll ||
        addr == NS_PPU::EPPU_RegisterAddress::Address  || 
        addr == NS_PPU::EPPU_RegisterAddress::Data ||
        addr == NS_PPU::EPPU_RegisterAddress::OAM_Data || 
        addr == NS_PPU::EPPU_RegisterAddress::Controller || 
        addr == NS_PPU::EPPU_RegisterAddress::Mask ||
        addr == NS_PPU::EPPU_RegisterAddress::OAM_Address
     )  
        return _ppu->write_PPU(addr, data);
   
    if(addr >= 0x2008 && addr <= 0x3FFF) {
          std::cout << "WRITING TO: Mirrored PPU registers" << (unsigned)addr<< std::endl;
        uint16_t mirrored_addr = (addr & 0xF)+0x2000;
                  std::cout << "WRITING TO: : Mirrored PPU registers" << (unsigned)mirrored_addr<< std::endl;
          return _ppu->write_PPU(addr,data);
    }


    if(addr >= 0x4000 && addr <= 0x4017) {

        std::cout << "Writing to APU" << std::endl;
        return;
    }

      //TODO: MIRROR 0x4000 - 0xFFFF RANGE OF PPU 
    if(addr == 0xFFFA || addr == 0xFFFB) {
 std::cout << "IRQ BRK NMI WRITE" << std::endl;
     return;
    }

    if(addr == 0xFFFC || addr == 0xFFFD) {
 std::cout << "IRQ BRK RESET WRITE" << std::endl;
     return;
    }

    if(addr == 0xFFFE || addr == 0xFFFF) {
        std::cout << "IRQ BRK VECTOR WRITE" << std::endl;
        return;
    }



    if(addr>=0x4020 && addr <= 0xFFFF) {
        std::cout << "Writing to CARTRIDGE???" << std::endl;
        return;
    }

    

     std::cout << "WRITING TO SOMETHIGN?????: " << std::hex << (unsigned)addr << std::endl;
    _mem->write_byte(addr,data);
}


   uint8_t Bus::tick(uint8_t cycles) {
    _cycles += cycles;
        _ppu->tickPPU(cycles*3);
    }


 bool Bus::Interrupt(HW::EInterrupt interrupt){
            _cpu->Interrupt(interrupt);
    return true;
 }

bool Bus::init_bus(IMemory* ram, ICPU* cpu, PPU* ppu) {
    _RAM = ram;
    _ppu = ppu;
    _cpu = cpu;
    return true;
}


bool Bus::loadCartridge(Cartridge* cartridge){
    _cartridge = cartridge;
    return true;
}
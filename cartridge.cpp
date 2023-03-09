#include "cartridge.hh"
#include "imemory.hh"
#include <iostream>

  void Cartridge::load(ROMData rom) {

    _CHR_ROM->setMemory(rom.chr_rom);
    std::cout<< "loaded chr rom, size: " << std::hex <<  (unsigned)_CHR_ROM->getMemoryLenghtBytes() << std::endl;

    _PRG_ROM->setMemory(rom.prg_rom);
    std::cout<< "loaded prg rom, size: " << std::hex << (unsigned)_PRG_ROM->getMemoryLenghtBytes() << std::endl;

  }

uint8_t Cartridge::readByte_PRG(uint16_t addr) {

     return _PRG_ROM->read_byte(addr);
  }

uint8_t  Cartridge::readByte_CHR (uint16_t addr) {
        return _CHR_ROM->read_byte(addr);
  }



uint8_t Cartridge::readByte(uint16_t addr) {

return 0;

}

uint16_t Cartridge::prg_rom_len()
{
  //  std::cout << "prg romn len " << std::endl;
    if(_PRG_ROM != nullptr) {

     return _PRG_ROM->getMemoryLenghtBytes();
    } else {
        std::cout << "_prg rom nullptr" << std::endl; throw;
        return 0;
    }
    
  
}
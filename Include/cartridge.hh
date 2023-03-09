
#ifndef CARTRIDGE_HH
#define CARTRIDGE_HH
#include <stdint.h>
#include <vector>
#include "LNES_utilities.hh"
#include "memory.hh"
#include <iostream>
class IBus;
class IMemory;
class ROMData;



class Cartridge {

public:

    /* iNES */
    // 16 bytes HEADER
    
    // OPTIONAL: 512 bytes trainer, famicom shit

    // PRG_ROM , size by byte4 in header
    // CHR ROM, size by byte 5 in header

    // Bare minimum to care: PRG; ROm, CHROM, mapper type, mirroring type: horizontal, vetical, 4 screen

Cartridge() {
    _CHR_ROM = new Memory();
    _PRG_ROM = new Memory();
    std::cout << "cartridgr created, mem pointers created " << std::endl;

}

Cartridge(std::string name) {
    _CHR_ROM = new Memory();
    _PRG_ROM = new Memory();
    _name = name;
    std::cout << "cartridgr created, mem pointers created " << name << std::endl;
}

void load(ROMData rom);
 uint8_t readByte(uint16_t addr);
 uint8_t readByte_CHR (uint16_t addr);
 uint8_t readByte_PRG (uint16_t addr);
 uint16_t prg_rom_len();


 std::vector<uint8_t> dump_prg_rom(int byteCount) {
    std::vector<uint8_t> temp;
    for(auto it = 0; it < byteCount; it++) {
        temp.push_back( _PRG_ROM->read_byte(it+0x4000));
    }
    std::cout << "Reutrning dump " << std::endl;
    return temp;
 }

  std::vector<uint8_t> dump_chr_rom(int byteCount) {
  
    std::vector<uint8_t> temp;
    for(auto it = 0; it < byteCount; it++) {
        temp.push_back( _CHR_ROM->read_byte(it));
    }
    return temp;
 }

public:
    IMemory* _CHR_ROM; //Game's video graphics data -> Connects cirectly to PPU
    IMemory* _PRG_ROM; //CPU instructions, games code -> connects directly to CPU


    private:
    IBus *_bus;

    uint8_t _mapper;
    Mirroring mirroringMode;
    
    public:
    std::string _name;

};

#endif
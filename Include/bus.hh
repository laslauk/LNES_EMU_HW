#ifndef BUS_HH
#define BUS_HH

#include "ibus.hh"
#include <stdint.h>
#include "LNES_utilities.hh"


class Cartridge;
class IMemory;
class ICPU;
class PPU;


class Bus: public IBus {

public:

    uint8_t read_byte(uint16_t addr) override;
    void write_byte(uint16_t addr, uint8_t data) override;
    bool init_bus(IMemory* memory, ICPU* cpu, PPU* ppu) override;
    bool loadCartridge(Cartridge* cartridge) override;
    uint8_t tick(uint8_t cycles) override;
    bool Interrupt(HW::EInterrupt interrupt) override;
    


    
   IMemory* _RAM; //[0x800. 0x1000] [x1000.. 0x1800] [0x1800...0x2000] mirrored
private:
    ICPU* _cpu;
    PPU* _ppu;
    IMemory* _mem;
    Cartridge* _cartridge;

    double _cycles;

};

#endif
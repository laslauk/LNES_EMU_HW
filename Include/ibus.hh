#ifndef IBUS_HH
#define IBUS_HH

#include <stdint.h>
#include "LNES_utilities.hh"

class IMemory;
class ICPU;
class Cartridge;
class PPU;

class IBus {

public:

    ~IBus() = default;
    virtual bool init_bus(IMemory* memory, ICPU* cpu, PPU* ppu) = 0;
    virtual uint8_t read_byte(uint16_t addr) = 0;
    virtual void write_byte(uint16_t addr, uint8_t data) = 0;
    virtual bool loadCartridge(Cartridge* cartridge) = 0;
    virtual bool Interrupt(HW::EInterrupt interrupt) = 0;
    virtual     uint8_t tick(uint8_t cycles) = 0;
  

    

private:


};
#endif
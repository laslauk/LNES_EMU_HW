#ifndef ICPU_HH
#define ICPU_HH

#include <stdint.h>
#include "LNES_utilities.hh"


class IBus;

class ICPU {
    public:


    ~ICPU() = default;
    virtual uint8_t readMem(uint16_t addr) = 0;
    virtual void writeMem(uint16_t addr, uint8_t data) = 0;
    virtual bool setBus(IBus* bus) = 0;
    virtual bool Interrupt(HW::EInterrupt interrupt) = 0;
    private:
};

#endif
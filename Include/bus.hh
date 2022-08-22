#ifndef BUS_HH
#define BUS_HH

#include "ibus.hh"
#include <cstdint>

class Bus: public IBus<uint8_t, uint16_t> {

public:
    uint8_t read_byte(uint16_t addr) override;
    void write_byte(uint16_t addr) override;

    virtual bool init(mem* memory, cpu* cpu) = 0;

private:


};

#endif
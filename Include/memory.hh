#ifndef MEMORY_HH
#define MEMORY_HH

#include "imemory.hh"
#include <vector>
#include <stdint.h>

class bus;


class Memory: public IMemory<uint8_t, uint16_t>{

public:


    uint8_t read_byte(uint16_t addr) override ;
    void write_byte(uint16_t addr, uint8_t data) override;
    void init(int size) override;
    void init_bus(bus* bus) override ;



private:
    std::vector<uint8_t> _memory;

};

#endif
#ifndef MEMORY_HH
#define MEMORY_HH

#include "imemory.hh"
#include <vector>
#include <stdint.h>

class IBus;

class Memory: public IMemory{

public:

    ~Memory();
    uint8_t read_byte(uint16_t addr) override ;
    void write_byte(uint16_t addr, uint8_t data) override;
    void init(int size) override;
    void init_bus(IBus* bus) override;
     void setMemory(const std::vector<uint8_t>& data) override;
    uint16_t getMemoryLenghtBytes() override;



private:
    std::vector<uint8_t> _memory;

};

#endif
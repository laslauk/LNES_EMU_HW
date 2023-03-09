
#ifndef IMEMORY_HH
#define IMEMORY_HH

#include <stdint.h>
#include <vector>

class IBus;


class IMemory {

public:

    virtual ~IMemory() {};

    virtual void init(int size) = 0;
    virtual uint8_t read_byte(uint16_t addr) = 0;
    virtual void write_byte(uint16_t addr, uint8_t data) = 0;
    virtual void init_bus(IBus* bus) = 0;
    virtual void setMemory(const std::vector<uint8_t>& data) = 0;
    virtual uint16_t getMemoryLenghtBytes() = 0;

};

#endif
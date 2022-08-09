#ifndef MEMORY_HH
#define MEMORY_HH

#include "imemory.hh"
#include <vector>

template<class MemSize, class AddrSize,class DataSize>
class Memory: public IMemory<uint8_t, uint16_t> {

public:
    Memory();
    MemSize read_byte(AddrSize addr) override;
    void write_byte(AddrSize addr, DataSize data) override;
    
private:

    std::vector<MemSize> _memory;

};

#endif

#ifndef IMEMORY_HH
#define IMEMORY_HH
class bus;

template<class MemSize, class AddrSize>
class IMemory {

public:

    virtual void init(int size) = 0;
    virtual MemSize read_byte(AddrSize addr) = 0;
    virtual void write_byte(AddrSize addr, MemSize data) = 0;
    virtual void init_bus(bus* bus) = 0;



};

#endif
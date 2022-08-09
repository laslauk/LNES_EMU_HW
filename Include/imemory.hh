
#ifndef IMEMORY_HH
#define IMEMORY_HH
class bus;

template<class S, class A>
class IMemory {

public:

    virtual void init_memory() = 0;
    virtual S read_byte(A addr) = 0;
    virtual void write_byte(A addr) = 0;
    virtual void init_bus(bus* bus);

private:

};
#endif
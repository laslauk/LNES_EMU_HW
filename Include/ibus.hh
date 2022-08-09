#ifndef IBUS_HH
#define IBUS_HH

class mem;
class cpu;

template<class S, class A>
class IBus {

public:
    IBus();
    virtual bool init_bus(mem* memory, cpu* cpu) = 0;
    virtual S read_byte(A addr) = 0;
    virtual void write_byte(A addr) = 0;

private:


};
#endif
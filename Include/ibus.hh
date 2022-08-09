
class IMemory;
class ICPU;

template<class S, class A>
class IBus {

public:
    IBus();
    bool init_bus(IMemory* memory, ICPU* cpu) = 0;
    S read_byte(A addr) = 0;
    void write_byte(A addr) = 0;

private:


};
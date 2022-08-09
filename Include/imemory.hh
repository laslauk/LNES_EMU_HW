class IBus;

template<class S, class A>
class IMemory {

public:

    void init_memory() = 0;
    S read_byte(A addr) = 0;
    void write_byte(A addr) = 0;
    void init_bus(IBus* bus);

private:

};
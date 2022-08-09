template<class S, class A>
class IBus {

public:
    IBus();
    S read_byte(A addr) = 0;
    void write_byte(A addr) = 0;

private:


};
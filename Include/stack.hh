#ifndef STACK_HH
#define STACK_HH

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>


class Stack {

    public:
    Stack();
    bool push(uint8_t data);
    uint8_t pop(); 

    void DebugPrintStackVals() {
        int indx = 0;
        for(auto it = _stack_mem.begin(); it != _stack_mem.end(); it++) {
            std::cout << std::hex << "[" << indx << "]: " << (int)*it << std::endl;
            indx++;
        }
    }


private:
    std::vector<uint8_t> _stack_mem;
    uint8_t _SP;

};

#endif
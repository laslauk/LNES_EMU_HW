#include "memory.hh"
#include <iostream>

    uint8_t Memory::read_byte(uint16_t addr){
        return 0;
    }
    void Memory::write_byte(uint16_t addr, uint8_t data) {
        
    }

    void Memory::init(int size) {
        std::cout << "gay" << std::endl;
        _memory.resize(size);
        std::fill(_memory.begin(), _memory.end(), 0);
        std::cout << "Memory size:" << _memory.size() << std::endl;
        
    }

    void Memory::init_bus(bus* bus) {
        
    }
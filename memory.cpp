#include "memory.hh"
#include <iostream>

uint8_t Memory::read_byte(uint16_t addr)
{
    return _memory.at(addr);
}

void Memory::write_byte(uint16_t addr, uint8_t data)
{
 
    //  std::cout << "at: " << std::hex << _memory.at(addr) << " saved:" <<(int)data<< "to addr:" << addr << std::endl;
    _memory.at(addr) = data;
     
}

Memory::~Memory()
{
}

void Memory::init(int size)
{
    _memory.resize(size);
    std::fill(_memory.begin(), _memory.end(), 0);
    std::cout << "Memory size:" << std::hex << (unsigned)_memory.size() << "bytes" << std::endl;
}

void Memory::init_bus(IBus *bus)
{
}


 void Memory::setMemory(const std::vector<uint8_t>& data){
     std::cout << "setting memory, data elements:" << std::hex << (unsigned)data.size() << std::endl;

    init(data.size());
    int i = 0;
    for(auto it = data.begin(); it != data.end(); it++) {
        _memory.at(i) = *it;
        i++;
    }

    std::cout << "copied mem" << std::endl;
 }

 uint16_t Memory::getMemoryLenghtBytes(){
  //  std::cout << "memory length bytes " << std::endl;
            return _memory.size();
        }
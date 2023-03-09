#include "stack.hh"
#include "NES_MemoryMap.hh"

#include <iostream>


 Stack::Stack(){
    _stack_mem.resize(0xFF);
    _SP = 0xFD;
        std::fill(_stack_mem.begin(), _stack_mem.end(),0);
      
 }

uint8_t Stack::pop() {
    if(!_stack_mem.empty()) {
        uint8_t val =  _stack_mem.at(_SP);
        _SP = _SP+1;
        
        return val;
    } else {
        return 0;
    }
  
}




bool Stack::push(uint8_t data) {
    if(_stack_mem.size() < STACK_SIZE) {
       _stack_mem.insert(_stack_mem.begin() + _SP, data);
       _SP = _SP -1;
        
        return true;

    } else {
    
        return false;
    
    }
}



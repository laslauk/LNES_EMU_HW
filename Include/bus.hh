#include "ibus.hh"
#include <cstdint>

class Bus: public IBus<uint8_t, uint16_t> {

public:
    uint8_t read_byte(uint16_t addr);
    uint8_t write_byte(uint16_t addr);
private:


};
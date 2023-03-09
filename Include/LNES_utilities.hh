#ifndef LUTILS_HH
#define LUTILS_HH
#include <stdint.h>
#include <vector>


enum Mirroring {
    VERTICAL,
    HORIZONTAL,
    FOUR_SCREEN
};


namespace HW {
    enum EInterrupt {
        None,
        NMI,

    };
}

struct ROMData {
    std::vector<uint8_t> header; //16bytes
    std::vector<uint8_t> prg_rom;
    std::vector<uint8_t> chr_rom;  
    bool iNESFormat = false;
    bool NES20Format = false;
    bool trainerPresent = false;
    uint8_t mapper;
    Mirroring mirroringMode;
};

namespace NS_PPU {

    enum NametableMap {
    TOP_LEFT,
    TOP_RIGHT,
    BOT_LEFT,
    BOT_RIGHT
};



enum EPPU_RegisterAddress {
    Controller = 0x2000,
    Mask = 0x2001,
    Scroll = 0x2005,
    Address = 0x2006,
    Data = 0x2007,
    OAM_Address = 0x2003,
    OAM_Data = 0x2004,
    Status = 0x2002,
};
}

 struct Color {
    uint8_t R, G, B;
    Color(int R, int G, int B): R(R), G(G), B(B){};
};

enum EControlRegister {
    NAMETABLE1 = 0x1,
    NAMETABLE2 = 0x2,
    VRAM_ADD_INCREMENT = 0x4,
    SPRITE_PATTERN_ADDR = 0x8,
    BACKGROUND_PATTERN_ADDR = 0x10,
    SPRITE_SIZE = 0x20,
    MASTER_SLAVE_SELECT = 0x40,
    GENERATE_NMI = 0x80
};



#endif
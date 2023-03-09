#ifndef PPU_HH
#define PPU_HH

#include <iostream>
#include "icpu.hh"
#include <stdint.h>
#include "LNES_utilities.hh"
#include "cartridge.hh"
class IBUS;



/* PPU MEMORY MAP */
/*
* 0x0000-0x0FFF $1000 Pattern Table 0 == CHR_ROM
* 0x1000-0x1FFF $1000 Pattern Table 1 == CHR_ROM
* 0x2000-0x23FF $0400 Name Table 0 - VRAM
* 0x2400-0x27FF $0400 Name Table 1 - VRAM
* 0x2800-0x2BFF $0400 Name Table 2 - VRAM
* 0x2C00-0x2FFF $0400 Name Table 3 - VRAM
* 0x3000-0x3EFF $0F00 Mirrors of $2000-2EFF = Palettes
* 0x3F00-3F1F $0020 Paletter RAM indexes 
* 0x3F20-0x3FFF $00E0 mirrors of $3F00-3F1F


OAM - Object attribute memory of 256 bytes: determines how sprites are renderer. CPu can manipulate 
these through mem mapped registers:

OAMADDR($2003), OAMDATA($2004), OAMDATA($014). Array with 64 entries, each entry 4 bytes (32bits)
    Entry: [Sprite y, sprite tile num, sprite attr, sprite x coord]


    address low nibble: desription
    $00,$04, $08, $0C - Y coord
    $01, $05, $09, $0D, - sprite tile #
    $02, $06,$0A, $0E - attr
    $03, $07,, $0B, $0F - X coord

Hardware mappings: 

$0000-1FFF -> CHROM or CHR-RAM often with bank switching
$2000-2FFF -> nes internal VRAM providing 2 nametables 
$3000-3EFF -> mirror of 2kb region from 2000-2eFF

$3F00-3FFF -> mapped to internal paletter control


*/


/* PPU Execution and TV screen */
/*
    ppu renders 262 scanlines per frame, 0-240 visible scanlines, rest are vertical overscan.

    -each scanline lasts for 341 PPU clock cycles. With each clock cycle producing one pixel.
    first 256 pixerls are visible, rest is horizontal overscan.


    NES screen resolution 320x240 -> 241 -262 are not visible.


    When entering 241st scanline, PPU triggers VBlank NMI on the CPU. PPU --> CPU NMI!
    PPU makes noi memory accesses during 241-262 scanlines, so PPU memory can be freely accessed by the program.
    majority of games update the screen state only during this period. preparing te view state for next frmae.



*/

/*
    Nametable:


uint16_t PPU::vram_mirror_addr(uint16_t original_addr);

1024 byte area of memory to lay out backgrounds. Each byte contrsol 8x8 pixel character cell.
Each nametabvle has 30 rows, 32 tiles for each. for 960 ($3C0) byrtes, rest is used by each

nametable's ATTRIBUTE table. total 256x240 pixels on one map.

Nametable

(0,0)     (256,0)   (511,0)
 +---------+---------+ 
 |         |         |
 |  $2000  | $2400   |
 |         |         |
 +(0,240)--+---------+ (511,240)
 |         |         |
 |  $2800  |  $2C00  |
 |         |         |
 +--------+----------+
(0,479)   (256,479)    (511,479)

NES has 4x Nametables. in 2x2 Pattern. $400 bytes address ero.
Jokanen nametable on 1KiB (1024 bytes) of PPU address table ($400);



*/








class IMemory;
class Cartridge;

class PPU: public ICPU {

public:

void generateVBLANK_NMI();
void setVBLANKStatus();
void resetVBLANKStatus();
//PPU Registers
    //PPUDATA - write to this to store at VRAM, at address PPUADDR, (ppuaddr increased after write)
    //PPUADDR, write twice to store address that PPUDATA will write to. firsthigh, second lolw bytes

    uint8_t Controller = 0, Mask = 0, Status= 0x10, OAM_addr= 0, OAM_data= 0, Scroll= 0, PPUDATA = 0, OAM_DMA= 0;
    uint16_t  PPUADDR = 0;
  

    PPU();

    uint8_t readMem(uint16_t addr) override;
    void writeMem(uint16_t addr, uint8_t data) override;
    bool setBus(IBus* bus) override;
    bool Interrupt(HW::EInterrupt interrupt) override {return true;};

    std::vector<uint8_t> getCHR_ROM(int bytes) {
        return (_cartridge->dump_chr_rom(bytes));
    }

    inline bool setVRAM(IMemory* mem) {_vram = mem;};
    inline bool setCartridge(Cartridge* cart) {_cartridge = cart;};
    inline Cartridge* getCartridge() {return _cartridge;};
    inline bool setMirroringMode(Mirroring mode) { mirroringMode = mirroringMode;};

    /* Read Access functions */
    uint8_t read_PPU(uint16_t addr);
    uint8_t read_PPUSTATUS();
    uint8_t read_PPUDATA();
    uint8_t read_OAMData();
    uint8_t read_PPU_VRAM_Bypass(uint16_t addr);

    void write_OAMAddress(uint16_t data);

    uint8_t readVRAM(uint16_t addr);

    bool tickPPU(uint8_t cycles);

   /* Write Access functions */
    void write_PPU(uint16_t addr, uint8_t data);
    void write_scroll(uint8_t data);
    void write_OAMData(uint8_t data);
    void write_mask(uint8_t data);
    void write_PPUADDR(uint8_t data);
    void write_ctrl(uint8_t data);
    void write_PPUDATA(uint8_t data);

    void writeToVRAM(uint16_t addr, uint8_t data) ;


static inline std::vector<Color> SYSTEM_PALLETE = {
   {0x80, 0x80, 0x80}, {0x00, 0x3D, 0xA6}, {0x00, 0x12, 0xB0}, {0x44, 0x00, 0x96}, {0xA1, 0x00, 0x5E},
   {0xC7, 0x00, 0x28}, {0xBA, 0x06, 0x00}, {0x8C, 0x17, 0x00}, {0x5C, 0x2F, 0x00}, {0x10, 0x45, 0x00},
   {0x05, 0x4A, 0x00}, {0x00, 0x47, 0x2E}, {0x00, 0x41, 0x66}, {0x00, 0x00, 0x00}, {0x05, 0x05, 0x05},
   {0x05, 0x05, 0x05}, {0xC7, 0xC7, 0xC7}, {0x00, 0x77, 0xFF}, {0x21, 0x55, 0xFF}, {0x82, 0x37, 0xFA},
   {0xEB, 0x2F, 0xB5}, {0xFF, 0x29, 0x50}, {0xFF, 0x22, 0x00}, {0xD6, 0x32, 0x00}, {0xC4, 0x62, 0x00},
   {0x35, 0x80, 0x00}, {0x05, 0x8F, 0x00}, {0x00, 0x8A, 0x55}, {0x00, 0x99, 0xCC}, {0x21, 0x21, 0x21},
   {0x09, 0x09, 0x09}, {0x09, 0x09, 0x09}, {0xFF, 0xFF, 0xFF}, {0x0F, 0xD7, 0xFF}, {0x69, 0xA2, 0xFF},
   {0xD4, 0x80, 0xFF}, {0xFF, 0x45, 0xF3}, {0xFF, 0x61, 0x8B}, {0xFF, 0x88, 0x33}, {0xFF, 0x9C, 0x12},
   {0xFA, 0xBC, 0x20}, {0x9F, 0xE3, 0x0E}, {0x2B, 0xF0, 0x35}, {0x0C, 0xF0, 0xA4}, {0x05, 0xFB, 0xFF},
   {0x5E, 0x5E, 0x5E}, {0x0D, 0x0D, 0x0D}, {0x0D, 0x0D, 0x0D}, {0xFF, 0xFF, 0xFF}, {0xA6, 0xFC, 0xFF},
   {0xB3, 0xEC, 0xFF}, {0xDA, 0xAB, 0xEB}, {0xFF, 0xA8, 0xF9}, {0xFF, 0xAB, 0xB3}, {0xFF, 0xD2, 0xB0},
   {0xFF, 0xEF, 0xA6}, {0xFF, 0xF7, 0x9C}, {0xD7, 0xE8, 0x95}, {0xA6, 0xED, 0xAF}, {0xA2, 0xF2, 0xDA},
   {0x99, 0xFF, 0xFC}, {0xDD, 0xDD, 0xDD}, {0x11, 0x11, 0x11}, {0x11, 0x11, 0x11}
};

   

    
    uint8_t vramAddressIncrement();
    uint16_t vram_mirror_addr(uint16_t original_addr);

    uint16_t getBGNametableAddr() {
        uint8_t nt = ( Controller & (1 << 0) | Controller & (1<<1));

        switch(nt) {
            case 0: return 0x2000; 
            case 1: return 0x2400; 
            case 2: return 0x2800; 
            case 3: return 0x2C00; 
        }
    }
  
  uint16_t getBackgroundCHR_ROM_Bank() {
     uint8_t bank = Controller & (1 << 4);
  }

  

  std::vector<std::vector<uint8_t>> readBackGroundNametableBytes(uint8_t bank, uint16_t nameTableAddr) {
    //read 960 bytes from specified nametable, screen 32x30 tiles (8x8 each tile)

        std::vector<std::vector<uint8_t>> frame;

        //Format [y][x]
        int i = 0;
       for(int y  = 0; y < 30; y++) {
        std::vector<uint8_t> rowBytes;
            for(int x = 0; x < 32; x++) {
                //uint8_t data = read_PPU(nameTableAddr+i+x);
               // std::cout << "READING VRAM ADDRESS:" << std::hex << (unsigned) (nameTableAddr+i+x) << std::endl;
                uint8_t data = readVRAM(nameTableAddr+i+x);
            //  std::cout << "Reading PPU addr: " << std::hex << (unsigned)(nameTableAddr+i+x) << ", data: " << (unsigned)data << std::endl;
              rowBytes.push_back(data);
            }
            frame.push_back(rowBytes);
            //every y row adds 0x20
            i += 0x20;
       }
      // std::cout << "SIZE OF BG NAEMTABLE BYTES FRAME: " << std::dec << (unsigned)frame.size() <<"x"<< (unsigned)frame.at(0).size() << std::endl;
       return frame;
  }

//maps frame of 32x30 bytes of values to specified pattern table

//takes [32x30 matrix of bytes read from vram]
 std::vector< std::vector<std::vector<std::vector<Color>>>>  mapBGNametableBytesToPatterntableTiles(  std::vector<std::vector<uint8_t>>  frame) {
    std::vector< std::vector<std::vector<std::vector<Color>>>> frameTiles;

//std::cout << "MAP BG NAME TABLE BYTES TO PATTERN TABLE START :::" << std::endl;
    int i = 0;
       for(int y  = 0; y < 30; y++) {
           std::vector<std::vector<std::vector<Color>>> rowTiles;
            for(int x = 0; x < 32; x++) {
              rowTiles.push_back(patternTableTiles.at(frame[y][x]));
            }
            frameTiles.push_back(rowTiles);
            //every y row adds 0x20
       }
       //std::cout << "MAP BG NAME TABLE BYTES TO PATTERN TABLE END !!!!" << std::endl;
  //  frame
    return frameTiles;
}


//NOTE: ALSO SETS INTERNAL PATTERNTABLETILES variable.
 std::vector< std::vector<std::vector<Color>> > getAllTiles() {

 std::vector<uint8_t> CHR_ROM = getCHR_ROM(0x2000);
 std::vector< std::vector<std::vector<Color>> > tiles;
    int i = 0;
    for(int tileN = 0 ; tileN < 512; tileN++) {
         std::vector<std::vector<Color>> _tile;

        for(int y=0; y<8; y++){ //Y
            uint8_t pixelRow = CHR_ROM.at(y+(16*tileN));
            uint8_t pixelRow2 = CHR_ROM.at((y+8)+(16*tileN));

               std::vector<Color> rowColors;

            for(int pixelCol = 0;pixelCol < 8;pixelCol++){ //X
                
                bool bitLow = pixelRow & (1<< 7 - pixelCol);
                bool bitHigh = pixelRow2 &(1<< 7- pixelCol );
                int index = bitHigh << 1 | bitLow & 0x01;
                Color color = getRGBValue(index);
                rowColors.push_back(color);

                
            }
            _tile.push_back(rowColors);
        }
    tiles.push_back(_tile);
          
    }

    patternTableTiles = tiles;
return tiles;
   

}

Color getRGBValue(int index) {
    switch(index){
        case 0: return PPU::SYSTEM_PALLETE.at(0x01);
        case 1: return  PPU::SYSTEM_PALLETE.at(0x23);
        case 2: return  PPU::SYSTEM_PALLETE.at(0x27);
        case 3: return  PPU::SYSTEM_PALLETE.at(0x30);
        default: return Color(0,0,0);
    }
}

 IMemory *_vram;

  std::vector<uint8_t> dump_vram() {
    std::vector<uint8_t> temp;
    for(auto it = 0; it < 2048; it++) {
        temp.push_back( _vram->read_byte(it));
    }
    std::cout << "Reutrning dump " << std::endl;
    return temp;
 }

private:

    IBus *_bus;
   
    Cartridge* _cartridge; //cart-ridge has CHR_ROM
   
    Mirroring mirroringMode;
    bool hiByte = true; // ADDRESS LATCH, true = reset
    uint16_t _dataBuffer = 0;
   
    std::vector<uint8_t> _palette_table; //internal me to keep paletter tables used by scren
    std::vector<uint8_t> _oam_data; //keep state of sprites

    bool NMI0, NMI1;
    bool NMI_Occured;
    long long _cycles = 0;
    uint16_t scanline = 0;
    uint8_t* framebufferSDLFormat[256*240*3];
    bool _powerup_reset = true;


    //Generated by:
    // PPUCTRL bit 7 == nmi occured == generate NMI
    // Start of vertical blanking: ste NMI_occured in PPo true
    // End of vertical blanking: set NMI_occured to false,
    //Write to PPUCTRL: set NMI_output to bit 7.
    // read PPUstatus: return old status of NMi_ occured in bit 7, then set NMI occured to false
    void generateNMItoCPU() {


    }
    

private:

    std::vector<std::vector<std::vector<Color>>> patternTableTiles;

    uint16_t nametable_VRAM_Addr_to_Index(uint16_t vramAddress);
};

#endif
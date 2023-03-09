#include "PPU.hh"
#include "icpu.hh"
#include "ibus.hh"



using namespace NS_PPU;



PPU::PPU()
{
}

uint8_t PPU::readMem(uint16_t addr)
{
    return _vram->read_byte(addr);
}

  uint8_t PPU::readVRAM(uint16_t addr) {
return _vram->read_byte(addr-0x2000);
}
//TODO: NOTE: Temp wrapper to write internal vram, have to index shift
void PPU::writeToVRAM(uint16_t addr, uint8_t data) 
{
  //VRAM starts at 0x2000
  uint16_t remappedADdr = PPUADDR - 0x2000;
    _vram->write_byte(remappedADdr, PPUDATA);


}

void PPU::writeMem(uint16_t addr, uint8_t data)
{
    _vram->write_byte(addr, data);
}

bool PPU::setBus(IBus *bus)
{
    _bus = bus;
    return true;
}

void PPU::generateVBLANK_NMI(){
    Controller = Controller | (1 << 7);
        std::cout << "generateVBLANK_NMI ppu" << std::endl; 
    _bus->Interrupt(HW::EInterrupt::NMI);
}

void PPU::setVBLANKStatus(){
    Status = Status | (1<<7);
    std::cout << "set vblank status "<< std::endl;
    std::cout << std::hex << (unsigned)Status << std::endl;
}
 void PPU::resetVBLANKStatus(){
    Status &= ~(1 << 7);
 }

uint8_t PPU::read_PPUSTATUS() { 

    uint8_t Stemp = Status;
    Status &= ~(1 << 7);
    hiByte = true; //true reset

    std::cout << "returning ppu status: " << std::hex << (unsigned)Stemp << std::endl;
    
  
    return Stemp; 
    
    }

uint8_t PPU::read_OAMData() { return OAM_Data; }

bool PPU::tickPPU(uint8_t cycles) {

      
        _cycles += cycles;
        if(_cycles >= 30000) _powerup_reset = false;
  //      std::cout << " PPU cycles: "  << std::dec << (unsigned)_cycles << std::endl;
//std::cout << " PPU snaline num: "  << std::dec << (unsigned)scanline << std::endl;
        //full scanline
        if(_cycles >= 341) {
            _cycles -= 341; //TODO: oli aikasemin -= 341;
            scanline += 1;

        //after 241 scanlines, raise NMI interrupt
        if(scanline == 241) {
              std::cout << "SCANLINE 241" << std::endl; 
                generateVBLANK_NMI();
                setVBLANKStatus();
        }

        if(scanline >= 262) {
            scanline = 0;
            resetVBLANKStatus();
                    //std::cout << "SCANLINE YLI 262" << std::endl;
             
                return true;
            }
        }
        return false;

   }

uint8_t PPU::read_PPUDATA()
{
    uint16_t addr = PPUADDR;
    PPUADDR += vramAddressIncrement();

    //CHROM Range
    if (addr >= 0 && addr <= 0x1FFF)
    {
        uint8_t result = _dataBuffer;
        _dataBuffer = _cartridge->readByte_CHR(addr);
        return result;
    }

    if (addr >= 0x2000 && addr <= 0x2FFF)
    {
        /* mirror todo */
    }
    return 0;
}

void PPU::write_PPUDATA(uint8_t data)
{
    PPUDATA = data;
    std::cout << "writing data to PPU:" << std::hex << (unsigned)data << ", addr: " << (unsigned)PPUADDR << std::endl;
    writeToVRAM(PPUADDR, PPUDATA);
    PPUADDR += vramAddressIncrement();
};

uint8_t PPU::vramAddressIncrement()
{
    uint8_t val = Controller & EControlRegister::VRAM_ADD_INCREMENT;
    if (Controller & EControlRegister::VRAM_ADD_INCREMENT == 1)
    {
        return 32;
    }
    else
    {
        return 1;
    }
};

void PPU::write_scroll(uint8_t data) { Scroll = data; }
void PPU::write_OAMData(uint8_t data) { OAM_data = data; }
void PPU::write_OAMAddress(uint16_t data) {OAM_addr = data;}
void PPU::write_mask(uint8_t data) { Mask = data; }

void PPU::write_ctrl(uint8_t data) { 
    if(_powerup_reset) return;
    uint8_t oldRegisterVal = Controller;
    Controller = data; 
  //  std::cout << "written to CTRL: value:" << std::hex << (unsigned)Controller << std::endl;

    if
    ((oldRegisterVal & (1 << EControlRegister::GENERATE_NMI) == 0) 
    && 
    (  Controller & (1 << EControlRegister::GENERATE_NMI) == 1)) 
    {
        _bus->Interrupt(HW::EInterrupt::NMI);
    }
  
};

// Read or write access to DATA register increments the PPU Address register (0x2006) value
// increment size determine from  VRAM_ADD_INcrement bit.
void PPU::write_PPUADDR(uint8_t data)
{
    if (hiByte)
    {
        uint16_t addr = data << 8;
        PPUADDR = addr;
        std::cout << "PPUADDR: " << std::hex << (unsigned)PPUADDR << ", added hibyte 0x" <<(unsigned)data << "XX" <<std::endl;
        hiByte = false;
    }
    else
    {
        PPUADDR = (PPUADDR | (data & 0xFF));
         std::cout << "PPUADDR: " << std::hex << (unsigned)PPUADDR << ", added LOW byte 0xXX"<<  (data & 0xFF) <<std::endl;
        hiByte = true;
    }

    std::cout << "PPUADDR: " << std::hex << (unsigned)PPUADDR << std::endl;
};

void PPU::write_PPU(uint16_t addr, uint8_t data)
{
    std::cout << "writing to ppu" << std::endl;
    switch (addr)
    {
       
    case EPPU_RegisterAddress::Data: std::cout << "PPU data write" << std::endl;
        write_PPUDATA(data);
        break;
    case EPPU_RegisterAddress::Controller:  std::cout << "PPU Controller write" << std::endl;
        write_ctrl(data);
        break;
    case EPPU_RegisterAddress::Mask:
        write_mask(data);
        break;
    case EPPU_RegisterAddress::Scroll:
        write_scroll(data);
        break;
    case EPPU_RegisterAddress::OAM_Data:
        write_OAMData(data);
        break;
    case EPPU_RegisterAddress::Address: std::cout << "PPU ADDR write" << std::endl;
        write_PPUADDR(data);
        break;
    case EPPU_RegisterAddress::OAM_Address: write_OAMAddress(data);
    }
}


//NAMETABLES == VRAM, 0x2000 - 0x3F00
uint16_t PPU::vram_mirror_addr(uint16_t original_addr) {


    // Mirror $3000-3EFF to $2000-2EFF. 
    // 3000 value maskataan 2000 yhden bitin ero,
    // mask 0b0010_1111_1111_1111
    uint16_t nameTableMirroredAddr =  original_addr & 0x2FFF;

    // Datastructure index downshift, VRAM starts at 0x2000, offset to 0 index
    //Mapped between 2000-2EFF
    uint16_t vram_vector_addr = nameTableMirroredAddr - 0x2000;

    //nametable: one of four. 400 value disparity
    /*
        |0|1|
        |2|3|
    */
   
    /* HUOM, VRAMIA 2KiB = 2048 bits == 0x800 !!*/
    /* Täytyy mahduttaa siten, että Vram_access addr on MAX 0-$800 */
    uint16_t nameTableIndex = vram_vector_addr / 0x400;
    
    //vram_vector_addr on mapattu alkamaan 0:sta. addr mahduttava value array[0...0x800]
    switch(mirroringMode) {
        case Mirroring::HORIZONTAL:{


            /* [0...400 <- | - 400 ... 800] == -0x400 1:stä indeksiin 0
               [800..2C00  | 2C00 ...3000 ] == indeks 2 pitää asetata 1 paikalle joten -400
                // index 3 pitää asettaa 1:sen paikalle joten -4002x == -0x800

            */
            switch(nameTableIndex) {
                case NS_PPU::NametableMap::TOP_RIGHT:   vram_vector_addr -= 0x400; break;
                case NS_PPU::NametableMap::BOT_RIGHT:   vram_vector_addr -= 0x800; break;
                case NS_PPU::NametableMap::BOT_LEFT:    vram_vector_addr -=0x400; break;
                default:   break;
            }

        }
        case Mirroring::VERTICAL:{
            
            switch(nameTableIndex) {
                case NS_PPU::NametableMap::BOT_LEFT:   vram_vector_addr -= 0x800; break;
                case NS_PPU::NametableMap::BOT_RIGHT:  vram_vector_addr -= 0x800; break;
                default:   break;
            }

        }

        default: break;
    }
    return vram_vector_addr;
}


uint8_t PPU::read_PPU_VRAM_Bypass(uint16_t addr) {


}

uint8_t PPU::read_PPU(uint16_t addr)
{
    uint8_t readData = 0;

    //CHR-Memory
    if(addr >= 0 && addr <= 0x1FFF) {
        return _cartridge->readByte_CHR(addr);
    }

    //NAMETABLE == VRAM AREA
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        //vram mirrror region 3000-3eff to 2000-2eff TODO:!!
        switch (addr)
        {
        case EPPU_RegisterAddress::Status: std::cout << "REAd ppu status" << std::endl;
            return read_PPUSTATUS();
            break;
        case EPPU_RegisterAddress::Data:
            return read_PPUDATA();
            break;
        case EPPU_RegisterAddress::OAM_Data:
            return read_OAMData();
            break;
        }

        //Memory map starts from 0x2000, memory size is 0x2000. Offset to 0 because 
        //implemented as vector 0 based.
    
        uint8_t data = readVRAM(PPUADDR);
        //    std::cout << "reading _vram at addr: " << std::hex << (unsigned)addr <<", data: " << (unsigned)data<< std::endl;
        return data;

    }
    return readData;
}

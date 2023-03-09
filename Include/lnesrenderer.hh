
#ifndef LR_HH
#define LR_HH


#include <stdint.h>
class PPU;
class SDL_Texture;
class SDL_Window;
class SDL_Renderer;

class LNES_Renderer {


const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;


public:
    PPU* ppu;
    uint8_t* framebuffer;
    void draw();
    void draw32x30(PPU* ppu, uint8_t* pixelDataBuffer);
    void initPicture();
    void drawTick(SDL_Texture* tex);
    void getInput() ;

    SDL_Window* window; 
    SDL_Renderer* renderer; 
    SDL_Texture* texture;
};

#endif
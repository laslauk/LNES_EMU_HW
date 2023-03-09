#include "lnesrenderer.hh"
#include <iostream>
#include "LNES_utilities.hh"
#include <vector>
#include "PPU.hh"
#include "SDL2/SDL.h"


void LNES_Renderer::draw(){

   // std::cout << "CALL CALLED Lnes renderer!!" << std::endl;
   // draw32x30(ppu, framebuffer);

SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 256,240);
//SDL_Rect rect2 = {0,0,16,16}; //x,y,w,h

    uint16_t ppuaddr = 0x2209;
     uint8_t tileID = ppu->readVRAM(ppuaddr);
    
     std::cout << "draw call " << std::hex << (unsigned)tileID<< std::endl;
       
       
       std::vector<uint8_t> vramMemory = ppu->dump_vram();
     //  for(auto entry: vramMemory) {
      //  std::cout << std::hex << (unsigned)entry << std::endl;
     //  }

  

    int _WIDTH =256;
    int _HEIGHT = 240;
    int gx = 0;
    int gy = 0;
    int emptyOffset = 0;

   
    
   // uint16_t  nameTableAddr =  ppu->getBGNametableAddr();
  //  std::vector<std::vector<uint8_t >> BGnametableBytes =  ppu->readBackGroundNametableBytes(0,nameTableAddr );
  //  std::vector< std::vector<std::vector<std::vector<Color>>>> tilesAtVRAM = ppu->mapBGNametableBytesToPatterntableTiles( BGnametableBytes );





SDL_UpdateTexture(tex,NULL, framebuffer,256*3);
drawTick(tex);
    

}



void LNES_Renderer::initPicture() {

        ppu->getAllTiles();
        SDL_Init(SDL_INIT_VIDEO);

//struct that handles all rendering, tied to window. Can only render withing the window

        /*
        Renderer functions:
            -SetRenderDrawColor(renderer,rgba); sets  color you clear screen to
            -renderClear(renderer); clears rendering target with color set above

            - SDl renderCopy() !! Important, UIsed for rendering SDL texture
            PARAMS:
                -renderer
                -texture (texture we want to render)

                -srcrect (part of texture to render) null if entire
                -dstrect SDL_Rect* (where to render in window)
            -SDL_RenderPresent(renderer); OTHER Render functions  DRAWS t oa hidden target.
                this function takes all that and draws it to the window tied to a renderer (shows to user)

            -SDL_Textures / SDL_Surface
            Renderer renders SDL_Texture, which stores pixel information of one element. (its new version of sdl surface)
            SDL_Surface is just a tsturct of pixel info, while sdl_texture is efficient represtation.
            -Surface == CPU 
            -Texture == GPU

            SDL_Rect:
                - simple rect struct x,y,w,h.

        */
       
        //holds all info about window itself
      //  SDL_Window* window; 
      //  SDL_Renderer* renderer; 
     //   SDL_Texture* texture;

        SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
        window = SDL_CreateWindow("hohoo",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 4*SCREEN_WIDTH, 4*SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
      //  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 128,256);

        //SDL_SetWindowSize(window, 256,240);
        SDL_SetWindowResizable(window, SDL_TRUE);


        


}

void LNES_Renderer::getInput() {
    SDL_Event event;


    while(SDL_PollEvent(&event)){

        switch(event.type) {
            case SDL_QUIT:
            exit(0);
            break;
            default: break;
        }
    }
}


void LNES_Renderer::drawTick(SDL_Texture* tex) {
 
            getInput();
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);
            //SDL_RenderCopy(renderer,tex,NULL,NULL);
            SDL_RenderCopy(renderer,tex,NULL, NULL);
            SDL_RenderPresent(renderer);
}

void LNES_Renderer::draw32x30(PPU* ppu, uint8_t* pixelDataBuffer) {
    //get tiles
   // std::cout << "DRAW32x30 START" << std::endl;
    int _WIDTH =256;
    int _HEIGHT = 240;
    int gx = 0;
    int gy = 0;
    int emptyOffset = 0;
    
    uint16_t  nameTableAddr =  ppu->getBGNametableAddr();
   // std::cout << "nameTableAddrCOMPLETE: " << std::hex << (unsigned)nameTableAddr << std::endl;
    std::vector<std::vector<uint8_t >> BGnametableBytes =  ppu->readBackGroundNametableBytes(0,nameTableAddr );
  // std::cout << "BGnametableBytes:" << std::dec << BGnametableBytes.size() << std::endl;
    std::vector< std::vector<std::vector<std::vector<Color>>>> tilesAtVRAM = ppu->mapBGNametableBytesToPatterntableTiles( BGnametableBytes );
 //   std::cout << "Tiles at VRAM COMPLETE" << std::endl;
    //Tiles at vram 
            // 0   1*8*3  2*3*8
    /* tileRow [ [8x8] [8x8] [8x8]  ]
    /* 
        
    */

    int rowNum = 0;
    int baseIndex = 0;
    for (auto tileRow : tilesAtVRAM)
    {

        int tileXoffset = 0;
        for (auto tile : tileRow)
        {
            int gx = 0; //+8x3 tilestä
            int gy = 0;
            int tileBaseIndex = 0;
            int emptyOffset = 0;
            for (int y = 0; y < 8; y++)
            {
                //jokanen y lisää 3*

                for (int x = 0; x < 8; x++)
                {
                    //(((_WIDTH * 3 * rowNum) + (gx * 3)) + Tilexoffset)+(x*3);

                    baseIndex = (_WIDTH * gy * 3) + (gx*3) + (x*3) + emptyOffset ;
                    Color color = tile[y][x];
                    if (baseIndex + 2 < _WIDTH * _HEIGHT * 3)
                    {
                    //    std::cout << "Baseindex: " << (unsigned)baseIndex << std::endl;
                        pixelDataBuffer[baseIndex] = color.R;
                        pixelDataBuffer[baseIndex + 1] = color.G;
                        pixelDataBuffer[baseIndex + 2] = color.B;
                      //  std::cout << "pixel color RGB:" << color.R << ", " << color.G << ", " << color.B << std::endl;
                       // std::cout << "ENd set pixel" << std::endl;
                    }
                    gx +=1;
                }
              
                //jokasen y:n lopussa 256-(gx+8*3)+gx
            }
              gy += 8;
               
                emptyOffset += ((((256) - (gx))+gx)*3);
            //jokasen tilen lopussa lisää 8x3 alkuun
         //  std::cout << "GX, GY: " << std::dec << (unsigned)gx <<", " << (unsigned)gy << std::endl;
        }
        /* Frame */
        /* RGB = 3 bytes, pitch == pixel width in bytes, so width*3 */
        // X to index = + x*3 (3 bytes for each data entry)
        // one y adds WIDTH amount of index * 3,
    }

        //transform tiles to SDL
 //std::cout << ">>>DRAW32x30 END" << std::endl;
    //draw each 8x8 tile
    //return SDL format 3x
}
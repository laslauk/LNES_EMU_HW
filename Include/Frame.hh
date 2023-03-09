#ifndef FRAME_HH
#define FRAME_HH

#include <stdint.h>
#include "LNES_utilities.hh"
#include <cstring>

class Frame {
    public:
    Frame() {
        memset(pixelData, 0, _WIDTH*_HEIGHT*3);
    }
    uint8_t* data() {return pixelData;}
    Frame(int width, int height);

    
    void setPixel(Color color, int x, int y) {
        std::cout << "start set pixel" << std::endl;
        /* Frame */
        /* RGB = 3 bytes, pitch == pixel width in bytes, so width*3 */
                // X to index = + x*3 (3 bytes for each data entry)
                // one y adds WIDTH amount of index * 3,
            int baseIndex = (_WIDTH * 3 * y) + (x * 3);

            if(baseIndex+2 < _WIDTH*_HEIGHT*3) {
                pixelData[baseIndex]   = color.R;
                pixelData[baseIndex+1] = color.G;
                pixelData[baseIndex+2] = color.B;
                    std::cout << "ENd set pixel" << std::endl;
                }
    }   

    private:

    static const int _WIDTH = 256;
    static const int _HEIGHT = 240;
    uint8_t pixelData[_WIDTH*_HEIGHT*3];

};

#endif
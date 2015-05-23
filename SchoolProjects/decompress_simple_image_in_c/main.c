#include "mylib.h"
#include "rle.h"

// Robert Thackston
void drawUncompressedImage(const u16* data);

int main(void)
{
    REG_DISPCNT = 1027;
  
    // Call this function with rle2 and rle1 these are HARDER test cases for you to ensure it works.  
    drawUncompressedImage(rle1);
    
    while(1);
}

/*
    Draws an RLE encoded image uncompressed onto the videoBuffer.
    Remember you may only call setPixel when the run length is 2 or less
    You must use DMA if the run length is 3 or greater! (Heavy penalty if you don't follow these rules)
*/
void drawUncompressedImage(const u16* data)
{
    vu32 x = 0;
    vu32 y = 0;
    vu32 i = 0;
    vu32 pixelCount = 0;
    while (pixelCount < (240 * 160)) {
        if (data[i] == 2) {
            if (x == 239) {
                setPixel(x, y, data[(i + 1)]);
                setPixel(0, y + 1, data[(i + 1)]);
                y++;
                x = 1;
            } else if (x == 238) {
                setPixel(x, y, data[(i + 1)]);
                setPixel(x + 1, y, data[(i + 1)]);
                x = 0;
                y++;
            } else {
                setPixel(x, y, data[(i + 1)]);
                setPixel(x + 1, y, data[(i + 1)]);
                x += 2;
            }
            pixelCount += 2;
        } else if (data[i] == 1) {
            if (x == 239) {
                setPixel(x, y, data[(i + 1)]);
                x = 0;
                y++;
            } else {
                setPixel(x, y, data[(i + 1)]);
                x++;
            }
            pixelCount += 1;
        } else {
            REG_DMA3SAD = (vu32)&(data[(i + 1)]);
            REG_DMA3DAD = (int)(videoBuffer + (240 * y) + x);
            REG_DMA3CNT = DMA_ON | (data[i]) | DMA_SOURCE_FIXED;
            x += data[i];
            pixelCount += data[i];
            while (x > 240) {
                x -= 240;
                y++;
            } 
        }
        i += 2;
    }
}

#include "mylib.h"

u16* videoBuffer = (u16*) 0x6000000;

void setPixel(int x, int y, u16 color)
{
    videoBuffer[y * 240 + x] = color;
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
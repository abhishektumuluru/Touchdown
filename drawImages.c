#include "mylib.h"
unsigned short *videoBuffer = (unsigned short  *)0x6000000;
typedef unsigned int u32;
extern unsigned int deltaTime;

#define GBA_WIDTH (240)
#define GBA_HEIGHT (160)

void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for (int h = 0; h < height; h++) {
		REG_DMA3SAD = (u32)&image[OFFSET(h, 0, width)];
		REG_DMA3DAD = (u32)(videoBuffer + OFFSET(h + r, c, GBA_WIDTH));
		REG_DMA3CNT = width | DMA_ON;
	}
}
void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void setBG(volatile u16 bg)
{
	REG_DMA3SAD = (u32)&bg;
	REG_DMA3DAD = (u32)videoBuffer;
	REG_DMA3CNT= 38400 |  DMA_ON | DMA_SOURCE_FIXED;
}


void placePlayer(Player p, const u16* image)
{
	drawImage3(p.x, p.y, p.width, p.height, image);
}


void drawRectangle(int row, int col, int height, int width, volatile unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, GBA_WIDTH)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}


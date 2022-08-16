#pragma once
#include "typedef_structs.h"

class ImageARGB
{
public:
	static const Uint32 ARGB_MAGENTA = 0x00ff00ff;
	static const Uint32 ARGB_RED = 0x00ff0000;
	static const Uint32 ARGB_BLUE = 0x000000ff;
	static const Uint32 ARGB_GREEN = 0x0000ff00;
	static const Uint32 ARGB_CYAN = 0x0000ffff;
	static const Uint32 ARGB_YELLOW = 0x00ffff00;
	static const Uint32 ARGB_WHITE = 0x00ffffff;
	static const Uint32 ARGB_BLACK = 0;
	static const int ScreenPixelSize = 4;
	static const int QuadSize = 8;

	Uint32 *pixels = NULL;
	int sizeX, sizeY;

	ImageARGB(BYTE* data, int dimX, int dimY);
	~ImageARGB();

	void SetPixel(int x, int y, Uint32 color);
	Uint32 GetPixel(int x, int y);
	void BresenhamLine(int x_1, int y_1, int x_2, int y_2, Uint32 color);
	void Clear();
};

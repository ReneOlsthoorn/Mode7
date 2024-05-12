#pragma once

#include <Windows.h>
#include <stb_image.h>
#include "typedef_structs.h"

class Graphics
{
public:
	static const int ScreenDIMx;
	static const int ScreenDIMy;
	static const int ScreenHalfDIMx;
	static const int ScreenHalfDIMy;
	static const int ScreenPixelSize;
	static const int ScreenLineSize;
	static const int ScreenSize;
	static const int QuadSize;

	BYTE* pixels = NULL;
	BYTE* racetrack = NULL;
	int racetrackDIMx, racetrackDIMy;

	Graphics();
	~Graphics();

	void FlipRedAndGreenInImage(BYTE* image, int SizeX, int SizeY);
	void LoadImages();
	static void SetPixelARGB(BYTE* buffer, int x, int y, int sizeX, int sizeY, Uint32 value);
};

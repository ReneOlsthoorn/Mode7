#include "Graphics.h"

const int Graphics::ScreenDIMx = 1024;
const int Graphics::ScreenDIMy = 512;
const int Graphics::ScreenHalfDIMx = ScreenDIMx / 2;
const int Graphics::ScreenHalfDIMy = ScreenDIMy / 2;
const int Graphics::ScreenPixelSize = 4;
const int Graphics::ScreenLineSize = ScreenDIMx * ScreenPixelSize;
const int Graphics::ScreenSize = ScreenDIMx * ScreenDIMy * ScreenPixelSize;
const int Graphics::QuadSize = 8;

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
	if (racetrack != NULL) {
		stbi_image_free(racetrack);
	}

	if (achtergrond != NULL) {
		stbi_image_free(achtergrond);
	}
}

void Graphics::FlipRedAndGreenInImage(BYTE* image, int SizeX, int SizeY)
{
	for (int i = 0; i < SizeX * SizeY; i++) {
		int tmp = image[(i*ScreenPixelSize) + 2];
		image[(i*ScreenPixelSize) + 2] = image[(i*ScreenPixelSize)];
		image[(i*ScreenPixelSize)] = tmp;
	}
}

void Graphics::LoadImages()
{
	int picx, picy, picn;
	
	racetrack = stbi_load("c:\\assembly\\Mode7\\Graphics\\do_not_use.png", &picx, &picy, &picn, 4); // 1024 * 1024
	FlipRedAndGreenInImage(racetrack, picx, picy);
	racetrackDIMx = picx;
	racetrackDIMy = picy;

	//path to executable...
	HMODULE hModule = GetModuleHandle(NULL);
	char path[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);
}

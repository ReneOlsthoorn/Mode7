#include "Mode7.h"

extern Graphics* graphics;

Mode7::Mode7()
{
}

Mode7::~Mode7()
{
}

void Mode7::Init() {
}

void Mode7::Update() {
	ImageARGB racetrackARGB(graphics->racetrack, graphics->racetrackDIMx, graphics->racetrackDIMy);
	ImageARGB screenARGB(graphics->pixels, Graphics::ScreenDIMx, Graphics::ScreenDIMy);

	float space_y = 100.0f;
	float scale_y = 200.0f;
	int horizon = 15;

	for (int y = 0; y < Graphics::ScreenDIMy; y++) {
		float distance = space_y * scale_y / (y + horizon);

		float fStartX = fWorldX + (cosf(fWorldAngle + fFoVHalf) * distance);
		float fStartY = fWorldY - (sinf(fWorldAngle + fFoVHalf) * distance);
		float fEndX = fWorldX + (cosf(fWorldAngle - fFoVHalf) * distance);
		float fEndY = fWorldY - (sinf(fWorldAngle - fFoVHalf) * distance);

		for (int x = 0; x < Graphics::ScreenDIMx; x++) {
			float fSampleWidth = (float)x / (float)Graphics::ScreenDIMx;
			float fSampleX = fStartX + ((fEndX - fStartX) * fSampleWidth);
			float fSampleY = fStartY + ((fEndY - fStartY) * fSampleWidth);

			Uint32 pixelColor = racetrackARGB.GetPixel(fSampleX, fSampleY);
			screenARGB.SetPixel(x, y, pixelColor);
		}
	}
}

void Mode7::GoUp() {
	fWorldX += 5.0f * cosf(fWorldAngle);
	fWorldY -= 5.0f * sinf(fWorldAngle);

	if (fWorldX >= nMapSize) { fWorldX = (float)nMapSize; }
	if (fWorldX < 0.0f) { fWorldX = 0.0f; }
	if (fWorldY >= nMapSize) { fWorldY = (float)nMapSize; }
	if (fWorldY < 0.0f) { fWorldY = 0.0f; }
}

void Mode7::GoDown() {
	fWorldX -= 5.0f * cosf(fWorldAngle);
	fWorldY += 5.0f * sinf(fWorldAngle);

	if (fWorldX >= nMapSize) { fWorldX = (float)nMapSize; }
	if (fWorldX < 0.0f) { fWorldX = 0.0f; }
	if (fWorldY >= nMapSize) { fWorldY = (float)nMapSize; }
	if (fWorldY < 0.0f) { fWorldY = 0.0f; }
}

void Mode7::TurnLeft() {
	fWorldAngle += 0.05;
}

void Mode7::TurnRight() {
	fWorldAngle -= 0.05;
}


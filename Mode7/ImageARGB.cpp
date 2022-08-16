#include "ImageARGB.h"
#include <stdlib.h>
#include <Windows.h>

ImageARGB::ImageARGB(BYTE* data, int sizeX, int sizeY)
{
	this->pixels = (Uint32 *)data;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
}

ImageARGB::~ImageARGB()
{
}

void ImageARGB::Clear() {
	BYTE *data = (BYTE *)pixels;
	memset(data, 0, (sizeX * sizeY * ImageARGB::ScreenPixelSize) / ImageARGB::QuadSize);
}

Uint32 ImageARGB::GetPixel(int x, int y) {
	if (x >= sizeX || x < 0) { return 0; }
	if (y >= sizeY || y < 0) { return 0; }
	int theIndex = (y * sizeX) + x;
	/*
	if ((theIndex < 0) || (theIndex >= sizeX * sizeY)) {
		return 0;
	}
	*/
	return pixels[theIndex];
}

void ImageARGB::SetPixel(int x, int y, Uint32 color) {
	if (x >= sizeX || x < 0) { return; }
	if (y >= sizeY || y < 0) { return; }
	int theIndex = (y * sizeX) + x;
	if ((theIndex < 0) || (theIndex >= sizeX * sizeY)) {
		return;
	}
	pixels[theIndex] = color;
}

void ImageARGB::BresenhamLine(int x_1, int y_1, int x_2, int y_2, Uint32 color)
{
	/*
	x_1 = MAX(x_1, 0);
	y_1 = MAX(y_1, 0);
	x_2 = MAX(x_2, 0);
	y_2 = MAX(y_2, 0);
	x_1 = MIN(x_1, sizeX);
	y_1 = MIN(y_1, sizeY);
	x_2 = MIN(x_2, sizeX);
	y_2 = MIN(y_2, sizeY);
	*/

	int x1 = x_1;
	int y1 = y_1;

	int x2 = x_2;
	int y2 = y_2;

	if (x_1 > x_2) {
		x1 = x_2;
		y1 = y_2;

		x2 = x_1;
		y2 = y_1;
	}

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int inc_dec = ((y2 >= y1) ? 1 : -1);

	if (dx > dy) {
		int two_dy = (2 * dy);
		int two_dy_dx = (2 * (dy - dx));
		int p = ((2 * dy) - dx);

		int x = x1;
		int y = y1;

		SetPixel(x, y, color);

		while (x < x2) {
			x++;
			if (p < 0) {
				p += two_dy;
			}
			else {
				y += inc_dec;
				p += two_dy_dx;
			}
			SetPixel(x, y, color);
		}
	}
	else {
		int two_dx = (2 * dx);
		int two_dx_dy = (2 * (dx - dy));
		int p = ((2 * dx) - dy);

		int x = x1;
		int y = y1;

		SetPixel(x, y, color);

		while (y != y2) {
			y += inc_dec;

			if (p < 0) {
				p += two_dx;
			}
			else {
				x++;
				p += two_dx_dy;
			}
			SetPixel(x, y, color);
		}
	}
}

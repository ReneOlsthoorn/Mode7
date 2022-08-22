#pragma once
#include <Windows.h>
#include <math.h>
#include "typedef_structs.h"
#include "Graphics.h"
#include "ImageARGB.h"
#include "chipmunk/chipmunk.h"


class Mode7
{
public:
	static const int nMapSize = 1024;

	float fWorldX = 132.8f;
	float fWorldY = 551.5f;
	float fWorldAngle = 1.57f; //(3.141592f / 2.0f);  // pi/2

	float fNear = (float)nMapSize * 0.025f;
	float fFar = (float)nMapSize * 0.60f;

	float fFoVHalf = 3.141592f / 4.0f; //90 graden gedeeld door twee, omdat we twee keer een half doen.

	cpSpace* space;
	cpShape* ground;
	cpBody* ballBody;
	cpShape* ballShape;

	Mode7();
	~Mode7();

	void Init();
	void Update();
	void TurnLeft();
	void TurnRight();
	void GoUp();
	void GoDown();
	void DrawBall(int x, int y);
	void SpacePressed();
};

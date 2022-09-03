#include "Mode7.h"
#include "ImageARGB.h"
#include "chipmunk/chipmunk.h"

extern Graphics* graphics;
static int frameCounter = 0;

Mode7::Mode7()
{
}

Mode7::~Mode7()
{
	// Clean up our objects and exit!
	cpBodyFree(driverBody);
	//cpBodyFree(dampedLeftBody);
	//cpBodyFree(dampedRightBody);
	cpSpaceFree(space);
}

void Mode7::Init() {
	// cpVect is a 2D vector and cpv() is a shortcut for initializing them.
	cpVect gravity = cpv(0, -500);

	// Create an empty space.
	space = cpSpaceNew();
	//cpSpaceSetDamping(space, cpFloat(0.5));
	//cpSpaceSetGravity(space, gravity);

	cpFloat radius = 5;
	cpFloat mass = 1;

	// The moment of inertia is like mass for rotation
	// Use the cpMomentFor*() functions to help you approximate it.
	cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

	// The cpSpaceAdd*() functions return the thing that you are adding.
	// It's convenient to create and add an object in one line.
	driverBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));

	//dampedLeftBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	//dampedRightBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));

	cpBodySetPosition(driverBody, cpv(fWorldX, fWorldY));
	cpBodySetAngle(driverBody, cpFloat(fWorldAngle));

	/*
	float vectorLength = -30.0f;
	float leftDampedfWorldX = fWorldX - (cosf(-fWorldAngle) * vectorLength);
	float leftDampedfWorldY = fWorldY + (sinf(-fWorldAngle) * vectorLength);

	cpBodySetPosition(dampedLeftBody, cpv(leftDampedfWorldX, leftDampedfWorldY));
	cpBodySetAngle(dampedLeftBody, cpFloat(fWorldAngle));

	vectorLength = 30.0f;
	float rightDampedfWorldX = fWorldX - (cosf(-fWorldAngle) * vectorLength);
	float rightDampedfWorldY = fWorldY + (sinf(-fWorldAngle) * vectorLength);

	cpBodySetPosition(dampedRightBody, cpv(rightDampedfWorldX, rightDampedfWorldY));
	cpBodySetAngle(dampedRightBody, cpFloat(fWorldAngle));
	*/

	//cpSpaceAddConstraint(space, cpDampedSpringNew(driverBody, dampedLeftBody, cpv(15, 0), cpv(-15, 0), 20.0f, 5.0f, 0.3f));
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

	// Now that it's all set up, we simulate all the objects in the space by
	// stepping forward through time in small increments called steps.
	// It is *highly* recommended to use a fixed size time step.
	const cpFloat timeStep = 1.0 / 60.0;

	cpVect pos = cpBodyGetPosition(driverBody);
	cpVect vel = cpBodyGetVelocity(driverBody);

	float vectorLength = 30.0f;
	float leftDampedfWorldX = pos.x - (cosf(fWorldAngle + 0.5 * 3.141592) * vectorLength);
	float leftDampedfWorldY = pos.y + (sinf(fWorldAngle + 0.5 * 3.141592) * vectorLength);
	cpVect leftPoint = cpv(leftDampedfWorldX, leftDampedfWorldY);

	vectorLength = 30.0f;
	float rightDampedfWorldX = pos.x - (cosf(fWorldAngle - 0.5 * 3.141592) * vectorLength);
	float rightDampedfWorldY = pos.y + (sinf(fWorldAngle - 0.5 * 3.141592) * vectorLength);
	cpVect rightPoint = cpv(rightDampedfWorldX, rightDampedfWorldY);

	//cpVect vAngle = cpvforangle(fWorldAngle);
	//cpVect subbed = cpvsub(vel, vAngle);

	cpVect toPoint = cpvsub(leftPoint, pos);
	bool goingLeft = cpvdot(toPoint, vel) > 0.0;

	toPoint = cpvsub(rightPoint, pos);
	bool goingRight = cpvdot(toPoint, vel) > 0.0;

	//frameCounter++;
	if (goingLeft) {
		cpBodyApplyForceAtWorldPoint(driverBody, cpvsub(pos, leftPoint), leftPoint);
	}
	if (goingRight) {
		cpBodyApplyForceAtWorldPoint(driverBody, cpvsub(pos, rightPoint), rightPoint);
	}

	cpSpaceStep(space, timeStep);

	fWorldX = pos.x;
	fWorldY = 1024.0 - pos.y;
	fWorldAngle = (float)cpBodyGetAngle(driverBody);


	int ballX = fWorldX;
	int ballY = fWorldY / 2;
	DrawBall(ballX, ballY, ImageARGB::ARGB_YELLOW);

	float dampedLeftfWorldX = leftDampedfWorldX;
	float dampedLeftfWorldY = (1024.0 - leftDampedfWorldY) / 2;

	DrawBall(dampedLeftfWorldX, dampedLeftfWorldY, goingLeft ? ImageARGB::ARGB_CYAN : ImageARGB::ARGB_RED);

	float dampedRightfWorldX = rightDampedfWorldX;
	float dampedRightfWorldY = (1024.0 - rightDampedfWorldY) / 2;

	DrawBall(dampedRightfWorldX, dampedRightfWorldY, goingRight ? ImageARGB::ARGB_CYAN : ImageARGB::ARGB_BLUE);
}

void Mode7::SpacePressed() {
	//cpBodySetForce(ballBody, cpv(0, 500));
}

void Mode7::DrawBall(int x, int y, Uint32 color) {
	const int width = 10;

	if (x < width || y < width) { return; }
	if ((x + width + 1 >= Graphics::ScreenDIMx) || (y + width + 1 >= Graphics::ScreenDIMy)) { return; };

	ImageARGB screenARGB(graphics->pixels, Graphics::ScreenDIMx, Graphics::ScreenDIMy);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			screenARGB.SetPixel(x+i, y+j, color);
		}
	}
}

void Mode7::GoUp() {
	/*
	fWorldX += 5.0f * cosf(fWorldAngle);
	fWorldY -= 5.0f * sinf(fWorldAngle);

	if (fWorldX >= nMapSize) { fWorldX = (float)nMapSize; }
	if (fWorldX < 0.0f) { fWorldX = 0.0f; }
	if (fWorldY >= nMapSize) { fWorldY = (float)nMapSize; }
	if (fWorldY < 0.0f) { fWorldY = 0.0f; }
	*/

	/*  De oorsponkelijk formule voor het draaien van een punt om te oorsprong is:
		float rotated_x = (relative_x * cosf(angle)) - (relative_y * sinf(angle));
		float rotated_y = (relative_x * sinf(angle)) + (relative_y * cosf(angle));
		Echter, omdat de origin linksboven ligt, moeten we sinf negatief maken, dus dat wordt: */

	cpVect force = cpv(200, 0);

	//double rotatedX = (force.x * cosf(angle)) + (force.y * sinf(angle));
	//double rotatedY = -(force.x * sinf(angle)) + (force.y * cosf(angle));

	//cpVect rotated = cpv(rotatedX, rotatedY);
	cpVect rotated = cpvrotate(force, cpvforangle(fWorldAngle));
	cpBodySetForce(driverBody, rotated);
	//cpBodySetForce(dampedLeftBody, rotated);
}

void Mode7::GoDown() {
	/*
	fWorldX -= 5.0f * cosf(fWorldAngle);
	fWorldY += 5.0f * sinf(fWorldAngle);

	if (fWorldX >= nMapSize) { fWorldX = (float)nMapSize; }
	if (fWorldX < 0.0f) { fWorldX = 0.0f; }
	if (fWorldY >= nMapSize) { fWorldY = (float)nMapSize; }
	if (fWorldY < 0.0f) { fWorldY = 0.0f; }
	*/
}

void Mode7::TurnLeft() {
	fWorldAngle += 0.05;
	cpBodySetAngle(driverBody, cpFloat(fWorldAngle));
	//cpBodySetAngle(dampedLeftBody, cpFloat(fWorldAngle));
}

void Mode7::TurnRight() {
	fWorldAngle -= 0.05;
	cpBodySetAngle(driverBody, cpFloat(fWorldAngle));
	//cpBodySetAngle(dampedLeftBody, cpFloat(fWorldAngle));
}


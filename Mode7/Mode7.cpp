#include "Mode7.h"
#include "ImageARGB.h"
#include "chipmunk/chipmunk.h"

extern Graphics* graphics;

Mode7::Mode7()
{
}

Mode7::~Mode7()
{
	// Clean up our objects and exit!
	cpShapeFree(ballShape);
	cpBodyFree(ballBody);
	cpShapeFree(ground);
	cpSpaceFree(space);
}

void Mode7::Init() {
	// cpVect is a 2D vector and cpv() is a shortcut for initializing them.
	cpVect gravity = cpv(0, -500);

	// Create an empty space.
	space = cpSpaceNew();
	cpSpaceSetDamping(space, cpFloat(0.1));
	//cpSpaceSetGravity(space, gravity);

	// Add a static line segment shape for the ground.
	// We'll make it slightly tilted so the ball will roll off.
	// We attach it to a static body to tell Chipmunk it shouldn't be movable.
	//ground = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(-20, 5), cpv(800, -5), 5);
	//cpShapeSetFriction(ground, 1);
	//cpShapeSetElasticity(ground, 1);
	//cpSpaceAddShape(space, ground);

	// Now let's make a ball that falls onto the line and rolls off.
	// First we need to make a cpBody to hold the physical properties of the object.
	// These include the mass, position, velocity, angle, etc. of the object.
	// Then we attach collision shapes to the cpBody to give it a size and shape.

	cpFloat radius = 5;
	cpFloat mass = 1;

	// The moment of inertia is like mass for rotation
	// Use the cpMomentFor*() functions to help you approximate it.
	cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

	// The cpSpaceAdd*() functions return the thing that you are adding.
	// It's convenient to create and add an object in one line.
	ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpBodySetPosition(ballBody, cpv(fWorldX, fWorldY));
	cpBodySetAngle(ballBody, cpFloat(fWorldAngle));

	// Now we create the collision shape for the ball.
	// You can create multiple collision shapes that point to the same body.
	// They will all be attached to the body and move around to follow it.
	ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
	cpShapeSetFriction(ballShape, 0.7);
	cpShapeSetElasticity(ballShape, 0.9);
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
	cpFloat timeStep = 1.0 / 60.0;

	cpVect pos = cpBodyGetPosition(ballBody);
	cpVect vel = cpBodyGetVelocity(ballBody);
	//printf("Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n", time, pos.x, pos.y, vel.x, vel.y);
	cpSpaceStep(space, timeStep);

	fWorldX = pos.x;
	fWorldY = 1024 - pos.y;
	fWorldAngle = (float)cpBodyGetAngle(ballBody);

	int ballX = fWorldX;
	int ballY = fWorldY / 2;
	DrawBall(ballX, ballY);
}

void Mode7::SpacePressed() {
	//cpBodySetForce(ballBody, cpv(0, 500));
}

void Mode7::DrawBall(int x, int y) {
	const int width = 10;

	if (x < width || y < width) { return; }
	if ((x + width + 1 >= Graphics::ScreenDIMx) || (y + width + 1 >= Graphics::ScreenDIMy)) { return; };

	ImageARGB screenARGB(graphics->pixels, Graphics::ScreenDIMx, Graphics::ScreenDIMy);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			screenARGB.SetPixel(x+i, y+j, ImageARGB::ARGB_YELLOW);
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

	cpVect force = cpv(500, 0);

	//double rotatedX = (force.x * cosf(angle)) + (force.y * sinf(angle));
	//double rotatedY = -(force.x * sinf(angle)) + (force.y * cosf(angle));

	//cpVect rotated = cpv(rotatedX, rotatedY);
	cpVect rotated = cpvrotate(force, cpvforangle(fWorldAngle));
	cpBodySetForce(ballBody, rotated);
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
	cpBodySetAngle(ballBody, cpFloat(fWorldAngle));
	//cpBodySetTorque(ballBody, cpFloat(50));
}

void Mode7::TurnRight() {
	fWorldAngle -= 0.05;
	cpBodySetAngle(ballBody, cpFloat(fWorldAngle));
	//cpBodySetTorque(ballBody, cpFloat(-50));
}


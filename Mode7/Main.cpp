#include <Windows.h>
#include <SDL.h>
#include <SDL_render.h>
#include "typedef_structs.h"
#include "Graphics.h"
#include "Mode7.h"

Graphics* graphics = NULL;
Mode7* mode7 = NULL;
volatile bool running = true;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow)
{
	HANDLE threadHandle = GetCurrentThread();
	DWORD oldThreadPrio = GetThreadPriority(threadHandle);
	bool res = SetThreadPriority(threadHandle, THREAD_PRIORITY_TIME_CRITICAL);   // Realtime priority voorkomt schokken

	/* construction */
	graphics = new Graphics();
	mode7 = new Mode7();

	/* init */
	graphics->LoadImages();
	mode7->Init();

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 0;
	}

	SDL_Window* window = SDL_CreateWindow("Mode7", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Graphics::ScreenDIMx, Graphics::ScreenDIMy, SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN */);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Graphics::ScreenDIMx, Graphics::ScreenDIMy);

	Uint64 frameCount = 0;
	BYTE* pixels = NULL;
	SDL_Event event;
	while (running)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_SPACE]) { mode7->SpacePressed(); }
		if (state[SDL_SCANCODE_LEFT]) {	mode7->TurnLeft(); }
		if (state[SDL_SCANCODE_RIGHT]) { mode7->TurnRight(); }
		if (state[SDL_SCANCODE_UP]) { mode7->GoUp(); }
		if (state[SDL_SCANCODE_DOWN]) { mode7->GoDown(); }

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						running = false;
						break;
					}
					break;
			}
		}

		int pitch = Graphics::ScreenLineSize; //the pitch is the length of one row in bytes
		SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch);
		graphics->pixels = pixels;

		mode7->Update();

		SDL_UnlockTexture(texture);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		frameCount++;
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	delete mode7;
	delete graphics;

	res = SetThreadPriority(threadHandle, oldThreadPrio);  // Priority of the thread back to the old value.
	return 0;
}

#include <SDL.h>
#include <stdio.h>

int main(int argc, char* args[])
{
    //The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	SDL_Renderer* renderer = NULL;

	SDL_Event event = { 0 };

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	//Create window
	window = SDL_CreateWindow("PiLauncher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 480, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

    SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);

    SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);

	int quit = 0;

	uint64_t now = SDL_GetPerformanceCounter();
	uint64_t last = 0;

	while (!quit)
	{
		last = now;
		now = SDL_GetPerformanceCounter();

		float deltaTime = (float)((now - last) / (float)SDL_GetPerformanceFrequency());

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = 1;
				}
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
    
    return 0;
}
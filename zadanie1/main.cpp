#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "lib/Circle.h"
#include "lib/MapLoader.h"

const int SCREEN_WIDTH = 1088;
const int SCREEN_HEIGHT = 640;

const int MAP_WIDTH = 1664;
const int MAP_HEIGHT = 1216;

int mouseX, mouseY;
int maxX, maxY;


SDL_Rect* fillRect1, *camera;

SDL_Surface* gScreenSurface = NULL;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* playerTexture = NULL;

SDL_Texture* grassTexture = NULL;

SDL_Texture* skyTexture = NULL;

SDL_Texture* wallTexture = NULL;



bool init();
void updatePlayerPosition(SDL_Rect* fillRect1, SDL_Rect* camera, float velocityOfRect);

//bool loadTextures();

void close();

float smoothingMotion(float targetSpeed, float smooth, float velocity);



int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;
		Uint8 alpha = 128;
		Uint64 currentTime = 0;
		Uint64 lastTime = 0;
		Uint64 deltaTime = 0;
		Uint64 desiredFrameTime = 17;

		camera = new SDL_Rect
		{
			SCREEN_WIDTH/2,
			SCREEN_HEIGHT/2,
			SCREEN_WIDTH,
			SCREEN_HEIGHT
		};

		float velocityOfRect = 2;
		fillRect1 = new SDL_Rect
		{
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT / 2,
			100,
			100
		};

		//Teksture for player
		playerTexture = loadTextureFromTheSurface("res/textures/player.png", gRenderer);


		//Tekstures from map loadres
		std::vector<SDL_Texture*> textures;
		grassTexture = loadTextureFromTheSurface("res/textures/grass.png", gRenderer);
		skyTexture = loadTextureFromTheSurface("res/textures/sky.png", gRenderer);
		wallTexture = loadTextureFromTheSurface("res/textures/wall.png", gRenderer);
		textures.push_back(grassTexture);
		textures.push_back(skyTexture);
		textures.push_back(wallTexture);

		//While application is running
		while (!quit)
		{
			lastTime = currentTime;
			currentTime = SDL_GetPerformanceCounter();
			deltaTime = (currentTime - lastTime) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency());

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					mouseX = e.motion.x;
					mouseY = e.motion.y;
					printf("We got a motion event.\n");
					printf("Current mouse position is: (%d, %d)\n", e.motion.x, e.motion.y);
				}

			}

			updatePlayerPosition(fillRect1, camera, velocityOfRect);
			

			//elementsOfMap = loadElementInfoFromFile("res/maps/legend.txt");
			std::vector<std::string> levelMap = loadFromFile("res/maps/map1.txt");

		
			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0x8B, 0xAC, 0xB7, 0xFF);
			SDL_RenderClear(gRenderer);

			int numberOfColumns = SCREEN_WIDTH / 64;
			int numberOfRows = SCREEN_HEIGHT / 64;

			for (int i = 0; i < MAP_HEIGHT / 64; i++) {
				std::string line = levelMap.at(i);
				for (int j = 0; j < MAP_WIDTH / 64; j++) {
					drawElement(j * 64 - camera->x, i * 64 - camera->y, line.at(j), textures, gRenderer);
				}
			}

			SDL_RenderCopy(gRenderer, playerTexture, NULL, fillRect1);

			//Update screen
			SDL_RenderPresent(gRenderer);

			if (desiredFrameTime > deltaTime)
			{
				SDL_Delay(desiredFrameTime - deltaTime);
			}
		}


		//}
	}

	//Free resources and close SDL
	close();

	return 0;
}

float smoothingMotion(float targetSpeed, float smooth, float velocity) {
	return targetSpeed * (1 - smooth) + velocity * smooth;
}

void updatePlayerPosition(SDL_Rect* fillRect1, SDL_Rect* camera, float velocityOfRect) {
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		fillRect1->y -= velocityOfRect;
		printf("We got a motion event.\n UP buton pressed");
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		fillRect1->y += velocityOfRect;
		printf("We got a motion event.\n DOWN buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		fillRect1->x -= velocityOfRect;
		printf("We got a motion event.\n LEFT buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		fillRect1->x += velocityOfRect;
		printf("We got a motion event.\n RIGHT buton pressed");

	}

	// Update the camera to follow the player
	camera->x = fillRect1->x - SCREEN_WIDTH / 2;
	camera->y = fillRect1->y - SCREEN_HEIGHT / 2;

	// Keep the camera in bounds
	if (camera->x < 0) {
		camera->x = 0;
	}
	if (camera->y < 0) {
		camera->y = 0;
	}
	if (camera->x > MAP_WIDTH - SCREEN_WIDTH) {
		camera->x = MAP_WIDTH - SCREEN_WIDTH;
	}
	if (camera->y > MAP_HEIGHT - SCREEN_HEIGHT) {
		camera->y = MAP_HEIGHT - SCREEN_HEIGHT;
	}
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Dwuwymiarowa grafika komputerowa", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{

			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}




void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}


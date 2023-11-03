#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "lib/Circle.h"
#include "lib/MapLoader.h"

const int SCREEN_WIDTH = 1088;
const int SCREEN_HEIGHT = 640;
int mouseX, mouseY;


SDL_Rect* fillRect1;

SDL_Surface* gScreenSurface = NULL;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* grassTexture = NULL;

SDL_Texture* skyTexture = NULL;

SDL_Texture* wallTexture = NULL;



bool init();

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

		Circle circle1 = Circle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50);
		float smooth = 0.1;
		float velocityOfCircle = 0;

		float velocityOfRect = 1;
		fillRect1 = new SDL_Rect
		{
			50,
			50,
			100,
			100
		};
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

			// Handle different combinations of key presses
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



			float deltaX = mouseX - circle1.getCenterX();
			float deltaY = mouseY - circle1.getCenterY();

			float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

			// Define a constant speed
			float speed = 1; // Adjust as needed

			// Apply smoothing to the velocity
			float targetVelocity = distance > 0 ? speed / distance : 0;
			velocityOfCircle = smoothingMotion(targetVelocity, smooth, velocityOfCircle);

			// Calculate the new position based on linear interpolation
			if (distance > 0)
			{
				circle1.calculateNewPosition(velocityOfCircle * deltaX, velocityOfCircle * deltaY);
			}
			

			//elementsOfMap = loadElementInfoFromFile("res/maps/legend.txt");
			std::vector<std::string> levelMap = loadFromFile("res/maps/map1.txt");

		
			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0x8B, 0xAC, 0xB7, 0xFF);
			SDL_RenderClear(gRenderer);

			int numberOfColumns = SCREEN_WIDTH / 64;
			int numberOfRows = SCREEN_HEIGHT / 64;

			for (int i = 0; i < numberOfRows; i++) {
				std::string line = levelMap.at(i);
				for (int j = 0; j < numberOfColumns; j++) {
					drawElement(j * 64, i * 64, line.at(j), textures, gRenderer);
				}
			}

			// Draw semi-transparent circle
			circle1.drawCircle(gRenderer, alpha);

			// Draw rectangles
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderFillRect(gRenderer, fillRect1);

			//Update screen
			SDL_RenderPresent(gRenderer);
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


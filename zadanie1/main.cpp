#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 680;

enum Textures
{
	TEXTURE_CLOUD,
	TEXTURE_BRICK,
	TEXTURE_TOTAL
};

SDL_Rect fillRect1;
SDL_Rect fillRect2;



SDL_Surface* loadSurface(std::string path);

SDL_Texture* loadTextureFromTheSurface(std::string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* Textures[TEXTURE_TOTAL];

SDL_Surface* gScreenSurface = NULL;

bool init();

bool loadTextures();

void close();



int main(int argc, char* args[])
{
	int fillRect1X = SCREEN_WIDTH / 8; // Initial X position
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadTextures())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			int fillRect1X = 100;
			float velocity = 1;

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
				}
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				fillRect1 = {
					fillRect1X,
					SCREEN_HEIGHT / 8,
					SCREEN_WIDTH / 4,
					SCREEN_HEIGHT / 4 };
				//put the textures on rect
				SDL_RenderCopy(gRenderer, Textures[TEXTURE_CLOUD], NULL, &fillRect1);

				fillRect2 = {
					SCREEN_WIDTH / 2,
					SCREEN_HEIGHT / 4,
					SCREEN_WIDTH / 4,
					SCREEN_HEIGHT / 2 };
				SDL_RenderCopy(gRenderer, Textures[TEXTURE_BRICK], NULL, &fillRect2);

				if (fillRect1X > SCREEN_WIDTH - 100)
				{
					fillRect1X = 0;
				}
				if (fillRect1X < 100)
				{
					fillRect1X = 100;
				}
				fillRect1X += velocity;

				//Update screen
				SDL_RenderPresent(gRenderer);
			}


		}
	}

	//Free resources and close SDL
	close();

	return 0;
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

bool loadTextures()
{
	//Loading success flag
	bool success = true;

	//Load TEXTURES
	Textures[TEXTURE_CLOUD] = loadTextureFromTheSurface("res/cloud.png");
	if (Textures[TEXTURE_CLOUD] == NULL)
	{
		printf("Failed to load default texture!\n");
		success = false;
	}

	//Load up surface
	Textures[TEXTURE_BRICK] = loadTextureFromTheSurface("res/brick.png");
	if (Textures[TEXTURE_BRICK] == NULL)
	{
		printf("Failed to load up texture!\n");
		success = false;
	}

	return success;
}

SDL_Texture* loadTextureFromTheSurface(std::string path) {
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}


void close() {
	for (int i = 0; i < TEXTURE_TOTAL; i++)
	{
		SDL_DestroyTexture(Textures[i]);
		Textures[i] = NULL;
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface(std::string path)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

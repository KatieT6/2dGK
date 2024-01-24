#include "../lib/MapLoader.h"




//loading map or other files to vector by line 
std::vector<std::string> loadFromFile(const std::string& fileName) {
	std::ifstream file(fileName);
	std::vector<std::string> levelDescription;
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			levelDescription.push_back(line);
		}
		file.close();
	}
	return levelDescription;
}


void drawElement(int x, int y, char sign, std::vector< SDL_Texture*> textures, SDL_Renderer* gRenderer, int x_m, int y_m, bool is3Layer) {
	SDL_Rect fillRect = { x, y,  32,  32 };
	VectorI2* position = new VectorI2{ x_m, y_m };
	
	if (sign == 'L') {
		SDL_RenderCopy(gRenderer, textures[0], NULL, &fillRect);
	}
	else if (sign == 'G') {
		SDL_RenderCopy(gRenderer, textures[1], NULL, &fillRect);
	}
	else if (sign == 'g') {
		SDL_RenderCopy(gRenderer, textures[2], NULL, &fillRect);
	}
	else if (sign == 'k') {
		if (is3Layer)
		{
			SDL_RenderCopy(gRenderer, textures[2], NULL, &fillRect);
		}
		SDL_RenderCopy(gRenderer, textures[3], NULL, &fillRect);
		//Wall wall = Wall(position, 32, 32);
	}
	else if(sign == 'K')
	{
		if (is3Layer)
		{
			SDL_RenderCopy(gRenderer, textures[2], NULL, &fillRect);
		}
		SDL_RenderCopy(gRenderer, textures[4], NULL, &fillRect);
	}
	else if (sign == 'm')
	{
		SDL_RenderCopy(gRenderer, textures[5], NULL, &fillRect);
	}
	else if (sign == 'M')
	{
		SDL_RenderCopy(gRenderer, textures[6], NULL, &fillRect);
	}
	else if (sign == 'w')
	{
		SDL_RenderCopy(gRenderer, textures[7], NULL, &fillRect);
	}
	else if (sign == 'D')
	{
		SDL_RenderCopy(gRenderer, textures[8], NULL, &fillRect);
	}
	else if (sign == 'd')
	{
		SDL_RenderCopy(gRenderer, textures[9], NULL, &fillRect);
	}
	else if (sign == 'h')
	{
		SDL_RenderCopy(gRenderer, textures[10], NULL, &fillRect);
	}
}


SDL_Texture* loadTextureFromTheSurface(std::string path, SDL_Renderer* gRenderer) {
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

SDL_Surface* loadSurface(std::string path, SDL_Surface* gScreenSurface)
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

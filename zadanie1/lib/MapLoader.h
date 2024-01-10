#pragma once

#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Wall.h"


std::vector<std::string> loadFromFile(const std::string& fileName);

SDL_Texture* loadTextureFromTheSurface(std::string path, SDL_Renderer* gRenderer);

SDL_Surface* loadSurface(std::string path, SDL_Surface* gScreenSurface);

void drawElement(int x, int y, char sign, std::vector< SDL_Texture*> textures, SDL_Renderer* gRenderer, int x_m, int y_m);




//void drawMap(SDL_Renderer* renderer, const std::vector<std::string> levelDescriptions, const std::string pathDescription);
#endif // !MAP_LOADER_H
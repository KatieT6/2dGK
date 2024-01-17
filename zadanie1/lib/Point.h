#pragma once
#include <SDL_system.h>
#include <SDL.h>
#include "Vector.h"

class Point
{
public:
	Point() {};
	~Point() {};

	Point(VectorI2* pos, int width, int height) : position(*pos), width(width), height(height) {};

	VectorI2 position;
	bool isCircle = false;
	int width = 32;
	int height = 32;
	bool isChecked = false;





private:

};

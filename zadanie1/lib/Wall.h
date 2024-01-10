#pragma once

#ifndef WALL_H
#define WALL_H

#include "Collider.h"
#include "Vector.h"

class Wall
{
public:
    VectorI2 position;
    //Collider* collider;
    bool isCircle;
    int width = 32;
    int height = 32;
    SDL_Rect* collider;
    
    Wall(VectorI2* pos, int width, int height) : position(*pos), width(width), height(height)
    {
    	//collider = new RectCollider(pos, width, height);
        isCircle = false;
        collider = new SDL_Rect{ position.x, position.y, width, height };
    }

    VectorI2 getPosition() const { return position; }
    SDL_Rect* getCollider() const { return collider; }
};
#endif // !WALL_H




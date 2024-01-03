#pragma once

#ifndef WALL_H
#define WALL_H


#include "Vector.h"
#include "Collider.h"

class Wall
{
private:
    

public:
    VectorI2 position;
    //Collider* collider;
    bool isCircle;
    int width = 32;
    int height = 32;
    
    Wall(VectorI2* pos, int width, int height) : position(*pos), width(width), height(height)
    {
    	//collider = new RectCollider(pos, width, height);
        isCircle = false;
    }

    VectorI2 getPosition() const { return position; }
};
#endif // !WALL_H




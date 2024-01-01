#pragma once
#include "Vector.h"

class Wall
{
private:
    VectorI2 position;

public:
    Wall(VectorI2 pos) : position(pos) {}

    VectorI2 getPosition() const { return position; }
};
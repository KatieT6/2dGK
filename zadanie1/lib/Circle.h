#include <stdio.h>
#include <string>
#include <SDL.h>
#include <vector>
#include "Vector.h"


class Circle
{
private:
    VectorI2 position;
    int radius;

    void setPosition(VectorI2 pos);

public:
    Circle(VectorI2 pos, int r);
    void drawCircle(SDL_Renderer* renderer, Uint8 alpha);

    // Getter for radius
    int getRadius() const;

    VectorI2 getPosition() const;


    void updatePosition(float x, float y);


};

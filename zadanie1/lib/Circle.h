#include <stdio.h>
#include <string>
#include <SDL.h>
#include <vector>
#include "Vector.h"


class Circle
{
private:
    VectorF2 velocity;
    VectorI2 position;
    int radius;

    void setPosition(VectorI2 pos);

    void setVelocity(VectorF2 vel);

public:
    Circle(VectorI2 pos, VectorF2 vel, int r);
    void drawCircle(SDL_Renderer* renderer, Uint8 alpha);

    // Getter for radius
    int getRadius() const;

    VectorI2 getPosition() const;

    VectorF2 getVelocity() const;

    void updatePosition();


};

#include "../lib/Circle.h"

Circle::Circle(VectorI2 pos, VectorF2 vel, int r)
{
    position = pos;
    velocity = vel;
    radius = r;
}

// Getter for radius
int Circle::getRadius() const
{
    return radius;
}

VectorI2 Circle::getPosition() const
{
    return VectorI2();
}

VectorF2 Circle::getVelocity() const
{
    return VectorF2();
}

void Circle::setPosition(VectorI2 pos)
{
	position = pos;
}

void Circle::setVelocity(VectorF2 vel)
{
	velocity = vel;
}

void Circle::updatePosition()
{
	position.x += velocity.x;
	position.y += velocity.y;
}

void Circle::drawCircle(SDL_Renderer* renderer, Uint8 alpha)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int w = 0; w <= radius * 2; ++w) {
        for (int h = 0; h <= radius * 2; ++h) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, position.x + dx, position.y + dy);
            }
        }
    }
}

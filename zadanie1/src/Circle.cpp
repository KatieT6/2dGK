#include "../lib/Circle.h"

Circle::Circle(float x, float y, int r)
{
    centerX = x;
    centerY = y;
    radius = r;
}

// Getter for center x
float Circle::getCenterX() const
{
    return centerX;
}

// Getter for center y
float Circle::getCenterY() const
{
    return centerY;
}

// Getter for radius
int Circle::getRadius() const
{
    return radius;
}

// Setter for center x
void Circle::setCenterX(float x)
{
    centerX = x;
}

// Setter for center y
void Circle::setCenterY(float y)
{
    centerY = y;
}

// Setter for radius
void Circle::setRadius(int r)
{
    radius = r;
}

void Circle::calculateNewPosition(float x, float y)
{
    centerX += x;
    centerY += y;
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
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

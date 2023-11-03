#include <stdio.h>
#include <string>
#include <SDL.h>
#include <vector>

class Circle
{
private:
    float centerX;
    float centerY;
    int radius;


    // Setter for center x
    void setCenterX(float x);

    // Setter for center y
    void setCenterY(float y);

    // Setter for radius
    void setRadius(int r);

public:
    Circle(float x, float y, int r);
    void drawCircle(SDL_Renderer* renderer, Uint8 alpha);

    // Getter for center x
    float getCenterX() const;

    // Getter for center y
    float getCenterY() const;
    // Getter for radius
    int getRadius() const;

    void calculateNewPosition(float x, float y);


};

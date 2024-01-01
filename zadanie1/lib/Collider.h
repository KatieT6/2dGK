#pragma once
#include "Vector.h"

class Collider
{
public:
	virtual bool checkCollision(Collider* other) = 0;
};

class CircleCollider : public Collider
{
private:
    VectorI2 position;
    int r;

public:
    CircleCollider(VectorI2 pos, int r) : position(pos), r(r) {}

    bool checkCollision(Collider* other) override {
		if (CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(other)) {
		
        }
		return false;
    }

    bool checkCircleCollision(CircleCollider* other) {
		VectorF2 distance = {
		abs((position.x + r / 2.0f) - (other->position.x + other->r / 2.0f)),
		abs((position.y + r / 2.0f) - (other->position.y + other->r / 2.0f)) };
		float length = sqrt(distance.x * distance.x + distance.y * distance.y);
		if (length <= r + other->r) {
            return true;
		}
        else {
            return false;
        }
	}

};

class RectCollider : public Collider
{
private:
    VectorI2 position;
    int width;
    int height;

public:
    RectCollider(VectorI2 pos, int w, int h) : position(pos), width(w), height(h) {}

    bool checkCollision(Collider* other) override {

    }

};
#pragma once
#include "Vector.h"

class Collider
{
public:
	virtual float getDistance(Collider* other) = 0;
	virtual bool checkRectCollision(Collider* other) = 0;
	virtual VectorI2 getSeparationVector(Collider* other) = 0;
};

//class CircleCollider : public Collider
//{
//private:
//    VectorI2 position;
//    int r;
//
//public:
//    CircleCollider(VectorI2 * pos, int r) : position(*pos), r(r) {}
//
//    VectorI2 getPosition() {
//		return position;
//	}
//
//    float checkCircleCollision(CircleCollider* other) {
//		
//		float length = getDistanceFromCircle(other);
//		if (length <= r + other->r) {
//            return true;
//		}
//        else {
//            return false;
//        }
//	}
//
//    bool checkRectCollision(RectCollider* other) override{
//        float closestX = clamp(position.x, other->getPosition().x, other->getPosition().x + other->getWidth());
//		float closestY = clamp(position.y, other->getPosition().y, other->getPosition().y + other->getHeight());
//    	float distanceX = position.x - closestX;
//		float distanceY = position.y - closestY;
//		float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));
//
//		if (distance < r)
//		{
//
//		}
//	}
//
//	float getDistance(Collider* other) override {
//		/*if (CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(other)) {
//			return getDistanceFromCircle(otherCircle);
//		}
//		else if (RectCollider* otherRect = dynamic_cast<RectCollider*>(other)) {
//			return getDistanceFromRect(otherRect);
//		}*/
//	}
//
//	VectorI2 getSeparationVector(Collider* other) override {}
//
//	float getDistanceFromCircle(CircleCollider* other) {
//		VectorF2 distance = {
//		abs((position.x + r / 2.0f) - (other->position.x + other->r / 2.0f)),
//		abs((position.y + r / 2.0f) - (other->position.y + other->r / 2.0f)) };
//		return sqrt(distance.x * distance.x + distance.y * distance.y);
//	}
//
//	float getDistanceFromRect(RectCollider* other)  {
//		float closestX = clamp(position.x, other->getPosition().x, other->getPosition().x + other->getWidth());
//		float closestY = clamp(position.y, other->getPosition().y, other->getPosition().y + other->getHeight());
//		float distanceX = position.x - closestX;
//		float distanceY = position.y - closestY;
//		return sqrt((distanceX * distanceX) + (distanceY * distanceY));
//	}
//
//	float clamp(float value, float min, float max) {
//		if (value < min) {
//			return min;
//		}
//		else if (value > max) {
//			return max;
//		}
//		else {
//			return value;
//		}
//	}
//
//};
//
//class RectCollider : public Collider
//{
//private:
//    VectorI2 position;
//    int width;
//    int height;
//
//public:
//    RectCollider(VectorI2 * pos, int w, int h) : position(*pos), width(w), height(h) {}
//
//	VectorI2 getPosition() {
//		return position;
//	}
//
//	int getWidth() {
//		return width;
//	}
//
//	int getHeight() {
//		return height;
//	}
//
//    bool checkRectCollision(RectCollider* other) override{
//		float left = position.x + width - other->position.x;
//		float right = other->position.x + other->width - position.x;
//		float top = position.y + height - other->position.y;
//		float bottom = other->position.y + other->height - position.y;
//
//		if (left > 0 && right > 0 && top > 0 && bottom > 0) {
//
//            // Znajdü wektor separacji
//            VectorI2 separation;
//
//            if (left < right) {
//                separation.x = -left;
//            }
//            else {
//                separation.x = right;
//            }
//
//            if (top < bottom) {
//                separation.y = -top;
//            }
//            else {
//                separation.y = bottom;
//            }
//
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//
//	float getDistance(Collider* other) override {
//		/*if (CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(other)) {
//			return getDistanceFromCircle(otherCircle);
//		}
//		else if (RectCollider* otherRect = dynamic_cast<RectCollider*>(other)) {
//			return getDistanceFromRect(otherRect);
//		}*/
//	}
//
//	float getDistanceFromCircle(CircleCollider* other)  {
//		float closestX = clamp(other->getPosition().x, position.x, position.x + width);
//		float closestY = clamp(other->getPosition().y, position.y, position.y + height);
//		float distanceX = other->getPosition().x - closestX;
//		float distanceY = other->getPosition().y - closestY;
//		return sqrt((distanceX * distanceX) + (distanceY * distanceY));
//	}
//
//	float getDistanceFromRect(RectCollider* other)  {
//		float left = position.x + width - other->position.x;
//		float right = other->position.x + other->width - position.x;
//		float top = position.y + height - other->position.y;
//		float bottom = other->position.y + other->height - position.y;
//
//		if (left > 0 && right > 0 && top > 0 && bottom > 0) {
//
//		}
//	}
//	VectorI2 getSeparationVector(Collider* other) override {
//		if (CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(other)) {
//			return getSeparationVector(otherCircle);
//		}
//		else if (RectCollider* otherRect = dynamic_cast<RectCollider*>(other)) {
//			return getSeparationVector(otherRect);
//		}
//	}
//
//	VectorI2 getSeparationVector(RectCollider* other) {
//		float left = position.x + width - other->position.x;
//		float right = other->position.x + other->width - position.x;
//		float top = position.y + height - other->position.y;
//		float bottom = other->position.y + other->height - position.y;
//
//			VectorI2 separation;
//
//			if (left < right) {
//				separation.x = -left;
//			}
//			else {
//				separation.x = right;
//			}
//
//			if (top < bottom) {
//				separation.y = -top;
//			}
//			else {
//				separation.y = bottom;
//			}
//
//			return separation;
//	}
//
//    float clamp(float value, float min, float max) {
//		if (value < min) {
//			return min;
//		}
//		else if (value > max) {
//			return max;
//		}
//		else {
//			return value;
//		}
//	}
//
//};
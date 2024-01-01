#include "lib/GameObject.h"


#pragma region getters/setters
VectorI2 GameObject::getPosition()
{
	return VectorI2();
}

int GameObject::getRadius()
{
	return r;
}

int GameObject::getWidth()
{
	return widht;
}

int GameObject::getHeight()
{
	return height;
}

bool GameObject::getIsWall()
{
	return isWall;
}

bool GameObject::getIsCircle()
{
	return isCircle;
}

void GameObject::setPosition(VectorI2 position)
{
	this->position = position;
}

void GameObject::setRadius(int radius)
{
	r = radius;
}

void GameObject::setWidth(int width)
{
	widht = width;
}

void GameObject::setHeight(int height)
{
	this->height = height;
}

void GameObject::setIsWall(bool isWall)
{
	this->isWall = isWall;
}

void GameObject::setIsCircle(bool isCircle)
{
	this->isCircle = isCircle;
}

void GameObject::setScreenPosition(VectorF2 screenPosition)
{
	this->screenPosition = screenPosition;
}

VectorF2 GameObject::getScreenPosition()
{
	return screenPosition;
}
#pragma endregion


GameObject::GameObject(VectorI2 position, int width, int height, bool isCircle, int r)
{
	this->position = position;
	this->widht = width;
	this->height = height;
	this->isCircle = isCircle;
	this->r = r;
}

GameObject::GameObject(VectorI2 position, int width, int height)
{
	this->position = position;
	this->widht = width;
	this->height = height;
}

GameObject::GameObject(VectorI2 position, int widtj, int height, bool isWall)
{
	this->position = position;
	this->widht = widtj;
	this->height = height;
	this->isWall = isWall;
}

GameObject::~GameObject()
{
}


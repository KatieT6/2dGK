#include "lib/GameObject.h"
#include "lib/Vector.h"

void GameObject::setPosition(VectorI2 position)
{
	this->position = position;
}

VectorI2 GameObject::getPosition()
{
	return position;
}

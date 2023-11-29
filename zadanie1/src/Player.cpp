#include "../lib/Player.h"

Player::Player()
{
	velocity = {0 , 0 };
	position = { 64, 64 };
	targetVelocity = { 0, 0 };
	screenPosition = { 0, 0 };
}

Player::~Player()
{
}



VectorF2 Player::getTargetVelocity()
{
	return targetVelocity;
}

VectorI2 Player::getPosition()
{
	return position;
}

VectorF2 Player::getVelocity()
{
	return velocity;
}

VectorI2 Player::setPosition(VectorI2& pos)
{
	return position = pos;
}

VectorF2 Player::setVelocity(VectorF2& vel)
{
	return velocity = vel;
}

VectorF2 Player::setTargetVelocity(VectorF2& vel)
{
	return targetVelocity = vel;
}



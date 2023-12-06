#include "../lib/Player.h"
#include <math.h>
#include <cmath>
#include <iostream>



Player::Player()
{
	velocity = {0 , 0 };
	position = { 64, 64 };
	targetVelocity = { 0, 0 };
	screenPosition = { 0, 0 };
	r = 0;
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

int Player::getRadius()
{
	return r;
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

void Player::setRadius(int radius)
{
	r = radius;
}

void Player::updatePlayerPosition()
{
	position.x += velocity.x;
	position.y += velocity.y;
}

void Player::handleCollision(Player otherPlayer)
{
	VectorF2 distance = { 
		abs((position.x + r / 2.0f) - (otherPlayer.position.x + otherPlayer.r / 2.0f)),
		abs((position.y + r / 2.0f) - (otherPlayer.position.y + otherPlayer.r / 2.0f)) };
	float length = sqrt(distance.x * distance.x + distance.y * distance.y);
	if (length < r + otherPlayer.r) {
		VectorF2 separation = { position.x - otherPlayer.getPosition().x, position.y - otherPlayer.getPosition().y };
		float overlap = (r + otherPlayer.r - length) / length;
		separation.x *= overlap;
		separation.y *=overlap;

		float separationlength = sqrt(separation.x * separation.x + separation.y * separation.y);
		separation.x /= separationlength;
		separation.y /= separationlength;

		VectorF2 newVel = { 
			velocity.x - 2 * (separation.x * velocity.x + separation.y * velocity.y) * separation.x,
			velocity.y - (2 * (separation.x * velocity.x + separation.y * velocity.y) * separation.y) };
		velocity = newVel;
	}
}

void Player::separate(Player otherPlayer)
{
	VectorF2 distance = {
		abs((position.x + r / 2.0f) - (otherPlayer.position.x + otherPlayer.r / 2.0f)),
		abs((position.y + r / 2.0f) - (otherPlayer.position.y + otherPlayer.r / 2.0f)) };
	float length = sqrt(distance.x * distance.x + distance.y * distance.y);

	if (length < r + otherPlayer.r)
	{
		float overlap = 0.5f * (length - r - otherPlayer.getRadius());
		VectorF2 localDistance = { position.x - otherPlayer.getPosition().x, position.y - otherPlayer.getPosition().y };
		position.x -= overlap * localDistance.x / length;
		position.y -= overlap * localDistance.y / length;
	}
}



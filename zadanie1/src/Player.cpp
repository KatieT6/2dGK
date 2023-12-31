#include "../lib/Player.h"
#include <math.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 640;

Player::Player()
{
	velocity = { 0 , 0 };
	position = { 64, 64 };
	targetVelocity = { 0, 0 };
	screenPosition = { 0, 0 };
}:GameObject()

Player::Player(bool isCircle)
{
	velocity = {0 , 0 };
	position = { 64, 64 };
	targetVelocity = { 0, 0 };
	screenPosition = { 0, 0 };
	r = 0;
	this->isCircle = isCircle;
}

Player::Player(bool isCircle, int width, int height)
{
	velocity = {0 , 0 };
	position = { 64, 64 };
	targetVelocity = { 0, 0 };
	screenPosition = { 0, 0 };
	r = 0;
	this->isCircle = isCircle;
	this->widht = width;
	this->height = height;
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

void Player::CircleCircleCollision(Player otherPlayer)
{
	VectorF2 distance = { 
		abs((position.x + r / 2.0f) - (otherPlayer.position.x + otherPlayer.r / 2.0f)),
		abs((position.y + r / 2.0f) - (otherPlayer.position.y + otherPlayer.r / 2.0f)) };
	float length = sqrt(distance.x * distance.x + distance.y * distance.y);
	if (length <= r + otherPlayer.r) {
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

void Player::RectRectCollision(Player otherPlayer) {
	if (this->isCircle && otherPlayer.isCircle) {
		if (true)
		{

		}
	}
}

void Player::handleWallCollision()
{
	if (position.x <= 0 + r) {
		if (velocity.x <= 0) {
			VectorF2 v = { -velocity.x, velocity.y };
			velocity = v;
		}
	}
	else if (position.x >= WIDTH - r)
	{
		if (velocity.x >= 0) {
			VectorF2 v = { -velocity.x, velocity.y };
			velocity = v;
		}
	}
	if (position.y <= 0 + r) {
		if (velocity.y <= 0) {
			VectorF2 v = { velocity.x, -velocity.y };
			velocity = v;
		}
	}
	else if (position.y >= HEIGHT - r)
	{
		if (velocity.y >= 0) {
			VectorF2 v = { velocity.x, -velocity.y };
			velocity = v;
		}

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



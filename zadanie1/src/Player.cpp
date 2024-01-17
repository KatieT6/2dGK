#include "../lib/Player.h"
#include <math.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 640;

Player::Player()
{
	position = { 0, 0 };
}

Player::Player(bool isCircle, int r, VectorI2 pos)
{
	VectorI2 temp = {pos.x - r, pos.y - r};
	position = temp;
	this->r = r;
	this->isCircle = isCircle;
}

Player::Player(int width, int height, VectorI2 pos)
{
	velocity = {0 , 0 };
	position = pos;
	targetVelocity = { 0, 0 };
	screenPosition = { 0, 0 };
	width = width;
	height = height;
}

Player::~Player()
{
	//delete collider;
}

#pragma region setters/getters



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

//SDL_Rect* Player::getCollider()
//{
//	return collider;
//}

void Player::setPosition(VectorI2& pos)
{
	 position = pos;
}

void Player::setVelocity(VectorF2& vel)
{
	 velocity = vel;
}

void Player::setTargetVelocity(VectorF2& vel)
{
	 targetVelocity = vel;
}

void Player::setRadius(int radius)
{
	r = radius;
}

#pragma endregion

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



void Player::RectRectCollision(Wall* otherPlayer) {
	if (!this->isCircle && !otherPlayer->isCircle) {
		float left = position.x + 2 * width - otherPlayer->position.x;
		float right = otherPlayer->position.x + 2 * otherPlayer->width - position.x;
		float top = position.y + 2 * height - otherPlayer->position.y;
		float bottom = otherPlayer->position.y + 2 * otherPlayer->height - position.y;

		
		if (left > 0 && right > 0 && top > 0 && bottom > 0) {
			printf(" Rect Rect Collision -- player 2 \n");
			//printf("x, y: (%d, %d)f\n\n", this->position.x, this->position.y);
			//printf("wall (%d, %d)\n\n", otherPlayer->getPosition().x, otherPlayer->getPosition().y);
			// Znajdü wektor separacji
			VectorI2 separation;

			if (left < right) {
				separation.x = -left;
			}
			else {
				separation.x = right;
			}

			if (top < bottom) {
				separation.y = -top;
			}
			else {
				separation.y = bottom;
			}

			if (abs(separation.x) < abs(separation.y)) {
				position.x += separation.x;
				velocity.x = 0; 
			}
			else {
				position.y += separation.y;
				velocity.y = 0; 
			}

		}

	}
}


bool checkCircleRectCollision(Player* circle, Wall* rect, bool collide)
{
	VectorF2 closestPoint;
	closestPoint.x = clamp(player->position.x, tileXIndex * TILE_SIZE, tileXIndex * TILE_SIZE + TILE_SIZE);
	closestPoint.y = clamp(player->position.y, tileYIndex * TILE_SIZE, tileYIndex * TILE_SIZE + TILE_SIZE);

	float distance;
	distance = sqrt(pow(player->position.x - closestPoint.x, 2) + pow(player->position.y - closestPoint.y, 2));

	if (distance < player->radius) {
		if (collide) separate(player, tileXIndex = rect->position.x, tileYIndex = rect->position.y, closestPoint);
		return true;
	}
	else {
		return false;
	}
}
}

//void Player::CircleRectCollision(Wall* otherPlayer)
//{
//	if (this->isCircle == true && otherPlayer->isCircle == false)
//	{
//		float left = position.x + 2 * width - otherPlayer->position.x;
//		float right = otherPlayer->position.x + 2 * otherPlayer->width - position.x;
//		float top = position.y + 2 * height - otherPlayer->position.y;
//		float bottom = otherPlayer->position.y + 2 * otherPlayer->height - position.y;
//
//		VectorF2 f = { clamp(this->getPosition().x, left, right), clamp(this->getPosition().y, top, bottom) };
//		float distance = sqrt((f.x - this->getPosition().x) * (f.x - this->getPosition().x)
//			+ (f.y - this->getPosition().y) * (f.y - this->getPosition().y));
//		if (distance < this->getRadius())
//		{
//		printf("Circle rect collision -- player 1");
//			if (static_cast<float>(f.x) == this->getPosition().x && static_cast<float>(f.y) == this->getPosition().y) {
//				// Znajdü wektor separacji
//				VectorI2 separation;
//
//				if (left < right) {
//					separation.x = -left;
//				}
//				else {
//					separation.x = right;
//				}
//
//				if (top < bottom) {
//					separation.y = -top;
//				}
//				else {
//					separation.y = bottom;
//				}
//
//				if (abs(separation.x) < abs(separation.y)) {
//					//position.x += separation.x;
//					velocity.x = 0;
//				}
//				else {
//					//position.y += separation.y;
//					velocity.y = 0;
//				}
//				printf("RECT COLLSION");
//			}
//			else
//			{
//				VectorF2 tmp = { this->getPosition().x - f.x, this->getPosition().y - f.y };
//				VectorF2 separation = { tmp.x / distance, tmp.y / distance };
//				float diff = this->getRadius() - distance;
//				VectorF2 separationVector = { separation.x * diff, separation.y * diff };
//				//this->position.x -= separationVector.x;
//				//this->position.y -= separationVector.y;
//				printf("CIRCLE COLLISION");
//			}
//		}
//	}
//}

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

float Player::clamp(float value, float min, float max)
{
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}
	else {
		return value;
	}
}

void Player::separate(Player otherPlayer)
{
	if (this->isCircle && otherPlayer.isCircle)
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
}

void Player:: separate(Player* player, float tileX, float tileY, VectorF2 closestPoint)
{
	VectorF2 separation{0, 0};
	if (player->position.x == closestPoint.x && player->position.y == closestPoint.y) {
		float left = player->position.x - tileX * 32 + player->r;
		float right = tileX * 32 + 32 - player->position.x + player->r;
		float top = player->position.y - tileY * 32 + player->r;
		float bottom = tileY * 32 + 32 - player->position.y + player->r;



		separation.x = left < right ? -left : right;
		separation.y = top < bottom ? -top : bottom;
	}
	else {
		separation.x = player->position.x - closestPoint.x;
		separation.y = player->position.y - closestPoint.y;

		float distance = sqrt(separation.x * separation.x + separation.y * separation.y);
		float factor = (player->r - distance) / distance;

		separation.x *= factor;
		separation.y *= factor;
	}
	player->position.x += separation.x;
	player->setPosition(player->position);
}




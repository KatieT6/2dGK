#pragma once

#include "Vector.h"
#include <SDL.h>
#include <stdio.h>
//#include "Collider.h"
#include "Wall.h"
#include <vector>

class Player
{
private:
	VectorF2 velocity = { 0 , 0 };
	VectorF2 targetVelocity = { 0 , 0 };
	VectorF2 screenPosition = { 0, 0 };
	bool isWall = false;
	int r = 0;
	int width = 0;
	int height = 0;
	//SDL_Rect* collider;

	//Collider* collider;
	

public:
	VectorI2 position;

#pragma region jumping

	bool isJumping = false;
	bool isFalling = false;
	bool isOnGround = true;
	float jumpTime = 0;
	float jumpTimeMax = 1.0;
	float jumoVelocityX = 4;
	float jumpHeight =  3;
	float jumpDistance = 2;
	float gravity = - 2 * jumpHeight * (jumoVelocityX * jumoVelocityX) / (jumpDistance * jumpDistance);
	float jumpVelocity0 = (2 * jumpHeight * jumoVelocityX )/ jumpDistance;
	int jumpcounter = 0;

	/// szybkie spadanie
	float gravity2 = gravity * 1.5;

#pragma endregion 

	bool isCircle = false;
	Player();
	Player(bool isCircle, int r, VectorI2 position);
	Player(int width, int height, VectorI2 position);
	~Player();

	//void update(int player1Velocity, int player2Velocity, Player* p1, Player* p2);
	//void updateWSAD(float playerVelocity);

	VectorF2 getTargetVelocity();
	VectorI2 getPosition();
	VectorF2 getVelocity();
	int getRadius();


	//SDL_Rect* getCollider();
	void setPosition(VectorI2& pos);
	void setVelocity(VectorF2& vel);
	void setTargetVelocity(VectorF2 &vel);
	void setRadius(int radius);

	void updatePlayerPosition();  
	void CircleCircleCollision(Player otherPlayer);
	//bool checkCircleRectCollision(Player* circle, Wall* rect, bool collide);
	//void CircleRectCollision(Wall* otherPlayer);
	void handleWallCollision();
	float clamp(float value, float min, float max);
	void separate(Player otherPlayer);
	void separate(Player* player, float tileX, float tileY, VectorF2 closestPoint);
	//void separate(Player* player, Wall* otherWall, VectorF2 closestPoint);
	bool checkCollisions();
	bool CircleRectCollision(Wall* otherPlayer);
	bool RectRectCollision(Wall* otherPlayer);
	bool checkCollisions(std::vector<Wall*> collders);
	void handleJumping();
	void jump(Uint64 deltatime, Uint64 currenttime, Uint64 prevtime);

};


#pragma once

#include "Vector.h"
#include <SDL.h>
#include <stdio.h>
//#include "Collider.h"
#include "Wall.h"

class Player
{
private:
	VectorI2 position;
	VectorF2 velocity = { 0 , 0 };
	VectorF2 targetVelocity = { 0 , 0 };
	VectorF2 screenPosition = { 0, 0 };
	bool isWall = false;
	bool isCircle = false;
	int r = 0;
	int width = 0;
	int height = 0;
	//SDL_Rect* collider;

	//Collider* collider;
	

public:
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
	void RectRectCollision(Wall* otherPlayer);
	bool checkCircleRectCollision(Player* circle, Wall* rect, bool collide);
	//void CircleRectCollision(Wall* otherPlayer);
	void handleWallCollision();
	float clamp(float value, float min, float max);
	void separate(Player otherPlayer);
	void separate(Player* player, float tileX, float tileY, VectorF2 closestPoint);
	void separate(Player* player, Wall* otherWall, VectorF2 closestPoint);
	bool checkCollisions();

};


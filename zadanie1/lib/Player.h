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
	VectorI2 dimensions;
	VectorF2 velocity = { 0 , 0 };
	VectorI2 tempVelocity;

	float radius;

	float gravity;
	bool jumpPressed = false;
	float jumpTime;
	int jumpCount;

	float MAX_H = 32 * 2;
	float MAX_DISTANCE = 32 * 6;
	float V_0;

	static const int VEL = 2;

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
	bool checkWallCollisions();
	float clamp(float value, float min, float max);
	void separate(Player otherPlayer);
	void separate(Player* player, float tileX, float tileY, VectorF2 closestPoint);
	//void separate(Player* player, Wall* otherWall, VectorF2 closestPoint);
	bool CircleRectCollision(Wall* otherPlayer);
	bool RectRectCollision(Wall* otherPlayer);
	bool checkCollisions(std::vector<Wall*> collders);
	void handleJumping();
	void jump(Uint64 deltatime, Uint64 currenttime, Uint64 prevtime, std::vector<Wall*> collders);
	void calculateNewGravity();
};



#include "Vector.h"
#include <SDL.h>
#include <stdio.h>
#include "GameObject.h"

class Player: public GameObject
{
private:
	VectorI2 position;
	VectorF2 velocity;
	VectorF2 targetVelocity;
	VectorF2 screenPosition;
	

public:
	Player();
	Player(bool isCircle);
	Player(bool isCircle, int width, int height);
	~Player();

	//void update(int player1Velocity, int player2Velocity, Player* p1, Player* p2);
	//void updateWSAD(float playerVelocity);

	VectorF2 getTargetVelocity();
	VectorI2 getPosition();
	VectorF2 getVelocity();
	int getRadius();

	VectorI2 setPosition(VectorI2& pos);
	VectorF2 setVelocity(VectorF2& vel);
	VectorF2 setTargetVelocity(VectorF2 &vel);
	void setRadius(int radius);

	void updatePlayerPosition();
	void CircleCircleCollision(Player otherPlayer);
	void RectRectCollision(Player otherPlayer);
	void handleWallCollision();
	void separate(Player otherPlayer);
};


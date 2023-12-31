#include "Vector.h"
#include <cmath>

class GameObject
{
private:
	VectorI2 position;
	bool isWall = false;
	bool isCircle = false;
	int r = 0;
	int widht = 0;
	int height = 0;

	void setPosition(VectorI2 position);

public:
	VectorI2 getPosition();

	GameObject() = default;
	GameObject(VectorI2 position, int width, int height, bool isCircle, int r);
	GameObject(VectorI2 position, int width, int height);
	GameObject(VectorI2 position, int widtj, int height, bool isWall);
	
};


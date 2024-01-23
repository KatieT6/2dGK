#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "lib/Circle.h"
#include "lib/MapLoader.h"
#include "lib/Player.h"
#include "lib/Point.h"
#include "lib/Score.h"

/// <summary>
/// trzeba naprawiæ kamere tak ¿eby relatywna pozycja by³a git
/// </summary>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

const int MAP_WIDTH = 60 * 32;
const int MAP_HEIGHT = 40 * 32;
int currentMap = 0;

int maxPoints = 3;
int actualPoints = 0;

int mouseX, mouseY;
int maxX, maxY;

bool isSeparation = false;
bool isBouncing = false;

int velocityOfPlayer = 4;

#pragma region SDL

SDL_Rect *camera;

SDL_Rect* snappingCamera;

SDL_Surface* gScreenSurface = NULL;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* playerTexture = NULL;

SDL_Texture* playerAmongusTexture = NULL;

SDL_Texture* grassTexture = NULL;

SDL_Texture* skyTexture = NULL;

SDL_Texture* wallTexture = NULL;

SDL_Texture* brickTexture = NULL;

SDL_Texture* checkPointTexture = NULL;

#pragma endregion

std::vector<Circle> circles;
std::vector<Wall*> ListWalls;
std::vector<Wall*> ListPoints;

VectorI2 treasurePoints;

std::vector<VectorI2> Positions;
Score score = Score();
VectorI2 normalize(VectorI2 v);

bool first = true;
bool firstTime = true;

bool init();

void drawCircle(Player* p1);
void updateCircle(Player* p1);
void updateCirclesPlayers(std::vector<Player>* player);
void drawCirclesPlayer(std::vector<Player>* player);

void handleWallColission(Player* player);

VectorI2 randPlayerPosition(std::vector<VectorI2> positions);

std::vector<Player> createPlayers(int quantity, int r, bool isCircle);
void drawPlayerWithTextures(SDL_Rect* camera, SDL_Texture* playerTexture, Player* p);
void drawCirclesPlayer(std::vector<Player>* player);

void updateCamera(SDL_Rect* camera, Player* p1, Player* p2, int* targetX, int *targetY);
void cameraInBounds(SDL_Rect* camera);
void playerInBounds(Player* player);
void playerInCameraWidth(SDL_Rect* camera, Player* player1, Player* player2);
bool checkCollision(SDL_Rect* a, SDL_Rect* b);

void DrawDottedLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1);
bool checkIfTreasureIsInCamera(SDL_Rect* camera, VectorI2* treasure);
VectorI2 calculateMidPoint(Player* p1, Player* p2);

void close();

void update(int player1Velocity, int player2Velocity, Player* p1, Player* p2);
void updatePlayersCollision(Player* player1, Player* player2, std::vector<Wall*> walls);
float smoothingMotion(float targetSpeed, float smooth, float velocity);




int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Main loop flag
		bool quit = false;


		//Event handler
		SDL_Event e;
		Uint8 alpha = 128;
		Uint64 currentTime = 0;
		Uint64 lastTime = 0;
		Uint64 deltaTime = 0;
		Uint64 desiredFrameTime = 17;

		
		VectorI2 v = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		Player player1 = Player(32, 32, v);
		Player player2 = Player(32, 32, v);
		//player1.setPosition(v);

		player2.setPosition(v);

		camera = new SDL_Rect
		{
			0,0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT
		};
		snappingCamera = new SDL_Rect
		{
			0, 0,
			SCREEN_WIDTH / 4,
			SCREEN_HEIGHT / 4
		};

		int targetX = 0;
		int targetY = 0;
		//Teksture for player
		playerTexture = loadTextureFromTheSurface("res/textures/player.png", gRenderer);
		playerAmongusTexture = loadTextureFromTheSurface("res/textures/player-amongus.png", gRenderer);
		checkPointTexture = loadTextureFromTheSurface("res/textures/Background/point.png", gRenderer);


		//Tekstures from map loadres
		std::vector<SDL_Texture*> textures;
		grassTexture = loadTextureFromTheSurface("res/textures/Background/Green.png", gRenderer);
		skyTexture = loadTextureFromTheSurface("res/textures/Background/Blue.png", gRenderer);
		wallTexture = loadTextureFromTheSurface("res/textures/Background/Yellow.png", gRenderer);
		brickTexture = loadTextureFromTheSurface("res/textures/Background/Brown.png", gRenderer);
		textures.push_back(grassTexture);
		textures.push_back(skyTexture);
		textures.push_back(wallTexture);
		textures.push_back(brickTexture);
		textures.push_back(checkPointTexture);

		//While application is running
		while (!quit)
		{
			lastTime = currentTime;
			currentTime = SDL_GetPerformanceCounter();
			deltaTime = (currentTime - lastTime) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency());

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					mouseX = e.motion.x;
					mouseY = e.motion.y;
					//printf("We got a motion event.\n");
					//printf("Current mouse position is: (%d, %d)\n", e.motion.x, e.motion.y);
				}
			}

			if (actualPoints != maxPoints)
			{ 
				if (currentMap == 0) {
					std::vector<std::string> levelMap = loadFromFile("res/maps/map0.txt");
				}
				if (currentMap == 1)
				{
					std::vector<std::string> levelMap = loadFromFile("res/maps/map1.txt");
				}
				if (currentMap == 2)
				{
					std::vector<std::string> levelMap = loadFromFile("res/maps/map2.txt");
				}

			}




			//elementsOfMap = loadElementInfoFromFile("res/maps/legend.txt");
			std::vector<std::string> levelMap = loadFromFile("res/maps/map0.txt");


			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0x8B, 0xAC, 0xB7, 0xFF);
			SDL_RenderClear(gRenderer);

			int numberOfColumns = SCREEN_WIDTH / 32;
			int numberOfRows = SCREEN_HEIGHT / 32;
			char sign;
			int x, y;
			int x_m;
			int y_m;
			for (int i = 0; i < MAP_HEIGHT / 32; i++) {
				std::string line = levelMap.at(i);
				for (int j = 0; j < MAP_WIDTH / 32; j++) {
					
					//drawElement((j * 32 - camera->x), (i * 32 - camera->y), line.at(j), textures, gRenderer, j * 32, i * 32);
					sign = line.at(j);
					x = j * 32 - camera->x;
					y = i * 32 - camera->y;
					
					SDL_Rect fillRect = { x, y,  32,  32 };
					VectorI2 position = {j * 32 - 32, i * 32 - 32 };
					//VectorI2* possiblePositions;
					if (sign == ';') {
						//possiblePositions = { x_m, y_m };
						Positions.push_back(position);
						SDL_RenderCopy(gRenderer, textures[0], NULL, &fillRect);
					}
					else if (sign == '#') {
						SDL_RenderCopy(gRenderer, textures[1], NULL, &fillRect);
					}
					else if (sign == '-') {
						SDL_RenderCopy(gRenderer, textures[2], NULL, &fillRect);
					}
					else if (sign == '=') {
						SDL_RenderCopy(gRenderer, textures[3], NULL, &fillRect);
						Wall* wall = new Wall(new VectorI2{ j * 32 - 32, i * 32 - 32}, 32, 32, false);
						ListWalls.push_back(wall);
					}
					else if (sign == 'P')
					{
						SDL_RenderCopy(gRenderer, textures[0], NULL, &fillRect);
						fillRect = { x, y,  32,  32 };
						SDL_RenderCopy(gRenderer, textures[4], NULL, &fillRect);
						Wall* wall = new Wall(new VectorI2{ j * 32 - 32, i * 32 - 32 }, 32, 32, false);
						wall->isPoint = true;
						ListWalls.push_back(wall);
						if (first)
						{
							treasurePoints = { j, i };
						printf("Positions of points: %d, %d \n",  position.x, position.y);
						}
						maxPoints++;
					}
				}
			}

			first = false;
			//VectorI2 rand = randPlayerPosition(Positions);
			VectorI2 rand = { 32, 64 };

			if (firstTime)
			{
				player1.setPosition(rand);
				player2.setPosition(rand);
				firstTime = false;

			}


			/*player1.setPosition(possiblePositions);
			player2.setPosition(possiblePositions);*/
			//updatePlayerPosition(fillRect1, camera, velocityOfRect);
			//update(velocityOfPlayer, velocityOfPlayer, &player1, &player2);
			updatePlayersCollision(&player1, &player2, ListWalls);
			drawPlayerWithTextures(camera, playerTexture, &player1);
			drawPlayerWithTextures(camera, playerAmongusTexture, &player2);
			updateCamera(camera, &player1, &player2, &targetX, &targetY);
			
			if (!checkIfTreasureIsInCamera(camera, &treasurePoints ))
			{
				VectorI2 midPoint = calculateMidPoint(&player1, &player2);
				VectorI2 normalized = normalize(VectorI2{ treasurePoints.x * 32 + 16 - midPoint.x, treasurePoints.y * 32 + 16 - midPoint.y });

				DrawDottedLine(gRenderer, normalized.x + ( midPoint.x + 16 - camera->x), normalized.y + (midPoint.y + 16 - camera->y),
				treasurePoints.x * 32 + 16 - camera->x, treasurePoints.y * 32 + 16 - camera->y);
			}

			playerInCameraWidth(camera, &player1, &player2);
			playerInBounds(&player1);
			playerInBounds(&player2);
			
			cameraInBounds(camera);

			

			//SDL_RenderCopy(gRenderer, playerTexture, NULL, fillRect1);

			//Update screen
			SDL_RenderPresent(gRenderer);
			for (Wall* wall : ListWalls) {
				delete wall; // zwolnij pamiêæ dla ka¿dego obiektu Wall
			}

			ListWalls.clear(); // wyczyœæ wektor

			if (desiredFrameTime > deltaTime)
			{
				SDL_Delay(desiredFrameTime - deltaTime);
			}

			if (maxPoints == actualPoints)
			{
				printf("YOU WON!!");
			}
		}


		//}
	}
	close();

	return 0;
}


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Dwuwymiarowa grafika komputerowa", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{

			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}


void updatePlayersCollision(Player* player1, Player* player2, std::vector<Wall*> walls) {
	update(velocityOfPlayer, velocityOfPlayer, player1, player2);

	for (int j = 0; j < walls.size(); j++)
	{
		//Player player; //placeholder
		//player1->separate(player); /// tu zmieniæ na walls
		//player2->separate(player); /// tu zmieniæ na walls

		//player1->RectRectCollision(walls.at(j));
		player1->RectRectCollision(walls.at(j));
		player2->RectRectCollision(walls.at(j));
	}

		//players->at(i).handleWallCollision();
}

void checkScore(Player* p1, Player* p2) {

}
 
VectorI2 calculateMidPoint(Player* p1, Player* p2) {
	VectorI2 midPoint = { (p1->getPosition().x + p2->getPosition().x) / 2, (p1->getPosition().y + p2->getPosition().y) / 2 };
	return midPoint;
}

VectorI2 normalize(VectorI2 v) {
	float length = sqrt(v.x * v.x + v.y * v.y);
	VectorI2 normalized = { v.x / length, v.y / length };
	return normalized;
}

void DrawDottedLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;
	int count = 0;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	while (1) {
		if (count < 10) { SDL_RenderDrawPoint(renderer, x0, y0); }
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 > dy) { err += dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
		count = (count + 1) % 20;
	}
}




std::vector<Player> createPlayers(int quantity, int r, bool isCircle) {
	Player player;
	std::vector<Player> players;
	for (int i = 0; i < quantity; i++) {
		if (isCircle)
		{
			Player player = Player(true, r, VectorI2{0,0});

		}
		else
		{
			Player player = Player(32, 32, VectorI2{ 0,0 });
		}
		VectorI2 p = { rand() % (SCREEN_WIDTH - 2 * r) + r, rand() % (SCREEN_HEIGHT - 2 * r) + r };
		VectorF2 v = { 4, 4 };
		player.setPosition(p);
		player.setVelocity(v);
		player.setRadius(r);
		players.push_back(player);
	}
	return players;
}

bool checkCollision(SDL_Rect* a, SDL_Rect* b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a->x;
	rightA = a->x + a->w;
	topA = a->y;
	bottomA = a->y + a->h;

	//Calculate the sides of rect B
	leftB = b->x;
	rightB = b->x + b->w;
	topB = b->y;
	bottomB = b->y + b->h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

void updateCirclesPlayers(std::vector<Player>* player) {
	for (int i = 0; i < player->size(); i++) {
		updateCircle(&player->at(i));


		if (isSeparation)
		{
			for (int j = 0; j < player->size(); j++)
			{
				if (i != j) {
					player->at(i).separate(player->at(j));
				}
			}

		}

		if (isBouncing) {
			for (int j = 0; j < player->size(); j++)
			{
				if (i != j) {
					player->at(i).CircleCircleCollision(player->at(j));
				}
			}
		}		

		
		player->at(i).handleWallCollision();
		
	}
}

void drawCirclesPlayer(std::vector<Player> *player) {
	for (int i = 0; i < player->size(); i++) {
		drawCircle(&player->at(i));
	}
}

void handleWallColission(Player* player) {
	if (player->getPosition().x <= 0 + player->getRadius() || player->getPosition().x >= SCREEN_WIDTH - player->getRadius()) {
		VectorF2 v = { -player->getVelocity().x, player->getVelocity().y };
		player->setVelocity(v);
	}
	if (player->getPosition().y <= 0 + player->getRadius() || player->getPosition().y >= SCREEN_HEIGHT - player->getRadius()) {
		VectorF2 v = { player->getVelocity().x, -player->getVelocity().y };
		player->setVelocity(v);
	}
}

VectorI2 randPlayerPosition(std::vector<VectorI2> positions) {
	//losowanie pozycji gracza z listy
	int index = rand() % positions.size();
	VectorI2 position = positions.at(index);
	return position;
}


#pragma region helper methods

void drawPlayer(Player* player) {
	SDL_Rect fillRect = { player->getPosition().x, player->getPosition().y, 32, 32 };
	SDL_RenderFillRect(gRenderer, &fillRect);
}

void drawPlayer(SDL_Rect* camera,Player* p) {
	SDL_Rect fillRect = { p->getPosition().x - camera->x, p->getPosition().y - camera->y, 32, 32 };
	SDL_RenderFillRect(gRenderer, &fillRect);
}

void drawCircle(Player* p1) {
	Circle circle = Circle( { p1->getPosition().x - camera->x, p1->getPosition().y - camera->y }, p1->getRadius());
	circle.drawCircle(gRenderer, 128);
}

void updateCircle(Player* p1) {
	p1->updatePlayerPosition();
	Circle circle = Circle(p1->getPosition(), p1->getRadius());
	circle.updatePosition(p1->getVelocity().x, p1->getVelocity().y);
}

void updateRect(Player* p1) {
	p1->updatePlayerPosition();
}

#pragma endregion

#pragma region old code
//Position locking
void updateCamera(SDL_Rect* camera, Player* p1, Player* p2, int* targetX,int *targetY) {

	//printf("Camera (x, y): %d %d\n", camera->x, camera->y);
	//printf("Player1 (x, y): %d %d\n", p1->getPosition().x, p1->getPosition().y);
	//printf("Player2 (x, y): %d %d\n", p2->getPosition().x, p2->getPosition().y);
	float offsetRight1 = camera->x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3 - p1->getPosition().x - 16;
	float offsetLeft1 = camera->x + SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3 - p1->getPosition().x - 16;
	float offsetRight2 = camera->x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3 - p2->getPosition().x - 16;
	float offsetLeft2 = camera->x + SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3 - p2->getPosition().x - 16;

	float offsetUp1 = camera->y + SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 3 - p1->getPosition().y - 16;
	float offsetDown1 = camera->y + SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3 - p1->getPosition().y - 16;
	float offsetUp2 = camera->y + SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 3 - p2->getPosition().y - 16;
	float offsetDown2 = camera->y + SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3 - p2->getPosition().y - 16;

	float deltaX = p1->getPosition().x - p2->getPosition().x;
	float deltaY = p1->getPosition().y - p2->getPosition().y;
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	//printf("distance x: %f\n", distance);


	if (abs(offsetRight1) >= snappingCamera->x || abs(offsetRight2) >= snappingCamera->x) {
		*targetX = p1->getPosition().x - SCREEN_WIDTH / 2 + 16 - deltaX / 2;

	}
	else if (abs(offsetLeft1) >= snappingCamera->x || abs(offsetLeft2) >= snappingCamera->x) {
		*targetX = p1->getPosition().x - SCREEN_WIDTH / 2 + 16 - deltaX / 2;
	}

	if (abs(offsetDown1) >= snappingCamera->y || abs(offsetDown2) >= snappingCamera->y) {
		*targetY = p1->getPosition().y - SCREEN_HEIGHT / 2 + 16 - deltaY / 2;
	}
	else if (abs(offsetUp1) >= snappingCamera->y || abs(offsetUp2) >= snappingCamera->y) {
		*targetY = p1->getPosition().y - SCREEN_HEIGHT / 2 + 16 - deltaY / 2;
	}

	float smooth = 0.92f;
	camera->x = *targetX * (1.0f - smooth) + camera->x * smooth;
	camera->y = *targetY * (1.0f - smooth) + camera->y * smooth;
	
}

bool checkIfTreasureIsInCamera(SDL_Rect* camera, VectorI2* treasure) {
	if (treasure->x * 32 - camera->x >= camera->x + SCREEN_WIDTH - 32 || treasure->x * 32 - camera->x <= camera->x ||
		treasure->y * 32 - camera->y >= camera->y + SCREEN_HEIGHT - 32 || treasure->y * 32 - camera->y <= camera->y)
	{
			return false;
	}
	else
	{
		return true;
	}

}

//Edge snapping
void cameraInBounds(SDL_Rect* camera) {
	if (camera->x < 0) {
		camera->x = 0;
	}
	if (camera->y < 0) {
		camera->y = 0;
	}
	if (camera->x > MAP_WIDTH - SCREEN_WIDTH) {
		camera->x = MAP_WIDTH - SCREEN_WIDTH;
	}
	if (camera->y > MAP_HEIGHT - SCREEN_HEIGHT) {
		camera->y = MAP_HEIGHT - SCREEN_HEIGHT;
	}
}

void playerInBounds(Player* player) {
	if (player->getPosition().x < 0) {
		VectorI2 v = { 0, player->getPosition().y };
		player->setPosition(v);
	}
	if (player->getPosition().y < 0) {
		VectorI2 v = { player->getPosition().x, 0 };
		player->setPosition(v);
	}
	if (player->getPosition().x > MAP_WIDTH - 32) {
		VectorI2 v = { MAP_WIDTH - 32, player->getPosition().y };
		player->setPosition(v);
	}
	if (player->getPosition().y > MAP_HEIGHT - 32) {
		VectorI2 v = { player->getPosition().x, MAP_HEIGHT - 32 };
		player->setPosition(v);
	}
}


void update(int player1Velocity, int player2Velocity, Player* p1, Player* p2)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	VectorI2 position;

	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		position = { p1->getPosition().x, p1->getPosition().y - player1Velocity };
		p1->setPosition(position);

		//printf("We got a motion event.\n UP buton pressed");
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		position = { p1->getPosition().x, p1->getPosition().y + player1Velocity };
		p1->setPosition(position);
		//printf("We got a motion event.\n DOWN buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		position = { p1->getPosition().x - player1Velocity, p1->getPosition().y };
		p1->setPosition(position);
		//printf("We got a motion event.\n LEFT buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		position = { p1->getPosition().x + player1Velocity, p1->getPosition().y };
		p1->setPosition(position);
		//printf("We got a motion event.\n RIGHT buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_W])
	{
		position = { p2->getPosition().x , p2->getPosition().y - player2Velocity };
		p2->setPosition(position);
		//printf("We got a motion event.\n UP buton pressed");
	}

	if (currentKeyStates[SDL_SCANCODE_S])
	{
		position = { p2->getPosition().x , p2->getPosition().y + player2Velocity };
		p2->setPosition(position);
		//printf("We got a motion event.\n DOWN buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_A])
	{
		position = { p2->getPosition().x - player2Velocity, p2->getPosition().y };
		p2->setPosition(position);
		//printf("We got a motion event.\n LEFT buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_D])
	{
		position = { p2->getPosition().x + player2Velocity, p2->getPosition().y };
		p2->setPosition(position);
		//printf("We got a motion event.\n RIGHT buton pressed");

	}



}



float smoothingMotion(float targetSpeed, float smooth, float velocity) {
	return targetSpeed * (1 - smooth) + velocity * smooth;
}

void drawPlayerWithTextures(SDL_Rect* camera, SDL_Texture* playerTexture, Player* p) {
	SDL_Rect fillRect = {p->getPosition().x -camera->x, p->getPosition().y - camera->y, 32, 32};
	SDL_RenderCopy(gRenderer, playerTexture, NULL, &fillRect);
}


void playerInCameraWidth(SDL_Rect* camera, Player* player1, Player* player2) {
	if (player1->getPosition().x >= camera->x + SCREEN_WIDTH - 32) {
		VectorI2 v = { camera->x + SCREEN_WIDTH - 32, player1->getPosition().y };
		player1->setPosition(v);
	}
	if (player1->getPosition().x <= camera->x + 8) {
		VectorI2 v = { camera->x + 8 , player1->getPosition().y };
		player1->setPosition(v);
	}

	if (player2->getPosition().x >= camera->x + SCREEN_WIDTH - 32) {
		VectorI2 v = { camera->x + SCREEN_WIDTH - 32, player2->getPosition().y };
		player2->setPosition(v);
	}
	if (player2->getPosition().x <= camera->x + 8 ) {
		VectorI2 v = { camera->x + 8, player2->getPosition().y };
		player2->setPosition(v);
	}
	if (player1->getPosition().y >= camera->y + SCREEN_HEIGHT - 32) {
				VectorI2 v = { player1->getPosition().x, camera->y + SCREEN_HEIGHT - 32 };
		player1->setPosition(v);
	}
	if (player1->getPosition().y <= camera->y + 32) {
		VectorI2 v = { player1->getPosition().x, camera->y + 8 };
		player1->setPosition(v);
	}

	if (player2->getPosition().y >= camera->y + SCREEN_HEIGHT - 32) {
		VectorI2 v = { player2->getPosition().x, camera->y + SCREEN_HEIGHT - 32 };
		player2->setPosition(v);
	}
	if (player2->getPosition().y <= camera->y + 32) {
		VectorI2 v = { player2->getPosition().x, camera->y + 8 };
		player2->setPosition(v);
	}

}

#pragma endregion

void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}


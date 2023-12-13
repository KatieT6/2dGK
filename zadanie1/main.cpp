#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "lib/Circle.h"
#include "lib/MapLoader.h"
#include "lib/Player.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

int mouseX, mouseY;
int maxX, maxY;

bool isSeparation = false;
bool isBouncing = false;


SDL_Rect *camera;

SDL_Rect* snappingCamera;

SDL_Surface* gScreenSurface = NULL;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;


std::vector<Circle> circles;

bool init();

void drawCircle(Player* p1);
void updateCircle(Player* p1);
void updateCirclesPlayers(std::vector<Player>* player);
void drawCirclesPlayer(std::vector<Player>* player);

void handleWallColission(Player* player);

std::vector<Player> createPlayers(int quantity, int r);


void close();

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

		camera = new SDL_Rect
		{
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			SCREEN_WIDTH,
			SCREEN_HEIGHT
		};

		std::vector<Player> players = createPlayers(10, 32);

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
				else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_1:
						if (isSeparation)
						{
							isSeparation = false;
							printf("Separation: OFF.\n");
						}
						else if (!isSeparation)
						{
							isSeparation = true;
							printf("Separation: ON.\n");
						}
						break;

					case SDLK_2:
						if (isBouncing)
						{
							isBouncing = false;
							printf("Bouncing: OFF.\n");
						}
						else if (!isBouncing)
						{
							isBouncing = true;
							printf("Bouncing: ON.\n");
						}
						break;
					}
				
				}

					
			}

				
			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0x8B, 0xAC, 0xB7, 0xFF);
			SDL_RenderClear(gRenderer);

			updateCirclesPlayers(&players);
			drawCirclesPlayer(&players);

			//Update screen
			SDL_RenderPresent(gRenderer);

			if (desiredFrameTime > deltaTime)
			{
				SDL_Delay(desiredFrameTime - deltaTime);
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



std::vector<Player> createPlayers(int quantity, int r) {
	std::vector<Player> players;
	for (int i = 0; i < quantity; i++) {
		Player player = Player();
		VectorI2 p = { rand() % (SCREEN_WIDTH - 2 * r) + r, rand() % (SCREEN_HEIGHT - 2 * r) + r };
		VectorF2 v = { 4, 4 };
		player.setPosition(p);
		player.setVelocity(v);
		player.setRadius(r);
		players.push_back(player);
	}
	return players;
}

void updateCirclesPlayers(std::vector<Player>* player) {
	for (int i = 0; i < player->size(); i++) {
		//player->at(i).updatePlayerPosition();
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
					player->at(i).handleCollision(player->at(j));
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



#pragma region helper methods

void drawPlayer(Player* player) {
	SDL_Rect fillRect = { player->getPosition().x, player->getPosition().y, 32, 32 };
	SDL_RenderFillRect(gRenderer, &fillRect);
}

void drawCircle(Player* p1) {
	Circle circle = Circle(p1->getPosition(), p1->getRadius());
	circle.drawCircle(gRenderer, 128);

}

void updateCircle(Player* p1) {
	p1->updatePlayerPosition();
	Circle circle = Circle(p1->getPosition(), p1->getRadius());
	circle.updatePosition(p1->getVelocity().x, p1->getVelocity().y);
}

#pragma endregion

#pragma region old code
//Position locking
void updateCamera(SDL_Rect* camera, Player* p1, Player* p2, int* target) {

	printf("Camera x: %d\n", camera->x);
	printf("Player1 x: %d\n", p1->getPosition().x);
	printf("Player2 x: %d\n", p2->getPosition().x);
	float offsetRight1 = camera->x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3 - p1->getPosition().x - 16;
	float offsetLeft1 = camera->x + SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3 - p1->getPosition().x - 16;
	float offsetRight2 = camera->x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3 - p2->getPosition().x - 16;
	float offsetLeft2 = camera->x + SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3 - p2->getPosition().x - 16;

	float deltaX = p1->getPosition().x - p2->getPosition().x;
	float deltaY = p1->getPosition().y - p2->getPosition().y;
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	printf("distance x: %f\n", distance);


	if (abs(offsetRight1) >= snappingCamera->x || abs(offsetRight2) >= snappingCamera->x) {
		*target = p1->getPosition().x - SCREEN_WIDTH / 2 + 16 - deltaX / 2;

	}
	else if (abs(offsetLeft1) >= snappingCamera->x || abs(offsetLeft2) >= snappingCamera->x) {
		*target = p1->getPosition().x - SCREEN_WIDTH / 2 + 16 - deltaX / 2;
	}

	float smooth = 0.92f;
	camera->x = *target * (1.0f - smooth) + camera->x * smooth;
	camera->y = p1->getPosition().y + 16 - SCREEN_HEIGHT / 2;

}


void update(int player1Velocity, int player2Velocity, Player* p1, Player* p2)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	VectorI2 position;

	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		position = { p1->getPosition().x, p1->getPosition().y - player1Velocity };
		p1->setPosition(position);

		printf("We got a motion event.\n UP buton pressed");
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		position = { p1->getPosition().x, p1->getPosition().y + player1Velocity };
		p1->setPosition(position);
		printf("We got a motion event.\n DOWN buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		position = { p1->getPosition().x - player1Velocity, p1->getPosition().y };
		p1->setPosition(position);
		printf("We got a motion event.\n LEFT buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		position = { p1->getPosition().x + player1Velocity, p1->getPosition().y };
		p1->setPosition(position);
		printf("We got a motion event.\n RIGHT buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_W])
	{
		position = { p2->getPosition().x , p2->getPosition().y - player2Velocity };
		p2->setPosition(position);
		printf("We got a motion event.\n UP buton pressed");
	}

	if (currentKeyStates[SDL_SCANCODE_S])
	{
		position = { p2->getPosition().x , p2->getPosition().y + player2Velocity };
		p2->setPosition(position);
		printf("We got a motion event.\n DOWN buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_A])
	{
		position = { p2->getPosition().x - player2Velocity, p2->getPosition().y };
		p2->setPosition(position);
		printf("We got a motion event.\n LEFT buton pressed");

	}

	if (currentKeyStates[SDL_SCANCODE_D])
	{
		position = { p2->getPosition().x + player2Velocity, p2->getPosition().y };
		p2->setPosition(position);
		printf("We got a motion event.\n RIGHT buton pressed");

	}

}

float smoothingMotion(float targetSpeed, float smooth, float velocity) {
	return targetSpeed * (1 - smooth) + velocity * smooth;
}

void drawPlayer(SDL_Rect* camera, SDL_Texture* playerTexture, Player* p) {
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


	if (player2->getPosition().y <= camera->y) {
		VectorI2 v = { player2->getPosition().x, camera->y };
		player2->setPosition(v);
	}
	if (player2->getPosition().y >= camera->y + SCREEN_HEIGHT - 32) {
		VectorI2 v = { player2->getPosition().x, camera->y + SCREEN_HEIGHT - 32 };
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


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

const int MAP_WIDTH = 60*32;
const int MAP_HEIGHT = 40*32;

int mouseX, mouseY;
int maxX, maxY;


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


bool init();
//void updatePlayerPosition(SDL_Rect* fillRect1, SDL_Rect* camera, float velocityOfRect);
void updateCamera(SDL_Rect* camera, Player* p, Player* p2, int* target);
void cameraInBounds(SDL_Rect* camera);
void playerInCameraWidth(SDL_Rect* camera, Player* player1, Player* player2);

void update(int player1Velocity, int player2Velocity, Player* p1, Player* p2);

void drawPlayer(SDL_Rect* camera, SDL_Texture* playerTexture, Player* p);
void playerInBounds(Player* player);

//bool loadTextures();

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

		int velocityOfPlayer = 4;
		Player player1 = Player();
		VectorI2 v = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		player1.setPosition(v);

		Player player2 = Player();
		player2.setPosition(v);

		snappingCamera = new SDL_Rect
		{
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			SCREEN_WIDTH / 4,
			SCREEN_HEIGHT / 4
		};
		
		int target = 0;
		//Teksture for player
		playerTexture = loadTextureFromTheSurface("res/textures/player.png", gRenderer);
		playerAmongusTexture = loadTextureFromTheSurface("res/textures/player-amongus.png", gRenderer);


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
					printf("We got a motion event.\n");
					printf("Current mouse position is: (%d, %d)\n", e.motion.x, e.motion.y);
				}

			}

			
			

			//elementsOfMap = loadElementInfoFromFile("res/maps/legend.txt");
			std::vector<std::string> levelMap = loadFromFile("res/maps/map1.txt");

		
			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0x8B, 0xAC, 0xB7, 0xFF);
			SDL_RenderClear(gRenderer);

			int numberOfColumns = SCREEN_WIDTH / 32;
			int numberOfRows = SCREEN_HEIGHT / 32;

			for (int i = 0; i < MAP_HEIGHT / 32; i++) {
				std::string line = levelMap.at(i);
				for (int j = 0; j < MAP_WIDTH / 32; j++) {
					drawElement((j * 32 - camera->x), (i * 32 - camera->y), line.at(j), textures, gRenderer);
				}
			}
			//updatePlayerPosition(fillRect1, camera, velocityOfRect);
			update(velocityOfPlayer ,velocityOfPlayer, &player1, &player2);
			drawPlayer(camera, playerTexture, &player1);
			drawPlayer(camera, playerAmongusTexture, &player2);
			
			updateCamera(camera, &player1, &player2, &target);
			playerInCameraWidth(camera, &player1, &player2);
			playerInBounds(&player1);
			playerInBounds(&player2);
			cameraInBounds(camera);

			
			
			//SDL_RenderCopy(gRenderer, playerTexture, NULL, fillRect1);

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






void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}


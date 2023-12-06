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

bool isColisions = false;
bool isSeparation = false;
bool isBouncing = false;


SDL_Rect *camera;

SDL_Rect* snappingCamera;

SDL_Surface* gScreenSurface = NULL;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

std::vector<Circle> createCircles(int quantity, int r);
void drawCircles(std::vector<Circle> circles);

std::vector<Circle> circles;

bool init();

void updareCirclesPosition(std::vector<Circle> circles);

void ciecleInBounds(Circle* circle);


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

	    circles = createCircles(10, 16);

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

					case SDLK_3:
						if (isColisions)
						{
							isColisions = false;
							printf("Bouncing: OFF.\n");
						}
						else if (!isColisions)
						{
							isColisions = true;
							printf("Bouncing: ON.\n");
						}
						break;
					}
					
				}

				updareCirclesPosition(circles);

			}

		
			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0x8B, 0xAC, 0xB7, 0xFF);
			SDL_RenderClear(gRenderer);

			

			drawCircles(circles);


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

std::vector<Circle> createCircles(int quantity, int r) {
	std::vector<Circle> circles;
	for (int i = 0; i < quantity; i++) {
		VectorF2 vel = { 10, 10};
		VectorI2 pos = { rand() % (SCREEN_WIDTH - 2 * r) + r, rand() % (SCREEN_HEIGHT - 2 * r) + r };
		Circle circle = Circle(pos, vel , r);	
		circles.push_back(circle);
	}

	return circles;
}

void drawCircles(std::vector<Circle> circles) {
	for (int i = 0; i < circles.size(); i++) {
		circles[i].drawCircle(gRenderer, 200);
	}
}

void updareCirclesPosition(std::vector<Circle> circles) {
	for (int i = 0; i < circles.size(); i++) {
		circles[i].updatePosition();
	}
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
void CiercleInBounds(Circle *circle) {

}

void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}


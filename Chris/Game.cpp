// Game.cpp -- main game functions

#include "Game.h"
#include <iostream>

// Default constructor
Game::Game()
{
	windowWidth = 640;
	windowHeight = 480;

	Running = true;

	Display = NULL;
	SpriteSheet = NULL;
	currentLevel = NULL;
};

// Game setup
bool Game::Init()
{
	// Init SDL stuff
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	// Init the display surface
	Display = SDL_SetVideoMode(windowWidth, windowHeight, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if (!Display)
		return false;

	// Load the sprite sheet
	SpriteSheet = LoadImage("resources/sprites.bmp");

	if (!SpriteSheet)
		return false;

	// Set sprite transparency to RGB(255, 0, 255) AKA magenta
	SDL_SetColorKey(SpriteSheet, SDL_SRCCOLORKEY | SDL_RLEACCEL,
			SDL_MapRGB(SpriteSheet->format, 255, 0, 255));


	// Create a new level
	currentLevel = new Level;
	if (!currentLevel->Init("resources/lvl0.txt"))
		return false;

	return true;
};

// Main execution loop
int Game::Run()
{
	if (!Init())
		return -1;

	// Event queue
	SDL_Event Event;

	while (Running)
	{
		while (SDL_PollEvent(&Event))
			OnEvent(&Event);

		Update();
		Render();
	}

	Cleanup();

	return 0;
};

// Event processing
void Game::OnEvent(SDL_Event *Event)
{
	switch (Event->type)
	{
		case SDL_QUIT:
			OnQuit();
			break;
		case SDL_KEYDOWN:
			OnKeyDown(Event->key.keysym.sym);
			break;
		case SDL_MOUSEMOTION:
			OnMouseMove(Event->motion.x, Event->motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (Event->button.button)
			{
				case SDL_BUTTON_LEFT:
					OnLClick( Event->button.x,
						  Event->button.y);
					break;
				case SDL_BUTTON_RIGHT:
					OnRClick( Event->button.x,
						  Event->button.y);
					break;
				case SDL_BUTTON_MIDDLE:
					OnMClick( Event->button.x,
						  Event->button.y);
					break;
			}
			break;
	};
};

// Stuff to update each frame
void Game::Update()
{

};

// Draw stuff
void Game::Render()
{
	// Draw the base grid
	for (unsigned int i = 0; i < currentLevel->Grid.size(); i++)
	{
		for (unsigned int j = 0; j < currentLevel->Grid[0].size(); j++)
		{
			Draw(Display, SpriteSheet, 50*j, 50*i,
					currentLevel->Grid[i][j]*50, 0, 50, 50);
		}
	}

	// ADD TOWERS HERE
	// ADD ENEMIES HERE

	// Highlight current mouse position
	DrawRect(Display, 50*(Mouse.x/50), 50*(Mouse.y/50), 50, 50,
				255, 255, 0, 64);

	// ADD MENU ITEMS HERE

	// Draw the display to screen (double buffer)
	SDL_Flip(Display);
};

// Cleanup before exiting, a bit like a deconstructor
void Game::Cleanup()
{
	SDL_FreeSurface(Display);
	SDL_FreeSurface(SpriteSheet);

	delete currentLevel;

	// Cleanup SDL stuff
	SDL_Quit();
};

// Utility to load and format a BMP image
SDL_Surface* Game::LoadImage(char* filename)
{
	SDL_Surface* Temp = NULL;
	SDL_Surface* Return = NULL;

	Temp = SDL_LoadBMP(filename);

	if (!Temp)
		return NULL;

	Return = SDL_DisplayFormat(Temp);
	SDL_FreeSurface(Temp);

	return Return;
};

// Blit an entire surface to another surface
bool Game::Draw(SDL_Surface* Dest, SDL_Surface* Src, int x, int y)
{
	if (!Dest || !Src)
		return false;

	// Destination rect
	SDL_Rect DestRect;

	DestRect.x = x;
	DestRect.y = y;

	// Draw
	SDL_BlitSurface(Src, NULL, Dest, &DestRect);

	return true;
};

// Blit part of a surface to another surface
bool Game::Draw(SDL_Surface* Dest, SDL_Surface* Src,
		int x1, int y1, int x2, int y2, int w, int h)
{
	if (!Dest || !Src)
		return false;

	// Destination rect
	SDL_Rect DestRect;

	DestRect.x = x1;
	DestRect.y = y1;

	// Source rect
	SDL_Rect SrcRect;

	SrcRect.x = x2;
	SrcRect.y = y2;
	SrcRect.w = w;
	SrcRect.h = h;

	// Draw
	SDL_BlitSurface(Src, &SrcRect, Dest, &DestRect);

	return true;
};

// Draw a rect on the screen; standard functional doesn't allow alpha blending
bool Game::DrawRect(SDL_Surface* Dest, int x, int y, int w, int h,
		int r, int g, int b, int a)
{
	if (!Dest)
		return false;
	SDL_Surface *tempSurf;	// Make a new surface
	tempSurf = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
			0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

	if (!tempSurf)
		return false;

	// Fill the entire surface with the given color;
	SDL_FillRect(tempSurf, NULL, SDL_MapRGBA(tempSurf->format, r, g, b, a));

	// Blit the surface
	Draw(Dest, tempSurf, x, y);	

	// Free the surface
	SDL_FreeSurface(tempSurf);

	return true;
};


//////////////////////////////// Events ////////////////////////////////////

// End the game
void Game::OnQuit()
{
	Running = false;
};

// Process key press
void Game::OnKeyDown(SDLKey sym)
{
	switch (sym)
	{
		case SDLK_SPACE:
			DrawRect(Display,
				Mouse.x-25, Mouse.y-25, 50, 50,
				255, 255, 0, 64);
			break;
		case SDLK_q:
			OnQuit();
			break;
		default:
			break;
	};
};

// Process left click
void Game::OnLClick(int x, int y)
{
	Draw(Display, SpriteSheet, x - 25, y - 25, 0, 0, 50, 50);
};

// Process right click
void Game::OnRClick(int x, int y)
{
	Draw(Display, SpriteSheet, x - 25, y - 25, 50, 0, 50, 50);
};

// Process middle click
void Game::OnMClick(int x, int y)
{
	Draw(Display, SpriteSheet, x - 25, y - 25, 100, 0, 50, 50);
};

// Trigger on mouse movement
void Game::OnMouseMove(int x, int y)
{
	// Store the new mouse position
	Mouse.x = x;
	Mouse.y = y;
};

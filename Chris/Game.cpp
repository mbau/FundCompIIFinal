// Game.cpp
// Main game functions

#include "Game.h"
#include "Surface.h"	// Graphics utilities

// Default constructor
Game::Game()
{
	windowWidth = 640;
	windowHeight = 480;

	Running = true;

	Display = NULL;
	SpriteSheet = NULL;
	currentLevel = NULL;

	Clicked = Hovered = NULL;
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
	SpriteSheet = Surface::LoadImage("resources/sprites.bmp");

	if (!SpriteSheet)
		return false;

	// Set sprite transparency to RGB(255, 0, 255) AKA magenta
	SDL_SetColorKey(SpriteSheet, SDL_SRCCOLORKEY | SDL_RLEACCEL,
			SDL_MapRGB(SpriteSheet->format, 255, 0, 255));


	// Create a new level
	currentLevel = new Level(Display, SpriteSheet);
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
	// Clear Screen
//	Surface::DrawRect(Display, 0, 0, Display->w, Display->h,
//			255, 255, 255, 255);
	currentLevel->Render();
	// ADD TOWERS HERE
	// ADD ENEMIES HERE

	// Highlight current mouse position
//	Surface::DrawRect(Display, 50*(Mouse.x/50), 50*(Mouse.y/50), 50, 50,
//				255, 255, 0, 64);

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
			Surface::DrawRect(Display,
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
	currentLevel->Grid[y/50][x/50].OnClick();
};

// Process right click
void Game::OnRClick(int x, int y)
{
	Surface::Draw(Display, SpriteSheet, x - 25, y - 25, 50, 0, 50, 50);
};

// Process middle click
void Game::OnMClick(int x, int y)
{
	Surface::Draw(Display, SpriteSheet, x - 25, y - 25, 100, 0, 50, 50);
};

// Trigger on mouse movement
void Game::OnMouseMove(int x, int y)
{
	// Store the new mouse position
	Mouse.x = x;
	Mouse.y = y;

	if (Hovered)
	{
		if (	(Mouse.x < Hovered->Rect.x) || 
			(Mouse.x > Hovered->Rect.x + Hovered->Rect.w) ||
			(Mouse.y < Hovered->Rect.y) ||
			(Mouse.y > Hovered->Rect.y + Hovered->Rect.h)	)
		{
			Hovered->OnUnHover();
		}
	}
	unsigned int i = Mouse.x/50;
	unsigned int j = Mouse.y/50;

	if (i < currentLevel->Grid[0].size() && j < currentLevel->Grid.size())
	{
		Hovered = &(currentLevel->Grid[y/50][x/50]);
		Hovered->OnHover();
	}
	else
	{
		Hovered = NULL;
	}
};

// Game.cpp
// Main game functions

#include "Game.h"
#include "Surface.h"	// Graphics utilities
#include <math.h>
#include <iostream>
using namespace std;

const double FRAMETIME = 1./60.; //Framerate for animation

// Default constructor
Game::Game(string lvlID) 
{
	windowWidth = 600; // Default window size, overwritten in Init
	windowHeight = 400;

	Running = true; 

	menu.On = false;

	levelID = lvlID;
};

// Game setup
bool Game::Init()
{
	// Init all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	// Create a new level
	currentLevel = new Level();
	string filename = "resources/lvl"+levelID+".txt";
	if (!currentLevel->Init(filename.c_str()))
		return false;

	Surface::Padding = TILESIZE;
	windowWidth = TILESIZE * currentLevel->Grid[0].size()
				+ Surface::Padding*2;//gets width from the file input grid vector
	windowHeight = TILESIZE * currentLevel->Grid.size()
				+ Surface::Padding*2;//same for height

	// Init the display surface, screen
	Surface::Display = SDL_SetVideoMode(windowWidth, windowHeight, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	SDL_WM_SetCaption("Save the Fort",NULL);//title for window

	if (!Surface::Display) //if there was an error in setting up the screen
		return false;

	// Load the sprite sheet
	Surface::SpriteSheet = Surface::LoadImage("resources/sprites.bmp");//get sprites for gfx

	if (!Surface::SpriteSheet) //if there was an error in getting pictures
		return false;

	Surface::IconSheet = Surface::LoadImage("resources/icons.bmp");//get icons

	if (!Surface::IconSheet) //if there was an error in the getting pictures
		return false;

	// Set sprite transparency to RGB(255, 255, 255) AKA white
	SDL_SetColorKey(Surface::SpriteSheet, SDL_SRCCOLORKEY | SDL_RLEACCEL,
			SDL_MapRGB(Surface::SpriteSheet->format, 255, 255, 255));//for the sprites

	SDL_SetColorKey(Surface::IconSheet, SDL_SRCCOLORKEY | SDL_RLEACCEL,
			SDL_MapRGB(Surface::IconSheet->format, 255, 255, 255));//for the icons

	return true;//returns true if everything is okay
};

// Main execution loop
int Game::Run()
{
	if (!Init())
		return -1;

	// Event queue
	SDL_Event Event;

	while (Running)//while the program in running
	{
		while (SDL_PollEvent(&Event))//sends event to be handled by OnEvent
			OnEvent(&Event);

		Update();//updates positions for enemies, bullets, etc.
		Render();//renders all graphics

		// Check for end of game
		if(currentLevel->isGameOver())
			GameOver();

		// Framerate limiter
		Wait();
	}

	Cleanup();//cleans up sdl surfaces

	return 0;
};

// Event processing
void Game::OnEvent(SDL_Event *Event)
{
	switch (Event->type)//switch based on what the input is
	{
		case SDL_QUIT://if the x on window is pressed
			OnQuit();//quits
			break;//exit switch
		case SDL_KEYDOWN://key is pressed down
			OnKeyDown(Event->key.keysym.sym);//sent to key down function
			break;//exit switch
		case SDL_MOUSEMOTION://mouse is moved
			OnMouseMove(Event->motion.x, Event->motion.y);//sent to mouse function
			break;//exit
		case SDL_MOUSEBUTTONDOWN://mouse button pressed down
			switch (Event->button.button)//switch based on which button is pressed down
			{
				case SDL_BUTTON_LEFT://left mouse botton
					OnLClick( Event->button.x,//left click function
						  Event->button.y);
					break;
				case SDL_BUTTON_RIGHT://right click
					OnRClick( Event->button.x,//right click function
						  Event->button.y);
					break;
				case SDL_BUTTON_MIDDLE://middle click (wheel)
					OnMClick( Event->button.x,//middle click funtion 
						  Event->button.y);
					break;
			}
			break;
		case SDL_MOUSEBUTTONUP://mouse button is release after click
			switch(Event->button.button)
			{
				case SDL_BUTTON_LEFT:
					OnLClickRelease(Event->button.x,
							Event->button.y);
					break;
				case SDL_BUTTON_RIGHT://right click
					OnRClickRelease(Event->button.x,//right click release function
							Event->button.y);
					break;
			}
			break;
	};
};

// Stuff to update each frame
void Game::Update()
{
	currentLevel->Update(FRAMETIME);
};

// Draw stuff
void Game::Render()
{
	// Clear Screen
//	Surface::DrawRect(Display, 0, 0, Display->w, Display->h,
//			255, 255, 255, 255);

	currentLevel->Render();//Render() pointed to currentLevel
	drawMargins();//draw margins (so controls will be visible everywhere)
	currentLevel->RenderHUD();//RenderHUD() pointed to currentLevel

	drawMenu();//draw menu function

	// Flip the display to screen (double buffer)
	SDL_Flip(Surface::Display);
};

void Game::drawMargins()//draws margins on the sides so controls will always be visible
{
	SDL_Rect rect[4] = {//array of 4 sdl rects
	{0, 0, windowWidth, Surface::Padding},//top margin
	{windowWidth-Surface::Padding, 0, Surface::Padding, windowHeight},//right margin
	{0, 0, Surface::Padding, windowHeight},//left margin
	{0, windowHeight-Surface::Padding,windowWidth, Surface::Padding}};//bottom margin
	for (int i = 0; i < 4; i++)//fills the rects with gray
	{
		SDL_FillRect(Surface::Display, &rect[i], SDL_MapRGB(
				Surface::Display->format,128,128,128));
	}
};

//draw pie menu
void Game::drawMenu()
{
	if (menu.On)//if the menu is on
	{
		int state = getMenuState();//get the which section the mouse is in
		double theta1, theta2, rad;
		theta1 = theta2 = 0;
		rad = 100;
		switch (state)
		{
			case 1:
				theta1 = 225;//section on left
				theta2 = 315;//same as -45
				break;
			case 2:
				theta1 = -45;//section on top
				theta2 = 45;
				break;
			case 3:
				theta1 = 45;//section on right
				theta2 = 135;
				break;
			case 4:
				theta1 = 135;//section bottom
				theta2 = 225;
				break;
			case 0://not in a proper state
			default:
				break;
		}

		filledCircleRGBA(Surface::Display,menu.x, menu.y, rad, 255, 255, 255, 64);//circle menu location and color
		
		// Highlight
		if (state != 0)//if in proper state
			filledPieRGBA(Surface::Display, menu.x, menu.y,//highlight the sector the mouse is in
					rad, theta1, theta2, 255, 255, 0, 128);

		// Draw Icons
		double offset = 3*rad/4;
		int x, y, icon[4] = {-1, -1, -1, -1};
		x = menu.x - Surface::Padding - ICONSIZE/2;///start points for the icons
		y = menu.y - Surface::Padding - ICONSIZE/2;

		if (menu.target)
		{
			switch (menu.target->type)//icon locations if something is already on the tile
			{
				case 0:
					icon[0] = DAMAGE;	// Top
					break;
				case 1:
					icon[0] = FREEZE;	
					break;
				default:
					icon[0] = DAMAGE;
					break;
			}
			//upgrades
			icon[1] = RANGE;	// Right
			icon[2] = SELL; 	// Bottom
			icon[3] = RATE;		// Left
		}
		else if (currentLevel->isValid(currentLevel->MouseGrid.x,
						currentLevel->MouseGrid.y))//if there is nothing on the screen
		{
			icon[0] = NORMAL;//normal turret
			icon[1] = SLOW;//slow turret
			icon[2] = -1;
			icon[3] = -1;
		}
		for (int i = 0; i < 4; i++) 
		{
			double angle = (i-1)*M_PI/2;
			Surface::Draw(Surface::Display, Surface::IconSheet,//draws the actual menu
				x + offset*cos(angle),
				y + offset*sin(angle),
				icon[i]*ICONSIZE, 0,//draws the icons
				ICONSIZE, ICONSIZE);
		}
	}
};

int Game::getMenuState()
{	
	int numState = 4;//4 states
	int diffX = Mouse.x - menu.x;//difference between mouse loc and menu loc
	int diffY = Mouse.y - menu.y;
	if (diffX*diffX + diffY*diffY < 50*50)
		return 0;	// Inactive selection, mouse is too far away
	else
	{
		double angle = 180*atan2(diffY,diffX)/M_PI + 180;//angle for between mouse and center
		int value = (angle/(360/numState))+.5;//which section of circle it is in
		if (value < 1)//accounts for numbers smaller than 1
			value += 4;

		return value;
	};

};

// Cleanup before exiting, a bit like a deconstructor
void Game::Cleanup()
{
	SDL_FreeSurface(Surface::Display);//free all surfaces
	SDL_FreeSurface(Surface::SpriteSheet);

	delete currentLevel;//gets rid of the current game

	// Cleanup SDL stuff
	SDL_Quit();
};





//////////////////////////////// Events ////////////////////////////////////

// End the game
void Game::OnQuit()
{
	Running = false;//quit
};

// Process key press
void Game::OnKeyDown(SDLKey sym)//event key pressed down
{
	switch (sym)
	{
		case SDLK_q://q key
			OnQuit();//quits
			break;
		default:
			break;
	};
};

// Process left click
void Game::OnLClick(int x, int y)
{
	// Alias to right click
	OnRClick(x,y);
};

// Process click release
void Game::OnLClickRelease(int x, int y)
{
	// Alias to right release
	OnRClickRelease(x,y);
};

// Process right click
void Game::OnRClick(int x, int y)
{
	if (!menu.On &&
		currentLevel->MouseGrid.x >= 0 && currentLevel->MouseGrid.y >= 0
		&& currentLevel->MouseGrid.y < (int)currentLevel->Grid.size() &&
		currentLevel->MouseGrid.x < (int)currentLevel->Grid[0].size() )
	{
		menu.On = true;//turn menu on
		menu.x = (.5 + currentLevel->MouseGrid.x)*TILESIZE + Surface::Padding;//start menu at the locations
		menu.y = (.5 + currentLevel->MouseGrid.y)*TILESIZE + Surface::Padding;
		menu.target = currentLevel->isTower(currentLevel->MouseGrid.x,
						currentLevel->MouseGrid.y);
	};
};

void Game::OnRClickRelease(int x, int y)//release of the mouse
{
	if (menu.On)
	{
		if (menu.target)	// Tower selected:
		{
			switch (getMenuState())
			{
			case 1:	// Power
				currentLevel->UpgradeTower(menu.target, 0);//upgrade
				break;
			case 2:	// Range
				currentLevel->UpgradeTower(menu.target, 1);
				break;
			case 3: // Sell
				currentLevel->DestroyTower(menu.target);
				break;
			case 4:	// Rate
				currentLevel->UpgradeTower(menu.target, 2);
				break;
			}
		}
		else	// Not a tower, can place one there
		{
			if (currentLevel->isValid(currentLevel->MouseGrid.x,
						currentLevel->MouseGrid.y))
			switch (getMenuState())
			{
			case 1:
				currentLevel->BuildTower(//builds normal turret
					currentLevel->MouseGrid.x,
					currentLevel->MouseGrid.y,
				       	0);
				break;
			case 2:currentLevel->BuildTower(//builds slow turret
					currentLevel->MouseGrid.x,
					currentLevel->MouseGrid.y,
				       	1);
			};
		}

		menu.On = false;
	}
};

// Process middle click
void Game::OnMClick(int x, int y)
{
	currentLevel->BuildTower(	currentLevel->MouseGrid.x,
					currentLevel->MouseGrid.y,
				       		0);//builds tower on middle click
};

// Trigger on mouse movement
void Game::OnMouseMove(int x, int y)
{
	// Store the new mouse position
	Mouse.x = x;

	Mouse.y = y;
	if (!menu.On)//if the menu is off
	{
		currentLevel->MouseGrid.x = (Mouse.x / TILESIZE)
			- (Surface::Padding / TILESIZE);//which grid box the mouse is in
		currentLevel->MouseGrid.y = (Mouse.y / TILESIZE)
			- (Surface::Padding / TILESIZE);
	}
};

// Waits for the next frame; based on libsdl.org/intro.en/usingtimers.html
void Game::Wait()
{
	static int next_time = 0;
	int now = SDL_GetTicks();
	if (next_time <= now)
		next_time = now + 1000*FRAMETIME;
	else
		SDL_Delay(next_time - now);
};

// Pseudo-Game.Run() loop for the game over state
void Game::GameOver()
{
	// Lives shouldn't be negative
	if (currentLevel->Player.lives < 0)
		currentLevel->Player.lives = 0;

	SDL_Event Event;
	while(Running)
	{
		// Exit on Q or window close
		while (SDL_PollEvent(&Event))
		{
			switch (Event.type)
			{
				case SDL_KEYDOWN:
					if (Event.key.keysym.sym == SDLK_q)
						OnQuit();
					break;
				case SDL_QUIT:
					OnQuit();
					break;
			}
		}

		// Fade to black
		Surface::DrawRect(Surface::Display, 0, 0,
				currentLevel->Grid[0].size()*TILESIZE,
				currentLevel->Grid.size()*TILESIZE,
				0, 0, 0, 1+127*FRAMETIME);
		// Draw text
		stringRGBA(Surface::Display,
				(windowWidth-8*9)/2 , (windowHeight-8)/2,
				"GAME OVER", 255, 255, 255, 255);

		SDL_Flip(Surface::Display);
		Wait();
	};
};

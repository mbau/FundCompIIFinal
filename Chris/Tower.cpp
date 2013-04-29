// Tower.cpp -- Tower class functions

#include "Tower.h"
#include "Surface.h"

Tower::Tower(int x, int y, int startType) :
	gridX(x), gridY(y), type(startType)
{
	SetType(type);
	reloadTime = 0;
};

void Tower::SetType(int newType)
{
	type = newType;

	switch (type)
	{
		default:
			SetParams(100, 100, 1);
	};
};

void Tower::SetParams(double rng, double pow, double rt)
{
	range = rng;
	power = pow;
	rate = rt;
};

void Tower::Render()
{
	Surface::DrawSprite(2, 0, gridX*TILESIZE, gridY*TILESIZE);
};

void Tower::DrawRange()
{
	int r = 255;
	int g = 0;
	int b = 0;

	filledCircleRGBA(Surface::Display, (gridX + .5)*TILESIZE,
			(gridY + .5)*TILESIZE, range, r, g, b, 64);
	circleRGBA(Surface::Display, (gridX + .5)*TILESIZE,
			(gridY + .5)*TILESIZE, range, r, g, b, 255);
};

void Tower::Update(double dt)
{
	reloadTime -= dt;
	if (reloadTime < 0) reloadTime = 0;
};

/*
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>

Tower::Tower(SDL_Surface *Dest, SDL_Rect &DestRect,
		SDL_Surface *SrcSprite, SDL_Rect &SrcRect):
	Entity(Dest, DestRect, SrcSprite, SrcRect)
{
	range = 0;
	power = 0;
	rate = 0;
	lastShotTime = SDL_GetTicks();
};

void Tower::SetParams(double rng, double pow, double rt)
{
	range = rng;
	power = pow;
	rate = rt;
};

void Tower::Render()
{
	if (IsClicked)
	{
		filledCircleRGBA(Display, Rect.x+Rect.w/2, Rect.y+Rect.h/2,
				range, 255, 0, 0, 128);
		circleRGBA(Display, Rect.x+Rect.w/2, Rect.y+Rect.h/2, range,
				255, 0, 0, 255);
	}

	Entity::Render();
};
*/

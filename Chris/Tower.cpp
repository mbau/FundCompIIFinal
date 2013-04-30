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

	int x = (gridX + .5)*TILESIZE + Surface::Padding;
	int y = (gridY + .5)*TILESIZE + Surface::Padding;

	filledCircleRGBA(Surface::Display, x, y, range, r, g, b, 64);
	circleRGBA(Surface::Display, x, y, range, r, g, b, 255);
};

void Tower::Update(double dt)
{
	reloadTime -= dt;
	if (reloadTime < 0) reloadTime = 0;
};

void Tower::Upgrade(int type)
{
	switch(type)
	{
		case 0:
			power += 100;
			break;
		case 1:
			range += 100;
			break;
		case 2:
			rate *= 2;
			break;
	}
};

int Tower::UpgradeCost(int type)
{
	switch (type)
	{
		case 0:	// power
			return 100;
			break;
		case 1:	// range
			return 100;
			break;
		case 2:	// rate
			return 100;
			break;
	}

	return 0;
};

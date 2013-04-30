// Tower.cpp -- Tower class functions

#include "Tower.h"
#include "Surface.h"
#include <math.h>

Tower::Tower(int x, int y, int startType) :
	gridX(x), gridY(y), type(startType)
{
	SetType(type);
	reloadTime = 0;
	direction = 0;
};

void Tower::SetType(int newType)
{
	type = newType;

	switch (type)
	{
		default:
			SetParams(2*TILESIZE, 1, 32);
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
	Surface::DrawSprite(direction, 1+type, gridX*TILESIZE, gridY*TILESIZE);
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

// Attempt to fire at enemy; return true if destroyed
bool Tower::Fire(Enemy &enemy,	vector <Bullet> &Shots)
{
	if (reloadTime <= 0)
	{
		double dx, dy, towerX, towerY, enemyX, enemyY;
		towerX = (gridX + .5)*TILESIZE;
		towerY = (gridY + .5)*TILESIZE;
		enemyX = enemy.x + .5*TILESIZE;
		enemyY = enemy.y + .5*TILESIZE;
		dx = enemyX - towerX;
		dy = enemyY - towerY;
		if (dx*dx + dy*dy <= range*range)
		{
			double angle = atan2(dy, dx)/M_PI + 1;
			direction = (int)((4*(angle))+6.5)%8;
			reloadTime+=1./rate;
			angle = (direction+2)*M_PI/4;
			int bulletX = towerX - TILESIZE*cos(angle)/2;
			int bulletY = towerY - TILESIZE*sin(angle)/2;
			Shots.push_back(Bullet(bulletX, bulletY,
						enemyX, enemyY));
			if (enemy.damage(power))
			{
				return true;
			}
		}
	}

	return false;
};

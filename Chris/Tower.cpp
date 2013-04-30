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
		case 0:
			SetParams(2*TILESIZE, 1, 32);
			break;
		case 1:
			SetParams(2*TILESIZE, .1, 32);
			break;
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

void Tower::Upgrade(int upgradeType)
{
	switch (type)
	{
		default:
		case 0:	// Normal
			switch(upgradeType)
			{
				case 0:
					power += 100;
					break;
				case 1:
					range += TILESIZE;
					break;
				case 2:
					rate *= 2;
					break;
			}
			break;
		case 1:	// Slow
			switch(upgradeType)
			{
				case 0:
					power = 1-((1-power)*.75);
					break;
				case 1:
					range += TILESIZE;
					break;
				case 2:
					rate *= 2;
					break;
			}
			break;
	}
};

int Tower::UpgradeCost(int upgradeType)
{
	int costs[3] = {0, 0, 0};
	switch (type)
	{
		default:
		case 0:	// Normal
			costs[0] = 100;	// power
			costs[1] = 100;	// range
			costs[2] = 100;	// rate
			break;
		case 1:	// Slow
			costs[0] = 100;	// power
			costs[1] = 100;	// range
			costs[2] = 100;	// rate
			break;
	}

	if (upgradeType < 3)
		return costs[upgradeType];

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
						enemyX, enemyY, type));
			switch (type)
			{
				default:
				case 0:
					if (enemy.damage(power))
					{
						return true;
					}
					break;
				case 1:
					enemy.slow(power);
					break;
			}
		}
	}

	return false;
};

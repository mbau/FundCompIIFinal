// Tower.cpp -- Tower class functions

#include "Tower.h"
#include "Surface.h"
#include <math.h>

//non-default constructor for Tower
Tower::Tower(int x, int y, int startType) :
	gridX(x), gridY(y), type(startType)
{
	SetType(type);
	reloadTime = 0;
	direction = 0;
};

//
void Tower::SetType(int newType)
{
	type = newType;

	switch (type)
	{
		default:
		case 0:
			SetParams(1.5*TILESIZE, 15, 2);
			break;
		case 1:
			SetParams(1.5*TILESIZE, .2, 30);
			break;
	};
};

//sets the range, power, and the fire rate
void Tower::SetParams(double rng, double pow, double rt)
{
	range = rng;
	power = pow;
	rate = rt;
};

//draws the towers
void Tower::Render()
{
	Surface::DrawSprite(direction, 1+type, gridX*TILESIZE, gridY*TILESIZE);
};

//draws the range when the mouse is over the tile
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

//handles tower upgrades
void Tower::Upgrade(int upgradeType)
{
	switch (type)
	{
		default:
		case 0:	// Normal
			switch(upgradeType)
			{
				case 0:
					power += 100;//increased power
					break;
				case 1:
					range += TILESIZE;//increased range
					break;
				case 2:
					rate *= 2;//increased fire rate
					break;
			}
			break;
		case 1:	// Slow
			switch(upgradeType)
			{
				case 0:
					power = 1-((1-power)*.75);//increased slow ability
					break;
				case 1:
					range += TILESIZE;//increased range
					break;
				case 2:
					rate *= 2;//increased fire rate
					break;
			}
			break;
	}
};

//handles the cost of the upgrades
int Tower::UpgradeCost(int upgradeType)
{
	int costs[3] = {0, 0, 0};
	switch (type)
	{
		default:
		case 0:	// Normal
			costs[0] = 500;	// power
			costs[1] = 500;	// range
			costs[2] = 500;	// rate
			break;
		case 1:	// Slow
			costs[0] = 500;	// power
			costs[1] = 500;	// range
			costs[2] = 500;	// rate
			break;
	}

	if (upgradeType < 3)
		return costs[upgradeType];
	else
		return 0;
};

//check to see if a certain enemy is in range of the tower
bool Tower::inRange(Enemy &enemy)
{
	double dx, dy, towerX, towerY, enemyX, enemyY;
	towerX = (gridX + .5)*TILESIZE;//tower pos
	towerY = (gridY + .5)*TILESIZE;
	enemyX = enemy.x + .5*TILESIZE;//enemy pos
	enemyY = enemy.y + .5*TILESIZE;
	dx = enemyX - towerX;//distance between tower and enemy
	dy = enemyY - towerY;
		
	if (dx*dx + dy*dy <= range*range) 
	{
		return true;
	}
	return false;
};

// Attempt to fire at enemy; return true if destroyed
bool Tower::Fire(Enemy &enemy,	vector <Bullet> &Shots)
{
	if (reloadTime <= 0)
	{
		double dx, dy, towerX, towerY, enemyX, enemyY;
		towerX = (gridX + .5)*TILESIZE;//tower pos
		towerY = (gridY + .5)*TILESIZE;
		enemyX = enemy.x + .5*TILESIZE;//enemy pos
		enemyY = enemy.y + .5*TILESIZE;
		dx = enemyX - towerX;//distance between tower and enemy
		dy = enemyY - towerY;
		if (inRange(enemy))//if enemy is within range
		{
			double angle = atan2(dy, dx)/M_PI + 1;//angle between tower and enemy
			direction = (int)((4*(angle))+6.5)%8;//fire direction reference for turret
			reloadTime+=1./rate;//reloads
			angle = (direction+2)*M_PI/4;//angle that it leaves the barrel
			int bulletX = towerX - TILESIZE*cos(angle)/2;//bullet x coords
			int bulletY = towerY - TILESIZE*sin(angle)/2;//bullet y coords
			Shots.push_back(Bullet(bulletX, bulletY,//add bullets to the shots vector
						enemyX, enemyY, type));
			switch (type)
			{
				default:
				case 0://if fired from a normal turret
					if (enemy.damage(power))
					{
						return true;
					}
					break;
				case 1://if fired from a slowing turret
					enemy.slow(.5);
					break;
			}
		}
	}

	return false;
};

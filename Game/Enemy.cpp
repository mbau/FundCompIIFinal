// Enemy.cpp -- Enemy class functions

#include "Enemy.h"
#include "Surface.h"
#include <math.h>
#include <stdlib.h>

// Enemy strength/value scales with i
Enemy::Enemy(int i)
{
	int num=rand()%5;
	switch(num){
		case 0://tank
			type=0;
			maxHealth = .7*50.*pow(2., i/10.);	// Double every 10, gets harder as time goes on
			v = .5*TILESIZE;
			break;
		case 1://tank
			type=0;
			maxHealth = .7*50.*pow(2., i/10.);	// Double every 10, gets harder as time goes on
			v = .5*TILESIZE;
			break;
		case 2://helo
			type=1;
			maxHealth = .5*50.*pow(2., i/10.);	// Double every 10, gets harder as time goes on
			v = TILESIZE;
			break;
		case 3://helo
			type=1;
			maxHealth = .5*50.*pow(2., i/10.);	// Double every 10, gets harder as time goes on
			v = TILESIZE;
			break;
		case 4://big
			type=2;
			maxHealth = 50.*pow(2., i/10.);	// Double every 10, gets harder as time goes on
			v = .3*TILESIZE;
			break;
	}
	health = maxHealth;//intial health
	value = maxHealth;
	x = y = pathSegment = 0;
	direction = 0;
	slowfactor = 1;
	regen_rate = 0.2;
	distance_traveled = 0;
};

// Deals damage to the enemy and return true if dead
bool Enemy::damage(double damage)
{
	health -= damage;//take health away  from enemy
	if (health < 0)
	{
		health = 0;
		return true;//dead
	}
	else
	{
		return false;//alive
	};
};

//slows enemy by a percentage
void Enemy::slow(double power)
{
	slowfactor *= (1-power);
};

void Enemy::Render()
{
	Surface::DrawSprite(direction, 3+type, x, y);//draw enemy
};

// Returns true when the end of the path is reached, defines enemy movement
bool Enemy::Move(double dt, vector<int> &pathX, vector<int> &pathY)
{
	bool pathDone = false;
	int dx = pathX[pathSegment+1] - x;
	int dy = pathY[pathSegment+1] - y;
	double movement = v * dt * slowfactor;//velocity

	if (dx*dx + dy*dy <= movement*movement)//
	{
		if ((++pathSegment) >= pathX.size()-1)
		{
			pathSegment = 0;
			pathDone = true;
		}

		x = pathX[pathSegment];
		y = pathY[pathSegment];
	}
	else
	{
		if (dx > 0)
		{
			x += movement;
			direction = 1;
		}
		else if (dx < 0)
		{
			x -= movement;
			direction = 3;
		}
		else if (dy > 0)
		{
			y += movement;
			direction = 2;
		}
		else if (dy < 0)
		{
			y -= movement;
			direction = 0;
		}
	}
	distance_traveled += movement;

	return pathDone;
};

// Enemy.cpp -- Enemy class functions

#include "Enemy.h"
#include "Surface.h"
#include <math.h>
#include <stdlib.h>

// Enemy strength/value scales with i
Enemy::Enemy(int i)
{
	maxHealth = 50.*pow(2., i/10.);	// Double every 10, gets harder as time goes on
	health = maxHealth;//intial health
	value = maxHealth;
	type = rand()%3;//random type
	x = y = pathSegment = 0;
	v = TILESIZE;
	direction = 0;
	slowfactor = 1;
	regen_rate = 0.2;
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
}

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

	return pathDone;
};

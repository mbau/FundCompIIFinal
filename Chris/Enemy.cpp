// Enemy.cpp -- Enemy class functions

#include "Enemy.h"
#include "Surface.h"
#include <math.h>

// Enemy strength/value scales with i
Enemy::Enemy(int i)
{
	maxHealth = 50.*pow(2., i/10.);	// Double every 10
	health = maxHealth;
	value = maxHealth;
	type = 0;
	x = y = pathSegment = 0;
	v = 50;
};

// Deals damage to the enemy and return true if dead
bool Enemy::damage(double damage)
{
	health -= damage;
	if (health < 0)
	{
		health = 0;
		return true;
	}
	else
	{
		return false;
	};
};

void Enemy::Render()
{
	Surface::DrawSprite(2, 0, x, y);
};

// Chris Croson
// Bullet.cpp

#include "Bullet.h"
#include "Surface.h"
#include <iostream>

//Non-default constructor for Bullet
Bullet::Bullet(int x1, int y1, int x2, int y2, int bulletType)
{
	startX = x1 + Surface::Padding;//start x pos for bullet (leaves turret)
	startY = y1 + Surface::Padding;//start y pos for bullet
	endX = x2 + Surface::Padding;//end x pos of bullet (enemy)
	endY = y2 + Surface::Padding;//end y pos of bullet
	type = bulletType;//which type the round if (damage or slow)
	timeLeft = timeMax = .5;
};

//shows the bullet lines
void Bullet::Render()
{
	double strength = timeLeft/timeMax;
	int r, g, b, a;
	switch (type)
	{
		default:
		case 0:	// Normal
			r = 255;//color values
			g = 255*strength;//making times strength changes color based on the location 
			b = 0;
			a = 255*strength;
			break;
		case 1:	// Slow
			r = 0;//color values
			g = 255*strength;
			b = 255;
			a = 255*strength;
			break;
	}

	aalineRGBA(Surface::Display, startX, startY, endX, endY, r, g, b, a);//line from point a to point b with the colors
};

//updates time
void Bullet::Update(double dt)
{
	timeLeft -= dt;//reduces time left, so changes color
	if (timeLeft < 0) timeLeft = 0;
};

// Chris Croson
// Bullet.cpp

#include "Bullet.h"
#include "Surface.h"
#include <iostream>

Bullet::Bullet(int x1, int y1, int x2, int y2, int bulletType)
{
	startX = x1 + Surface::Padding;
	startY = y1 + Surface::Padding;
	endX = x2 + Surface::Padding;
	endY = y2 + Surface::Padding;
	type = bulletType;
	timeLeft = timeMax = .5;
};

void Bullet::Render()
{
	double strength = timeLeft/timeMax;
	int r, g, b, a;
	switch (type)
	{
		default:
		case 0:	// Normal
			r = 255;
			g = 255*strength;
			b = 0;
			a = 255*strength;
			break;
		case 1:	// Slow
			r = 0;
			g = 255*strength;
			b = 255;
			a = 255*strength;
			break;
	}

	aalineRGBA(Surface::Display, startX, startY, endX, endY, r, g, b, a);
};

void Bullet::Update(double dt)
{
	timeLeft -= dt;
	if (timeLeft < 0) timeLeft = 0;
};

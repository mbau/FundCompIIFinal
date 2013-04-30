// Chris Croson
// Bullet.cpp

#include "Bullet.h"
#include "Surface.h"
#include <iostream>

Bullet::Bullet(int x1, int y1, int x2, int y2)
{
	startX = x1 + Surface::Padding;
	startY = y1 + Surface::Padding;
	endX = x2 + Surface::Padding;
	endY = y2 + Surface::Padding;
	timeLeft = timeMax = .5;
};

void Bullet::Render()
{
	double strength = timeLeft/timeMax;
	int r = 255;
	int g = 255*strength;
	int b = 0;
	int a = 255*strength;
	aalineRGBA(Surface::Display, startX, startY, endX, endY, r, g, b, a);
};

void Bullet::Update(double dt)
{
	timeLeft -= dt;
	if (timeLeft < 0) timeLeft = 0;
};

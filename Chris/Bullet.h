// Chris Croson
// Bullet.h

#ifndef BULLET_H
#define BULLET_H

class Bullet {
	public:
		Bullet(int x1, int y1, int x2, int y2, int bulletType);
		void Render();
		void Update(double dt);

		int startX, startY, endX, endY;
		int type;

		double timeMax;
		double timeLeft;
	private:
};

#endif	// BULLET_H

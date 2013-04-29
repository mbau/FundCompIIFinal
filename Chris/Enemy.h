// Enemy.h -- Enemy class definition

#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
	public:
		Enemy(int i);
		void Render();
		bool damage(double damage);

		double health;
		double maxHealth;
		int value;
		int type;

		double x;
		double y;
		int pathSegment;
		double v;	// Velocity, px/sec
	private:
};

#endif	// ENEMY_H

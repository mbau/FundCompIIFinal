// Enemy.h -- Enemy class definition

#ifndef ENEMY_H
#define ENEMY_H

#include <vector>

using namespace std;

class Enemy {
	public:
		Enemy(int i);
		void Render();
		bool damage(double damage);
		void slow(double);
		bool Move(double dt, vector<int> &pathX, vector<int> &pathY);

		// Game vars
		double health;
		double maxHealth;
		int value;
		int type;

		// Position vars
		double x;
		double y;
		int pathSegment;
		double v;	// Velocity, px/sec
		int direction;
		double slowfactor;
		double regen_rate;
	private:
};

#endif	// ENEMY_H

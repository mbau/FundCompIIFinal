// Level.h -- Encapsulates the level layout and state

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"

//enum Grid_Type {LAND, PATH, TOWER};

using namespace std;

class Level {
	public:
		Level();
		bool Init(const char* filename);
		void Render();
		void Update(double dt);

		void RenderGrid();
		void RenderHighlight();
		void RenderTowers();
		void RenderEnemies();
		void RenderShots();
		void RenderHUD();

		bool BuildTower(int x, int y, int type);
		bool UpgradeTower(Tower *tower, int upgradeType);
		bool DestroyTower(Tower *tower);
		Tower* isTower(int x, int y);
		bool isValid(int x, int y);

		bool isGameOver();

		void setSpawnRate(double rt);

		vector<Tower> Towers;
		vector<Enemy> Enemies;
		vector<Bullet> Shots;
		vector< vector <int> > Grid; // Grid-based level layout
		vector <int> pathX;
		vector <int> pathY;
		struct {int x, y;} MouseGrid;

		struct {int score, money, lives;} Player;

	private:
		// Update functions
		void spawnEnemies(double dt);
		void moveEnemies(double dt);
		void updateTowers(double dt);
		void updateShots(double dt);
		void updateSlow();
		void fire();
		void destroyEnemy(unsigned int i);
		
		double enemyRate;	// Enemy spawns per second
		double enemyWait;	// Time until next spawn
		int enemyCount;
};

#endif	// LEVEL_H

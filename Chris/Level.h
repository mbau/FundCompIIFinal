// Level.h -- Encapsulates the level layout and state

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Entity.h"
#include "Enemy.h"
#include "Tower.h"

using namespace std;

class Level {
	public:
		bool Init(char* filename);
		vector<Tower> Towers;
		vector<Enemy> Enemies;
		vector< vector <int> > Grid;	// Grid-based level layout
						// based on tile indices
	private:
};

#endif	// LEVEL_H

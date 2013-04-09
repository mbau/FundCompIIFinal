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
		Level(SDL_Surface *Disp, SDL_Surface *Sprite);
		SDL_Surface *Display;
		SDL_Surface *SpriteSheet;

		bool Init(char* filename);
		void Render();

		vector<Tower> Towers;
		vector<Enemy> Enemies;
		vector< vector <Entity> > Grid;	// Grid-based level layout
	private:
};

#endif	// LEVEL_H

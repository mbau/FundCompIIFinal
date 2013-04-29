// Level.cpp

#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Surface.h"

using namespace std;

Level::Level()
{
	setSpawnRate(.5);
	enemyCount = 0;

	Player.score = 0;
	Player.lives = 25;
	Player.money = 1000;
};

bool Level::Init(char *filename)
{
	fstream File;
	File.open(filename, fstream::in);

	if (!File)
		return false;

	// Read in level size
	int xMax, yMax;
	File >> xMax >> yMax;

	// Read and construct grid
	for (int i = 0; i < yMax; i++)
	{
		vector<int> temp;
		
		for(int j = 0; j < xMax; j++)
		{
			int GridValue;
			File >> GridValue;
			temp.push_back(GridValue);
		}
		Grid.push_back(temp);
	}

	File.close();

	return true;
};

void Level::Render()
{
	RenderGrid();
	Tower* temp = NULL;
	if ((temp = isTower(MouseGrid.x, MouseGrid.y)))
	{
		temp->DrawRange();
	}
	else if (isValid(MouseGrid.x, MouseGrid.y))
	{
		Surface::DrawRect(Surface::Display,
			MouseGrid.x*TILESIZE, MouseGrid.y*TILESIZE,
			TILESIZE, TILESIZE, 0, 255, 0, 64); 
	}
	else
	{
		Surface::DrawRect(Surface::Display,
			MouseGrid.x*TILESIZE, MouseGrid.y*TILESIZE,
			TILESIZE, TILESIZE, 255, 0, 0, 64); 
	}
	RenderTowers();	
	RenderEnemies();
	RenderShots();
	RenderHUD();
};

// Draw the base grid
void Level::RenderGrid()
{
	for (unsigned int i = 0; i < Grid.size(); i++)
	{
		for (unsigned int j = 0; j < Grid[0].size(); j++)
		{
			Surface::DrawSprite(Grid[i][j], 0,
					j*TILESIZE, i*TILESIZE);
		}
	}
};

void Level::RenderTowers()
{
	for (unsigned int i = 0; i < Towers.size(); i++)
	{
		Towers[i].Render();
	};
};

void Level::RenderEnemies()
{
	for (unsigned int i = 0; i < Enemies.size(); i++)
	{
		Enemies[i].Render();
	};
};

void Level::RenderShots()
{
	for (unsigned int i = 0; i < Shots.size(); i++)
	{
		Shots[i].Render();
	};
};

void Level::RenderHUD()
{
	stringstream money;
       	money << "$" << Player.money;
	stringRGBA(Surface::Display, 0, 10, money.str().c_str(), 0,0,0,255);

	stringstream lives;
	lives << "Lives: " << Player.lives;
	stringRGBA(Surface::Display, 250, 10, lives.str().c_str(), 0,0,0,255);

	stringstream score;
	score << "Score: " << Player.score;
	stringRGBA(Surface::Display, 500, 10, score.str().c_str(), 0,0,0,255);
};

bool Level::BuildTower(int x, int y, int type)
{
	int cost = 100;
	if (Player.money >= cost && isValid(x, y) && !isTower(x, y))
	{
		Player.money -= cost;
		Towers.push_back(Tower(x, y, type));
		return true;
	}
	else
	{
		return false;
	}
};

// Returns a pointer to the tower at the position if it exists, otherwise NULL
Tower* Level::isTower(int x, int y)
{
	for (unsigned int i = 0; i < Towers.size(); i++)
	{
		if (x == Towers[i].gridX && y == Towers[i].gridY)
			return &Towers[i];
	};
	return NULL;
};

bool Level::isValid(int x, int y)
{
	if (x < 0 || y < 0 || y >= (int)Grid.size() || x >= (int)Grid[0].size())
		return false;

	switch(Grid[y][x])
	{
		case 0:
			return true;
			break;
		case 1:
			return false;
			break;
		default:
			return true;
			break;
	}
};

void Level::setSpawnRate(double rt)
{
	enemyRate = rt;
	enemyWait = 0;
};

void Level::Update(double dt)
{
	// Update state
	
	spawnEnemies(dt);
	moveEnemies(dt);
	updateTowers(dt);
	updateShots(dt);

	// Execute state based interactions
	fire();
};

void Level::spawnEnemies(double dt)
{
	enemyWait -= dt;
	while (enemyWait < 0)
	{
		Enemy temp(enemyCount++);
		temp.x = 100;
		temp.y = -TILESIZE;
		Enemies.push_back(temp);

		enemyWait += 1/enemyRate;
	}
};

void Level::moveEnemies(double dt)
{
	for (unsigned int i = 0; i < Enemies.size(); i++)
	{
		Enemies[i].y += Enemies[i].v*dt;
		
		if (Enemies[i].y > Grid.size()*TILESIZE)
		{
			Enemies[i].y = -TILESIZE;
			--Player.lives;
		}
	};
};

void Level::updateTowers(double dt)
{
	for (unsigned int i = 0; i < Towers.size(); i++)
	{
		Towers[i].Update(dt);
	};
};

void Level::updateShots(double dt)
{
	unsigned int i = 0;
	while (i < Shots.size())
	{
		Shots[i].Update(dt);
		if (Shots[i].timeLeft <= 0)
		{
			Shots.erase(Shots.begin()+i);
		}
		else
			i++;
	};
};

void Level::fire()
{
	for (unsigned int i = 0; i < Towers.size(); i++)
	{
		if (Towers[i].reloadTime <= 0)
		{
		for (unsigned int j = 0; j < Enemies.size(); j++)
		{
			double dx, dy, towerX, towerY, enemyX, enemyY;
			towerX = (Towers[i].gridX + .5)*TILESIZE;
			towerY = (Towers[i].gridY + .5)*TILESIZE;
			enemyX = Enemies[j].x + .5*TILESIZE;
			enemyY = Enemies[j].y + .5*TILESIZE;
			dx = enemyX - towerX;
			dy = enemyY - towerY;
			if (dx*dx + dy*dy <= Towers[i].range*Towers[i].range)
			{
				Towers[i].reloadTime+=1./Towers[i].rate;
				Shots.push_back(Bullet(towerX, towerY,
							enemyX, enemyY));
				if (Enemies[j].damage(Towers[i].power))
				{
					destroyEnemy(j);
				}
				break;
			}
		}
		}
	}
};

void Level::destroyEnemy(unsigned int i)
{
	Player.score += Enemies[i].value;
	Player.money += Enemies[i].value;
	Enemies.erase(Enemies.begin() + i);
};

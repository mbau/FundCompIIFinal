// Level.cpp

#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
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

	// Read in enemy path
	while (true)
	{
		int temp;
		File >> temp;
		if (File.eof()) break;
		pathX.push_back(temp*TILESIZE);
		File >> temp;
		pathY.push_back(temp*TILESIZE);
	}

	File.close();

	return true;
};

void Level::Render()
{
	RenderGrid();

	// Highlight stuff
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
	
	// Draw the last path tile on top; enemies 'enter' the fort
	int i = pathX.back()/TILESIZE;
	int j = pathY.back()/TILESIZE;
	if (j < (int)Grid.size() && j >= 0 && i < (int)Grid[0].size() && i >= 0)
	{
		Surface::DrawSprite(Grid[j][i], 0, j*TILESIZE, i*TILESIZE);
	}
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
	// FYI, char width and height is 8 by default
	stringstream money;
       	money << "$" << Player.money;
	stringRGBA(Surface::Display, 8*10, 10,
			money.str().c_str(), 0,0,0,255);

	stringstream lives;
	lives << "Lives: " << Player.lives;
	stringRGBA(Surface::Display, Surface::Display->w/2 - 8*6, 10,
			lives.str().c_str(), 0,0,0,255);

	stringstream score;
	score << "Score: " << Player.score;
	stringRGBA(Surface::Display, Surface::Display->w - 8*20, 10,
			score.str().c_str(), 0,0,0,255);
};

bool Level::BuildTower(int x, int y, int type)
{
	int cost = 500;
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

// Attempts to upgrade a tower
bool Level::UpgradeTower(Tower *tower, int upgradeType)
{
	int cost = tower->UpgradeCost(upgradeType);
	if (cost <= Player.money)
	{
		Player.money -= cost;
		tower->Upgrade(upgradeType);
		return true;
	}
	else
	{
		return false;
	}
};

//When prompted, tower off vector and off screen
bool Level::DestroyTower(Tower *tower)
{
	for(unsigned int i=0;i<Towers.size();i++){
		if(tower == &(Towers[i]))
		{
			Towers.erase(Towers.begin()+i);
			return true;
		}
	}

	return false;
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
		case 2:
			return false;
		default:
			return false;
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
	updateSlow();

	// Execute state based interactions
	fire();
};

void Level::spawnEnemies(double dt)
{
	enemyWait -= dt;
	while (enemyWait < 0)
	{
		Enemy temp(enemyCount++);
		temp.x = pathX[0];
		temp.y = pathY[0];
		Enemies.push_back(temp);

		enemyWait += 1/enemyRate;
	}
};

void Level::moveEnemies(double dt)
{
	for (unsigned int i = 0; i < Enemies.size(); i++)
	{
		if (Enemies[i].Move(dt, pathX, pathY))
		{
			--Player.lives;
			Enemies.erase(Enemies.begin()+i);
		}
	}
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

void Level::updateSlow()
{
	if(!Enemies.empty())
	{
	
	for (unsigned int i = 0; i < Enemies.size(); i++)
	{
		if (Enemies[i].slowfactor < 1)
		{
			Enemies[i].slowfactor += Enemies[i].regen_rate;
		}
		if (Enemies[i].slowfactor > 1)
		{
			Enemies[i].slowfactor = 1;
		}
	}
	}
};

void Level::fire()
{
	double distance;
	int furthest;
	if(!Enemies.empty())
	{
	
	for (unsigned int i = 0; i < Towers.size(); i++)
	{
		if (Towers[i].reloadTime <= 0)
		{
			distance = 0;
			furthest = 0;

			for (unsigned int j = 0; j < Enemies.size(); j++)
			{
				if (Enemies[j].distance_traveled > distance && Towers[i].inRange(Enemies[j]))
				{
					distance = Enemies[j].distance_traveled;
					furthest = j;
				}
			}
			
			if (Towers[i].Fire(Enemies[furthest], Shots))
			{
				destroyEnemy(furthest);
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

bool Level::isGameOver()
{
	return (Player.lives <= 0);
};

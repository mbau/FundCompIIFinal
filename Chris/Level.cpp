// Level.cpp

#include "Level.h"
#include "Surface.h"
#include <iostream>
#include <fstream>

Level::Level(SDL_Surface *Disp, SDL_Surface *Sprites)
{
	Display = Disp;
       	SpriteSheet = Sprites;
};

bool Level::Init(char *filename)
{
	fstream File;
	File.open(filename, fstream::in);

	if (!File)
		return false;

	for (int i = 0; i < 8; i++)
	{
		vector<Entity> temp;
		
		for(int j = 0; j < 12; j++)
		{
			int GridValue;
			File >> GridValue;

			SDL_Rect DestRect;
			DestRect.x = 50*j;
			DestRect.y = 50*i;
			DestRect.w = DestRect.h = 50;

			SDL_Rect SpriteRect;
			SpriteRect.x = GridValue*50;
			SpriteRect.y = 0;
			SpriteRect.w = SpriteRect.h = 50;

			Entity tempEntity(Display, DestRect,
					SpriteSheet, SpriteRect);

			temp.push_back(tempEntity);
		}
		Grid.push_back(temp);
	}

	File.close();

	return true;
};

void Level::Render()
{
	// Draw the base grid
	for (unsigned int i = 0; i < Grid.size(); i++)
	{
		for (unsigned int j = 0; j < Grid[0].size(); j++)
		{
			Grid[i][j].Render();
		}
	}

};

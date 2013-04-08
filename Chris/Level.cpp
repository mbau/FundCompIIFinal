// Level.cpp

#include "Level.h"
#include <fstream>

bool Level::Init(char *filename)
{
	fstream File;
	File.open(filename, fstream::in);

	if (!File)
		return false;

	for (int i = 0; i < 8; i++)
	{
		vector<int> temp;
		
		for(int j = 0; j < 12; j++)
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

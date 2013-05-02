// main.cpp -- game driver

#include "Game.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[])
{
	string level;
	if (argc > 1)
	{
		level = argv[1];
	}
	else
	{
		level = "0";
	}
	srand(time(NULL));
	Game game(level);
	game.Run();
	return 0;
};

// main.cpp -- game driver

#include "Game.h"
#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(NULL));
	Game game;
	game.Run();
	return 0;
};

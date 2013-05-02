// Game.h
// Game class definition

#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "Level.h"
#include "Tower.h"
#include <string>

using namespace std;

class Game {
	public:
		Game(string lvlID);
		int Run();
	private:
		// Main game functions
		bool Init();
		void OnEvent(SDL_Event* Event);
		void Update();
		void Render();
		void Cleanup();
		void Wait();
		void GameOver();

		int windowWidth;
		int windowHeight;
		struct {int x, y;} Mouse;

		bool Running;

		string levelID;
		Level *currentLevel;

		// Pie menu stuff
		struct {
			int x, y, numStates;
			bool On;
			Tower *target;} menu;
		void drawMenu();
		int getMenuState();

		void drawMargins();

		// Events
		void OnQuit();
		void OnKeyDown(SDLKey sym);
		void OnLClick(int x, int y);
		void OnLClickRelease(int x, int y);
		void OnRClick(int x, int y);
		void OnRClickRelease(int x, int y);
		void OnMClick(int x, int y);
		void OnMouseMove(int x, int y);
};

#endif	// GAME_H

// Game.h
// Game class definition

#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "Level.h"

class Game {
	public:
		Game();
		int Run();
	private:
		// Main game functions
		bool Init();
		void OnEvent(SDL_Event* Event);
		void Update();
		void Render();
		void Cleanup();
		void Wait();

		int windowWidth;
		int windowHeight;
		struct {int x, y;} Mouse;

		bool Running;

		Level *currentLevel;

		bool menuOn;
		struct {int x, y;} menu;

		// Events
		void OnQuit();
		void OnKeyDown(SDLKey sym);
		void OnLClick(int x, int y);
		void OnRClick(int x, int y);
		void OnRClickRelease(int x, int y);
		void OnMClick(int x, int y);
		void OnMouseMove(int x, int y);
};

#endif	// GAME_H

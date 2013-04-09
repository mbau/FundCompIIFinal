// Game.h
// Game class definition

#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "Level.h"

class Game {
	public:
		Game();
		bool Init();
		int Run();
		void OnEvent(SDL_Event* Event);
		void Update();
		void Render();
		void Cleanup();
	private:
		int windowWidth;
		int windowHeight;
		struct {int x, y;} Mouse;

		bool Running;

		SDL_Surface* Display;
		SDL_Surface* SpriteSheet;
		Level *currentLevel;

		Entity *Clicked, *Hovered;

		// Events
		void OnQuit();
		void OnKeyDown(SDLKey sym);
		void OnLClick(int x, int y);
		void OnRClick(int x, int y);
		void OnMClick(int x, int y);
		void OnMouseMove(int x, int y);
};

#endif	// GAME_H

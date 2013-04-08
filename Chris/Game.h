// Game.h -- Game class definition

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

		// Events
		void OnQuit();
		void OnKeyDown(SDLKey sym);
		void OnLClick(int x, int y);
		void OnRClick(int x, int y);
		void OnMClick(int x, int y);
		void OnMouseMove(int x, int y);

		// Graphics utilities
		SDL_Surface* LoadImage(char* filename);
		static bool Draw(SDL_Surface* Dest, SDL_Surface* Src,
				int x, int y);
		static bool Draw(SDL_Surface* Dest, SDL_Surface* Src,
				int x1,int y1, int x2, int y2, int w, int h);
		static bool DrawRect(SDL_Surface* Dest,
				int x, int y, int w, int h,
				int r, int g, int b, int a);
};

#endif	// GAME_H

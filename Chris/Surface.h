// Surface.h
// Header for graphics utilities

#ifndef SURFACE_H
#define SURFACE_H

const int TILESIZE = 100;
const int ICONSIZE = 50;

enum ICONINDEX {NORMAL, SLOW, RANGE, DAMAGE, FREEZE, RATE, SELL};

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

class Surface {
	public:
		static SDL_Surface* Display;
		static SDL_Surface* SpriteSheet;
		static SDL_Surface* IconSheet;
		static int Padding;

		static SDL_Surface* LoadImage(char* filename);
		static bool Draw(SDL_Surface* Dest, SDL_Surface* Src,
				int x, int y);
		static bool Draw(SDL_Surface* Dest, SDL_Surface* Src,
				int x1,int y1, int x2, int y2, int w, int h);
		static bool DrawRect(SDL_Surface* Dest,
				int x, int y, int w, int h,
				int r, int g, int b, int a);
		static bool DrawSprite(int tileX, int tileY, int x, int y);
};

#endif	// SURFACE_H

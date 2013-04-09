// Surface.h
// Header for graphics utilities

#ifndef SURFACE_H
#define SURFACE_H

#include <SDL/SDL.h>

class Surface {
	public:
		static SDL_Surface* LoadImage(char* filename);
		static bool Draw(SDL_Surface* Dest, SDL_Surface* Src,
				int x, int y);
		static bool Draw(SDL_Surface* Dest, SDL_Surface* Src,
				int x1,int y1, int x2, int y2, int w, int h);
		static bool DrawRect(SDL_Surface* Dest,
				int x, int y, int w, int h,
				int r, int g, int b, int a);
};

#endif	// SURFACE_H

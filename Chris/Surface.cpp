// Surface.cpp
// Graphics utility definitions

#include "Surface.h"

SDL_Surface* Surface::Display = NULL;//pointers for the image files
SDL_Surface* Surface::SpriteSheet = NULL;
SDL_Surface* Surface::IconSheet = NULL;
int Surface::Padding = 50;

// Utility to load and format a BMP image
SDL_Surface* Surface::LoadImage(char* filename)//load image
{
	SDL_Surface* Temp = NULL;
	SDL_Surface* Return = NULL;

	Temp = SDL_LoadBMP(filename);//loads the .bmp

	if (!Temp)//if there is nothing loaded into the temp
		return NULL;

	Return = SDL_DisplayFormat(Temp);//sets the return to the image that was loaded
	SDL_FreeSurface(Temp);//frees the temp

	return Return;//returns the loaded image
};

// Blit an entire surface to another surface
bool Surface::Draw(SDL_Surface* Dest, SDL_Surface* Src, int x, int y)
{
	if (!Dest || !Src)//if the pointers do not point to anything
		return false;

	// Destination rect
	SDL_Rect DestRect;

	DestRect.x = x + Padding;
	DestRect.y = y + Padding;

	// Draw
	SDL_BlitSurface(Src, NULL, Dest, &DestRect);

	return true;
};

// Blit part of a surface to another surface
bool Surface::Draw(SDL_Surface* Dest, SDL_Surface* Src,
		int x1, int y1, int x2, int y2, int w, int h)
{
	if (!Dest || !Src)
		return false;

	// Destination rect
	SDL_Rect DestRect;

	DestRect.x = x1+Padding;
	DestRect.y = y1+Padding;

	// Source rect
	SDL_Rect SrcRect;

	SrcRect.x = x2;
	SrcRect.y = y2;
	SrcRect.w = w;
	SrcRect.h = h;

	// Draw
	SDL_BlitSurface(Src, &SrcRect, Dest, &DestRect);

	return true;
};

// Draw a rect on the screen; standard functional doesn't allow alpha blending
bool Surface::DrawRect(SDL_Surface* Dest, int x, int y, int w, int h,
		int r, int g, int b, int a)
{
	if (!Dest)
		return false;
	SDL_Surface *tempSurf;	// Make a new surface
	tempSurf = SDL_CreateRGBSurface(Dest->flags, w, h, 32,
			0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

	if (!tempSurf)
		return false;

	// Fill the entire surface with the given color;
	SDL_FillRect(tempSurf, NULL, SDL_MapRGBA(tempSurf->format, r, g, b, a));

	// Blit the surface
	Draw(Dest, tempSurf, x, y);

	// Free the surface
	SDL_FreeSurface(tempSurf);

	return true;
};

//draws the sprite tiles
bool Surface::DrawSprite(int tileX, int tileY, int x, int y)
{
	if (!Display || !SpriteSheet) return false;

	Draw(Display, SpriteSheet, x, y,
			tileX*TILESIZE, tileY*TILESIZE,
			TILESIZE, TILESIZE);
	return true;
};

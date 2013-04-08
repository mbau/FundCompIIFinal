// Chris Croson
// Entity.h

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL/SDL.h>

class Entity {
	public:
		Entity();

		SDL_Rect Rect;	// Bounding rect; also, blitting dest rect
		SDL_Rect SrcRect;
		SDL_Surface *Sprite;

		virtual void Render(SDL_Surface *Dest);
		virtual void OnClick();
		virtual void OnHover();
	private:
};

#endif	// ENTITY_H

// Entity.h
// Generic on-screen item class

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL/SDL.h>

class Entity {
	public:
		Entity(SDL_Surface *Dest, SDL_Rect &DestRect,
				SDL_Surface *SrcSprite, SDL_Rect &SrcRect);
		Entity(const Entity &);
		~Entity();

		SDL_Surface *Display;	// The surface on which it is drawn
		SDL_Rect Rect;	// Bounding rect; also, blitting dest rect
		SDL_Surface *Sprite;	// Locally stored sprite sheet
		SDL_Surface *DrawingSurf;	// Prerendering surface

		bool IsClicked, IsHovered;

		virtual void Render();
		virtual void OnClick();
		virtual void OnUnClick();
		virtual void OnHover();
		virtual void OnUnHover();
	private:
};

#endif	// ENTITY_H

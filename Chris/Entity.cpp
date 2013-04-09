// Entity.cpp
// Functions for a generic on-screen item

#include "Entity.h"
#include "Surface.h"
#include <iostream>

Entity::Entity(SDL_Surface *Dest, SDL_Rect &DestRect, SDL_Surface *SrcSprite, SDL_Rect &SrcRect)
{
	IsClicked = false;
	IsHovered = false;
	Display = Dest;
	Rect = DestRect;
	Sprite = SDL_CreateRGBSurface(SDL_HWSURFACE,
			SrcRect.w, SrcRect.h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(SrcSprite, &SrcRect, Sprite, NULL);

	DrawingSurf = SDL_CreateRGBSurface(SDL_HWSURFACE,
			DestRect.w, DestRect.h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(Sprite, NULL, DrawingSurf, NULL);
};

Entity::Entity(const Entity &copy)
{
	*this = copy;
	Sprite = SDL_ConvertSurface(copy.Sprite, copy.Sprite->format,
			copy.Sprite->flags);
	DrawingSurf = SDL_ConvertSurface(copy.DrawingSurf,
			copy.DrawingSurf->format, copy.DrawingSurf->flags);
};

Entity::~Entity()
{
	SDL_FreeSurface(Sprite);
	SDL_FreeSurface(DrawingSurf);
};

// By default, draw the whole thing
void Entity::Render()
{
	Surface::Draw(DrawingSurf, Sprite, 0, 0);

	if (IsClicked)
	{
		Surface::DrawRect(DrawingSurf, 0, 0, DrawingSurf->w,
				DrawingSurf->h, 0, 255, 0, 128);
	}
	if (IsHovered)
	{
		Surface::DrawRect(DrawingSurf, 0, 0,
			DrawingSurf->w, DrawingSurf->h, 255, 255, 0, 128);
	}

	Surface::Draw(Display, DrawingSurf, Rect.x, Rect.y);
};

// Execute on click
void Entity::OnClick()
{
	IsClicked ^= 1;	// Toggle bool
};

// Execute on unclick
void Entity::OnUnClick()
{
	IsClicked = false;
};

// Execute when hovered
void Entity::OnHover()
{
	IsHovered = true;
};

// Execute when unhovered
void Entity::OnUnHover()
{
	IsHovered = false;
//	// Clear the Drawing surface
//	SDL_FillRect(DrawingSurf, NULL, SDL_MapRGBA(DrawingSurf->format,
//				0, 0, 0, 0));
//	// Redraw the original
//	Surface::Draw(DrawingSurf, Sprite, 0, 0);
};

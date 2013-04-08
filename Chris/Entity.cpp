// Chris Croson
// Entity.cpp

#include "Entity.h"

void Entity::Render(SDL_Surface *Dest)
{
	SDL_BlitSurface(Sprite, &SrcRect, Dest, &Rect);
};

void Entity::OnClick()
{
};

void Entity::OnHover()
{
};

#include "SpriteHolder.h"
SpriteHolder::SpriteHolder(Texture *texture, Vector2f pos)
{
	sprite.setTexture(*texture);
	sprite.setScale(V2f(1.f, 1.f));
	sprite.setPosition(pos);
}
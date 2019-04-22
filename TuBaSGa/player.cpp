#include "player.h"

player::player(std::vector<sf::Texture>& tex, sf::Vector2i mapPos, map& Map) : mapPosition(mapPos), textures(tex)
{
	sprite.setTexture(tex[1]);
	sprite.setScale(1.0f/5.0f, 1.0f/5.0f);
	moveSprite(Map);
	tex_it = textures.begin();
}

void player::moveSprite(map& Map)
{
	scale = Map.tilesSizes/10;
	position = sf::Vector2f(Map.position.x + mapPosition.x*Map.tilesSizes, Map.position.y + mapPosition.y*Map.tilesSizes);
	sprite.setPosition(position);
	sprite.setScale(scale/5,scale/5);
}

void player::move(map& Map, sf::Vector2i pos)
{
	if (Map.canPass(pos.y, pos.x))
	{
		mapPosition = pos;
		moveSprite(Map);
	}
}

void player::move(map& Map, std::vector<sf::Vector2i>& p)
{
	busy = true;
	path = p;
	path_it = path.rbegin();
}

void player::step(map& Map)
{
	if (busy)
	{
		if (path_it->x == -1)
		{
			sprite.setOrigin(24.5,24.5);
			sprite.setRotation(0);
			sprite.setOrigin(0, 0);
		}
		else if (path_it->x == 1)
		{
			sprite.setOrigin(24.5, 24.5);
			sprite.setRotation(180);
			sprite.setOrigin(50, 50);
		}
		else if (path_it->y == -1)
		{
			sprite.setOrigin(24.5, 24.5);
			sprite.setRotation(90);
			sprite.setOrigin(0, 50);
			
		}
		else if (path_it->y == 1)
		{
			sprite.setOrigin(24.5, 24.5);
			sprite.setRotation(-90);
			sprite.setOrigin(50, 0);
		}
		mapPosition -= *path_it++;
		move(Map, mapPosition);
		sprite.setTexture(*tex_it++);
		if (tex_it == textures.end())
			tex_it = textures.begin();
		if (path_it == path.rend())
		{
			busy = false;
			sprite.setTexture(textures[1]);
		}
	}
}
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h"

class player
{
private:
	sf::Vector2f position;
	float scale;
	std::vector<sf::Texture> textures;
	std::vector<sf::Texture>::iterator tex_it;
	std::vector<sf::Vector2i> path;
	std::vector<sf::Vector2i>::reverse_iterator path_it;

public:
	sf::Sprite sprite;
	sf::Vector2i mapPosition;
	bool busy = false;

	player(std::vector<sf::Texture>&, sf::Vector2i, map&);
	void moveSprite(map&);
	void move(map&, sf::Vector2i);
	void move(map&, std::vector<sf::Vector2i>&);
	void step(map&);
};
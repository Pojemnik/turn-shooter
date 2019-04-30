#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class buttonCode { walk, shoot, none, endTurn };

class UIbutton
{
private:
	std::vector<sf::Texture> textures;
	std::vector<sf::Texture>::iterator textures_it;
public:
	sf::Sprite sprite;
	const sf::Vector2f size = sf::Vector2f(40, 40);
	const sf::Vector2f position;
	const buttonCode code;
	unsigned int redrawCounter = 0;

	UIbutton(const std::vector<sf::Texture> tex, const sf::Vector2f pos, const buttonCode c);
	void step();
	void reset();
};

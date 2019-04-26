#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class buttonCode {walk, shoot, none};

class UIbutton
{
private:
	std::vector<sf::Texture> textures;
	std::vector<sf::Texture>::iterator textures_it;
public:
	sf::Sprite sprite;
	const sf::Vector2f size = sf::Vector2f(40, 40);
	sf::Vector2f position;
	buttonCode code;

	UIbutton(std::vector<sf::Texture> tex, sf::Vector2f pos, buttonCode c);
	void step();
	void reset();
};

class gameUI
{
private:
	sf::Sprite sprite, apSprite, activeSprite;
	sf::Texture texture, apTex, activeTex;
	std::vector<UIbutton> buttons;
	int phase, actionPoints;

public:
	sf::Vector2f size;
	sf::Vector2f position;
	gameUI(sf::Vector2f pos);
	void draw(sf::RenderWindow &window);
	void hoover(sf::Vector2i mousePos);
	bool isHovered(sf::Vector2i mousePos);
	buttonCode clicked(sf::Vector2i mousePos);
	void setActivePhase(int p);
	void setAP(int ap);
};

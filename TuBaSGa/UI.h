#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "UIButton.h"

class gameUI
{
private:
	sf::Sprite spriteLeftBottom, spriteRightTop, apSprite, activeSprite;
	sf::Texture textureLeftBottom, textureRightTop, apTex, activeTex;
	std::vector<UIbutton> buttons;
	int phase, actionPoints;
	const sf::Vector2f sizeLeftBottom, sizeRightTop;
	const sf::Vector2f positionLeftBottom, positionRightTop;

public:	
	gameUI(const sf::Vector2f posLB, const sf::Vector2f posRT);
	void draw(sf::RenderWindow &window);
	void hoover(const sf::Vector2i mousePos);
	bool isHovered(const sf::Vector2i mousePos);
	buttonCode clicked(const sf::Vector2i mousePos);
	void setActivePhase(const int p);
	void setAP(const int ap);
};

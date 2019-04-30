#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "UIButton.h"

enum class topState {opening, closing, open, closed};

class gameUI
{
private:
	sf::Sprite spriteLeftBottom, spriteRightTop, spriteTop, apSprite, activeSprite;
	sf::Texture textureLeftBottom, textureRightTop, textureTopRed, textureTopGreen, apTex, activeTex;
	sf::Text turnCounter;
	sf::Font font;
	std::vector<UIbutton> buttons;
	int phase, actionPoints;
	const sf::Vector2f sizeLeftBottom, sizeRightTop, sizeTop;
	const sf::Vector2f positionLeftBottom, positionRightTop;
	const sf::Vector2f positionTopOpen, positionTopClosed;
	sf::Vector2f positionTop;
	topState stateTop;

public:	
	gameUI(const sf::Vector2f posLB, const sf::Vector2f posRT, const sf::Vector2f posT);
	void draw(sf::RenderWindow &window);
	void hoover(const sf::Vector2i mousePos);
	bool isHovered(const sf::Vector2i mousePos);
	buttonCode clicked(const sf::Vector2i mousePos);
	void setActivePhase(const int p);
	void setAP(const int ap);
};

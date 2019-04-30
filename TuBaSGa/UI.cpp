#include "UI.h"

gameUI::gameUI(const sf::Vector2f posLB, const sf::Vector2f posRT, const sf::Vector2f posT) : positionLeftBottom(posLB), sizeLeftBottom(200, 80), positionRightTop(posRT), sizeRightTop(127, 80), sizeTop(200, 100), positionTopClosed(posT), positionTopOpen(posT.x, posT.y + 90)
{
	textureLeftBottom.loadFromFile("../img/ui_left_bottom.png");
	spriteLeftBottom.setTexture(textureLeftBottom);
	spriteLeftBottom.setPosition(posLB);
	textureRightTop.loadFromFile("../img/ui_right_top.png");
	spriteRightTop.setTexture(textureRightTop);
	spriteRightTop.setPosition(posRT);
	textureTopRed.loadFromFile("../img/ui_top_red.png");
	textureTopGreen.loadFromFile("../img/ui_top_green.png");
	spriteTop.setTexture(textureTopRed);
	spriteTop.setPosition(positionTopClosed);
	apTex.loadFromFile("../img/ap.png");
	apSprite.setTexture(apTex);
	activeTex.loadFromFile("../img/active_phase.png");
	activeSprite.setTexture(activeTex);
	std::vector<sf::Texture> textures;
	sf::Texture temp;
	temp.loadFromFile("../img/walk_ico/walk_ico1.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/walk_ico/walk_ico2.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/walk_ico/walk_ico3.png");
	textures.push_back(temp);
	buttons.push_back(UIbutton(textures, sf::Vector2f(posLB.x + 20, posLB.y + 20), buttonCode::walk));
	textures.clear();
	temp.loadFromFile("../img/shoot_ico/shoot_ico1.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/shoot_ico/shoot_ico2.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/shoot_ico/shoot_ico3.png");
	textures.push_back(temp);
	buttons.push_back(UIbutton(textures, sf::Vector2f(posLB.x + 80, posLB.y + 20), buttonCode::shoot));
	font.loadFromFile("../misc/lucon.ttf");
	textureTopRed.loadFromFile("../img/ui_top_red.png");
	textureTopGreen.loadFromFile("../img/ui_top_green.png");
	phase = 1;
	actionPoints = 2;
	stateTop = topState::closed;
}

void gameUI::draw(sf::RenderWindow &window)
{
	window.draw(spriteLeftBottom);
	window.draw(spriteRightTop);
	window.draw(spriteTop);
	for (const auto& it : buttons)
	{
		window.draw(it.sprite);
	}
	switch (phase)
	{
	case 1:
		activeSprite.setPosition(sf::Vector2f(positionLeftBottom.x + 29, positionLeftBottom.y + 68));
		break;
	case 2:
		activeSprite.setPosition(sf::Vector2f(positionLeftBottom.x + 90, positionLeftBottom.y + 68));
	}
	if (actionPoints > 0)
	{
		apSprite.setPosition(sf::Vector2f(positionLeftBottom.x + 8, positionLeftBottom.y + 40));
		window.draw(apSprite);
		if (actionPoints > 1)
		{
			apSprite.setPosition(sf::Vector2f(positionLeftBottom.x + 8, positionLeftBottom.y + 7));
			window.draw(apSprite);
		}
	}
	window.draw(activeSprite);
}

void gameUI::hoover(const sf::Vector2i mousePos)
{
	if (isHovered(mousePos) || actionPoints == 0)
	{
		for (auto& it : buttons)
		{
			if (mousePos.x > it.position.x && mousePos.x < it.position.x + it.size.x && mousePos.y > it.position.y && mousePos.y < it.position.y + it.size.y)
			{
				if (it.redrawCounter++ >= 10)
				{
					it.step();
					it.redrawCounter = 0;
				}
			}
		}
		if (mousePos.x > positionTopOpen.x && mousePos.x < positionTopOpen.x + sizeTop.x && mousePos.y > positionTopOpen.y && mousePos.y < positionTopOpen.y + sizeTop.y || actionPoints == 0)
		{

			if (stateTop == topState::opening)
			{
				positionTop = sf::Vector2f(spriteTop.getPosition().x, spriteTop.getPosition().y + 2);
				spriteTop.setPosition(positionTop);
				if (spriteTop.getPosition() == positionTopOpen)
					stateTop = topState::open;
			}
			if(stateTop != topState::open)
			stateTop = topState::opening;
		}
		else
		{
			stateTop = topState::closing;
			if (spriteTop.getPosition() == positionTopClosed)
				stateTop = topState::closed;
			else
			{
				positionTop = sf::Vector2f(spriteTop.getPosition().x, spriteTop.getPosition().y - 2);
				spriteTop.setPosition(positionTop);
			}
		}
	}
	else
	{
		stateTop = topState::closing;
		if (spriteTop.getPosition() == positionTopClosed)
			stateTop = topState::closed;
		else
		{
			positionTop = sf::Vector2f(spriteTop.getPosition().x, spriteTop.getPosition().y - 2);
			spriteTop.setPosition(positionTop);
		}
	}
}

bool gameUI::isHovered(const sf::Vector2i mousePos)
{
	return (mousePos.x > positionLeftBottom.x && mousePos.x < positionLeftBottom.x + sizeLeftBottom.x && mousePos.y > positionLeftBottom.y && mousePos.y < positionLeftBottom.y + sizeLeftBottom.y)
		|| (mousePos.x > positionRightTop.x && mousePos.x < positionRightTop.x + sizeRightTop.x && mousePos.y > positionRightTop.y && mousePos.y < positionRightTop.y + sizeRightTop.y)
		|| (mousePos.x > positionTopOpen.x && mousePos.x < positionTopOpen.x + sizeTop.x && mousePos.y > positionTopOpen.y && mousePos.y < positionTopOpen.y + sizeTop.y);
}

buttonCode gameUI::clicked(const sf::Vector2i mousePos)
{
	for (auto& it : buttons)
	{
		if (mousePos.x > it.position.x && mousePos.x < it.position.x + it.size.x && mousePos.y > it.position.y && mousePos.y < it.position.y + it.size.y)
		{
			return it.code;
		}
	}
	if (mousePos.x > positionTop.x+20 && mousePos.x < positionTop.x + sizeTop.x - 20 && mousePos.y > positionTop.y + 15 && mousePos.y < positionTop.y + sizeTop.y - 25)
	{
		return buttonCode::endTurn;
	}
	return buttonCode::none;
}

void gameUI::setActivePhase(const int p)
{
	phase = p;
}

void gameUI::setAP(const int ap)
{
	actionPoints = ap;
	if (actionPoints == 2)
		spriteTop.setTexture(textureTopRed);
	else
		if(actionPoints == 0)
			spriteTop.setTexture(textureTopGreen);
}
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "map.h"
#include "player.h"
#include "UI.h"

const float frameRate = 60;
const unsigned int windowWidth = 1280;
const unsigned int windowHeight = 720;

enum class turnPhase { moving, shooting };
enum class turnUser { player, ai };

struct turnStruct
{
	turnUser user = turnUser::player;
	turnPhase phase = turnPhase::moving;
	unsigned int number = 1;
	unsigned int actionPoints = 2;
};

int gameLoop(sf::RenderWindow &window)
{
	std::vector<sf::Drawable*> vect;
	sf::Clock clock;
	int zoomLevel = 0;
	std::ifstream file;
	volatile unsigned int framesRendered = 0;
	map Map(30, sf::Vector2f(10, 10));
	sf::Image mapImage;
	mapImage.loadFromFile("../maps/map2.png");
	if (Map.loadFromBitmap(mapImage))
	{
		std::cout << "Map error";
		system("pause");
		return 0;
	}
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	std::vector<sf::Texture> playerTex;
	sf::Texture tex;
	tex.loadFromFile("../img/player/2.png");
	playerTex.push_back(tex);
	tex.loadFromFile("../img/player/1.png");
	playerTex.push_back(tex);
	tex.loadFromFile("../img/player/3.png");
	playerTex.push_back(tex);
	player Player(playerTex, sf::Vector2i(1, 1), Map);
	vect.push_back(&Player.sprite);
	sf::Vector2i target = sf::Vector2i(-1, -1);
	std::vector<sf::Vector2i> path(1, sf::Vector2i(0, 0));
	gameUI ui(sf::Vector2f(0, 640), sf::Vector2f(windowWidth-127,0), sf::Vector2f(windowWidth/2-100, -90));
	turnStruct turn;
	while (window.isOpen())
	{
		bool pathRefresh = false;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return -2;
			}
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				sf::Vector2f diff = Map.position - sf::Vector2f(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
				Map.scroll(event.mouseWheelScroll.delta * 3, diff);
				Player.moveSprite(Map);
			}
			if (event.type == sf::Event::MouseMoved)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
				{
					Map.scroll(0, sf::Vector2f(mousePos.x - event.mouseMove.x, mousePos.y - event.mouseMove.y));
					Player.moveSprite(Map);
				}
				mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
				if (mousePos.x > Map.position.x && mousePos.y > Map.position.y && mousePos.x <= Map.position.x + Map.size.x * Map.tilesSizes && mousePos.y <= Map.position.y + Map.size.y * Map.tilesSizes)
				{
					target = sf::Vector2i((mousePos.x - Map.position.x - 1) / Map.tilesSizes, (mousePos.y - Map.position.y - 1) / Map.tilesSizes);
				}
				else
				{
					target = sf::Vector2i(-1, -1);
				}
				std::cout << target.x << ' ' << target.y << std::endl;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					if (ui.isHovered(mousePos))
					{
						switch (ui.clicked(mousePos))
						{
						case buttonCode::walk:
							turn.phase = turnPhase::moving;
							ui.setActivePhase(1);
							std::cout << "walk button pressed!" << std::endl;
							break;
						case buttonCode::shoot:
							turn.phase = turnPhase::shooting;
							ui.setActivePhase(2);
							std::cout << "shoot button pressed!" << std::endl;
							break;
						case buttonCode::endTurn:
							std::cout << "End turn!" << std::endl;
							break;
						case buttonCode::none:
							break;
						}
					}
					else if (turn.phase == turnPhase::moving)
					{
						if (target != sf::Vector2i(-1, -1) && path[0] != sf::Vector2i(0, 0) && !Player.busy && path.size() <= Player.range && turn.actionPoints > 0)
						{
							turn.actionPoints--;
							ui.setAP(turn.actionPoints);
							Player.move(Map, path);
						}
					}
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
				case sf::Keyboard::Num1:
					turn.phase = turnPhase::moving;
					ui.setActivePhase(1);
					std::cout << "walk button pressed!" << std::endl;
					break;
				case sf::Keyboard::Num2:
					turn.phase = turnPhase::shooting;
					ui.setActivePhase(2);
					std::cout << "shoot button pressed!" << std::endl;
					break;
				}
			}
		}
		window.clear();
		Map.draw(window);
		ui.hoover(mousePos);
		if (target != sf::Vector2i(-1, -1) && !Player.busy && turn.phase == turnPhase::moving && !ui.isHovered(mousePos))
		{
			path = Map.drawPath(Player.mapPosition, target, window);
			if (path[0] != sf::Vector2i(0, 0))
				Map.drawRange(Player.range, Player.mapPosition, window);
		}
		if (++framesRendered == UINT_MAX)
		{
			framesRendered = 0;
		}
		if (framesRendered % 10 == 0)
		{
			Player.step(Map);
		}
		
		window.draw(Player.sprite);
		ui.draw(window);
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / frameRate);
		clock.restart();
	}
	return 0;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "NAME");
	gameLoop(window);
	return 0;
}
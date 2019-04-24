#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "map.h"
#include "player.h"
#include "UI.h"

float frameRate = 60;

enum class turnPhase { moving, shooting };

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
	//Map.print();
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
	gameUI ui(sf::Vector2f(0, 640));
	turnPhase phase = turnPhase::moving;
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
				}
				mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
				Player.moveSprite(Map);
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
				if (event.mouseButton.button == sf::Mouse::Button::Left && mousePos.x > ui.position.x && mousePos.x < ui.position.x + ui.size.x && mousePos.y > ui.position.y && mousePos.y < ui.position.y + ui.size.y)
				{
					switch (ui.clicked(mousePos))
					{
					case buttonCode::walk:
						phase = turnPhase::moving;
						std::cout << "walk button pressed!" << std::endl;
						break;
					case buttonCode::shoot:
						phase = turnPhase::shooting;
						break;
					case buttonCode::none:
						break;
					}
				}
				else if (phase == turnPhase::moving)
				{
					if (event.mouseButton.button == sf::Mouse::Button::Left && target != sf::Vector2i(-1, -1) && path[0] != sf::Vector2i(0, 0) && !Player.busy)
					{
						Player.move(Map, path);
					}
				}
			}
		}
		window.clear();
		Map.draw(window);
		if (target != sf::Vector2i(-1, -1) && !Player.busy && phase == turnPhase::moving && !(mousePos.x > ui.position.x && mousePos.x < ui.position.x + ui.size.x && mousePos.y > ui.position.y && mousePos.y < ui.position.y + ui.size.y))
		{
			path = Map.drawPath(Player.mapPosition, target, window);
			if (path[0] != sf::Vector2i(0, 0))
				Map.drawRange(20, Player.mapPosition, window);
		}
		if (++framesRendered == UINT_MAX)
		{
			framesRendered = 0;
		}
		if (framesRendered % 10 == 0)
		{
			Player.step(Map);
			ui.hoover(mousePos);
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
	sf::RenderWindow window(sf::VideoMode(1280, 720), "NAME");
	gameLoop(window);
	return 0;
}
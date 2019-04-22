#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <string>

struct tileData
{
	sf::Texture texture;
	int type;
	sf::Color color;
};

class map
{
private:
	int** tiles;
	sf::Sprite** tilesSprites;
	std::vector<tileData> tilesData;
	sf::Texture targetTex;
	sf::Texture lineTex[2];
	sf::Texture rangeTex[2];
	sf::Texture inRangeTex;
	int** vis;

public:
	sf::Vector2f position;
	float tilesSizes;
	sf::Vector2u size;

	map(float, sf::Vector2f);
	int loadFromBitmap(sf::Image);
	void print();
	void draw(sf::RenderWindow&);
	void scroll(int, sf::Vector2f);
	std::vector<sf::Vector2i> drawPath(sf::Vector2i, sf::Vector2i, sf::RenderWindow&);
	bool canPass(int, int);
	void drawRange(int range, sf::Vector2i startPos, sf::RenderWindow& window);
};
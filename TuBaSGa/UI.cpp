#include "UI.h"

gameUI::gameUI(sf::Vector2f pos) : position(pos)
{
	size = sf::Vector2f(200, 80);
	texture.loadFromFile("../img/ui_left_bottom.png");
	sprite.setTexture(texture);
	sprite.setPosition(pos);
	std::vector<sf::Texture> textures;
	sf::Texture temp;
	temp.loadFromFile("../img/walk_ico/walk_ico1.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/walk_ico/walk_ico2.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/walk_ico/walk_ico3.png");
	textures.push_back(temp);
	buttons.push_back(UIbutton(textures, sf::Vector2f(pos.x + 20, pos.y + 20), buttonCode::walk));
	textures.clear();
	temp.loadFromFile("../img/shoot_ico/shoot_ico1.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/shoot_ico/shoot_ico2.png");
	textures.push_back(temp);
	temp.loadFromFile("../img/shoot_ico/shoot_ico3.png");
	textures.push_back(temp);
	buttons.push_back(UIbutton(textures, sf::Vector2f(pos.x + 80, pos.y + 20), buttonCode::shoot));
}

void gameUI::draw(sf::RenderWindow &window)
{
	window.draw(sprite);
	for (auto& it : buttons)
	{
		window.draw(it.sprite);
	}
}

void gameUI::hoover(sf::Vector2i mousePos)
{
	if (isHovered(mousePos))
	{
		for (auto& it : buttons)
		{
			if (mousePos.x > it.position.x && mousePos.x < it.position.x + it.size.x && mousePos.y > it.position.y && mousePos.y < it.position.y + it.size.y)
			{
				it.step();
			}
		}
	}
}

bool gameUI::isHovered(sf::Vector2i mousePos)
{
	return (mousePos.x > position.x && mousePos.x < position.x + size.x && mousePos.y > position.y && mousePos.y < position.y + size.y);
}

buttonCode gameUI::clicked(sf::Vector2i mousePos)
{
	for (auto& it : buttons)
	{
		if (mousePos.x > it.position.x && mousePos.x < it.position.x + it.size.x && mousePos.y > it.position.y && mousePos.y < it.position.y + it.size.y)
		{
			return it.code;
		}
	}
	return buttonCode::none;
}

UIbutton::UIbutton(std::vector<sf::Texture> tex, sf::Vector2f pos, buttonCode c) : textures(tex), position(pos), code(c)
{
	sprite.setTexture(textures[0]);
	sprite.setPosition(position);
	textures_it = textures.begin();
}

void UIbutton::step()
{
	if (++textures_it == textures.end())
		textures_it = textures.begin();
	sprite.setTexture(*textures_it);
}

void UIbutton::reset()
{
	textures_it = textures.begin();
	sprite.setTexture(*textures_it);
}
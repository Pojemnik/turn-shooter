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
	buttons.push_back(UIbutton(textures, sf::Vector2f(pos.x + 20, pos.y + 20),buttonCode::walk));
}

void gameUI::draw(sf::RenderWindow &window)
{
	window.draw(sprite);
	for (std::vector<UIbutton>::iterator it = buttons.begin(); it != buttons.end(); it++)
	{
		window.draw(it->sprite);
	}
}

void gameUI::hoover(sf::Vector2i mousePos)
{
	for (std::vector<UIbutton>::iterator it = buttons.begin(); it != buttons.end(); it++)
	{
		if (mousePos.x > it->position.x && mousePos.x < it->position.x + it->size.x && mousePos.y > it->position.y && mousePos.y < it->position.y + it->size.y)
		{
			it->step();
		}
	}
}

buttonCode gameUI::clicked(sf::Vector2i mousePos)
{
	for (std::vector<UIbutton>::iterator it = buttons.begin(); it != buttons.end(); it++)
	{
		if (mousePos.x > it->position.x && mousePos.x < it->position.x + it->size.x && mousePos.y > it->position.y && mousePos.y < it->position.y + it->size.y)
		{
			return it->code;
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
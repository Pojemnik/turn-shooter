#include "UIButton.h"

UIbutton::UIbutton(const std::vector<sf::Texture> tex, const sf::Vector2f pos, const buttonCode c) : textures(tex), position(pos), code(c)
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
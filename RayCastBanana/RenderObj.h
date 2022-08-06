#pragma once
#include <SFML/Graphics/Sprite.hpp>

struct RenderObj
{
	sf::Image img;
	double dist;
	int screenX;

	RenderObj(sf::Image img, double dist, int projectX)
	{
		this->img = img;
		this->dist = dist;
		this->screenX = projectX;
	}
};

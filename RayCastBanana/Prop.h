#pragma once
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Prop
{
private:
	sf::Image m_img;
	double m_posX;
	double m_posY;
public:
	double getPosX();
	double getPosY();
	sf::Image getImage();
	Prop(std::string path, double x, double y);
};

#include "Prop.h"
#include "texture_loader.h"

double Prop::getPosX()
{
	return m_posX;
}

double Prop::getPosY()
{
	return m_posY;
}

sf::Image Prop::getImage()
{
	return m_img;
}

Prop::Prop(std::string path, double x, double y)
{
	m_img = load_img(path);
	m_posX = x;
	m_posY = y;
}

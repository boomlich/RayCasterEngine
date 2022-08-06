#include "Door.h"

sf::Image Door::getDoorImg()
{
	return m_doorImg;
}

sf::Image Door::getFrameImg()
{
	return m_frameImg;
}

sf::Image Door::getFloorImg()
{
	return m_floorImg;
}

double Door::getPosX()
{
	return m_posX;
}

double Door::getPosY()
{
	return m_posY;
}

double Door::getOpenness()
{
	return m_openness;
}

bool Door::isLocked()
{
	return m_isLocked;
}

void Door::unlock()
{
	m_isLocked = false;
}

void Door::lock()
{
	m_isLocked = true;
}

void Door::open()
{
	m_openness = 1;
}

void Door::close()
{
	m_openness = 0;
}

void Door::update(double dt)
{
}

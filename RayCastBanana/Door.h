#pragma once
#include <SFML/Graphics/Image.hpp>

class Door
{
private:
	int m_posX;
	int m_posY;
	double m_openness;
	bool m_isLocked;
	sf::Image m_doorImg;
	sf::Image m_frameImg;
	sf::Image m_floorImg;

public:
	sf::Image getDoorImg();
	sf::Image getFrameImg();
	sf::Image getFloorImg();

	double getPosX();
	double getPosY();

	double getOpenness();
	bool isLocked();

	void unlock();
	void lock();
	void open();
	void close();
	void update(double dt);
};

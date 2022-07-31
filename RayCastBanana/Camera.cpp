#include "Camera.h"

#include <iostream>

#include "MathMethods.h"

void Camera::attachToPlayer(Player& player)
{
	m_posX = player.getPos().x;
	m_posY = player.getPos().y;
	m_dirX = player.getDir().x;
	m_dirY = player.getDir().y;

	std::cout << *player.getPos().x << std::endl;
}

void Camera::update()
{
	for (int i = 0; i < 1; ++i)
	{
		const double rayX = *m_dirX;
		const double rayY = *m_dirY;

		const double rayLength = vectorLength(sf::Vector2f(rayX, rayY));
		const double deltaDistX = abs(1 / rayX) * rayLength;
		const double deltaDistY = abs(1 / rayY) * rayLength;




	}
}

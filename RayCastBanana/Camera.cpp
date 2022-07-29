#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>

#include "camera.h"


Camera::Camera()
{
	m_posX = 10.0;
	m_posY = 10.0;
	m_dirX = 0.0;
	m_dirY = -1.0;
	m_planeX = 1.0f;
	m_planeY = 1.0f;
}


void Camera::rayCast(sf::RenderWindow& window)
{
	std::cout << "test" << std::endl;
}

void Camera::calculateStepAndInitDist(double& rayDir, double& sideDist, double& deltaDist, int& step, int& map)
{
	if (rayDir < 0)
	{
		sideDist = (m_posX - map) * deltaDist;
		step = -1;
	}
	else
	{
		sideDist = (map + 1.0 - m_posX) * deltaDist;
		step = 1;
	}
}


void Camera::update(sf::RenderWindow& window, Grid& grid)
{
	// rate of increase per cell for each axis
	const double rayLength = sqrt(m_dirX * m_dirX + m_dirY * m_dirY);
	const double deltaDistX = abs(1.0 / m_dirX * rayLength);
	const double deltaDistY = abs(1.0 / m_dirY * rayLength);

	// Current cell the camera is inside
	int mapX = m_posX;
	int mapY = m_posY;

	double sideDistX;
	double sideDistY;

	bool verticalWall;
	bool hit = false;
	int stepX;
	int stepY;

	double perpWallDist;


	if (m_dirX < 0)
	{
		sideDistX = (m_posX - mapX) * deltaDistX;
		stepX = -1;
	}
	else
	{
		sideDistX = (mapX + 1.0 - m_posX) * deltaDistX;
		stepX = 1;
	}
	if (m_dirY < 0)
	{
		sideDistY = (m_posY - mapY) * deltaDistY;
		stepY = -1;
	}
	else
	{
		sideDistY = (mapY + 1.0 - m_posY) * deltaDistY;
		stepY = 1;
	}

	// Enable deltaDist after first leaving first tile
	int firstTileX = 0;
	int firstTileY = 0;

	// First check closest
	while (!hit)
	{
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX * firstTileX;
			mapX += stepX;
			verticalWall = true;
			firstTileX = 1;
		} else
		{
			sideDistY += deltaDistY * firstTileY;
			mapY += stepY;
			verticalWall = false;
			firstTileY = 1;
		}

		if (grid.getCell(sf::Vector2i(mapX, mapY)) == WALL1)
		{
			hit = true;
			std::cout << "Wall hit at: " << mapX << " : " << mapY << std::endl;
		}
	}

	if (verticalWall)
	{
		perpWallDist = sideDistX;
	} else
	{
		perpWallDist = sideDistY;
	}

	std::cout << perpWallDist << std::endl;
	
	// Height of the wall
	const int height = window.getSize().y; // screen height
	const int lineHeight = (int)(height / perpWallDist); 

	int drawStart = (height - lineHeight) / 2;
	

	// Draw wall
	sf::RectangleShape wall(sf::Vector2f(1.0f, lineHeight));
	wall.setPosition(window.getSize().x / 2, drawStart);
	window.draw(wall);
}

void Camera::setPosition(sf::Vector2f pos)
{
	m_posX = pos.x;
	m_posY = pos.y;
}

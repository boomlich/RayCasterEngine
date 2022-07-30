#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>

#include "camera.h"
#include "MathMethods.h"


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

void Camera::calculateStepAndInitDist(const double& rayDir, double& sideDist, const double& deltaDist, int& step, const int& map, const double& pos)
{
	if (rayDir < 0)
	{
		sideDist = (pos - map) * deltaDist;
		step = -1;
	}
	else
	{
		sideDist = (map + 1.0 - pos) * deltaDist;
		step = 1;
	}
}

void Camera::digDiffAnalysis(
		double& sideDistX, double& sideDistY, int& mapX, int& mapY, const int& stepX, const int& stepY,
		bool& vWall, const double& deltaDistX, const double& deltaDistY, Grid& grid)
{

	// Max limit of iterations
	const int max = 100;
	int cnt = 0;

	while (cnt < max)
	{
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			mapX += stepX;
			vWall = true;
		}
		else
		{
			sideDistY += deltaDistY;
			mapY += stepY;
			vWall = false;
		}

		if (grid.getCell(sf::Vector2i(mapX, mapY)) == WALL1)
		{
			break;
		}
		cnt++;
	}
}


void Camera::update(sf::RenderWindow& window, Grid& grid)
{
	const sf::Vector2f camPlane = vectorNormal(sf::Vector2f((float)m_dirX, (float)m_dirY));
	m_planeX = (double)camPlane.x * 0.5;
	m_planeY = (double)camPlane.y * 0.5;

	const int width = window.getSize().x;
	const int height = window.getSize().y;

	for (int i = 0; i < width; ++i)
	{
		double cameraX = 2 * i / double(width) - 1; // -1 and 1;
		double rayX = m_dirX + m_planeX * cameraX;
		double rayY = m_dirY + m_planeY * cameraX;

		// rate of increase per cell for each axis
		const double rayLength = vectorLength(sf::Vector2f((float)rayX, (float)rayY));
		const double deltaDistX = abs(1.0 / rayX * rayLength);
		const double deltaDistY = abs(1.0 / rayY * rayLength);

		// Angle between the direction and the ray. Calculated with the dot product.
		double rayAngle = acos((rayX * m_dirX + rayY * m_dirY) / rayLength);

		// Current cell the camera is inside
		int mapX = (int)m_posX;
		int mapY = (int)m_posY;

		// Euclidean distance to the wall;
		double sideDistX, sideDistY;

		bool verticalWall; // True if the wall detected is a vertical wall
		int stepX, stepY; // Direction of the ray. If moving left -> stepX = -1.

		double perpWallDist;

		calculateStepAndInitDist(rayX, sideDistX, deltaDistX, stepX, mapX, m_posX);
		calculateStepAndInitDist(rayY, sideDistY, deltaDistY, stepY, mapY, m_posY);

		digDiffAnalysis(sideDistX, sideDistY, mapX, mapY, stepX, stepY, verticalWall, deltaDistX, deltaDistY, grid);

		sf::Color wallColor;
		if (verticalWall)
		{
			perpWallDist = (sideDistX - deltaDistX) * cos(rayAngle);
			wallColor = sf::Color(73 / perpWallDist, 230 / perpWallDist, 97 / perpWallDist, 255);
			//wallColor = sf::Color(73, 230, 97, 255);
		}
		else
		{
			perpWallDist = (sideDistY - deltaDistY) * cos(rayAngle);
			wallColor = sf::Color(59 / perpWallDist, 191 / perpWallDist, 79 / perpWallDist, 255);
			//wallColor = sf::Color(59, 191, 79, 255);
		}
		

		// Calculate height of the wall
		const int lineHeight = (int)(height / perpWallDist);
		const int drawStart = (height - lineHeight) / 2;


		// std::cout << "COLUMN: " << i << " WALLTYPE: " << (verticalWall ? "vertical" : "horizontal") << std::endl;
		// std::cout << "wall dist: " << perpWallDist << std::endl;
		// std::cout << "lineHeight: " << lineHeight << std::endl << std::endl;

		// Draw wall
		sf::RectangleShape wall(sf::Vector2f(1.0f, lineHeight));
		wall.setPosition(i, drawStart);
		wall.setFillColor(wallColor);
		window.draw(wall);
	}
}

void Camera::setPosition(sf::Vector2f pos)
{
	m_posX = pos.x;
	m_posY = pos.y;
}

void Camera::moveForward(double x)
{
	m_posX += m_dirX * x;
	m_posY += m_dirY * x;
	std::cout << "updated pos: " << m_posX << " : " << m_posY << std::endl;
}

void Camera::rotate(double angle)
{
	double oldDirX = m_dirX;
	m_dirX = m_dirX * cos(angle) - m_dirY * sin(angle);
	m_dirY = oldDirX * sin(angle) + m_dirY * cos(angle);

	std::cout << m_dirX << " : " << m_dirY << std::endl;

}

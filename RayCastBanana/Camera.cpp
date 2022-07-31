#include "Camera.h"

#include <iostream>
#include <SFML/Graphics/Color.hpp>

#include "MathMethods.h"

#define PI 3.14159265359

void Camera::attachToPlayer(Player& player)
{
	m_posX = player.getPos().x;
	m_posY = player.getPos().y;
	m_dirX = player.getDir().x;
	m_dirY = player.getDir().y;
}

void Camera::update(int screenWidth, Grid& grid)
{
	m_rays.clear();
	double fov = PI / 2;

	for (int i = 0; i < screenWidth; ++i)
	{
		double cameraX = (i * 2.0 / (double)(screenWidth - 1)) - 1.0;
		double angle = fov / 2.0 * cameraX;
		sf::Vector2<double> rotatedDir = rotateVector(sf::Vector2<double>(*m_dirX, *m_dirY), angle);

		const double rayDirX = rotatedDir.x;
		const double rayDirY = rotatedDir.y;

		const double deltaDistX = abs(1 / rayDirX);
		const double deltaDistY = abs(1 / rayDirY);

		double sideDistX;
		double sideDistY;

		int mapX = (int)*m_posX;
		int mapY = (int)*m_posY;

		int stepX;
		int stepY;


		if (rayDirX < 0) // left direction
		{
			sideDistX = (*m_posX - (double)mapX) * deltaDistX;
			stepX = -1;
		} else
		{
			sideDistX = ((double)mapX + 1.0 - *m_posX) * deltaDistX;
			stepX = 1;
		}
		if (rayDirY < 0) // Up direction
		{
			sideDistY = (*m_posY - (double)mapY) * deltaDistY;
			stepY = -1;
		}
		else
		{
			sideDistY = ((double)mapY + 1.0 - *m_posY) * deltaDistY;
			stepY = 1;
		}

		const int max = 100;
		int cnt = 0;

		bool verticleWall;

		while (cnt < max)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				verticleWall = true;
			} else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				verticleWall = false;
			}

			if (grid.getCell(sf::Vector2i(mapX, mapY)) == WALL1)
			{
				break;
			}
		}

		double perpWallDist;
		double rayLength;

		if (verticleWall)
		{
			rayLength = sideDistX - deltaDistX;
			perpWallDist = rayLength * cos(angle);
			//wallColor = sf::Color(73 / perpWallDist, 230 / perpWallDist, 97 / perpWallDist, 255);
		} else
		{
			rayLength = sideDistY - deltaDistY;
			perpWallDist = rayLength * cos(angle);
			//wallColor = sf::Color(59 / perpWallDist, 191 / perpWallDist, 79 / perpWallDist, 255);
		}

		m_rays.push_back(Ray(perpWallDist, rayLength, rayDirX, rayDirY, verticleWall));
	}
}

#include "Camera.h"

#include <iostream>

#include "MathMethods.h"

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

	planeX = -*m_dirY;
	planeY = *m_dirX;

	for (int i = 0; i < screenWidth; ++i)
	{
		double cameraX = (i * 2.0 / (double)(screenWidth - 1)) - 1.0; // from -1 to 1.
		const double rayDirX = *m_dirX + planeX * cameraX; 
		const double rayDirY = *m_dirY + planeY * cameraX;
		const double rayDirLength = vectorLength(rayDirX, rayDirY);
		const double angle = acos((rayDirX * *m_dirX + rayDirY * *m_dirY)/ rayDirLength);

		const double deltaDistX = abs(1 / rayDirX) * rayDirLength;
		const double deltaDistY = abs(1 / rayDirY) * rayDirLength;

		double sideDistX;
		double sideDistY;

		int mapX = (int)*m_posX;
		int mapY = (int)*m_posY;

		int stepX;
		int stepY;

		bool verticleWall;

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
		double wallIntersectPoint;
		sf::Vector2<double> unitDir = vectorUnit(rayDirX, rayDirY, rayDirLength);

		if (verticleWall)
		{
			rayLength = sideDistX - deltaDistX;
			perpWallDist = rayLength * cos(angle);
			wallIntersectPoint = (*m_posY + unitDir.y * rayLength) - (double)mapY;
		} else
		{
			rayLength = sideDistY - deltaDistY;
			perpWallDist = rayLength * cos(angle);
			wallIntersectPoint = (*m_posX + unitDir.x * rayLength) - (double)mapX;
		}


		m_rays.push_back(Ray(perpWallDist, wallIntersectPoint, rayDirX, rayDirY, verticleWall));
	}
}

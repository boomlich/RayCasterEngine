#include "Camera.h"

#include <iostream>

#include "MathMethods.h"
#include "Prop.h"

double Camera::angleFromDir(double x, double y, double length)
{
	int left = 1;
	if (isLeft(0.0, 0.0, *m_dirX, *m_dirY, x, y))
	{
		
		left = -1;
	}

	return acos((x * *m_dirX + y * *m_dirY) / length) * left;
}

void Camera::attachToPlayer(Player& player)
{
	m_posX = player.getPos().x;
	m_posY = player.getPos().y;
	m_dirX = player.getDir().x;
	m_dirY = player.getDir().y;
}

void Camera::update(int screenWidth, int screenHeight, Grid& grid, std::vector<Prop> props)
{
	m_rays.clear();
	m_renderObj.clear();

	planeX = -*m_dirY;
	planeY = *m_dirX;

	// WALL CASTING
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

		m_rays.push_back(Ray(perpWallDist, wallIntersectPoint, rayDirX, rayDirY, verticleWall, angle));
	}

	// Raycast props
	double halfW = screenWidth / 2.0;
	for (auto prop : props)
	{
		double propDirX = prop.getPosX() - *m_posX;
		double propDirY = prop.getPosY() - *m_posY;

		// euclidean distance to the object
		double propWorldDist = vectorLength(propDirX, propDirY);

		// Angle from player direction to object
		double propAngle = angleFromDir(propDirX, propDirY, propWorldDist);

		// Calculate the position of the object on the projection screen
		double projectonX = halfW - tan(propAngle) * halfW;

		// Perpendicular distance to the object from the camera
		double perpDist = propWorldDist * cos(propAngle);

		m_renderObj.push_back(RenderObj(prop.getImage(), perpDist, projectonX));
	}
}

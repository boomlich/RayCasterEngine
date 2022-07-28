#include "camera.h"
#include "m_grid.h"

Camera::Camera()
{
	m_screenWidth = 300;
	m_position = sf::Vector2f(1.0f, 1.0f);
	m_direction = sf::Vector2f(-1.0f, 0.0f);
	m_cameraPlane = sf::Vector2f(0.0f, 0.6666f);
}

Camera::Camera(int screenWidth, int screenHeight, sf::Vector2f position, sf::Vector2f direction, sf::Vector2f cameraPlane, Grid& grid)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_position = position;
	m_direction = direction;
	m_cameraPlane = cameraPlane;
	m_grid = grid;
}

void Camera::ray_trace()
{
	for (int i = 0; i < m_screenWidth; ++i)
	{
		// Ray position and direction
		const double cameraX = 2 * i / float(m_screenWidth) - 1;
		const double rayX = m_direction.x + m_cameraPlane.x * cameraX;
		const double rayY = m_direction.y + m_cameraPlane.y * cameraX;

		// variables for DDA
		int mapX = (int)m_position.x;
		int mapY = (int)m_position.y;


		


		// DDA

		// Direction of the ray
		int stepX; // Left = 0, Right = 1
		int stepY; // Up = 0, Down = 1

		double sideDistX;
		double sideDistY;

		const double deltaDistX = abs(1 / rayX);
		const double deltaDistY = abs(1 / rayY);

		bool hit = false; // True if any walls detected
		bool verticalWall; // hit on the verticle -> true, if horizontal -> false

		double perpWallDistance; // Perpendicular distance to the wall

		// Direction and distance internal cell distance
		if (rayX < 0)
		{
			stepX = -1;
			sideDistX = (m_position.x - mapX) * deltaDistX;
		} else
		{
			stepX = 1;
			sideDistX = (m_position.x + 1.0 - stepX) * deltaDistX;
		}
		if (rayY < 0)
		{
			stepY = -1;
			sideDistY = (m_position.y - mapY) * deltaDistY;
		} else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - m_position.y) * deltaDistY;
		}

		// DDA
		while (!hit)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				verticalWall = true;
			} else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				verticalWall = false;
			}

			if (m_grid.getCell(sf::Vector2i(mapX, mapY)) == WALL1)
			{
				hit = true;
			}
		}


		// Distance projected on camera direction
		if (verticalWall)
		{
			perpWallDistance = (sideDistX - deltaDistX);
		} else
		{
			perpWallDistance = (sideDistY - deltaDistY);
		}

		// Calculate height of line to draw on screen
		int lineHeight = (int)(m_screenHeight / perpWallDistance);

		// lowest and highest pixel
		int drawStart = -lineHeight / 2 + m_screenHeight / 2;
		if (drawStart < 0)
		{
			drawStart = 0;
		}
		int drawEnd = lineHeight / 2 + m_screenHeight / 2;
		if (drawEnd > m_screenHeight)
		{
			drawEnd = m_screenHeight - 1;
		}


	}
}

void Camera::update()
{
	ray_trace();
}

#include "camera.h"

Camera::Camera()
{
	m_screenWidth = 300;
	m_position = sf::Vector2f(1.0f, 1.0f);
	m_direction = sf::Vector2f(-1.0f, 0.0f);
	m_cameraPlane = sf::Vector2f(0.0f, 0.6666f);
}

Camera::Camera(int screenWidth, sf::Vector2f position, sf::Vector2f direction, sf::Vector2f cameraPlane)
{
	m_screenWidth = screenWidth;
	m_position = position;
	m_direction = direction;
	m_cameraPlane = cameraPlane;
}

void Camera::ray_trace()
{
	for (int i = 0; i < m_screenWidth; ++i)
	{
		double cameraX = 2 * i / float(m_screenWidth) - 1;
		double rayX = m_direction.x + m_cameraPlane.x * cameraX;
	}
}

void Camera::update()
{
	ray_trace();
}

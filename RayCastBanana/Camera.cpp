#include "Camera.h"


Camera::Camera(int screenWidth, Vec2 position, Vec2 direction, Vec2 cameraPlane)
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

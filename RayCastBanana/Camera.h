#pragma once
#include <SFML/System.hpp>

#include "m_grid.h"

class Camera
{
private:
	int m_screenHeight;
	int m_screenWidth;
	sf::Vector2f m_position; // Position in 2d space of the Camera
	sf::Vector2f m_direction; // Direction of the Camera
	sf::Vector2f m_cameraPlane; // 2d version of the Camera plane
	Grid m_grid;
public:

private:
	void ray_trace();
public:
	Camera();
	Camera(int screenWidth, int screenHeight, sf::Vector2f position, sf::Vector2f direction, sf::Vector2f cameraPlane, Grid& m_grid);
	void update();
};

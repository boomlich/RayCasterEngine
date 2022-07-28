#pragma once
#include <SFML/System.hpp>

class Camera
{
private:
	int m_screenWidth = 300;
	sf::Vector2f m_position = sf::Vector2f(1.0f, 1.0f); // Position in 2d space of the Camera
	sf::Vector2f m_direction = sf::Vector2f(-1.0f, 0.0f); // Direction of the Camera
	sf::Vector2f m_cameraPlane = sf::Vector2f(0.0f, 0.6666f); // 2d version of the Camera plane
public:

private:
	void ray_trace();
public:
	Camera();
	Camera(int screenWidth, sf::Vector2f position, sf::Vector2f direction, sf::Vector2f cameraPlane);
	void update();
};

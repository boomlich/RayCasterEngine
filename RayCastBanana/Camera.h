#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "m_grid.h"

class Camera
{

	// Variables
private:
	double m_posX, m_posY; // Position of the camera
	double m_dirX, m_dirY; // Direction of the camera
	double m_planeX, m_planeY; // Camera plane


	// Methods
private:
	void rayCast(sf::RenderWindow& window);

	void calculateStepAndInitDist(double& rayDir, double& sideDist, double& deltaDist, int& step, int& map);
public:
	Camera();
	void update(sf::RenderWindow& window, Grid& grid);
	void setPosition(sf::Vector2f pos);

};

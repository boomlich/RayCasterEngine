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

	void calculateStepAndInitDist(const double& rayDir, double& sideDist, const double& deltaDist, int& step, const int& map, const double& pos);
	void digDiffAnalysis(
		double& sideDistX, double& sideDistY, int& mapX, int& mapY, const int& stepX, const int& stepY,
		bool& vWall, const double& deltaDistX, const double& deltaDistY, Grid& grid);
public:
	Camera();
	void update(sf::RenderWindow& window, Grid& grid);
	void setPosition(sf::Vector2f pos);
	void moveForward(double x);
	void rotate(double angle);

};

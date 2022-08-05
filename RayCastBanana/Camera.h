#pragma once
#include "m_grid.h"
#include "Player.h"
#include "Ray.h"

class Camera
{
public:
	double* m_posX;
	double* m_posY;
	double* m_dirX;
	double* m_dirY;
	double planeX;
	double planeY;
	std::vector<Ray> m_rays;

public:
	void attachToPlayer(Player& player);
	void update(int width, int height, Grid& grid);
};

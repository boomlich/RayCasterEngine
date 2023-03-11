#pragma once
#include "grid.h"
#include "Player.h"
#include "Ray.h"

class Camera
{
private:
	double angleFromDir(double x, double y, double length);
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

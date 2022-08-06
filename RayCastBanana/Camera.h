#pragma once
#include "m_grid.h"
#include "Player.h"
#include "Prop.h"
#include "Ray.h"
#include "RenderObj.h"

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
	std::vector<RenderObj> m_renderObj;

public:
	void attachToPlayer(Player& player);
	void update(int width, int height, Grid& grid, std::vector<Prop> props);
};

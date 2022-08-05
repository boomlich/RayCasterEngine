#pragma once

struct Ray
{
	double wallIntersectPoint;
	double wallDist;
	double dirX;
	double dirY;
	bool verticleWall;
	double angle;
	Ray(double dist, double wallPoint, double x, double y, bool vWall, double angle)
	{
		wallDist = dist;
		wallIntersectPoint = wallPoint;
		dirX = x;
		dirY = y;
		verticleWall = vWall;
		this->angle = angle;
	}
};

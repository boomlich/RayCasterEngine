#pragma once

struct Ray
{
	double wallIntersectPoint;
	double wallDist;
	double dirX;
	double dirY;
	bool verticleWall;
	Ray(double dist, double wallPoint, double x, double y, bool vWall)
	{
		wallDist = dist;
		wallIntersectPoint = wallPoint;
		dirX = x;
		dirY = y;
		verticleWall = vWall;
	}
};

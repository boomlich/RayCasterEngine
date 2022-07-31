#pragma once

struct Ray
{
	double length;
	double wallDist;
	double dirX;
	double dirY;
	bool verticleWall;
	Ray(double dist, double len, double x, double y, bool vWall)
	{
		wallDist = dist;
		length = len;
		dirX = x;
		dirY = y;
		verticleWall = vWall;
	}
};

#pragma once

struct Vec2
{
	int x, y;

	Vec2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2()
	{
		x = 0;
		y = 0;
	}
};
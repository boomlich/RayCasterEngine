#pragma once

#include <vector>
#include "vec.h"

enum cell_state
{
	WALL1,
	WALL2,
	FLOOR1,
	FLOOR2,
	PLAYER_SPAWN,
	KEY,
	DOOR,
	EXIT
};




class Grid
{
private:
	int width, height;
	std::vector<std::vector<cell_state>> grid;

private:
	bool in_bounds(Vec2 position);
	void resetGrid();
public:
	Grid(int width, int height);
	void setCell(Vec2 position, cell_state state);
	cell_state getCell(Vec2 position);
};

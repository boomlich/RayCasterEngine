#include "Grid.h"

Grid::Grid(const int x, const int y)
{
	this->width = x;
	this->height = y;

	resetGrid();
}

void Grid::resetGrid()
{
	for (int y = 0; y < height; ++y) {
		grid.push_back(std::vector<cell_state>());
		for (int x = 0; x < width; ++x) {
			grid[y].push_back(FLOOR1);
		}
	}
}

void Grid::setCell(const Vec2 position, cell_state state)
{
	if (in_bounds(position))
	{
		grid[position.y][position.x] = state;
	}
}

cell_state Grid::getCell(const Vec2 position)
{
	if (in_bounds(position))
	{
		return grid[position.y][position.x];
	}
	return grid[0][0];
}

bool Grid::in_bounds(const Vec2 position)
{
	return position.x > 0 && position.x < width&& position.y > 0 && position.y < height;
}


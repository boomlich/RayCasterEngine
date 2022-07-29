#include "m_grid.h"

Grid::Grid()
{
	m_width = 0;
	m_height = 0;
}

Grid::Grid(const int x, const int y)
{
	this->m_width = x;
	this->m_height = y;

	resetGrid();
}

void Grid::resetGrid()
{
	for (int y = 0; y < m_height; ++y) {
		m_grid.push_back(std::vector<CellState>());
		for (int x = 0; x < m_width; ++x) {
			m_grid[y].push_back(FLOOR1);
		}
	}
}

void Grid::setCell(const sf::Vector2i position, CellState state)
{
	if (inBounds(position))
	{
		m_grid[position.y][position.x] = state;
	}
}

CellState Grid::getCell(const sf::Vector2i position)
{
	if (inBounds(position))
	{
		return m_grid[position.y][position.x];
	}
	return m_grid[0][0];
}

sf::Vector2i Grid::getPlayerSpawn()
{
	return m_playerSpawn;
}


void Grid::setPlayerSpawn(sf::Vector2i position)
{
	m_playerSpawn = position;
}

bool Grid::inBounds(const sf::Vector2i position)
{
	return position.x > 0 && position.x < m_width&& position.y > 0 && position.y < m_height;
}


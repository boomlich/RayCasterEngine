﻿#include "m_grid.h"

#include <iostream>

Grid::Grid()
{
	m_width = 0;
	m_height = 0;
}

Grid::Grid(const int width, const int height)
{
	this->m_width = width;
	this->m_height = height;

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
	return WALL1;
}

sf::Vector2i Grid::getPlayerSpawn()
{
	return m_playerSpawn;
}


void Grid::setPlayerSpawn(sf::Vector2i position)
{
	m_playerSpawn = position;
}

std::string Grid::toString()
{
	std::string result;
	for (auto row : m_grid)
	{
		for (auto column : row)
		{
			result += std::to_string(column) + " ";
		}
		result += "\n";
	}

	return result;
}

std::vector<std::vector<CellState>> Grid::getCells()
{
	return m_grid;
}

bool Grid::inBounds(const sf::Vector2i position)
{
	return position.x > -1 && position.x < m_width&& position.y > -1 && position.y < m_height;
}


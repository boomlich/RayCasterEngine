#include "CellGrid.h"

CellGrid::CellGrid(int width, int height) :
	disabledCell(CELL_DISABLED, 0, 0)
{
	m_width = width;
	m_height = height;
	init();
}

void CellGrid::init()
{
	for (int y = 0; y < m_height; ++y) 
	{
		m_cells.emplace_back(std::vector<Cell>());
		for (int x = 0; x < m_width; ++x) 
		{
			if (x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1)
			{
				m_cells[y].emplace_back(Cell(CELL_WALL, x, y));
			}
			else
			{
				m_cells[y].emplace_back(Cell(CELL_FREE, x, y));
			}
		}
	}
}

bool CellGrid::inBounds(int x, int y)
{
	return x > -1 && x < m_width && y > -1 && y < m_height;
}



void CellGrid::resize(int x, int y)
{
}

Cell* CellGrid::getCell(int x, int y)
{
	if (inBounds(x, y))
	{
		return &m_cells[y][x];
	}
	return nullptr;
}

std::vector<std::vector<Cell>> CellGrid::getCells()
{
	return std::vector<std::vector<Cell>>();
}

void CellGrid::setCell(int x, int y, Cell cell)
{
	if (inBounds(x, y))
	{
		m_cells[y][x] = cell;
	}
}
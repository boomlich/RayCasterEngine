#pragma once
#include "Cell.h"

class CellGrid
{
private:
	int m_width = 0;
	int m_height = 0;
	std::vector<std::vector<Cell>> m_cells;
	Cell disabledCell;

private:
	void init();
	bool inBounds(int x, int y);



public:
	CellGrid(int width, int height);
	void resize(int width, int height);
	Cell* getCell(int x, int y);
	std::vector<std::vector<Cell>> getCells();
	void setCell(int x, int y, Cell cell);


};

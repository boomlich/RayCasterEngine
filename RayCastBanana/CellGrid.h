#pragma once
#include "Cell.h"
#include "Prop.h"

class CellGrid
{
private:
	int m_width = 0;
	int m_height = 0;
	std::vector<std::vector<Cell>> m_cells;
	Cell disabledCell;
	std::vector<Prop> m_props;

private:
	void init();
	bool inBounds(int x, int y);
	TextureID getMostCommonTxID(std::unordered_map<TextureID, int> allTxIDCount);
	std::string getCellTypeTextures(std::unordered_map<int, std::vector<TextureID>> txIndexes, std::vector<int> cellIndexes, TextureID mostCommonTX);

public:
	CellGrid(int width, int height);
	void resize(int width, int height);
	Cell* getCell(int x, int y);
	Cell* getCell(int index);
	std::vector<std::vector<Cell>> getCells();
	void setCell(int x, int y, Cell cell);
	Prop* addProp(Prop prop);
	std::vector<Prop> getAllProps();
	Prop* getProp(int index);
	std::string saveGrid();


};

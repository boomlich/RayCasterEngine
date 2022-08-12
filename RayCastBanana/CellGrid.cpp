#include "CellGrid.h"

#include "MathMethods.h"


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

Cell* CellGrid::getCell(int index)
{
	int x = index % m_width;
	int y = (index - x) / m_width;
	return getCell(x, y);
}

std::vector<std::vector<Cell>> CellGrid::getCells()
{
	return m_cells;
}

void CellGrid::setCell(int x, int y, Cell cell)
{
	if (inBounds(x, y))
	{
		m_cells[y][x] = cell;
	}
}

Prop* CellGrid::addProp(Prop prop)
{
	m_props.emplace_back(prop);
	return &m_props.at(m_props.size() - 1);
}

std::vector<Prop> CellGrid::getAllProps()
{
	return m_props;
}

Prop* CellGrid::getProp(int index)
{
	if (index < 0 || index > m_props.size() - 1)
	{
		return nullptr;
	}
	return &m_props.at(index);
}

std::string CellGrid::saveGrid()
{
	

	int vesionID = 0;

	std::string result = std::to_string(vesionID);

	result += "|" + std::to_string(m_width) + "x" + std::to_string(m_height);

	int prevStart = 0;
	Cell* prevCell = getCell(0);

	bool wallsAdded = false;
	int wallsInARow = 0;
	int wallLineStart = 0;

	std::vector<int> wallIndexes;


	std::unordered_map<TextureID, int> txWallCnt;
	std::vector<int> wallTxIndexes;



	int i = 0;
	int n = 0;
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			Cell* currentCell = getCell(i);
			bool isWall = currentCell->m_type == CELL_WALL;

			if (isWall)
			{
				// Find all index's of walls (excluding the borders)
				if (x > 0 && x < m_width - 1 && y > 0 && y < m_height - 1)
				{
					wallIndexes.emplace_back(n);
					n++;
				}

				// Count and identify all wall textures
				for (int j = 0; j < 4; ++j)
				{
					TextureID txID = currentCell->m_textures[j];
					if (txWallCnt.find(txID) == txWallCnt.end()) txWallCnt[txID] = 1;
					else txWallCnt[txID]++;
					wallTxIndexes.emplace_back(i);
				}
			}
			//int numOfTx = 4;
			//if (!isWall) numOfTx = 2;

			//for (int j = 0; j < numOfTx; ++j)
			//{
			//	TextureID txID = currentCell->m_textures[j];
			//	if (txWallCnt.find(txID) == txWallCnt.end()) txWallCnt[txID] = 1;
			//	else txWallCnt[txID]++;
			//}

			//for (auto tx : currentCell->m_textures)
			//{
			//	if (txWallCnt.find(tx) == txWallCnt.end()) txWallCnt[tx] = 1;
			//	else txWallCnt[tx]++;
			//}
			


			i++;
		}
	}

	for (auto pair : txWallCnt)
	{
		std::cout << "TX COUNT: " << pair.first << " : " << pair.second << std::endl;
	}


	if (!wallIndexes.empty())
	{
		result += "|W:";

		int i = 0;
		while (i < wallIndexes.size())
		{
			int count = 0;
			while (i + count + 1 < wallIndexes.size())
			{
				if (wallIndexes[i + count + 1] - wallIndexes[i + count] > 1) break;
				count++;
			}
			result += convertToHexString(wallIndexes[i]);
			if (count > 0) result += "+" + convertToHexString(count);
			i += count + 1;
			if (i < wallIndexes.size()) result += ",";
		}
	}

	return result;
}

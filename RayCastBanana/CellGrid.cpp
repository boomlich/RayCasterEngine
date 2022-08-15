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

TextureID CellGrid::getMostCommonTxID(std::unordered_map<TextureID, int> allTxIDCount)
{
	TextureID mostCommonTx = TX_WALL_SCIFI_01;
	int largestNum = 0;
	for (auto pair : allTxIDCount)
	{
		if (pair.second > largestNum)
		{
			largestNum = pair.second;
			mostCommonTx = pair.first;
		}
	}
	return mostCommonTx;
}

std::string CellGrid::getCellTypeTextures(std::unordered_map<int, std::vector<TextureID>> txIndexes, std::vector<int> cellIndexes, TextureID mostCommonTX)
{
	std::string result = "";
	std::vector<std::vector<std::string>> txInString;

	for (auto pair : txIndexes)
	{
		txInString.emplace_back(std::vector<std::string>());

		unsigned int i = 0;
		while (i < cellIndexes.size())
		{
			if (pair.second[i] == mostCommonTX)
			{
				i++;
				continue;
			}
			unsigned int count = 0;
			while (i + count + 1 < cellIndexes.size())
			{
				if (pair.second[i + count] == mostCommonTX) break;
				if (cellIndexes[i + count + 1] - cellIndexes[i + count] > 1) break;
				if (pair.second[i + count + 1] != pair.second[i + count]) break;
				count++;
			}
			std::string txSide = convertToHexString(cellIndexes[i]) + ";" + convertToHexString(pair.second[i]);
			if (count > 0) txSide += "+" + convertToHexString(count);
			i += count + 1;
			txInString[pair.first].emplace_back(txSide);
		}
	}

	int q = 0;
	for (auto txSide : txInString)
	{
		if (txSide.empty()) continue;

		result += ":" + std::to_string(q) + ":";
		for (int p = 0; p < txSide.size(); ++p)
		{
			result += txSide[p];
			if (p < txSide.size() - 1) result += ",";
		}
		q++;
	}

	return result;
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

	result += "|" + convertToHexString(m_width) + "x" + convertToHexString(m_height);

	std::vector<int> innerWallIndexes;
	std::vector<int> wallIndexes;

	std::vector<int> innerFreeIndexes;

	std::unordered_map<TextureID, int> txWallCnt;
	std::unordered_map<int, std::vector<TextureID>> wallTxIndexes = {
		{0, std::vector<TextureID>()},
		{1, std::vector<TextureID>()},
		{2, std::vector<TextureID>()},
		{3, std::vector<TextureID>()},
	};

	std::unordered_map<TextureID, int> txFloorCnt;
	std::unordered_map<TextureID, int> txCeilCnt;

	std::unordered_map<int, std::vector<TextureID>> floorTxIndexes = { {0, std::vector<TextureID>()} };
	std::unordered_map<int, std::vector<TextureID>> ceilTxIndexes = { {0, std::vector<TextureID>()} };

	int i = 0;
	int n = 0;
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			Cell* currentCell = getCell(i);
			bool isWall = currentCell->m_type == CELL_WALL;

			// Find all index's of inner walls (border excluded)
			if (x > 0 && x < m_width - 1 && y > 0 && y < m_height - 1)
			{
				if (isWall) innerWallIndexes.emplace_back(n);
				else
				{
					// Count and add floor texture
					TextureID txID = currentCell->m_textures[0];
					if (txFloorCnt.find(txID) == txFloorCnt.end()) txFloorCnt[txID] = 1;
					else txFloorCnt[txID]++;
					floorTxIndexes.at(0).emplace_back(txID);

					// Count and add ceiling texture
					txID = currentCell->m_textures[1];
					if (txCeilCnt.find(txID) == txCeilCnt.end()) txCeilCnt[txID] = 1;
					else txCeilCnt[txID]++;
					ceilTxIndexes.at(0).emplace_back(txID);

					innerFreeIndexes.emplace_back(n);
				}
				n++;
			}

			if (isWall)
			{
				wallIndexes.emplace_back(i);
				// Count and identify all wall textures
				for (int j = 0; j < 4; ++j)
				{
					TextureID txID = currentCell->m_textures[j];
					if (txWallCnt.find(txID) == txWallCnt.end()) txWallCnt[txID] = 1;
					else txWallCnt[txID]++;
					wallTxIndexes.at(j).emplace_back(txID);
				}
			}
			i++;
		}
	}

	TextureID mostCommonWallTX = getMostCommonTxID(txWallCnt);
	TextureID mostCommonFloorTX = getMostCommonTxID(txFloorCnt);
	TextureID mostCommonCeilTX = getMostCommonTxID(txCeilCnt);

	// Add wall cells
	if (!innerWallIndexes.empty())
	{
		result += "|W:";

		int i = 0;
		while (i < innerWallIndexes.size())
		{
			int count = 0;
			while (i + count + 1 < innerWallIndexes.size())
			{
				if (innerWallIndexes[i + count + 1] - innerWallIndexes[i + count] > 1) break;
				count++;
			}
			result += convertToHexString(innerWallIndexes[i]);
			if (count > 0) result += "+" + convertToHexString(count);
			i += count + 1;
			if (i < innerWallIndexes.size()) result += ",";
		}
	}

	// Add texture info
	result += "|WT:C;" + std::to_string(mostCommonWallTX); // Add the most common texture
	result += getCellTypeTextures(wallTxIndexes, wallIndexes, mostCommonWallTX);

	result += "|FT:C;" + std::to_string(mostCommonFloorTX); // Add the most common texture
	result += getCellTypeTextures(floorTxIndexes, innerFreeIndexes, mostCommonFloorTX);

	result += "|CT:C;" + std::to_string(mostCommonCeilTX); // Add the most common texture
	result += getCellTypeTextures(ceilTxIndexes, innerFreeIndexes, mostCommonCeilTX);

	return result;
}

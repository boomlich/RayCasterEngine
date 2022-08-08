#include "Cell.h"

int Cell::getPosX()
{
	return m_posX;
}

int Cell::getPosY()
{
	return m_posY;
}

Cell::Cell(CellType type, int x, int y)
{
	m_type = type;
	m_posX = x;
	m_posY = y;

	if (type == CELL_WALL)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_textures[i] = TX_WALL_SCIFI_01;
		}
	} else if (type == CELL_FREE)
	{
		m_textures[0] = TX_FLOOR_SCIFI_01;
		m_textures[1] = TX_CEILING_SCIFI_01;
	}
}

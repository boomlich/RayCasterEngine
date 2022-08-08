#pragma once
#include <SFML/Graphics/Image.hpp>

#include "mediaManager.h"

enum CellType
{
	CELL_DISABLED,
	CELL_WALL,
	CELL_FREE,
	CELL_DOOR,
	CELL_EXIT
};

class Cell
{
private:
	int m_posX = 0;
	int m_posY = 0;
public:
	TextureID m_textures[4];
	CellType m_type;
public:
	int getPosX();
	int getPosY();
	Cell(CellType type, int x, int y);





};

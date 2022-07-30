#pragma once

#include <vector>
#include <SFML/System.hpp>

enum CellState
{
	WALL1,
	WALL2,
	FLOOR1,
	FLOOR2,
	PLAYER_SPAWN,
	KEY,
	DOOR,
	EXIT
};

class Grid
{
private:
	int m_width, m_height;
	sf::Vector2i m_playerSpawn = sf::Vector2i(1, 1);
	std::vector<std::vector<CellState>> m_grid;

private:
	bool inBounds(sf::Vector2i position);
	void resetGrid();
public:
	Grid();
	Grid(int width, int height);
	void setCell(sf::Vector2i position, CellState state);
	CellState getCell(sf::Vector2i position);
	sf::Vector2i getPlayerSpawn();
	void setPlayerSpawn(sf::Vector2i position);
	std::string toString();
};

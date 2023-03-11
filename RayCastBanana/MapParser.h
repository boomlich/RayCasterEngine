#pragma once
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "grid.h"




class MapParser
{
private:
	CellState parse_color(sf::Color color);
public:
	Grid parse_img(const std::string& path);
	Grid parseLevelString(const std::string level);
};

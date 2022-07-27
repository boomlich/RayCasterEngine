#pragma once
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Grid.h"




class MapParser
{
private:
	cell_state parse_color(sf::Color color);
public:
	Grid parse_img(const std::string& path);
};

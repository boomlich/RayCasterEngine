#include "MapParser.h"
#include "texture_loader.h"

Grid MapParser::parse_img(const std::string& path)
{
	const sf::Image img = load_img(path);
	const int width = img.getSize().x;
	const int height = img.getSize().y;
	Grid grid(width, height);

	for (int y = 0; y < width; ++y)	{
		for (int x = 0; x < height; ++x) {
			const sf::Color cellColor = img.getPixel(x, y);
			const CellState cellState = parse_color(cellColor);
			const sf::Vector2i pos(x, y);

			grid.setCell(pos, cellState);

			if (cellState == PLAYER_SPAWN)
			{
				grid.setPlayerSpawn(pos);
			}
		}
	}
	return grid;
}

CellState MapParser::parse_color(sf::Color color)
{
	const std::string color_value = std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ", " + std::to_string(color.a);


	if (color_value == "0, 0, 0, 255") { // Black
		return WALL1;
	}
	if (color_value == "0, 255, 0, 255") { // Green
		return PLAYER_SPAWN;
	}
	return FLOOR1;
}




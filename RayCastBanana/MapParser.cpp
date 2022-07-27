#include "MapParser.h"
#include "texture_loader.h"
#include "vec.h"


Grid MapParser::parse_img(const std::string& path)
{
	const sf::Image img = load_img(path);
	const int width = img.getSize().x;
	const int height = img.getSize().y;
	Grid grid(width, height);

	for (int y = 0; y < width; ++y)	{
		for (int x = 0; x < height; ++x) {
			const sf::Color cellColor = img.getPixel(x, y);
			const cell_state cellState = parse_color(cellColor);
			grid.setCell(Vec2(x, y), cellState);
		}
	}
	return grid;
}

cell_state MapParser::parse_color(sf::Color color)
{
	const std::string color_value = std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ", " + std::to_string(color.a);

	if (color_value == "0, 0, 0, 255") { // Black
		return WALL1;
	}
	if (color_value == "132, 126, 135, 255") { // Gray
		return FLOOR1;
	}
	if (color_value == "0, 255, 0, 255") { // Green
		return PLAYER_SPAWN;
	}
	return WALL1;
}




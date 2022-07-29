#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"

class View
{
private:
	sf::RenderWindow m_window;
	Camera m_camera;
	Grid m_mapGrid;
public:
	int m_width;
	int m_height;

public:
	View(int width, int height);
	void setGrid(Grid grid);
	void render_loop();
};

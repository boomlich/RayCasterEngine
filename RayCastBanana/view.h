#pragma once
#include <SFML/Graphics.hpp>

class View
{
private:
	sf::RenderWindow m_window;
public:
	int m_width;
	int m_height;

private:
	void render_loop();

public:
	View(int width, int height);
};
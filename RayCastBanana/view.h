#pragma once
#include <SFML/Graphics.hpp>

class View
{
private:
	sf::RenderWindow window;
public:
	int width;
	int height;

private:
	void render_loop();

public:
	View(int width, int height);
};

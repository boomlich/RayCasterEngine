#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Model.h"
#include "m_grid.h"

class Renderer
{
private:
	sf::RenderWindow m_window;
	Camera m_camera;
	sf::Image m_wallTexture;
	sf::Image m_pixelBufferClear;
	sf::Image m_pixelBuffer;
	sf::Texture m_screenTexture;
	sf::Sprite m_screenSprite;
public:
	int m_width;
	int m_height;
	Model* m_model;

private:
	void drawMinimap(Grid& grid, CtrlPawn* player);
	void drawWorld();
public:
	Renderer(int width, int height, Model* model);
	void update(float dt);
	sf::Vector2i getWindowPosition();
	int getWidth();
	int getHeight();
};

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
public:
	int m_width;
	int m_height;
	Model* m_model;

private:
	void drawMinimap(Grid& grid, CtrlPawn* player);
public:
	Renderer(int width, int height, Model* model);
	void update(float dt);
	
};

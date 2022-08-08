#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Model.h"
#include "m_grid.h"
#include "UIManager.h"


struct Pixel
{
	int x;
	int y;
	sf::Color color;
	Pixel(int x, int y, sf::Color color)
	{
		this->x = x;
		this->y = y;
		this->color = color;
	}
};


class Renderer
{
private:
	sf::RenderWindow m_window;
	Camera m_camera;
	sf::Image m_wallTexture;
	sf::Image m_floorTexture;
	sf::Image m_ceilingTexture;


	sf::Image m_pixelBufferClear;
	sf::Image m_pixelBuffer;
	sf::Texture m_screenTexture;
	sf::Sprite m_screenSprite;

	std::unordered_map<int, std::vector<Pixel>> m_depthBuffer;

	UIManager m_uiManager;

public:
	int m_width;
	int m_height;
	Model* m_model;
	static sf::Texture testTX;

private:
	void drawMinimap(Grid& grid, CtrlPawn* player);
	void drawWorld();
	void drawObjects();
	double calculateFog(double dist, double minDist, double maxDist);
	void clearDepthBuffer();
	void drawFromDepth();
	void addShapeToPixelBuffer(sf::RectangleShape &shape, double posX, double posY, sf::Color color);

public:
	Renderer(int width, int height, Model* model);
	void update(sf::Time dt);
	sf::Vector2i getWindowPosition();
	int getWidth();
	int getHeight();
};

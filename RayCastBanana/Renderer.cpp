#include "Renderer.h"

#include "m_grid.h"
#include "texture_loader.h"

Renderer::Renderer(int width, int height, Model* model) :
	m_window(sf::VideoMode(width, height), "Banana"),
	m_model(model)
{
    m_width = width;
    m_height = height;
    m_window.setFramerateLimit(60);
    m_camera.attachToPlayer(*m_model->getPlayer());
}

void Renderer::update(float dt)
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            exit(1);
        }
    }

	m_window.clear();

    drawMinimap(m_model->getGrid(), m_model->getPawn());

	m_window.display();
}

void Renderer::drawMinimap(Grid& grid, CtrlPawn* player)
{
	int gridWidth = grid.getCells().size();
    int gridHeight = grid.getCells()[0].size();
    float size = 11.0f;
    sf::Color color;

	float padding = 10.0f; // Padding of the entire minimap
    float border = 2.0f; // Inner border of the minimap contents
    float cellOffset = 1.0f;
    float mapOffset = padding + border;

    float mapWidth = (float)gridWidth * size + border + cellOffset;
    float mapHeight = (float)gridHeight * size + border + cellOffset;

    sf::RectangleShape bg(sf::Vector2f(mapWidth, mapHeight));
    bg.setPosition(padding, padding);
    bg.setFillColor(sf::Color(155, 173, 183, 255));
    m_window.draw(bg);

	for (int y = 0; y < gridHeight; ++y)
	{
		for (int x = 0; x < gridWidth; ++x)
		{
            sf::RectangleShape cell(sf::Vector2f(size - cellOffset, size - cellOffset));
            cell.setPosition(size * x + mapOffset, size * y + mapOffset);

            switch (grid.getCell(sf::Vector2i(x, y)))
            {
            case WALL1:
                color = sf::Color(0, 0, 0, 255);
                break;
            case FLOOR1:
                color = sf::Color(132, 126, 135, 255);
                break;
            }
            cell.setFillColor(color);
            m_window.draw(cell);
		}
	}



    // Draw player
    float dotRadius = 5.0f;
    sf::CircleShape playerDot(dotRadius, 8);

    // Calculate size of one grid cell in pixels on the minimap
    float mapCellSizeX = 1 / (float)gridWidth * mapWidth;
    float mapCellSizeY = 1 / (float)gridHeight * mapHeight;

    // Convert player world coordinates to minimap position
    float mapPosX = padding - cellOffset + (float)*m_camera.m_posX * mapCellSizeX;
    float mapPosY = padding - cellOffset + (float)*m_camera.m_posY * mapCellSizeY;

    // Draw direction
    // Calculate direction end position
    float dirEndX = mapPosX + (float)*m_camera.m_dirX * mapCellSizeX * 2;
    float dirEndY = mapPosY + (float)*m_camera.m_dirY * mapCellSizeY * 2;
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(mapPosX, mapPosY)),
        sf::Vertex(sf::Vector2f(dirEndX, dirEndY))
    };
    m_window.draw(line, 2, sf::Lines);

    // Draw circle
    color = sf::Color(0, 255, 0, 255);
    playerDot.setPosition(mapPosX - dotRadius, mapPosY - dotRadius);
    playerDot.setFillColor(color);
    m_window.draw(playerDot);
}

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
    m_window.setMouseCursorVisible(false);
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

    m_camera.update(m_window.getSize().x, m_model->getGrid());
    drawWorld();
    drawMinimap(m_model->getGrid(), m_model->getPawn());

	m_window.display();
}

sf::Vector2i Renderer::getWindowPosition()
{
    return m_window.getPosition();
}

int Renderer::getWidth()
{
    return m_window.getSize().x;
}

int Renderer::getHeight()
{
    return m_window.getSize().y;
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

    float mapWidth = (float)gridWidth * size + cellOffset;
    float mapHeight = (float)gridHeight * size + cellOffset;

    sf::RectangleShape bg(sf::Vector2f(mapWidth + border, mapHeight + border));
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


    // Calculate size of one grid cell in pixels on the minimap
    float mapCellSizeX = 1 / (float)gridWidth * mapWidth;
    float mapCellSizeY = 1 / (float)gridHeight * mapHeight;

    // Convert player world coordinates to minimap position
    float mapPosX = mapOffset - cellOffset + (float)*m_camera.m_posX * mapCellSizeX;
    float mapPosY = mapOffset - cellOffset + (float)*m_camera.m_posY * mapCellSizeY;

    // Draw rays
	for (auto &ray : m_camera.m_rays)
	{
        float dirEndX = mapPosX + (float)(ray.dirX * (double)mapCellSizeX * ray.wallDist);
        float dirEndY = mapPosY + (float)(ray.dirY * (double)mapCellSizeY * ray.wallDist);
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(mapPosX, mapPosY)),
            sf::Vertex(sf::Vector2f(dirEndX, dirEndY))
        };
        m_window.draw(line, 2, sf::Lines);
	}

    // Draw circle
    float dotRadius = 2.5f;
    sf::CircleShape playerDot(dotRadius, 8);
    color = sf::Color(0, 255, 0, 255);
    playerDot.setPosition(mapPosX - dotRadius, mapPosY - dotRadius);
    playerDot.setFillColor(color);
    m_window.draw(playerDot);
}

void Renderer::drawWorld()
{
    int i = 0;
    for (auto& ray : m_camera.m_rays)
    {
        float h = (float)m_window.getSize().y;
        float lineHeight = h / (float)ray.wallDist;
        float drawStart = (h - lineHeight) / 2;
        // if (drawStart < 0) drawStart = 0;

        sf::RectangleShape wall(sf::Vector2f(1.0f, lineHeight));
        wall.setPosition(i, drawStart);

        sf::Color color;
        const double wallDarkness = (ray.wallDist > 1) ? ray.wallDist : 1.0;
        if (ray.verticleWall)
        {
            color = sf::Color(73 / wallDarkness, 230 / wallDarkness, 97 / wallDarkness, 255);
        } else
        {
            color = sf::Color(59 / wallDarkness, 191 / wallDarkness, 79 / wallDarkness, 255);
        }
        wall.setFillColor(color);
        m_window.draw(wall);
        i++;
    }
}

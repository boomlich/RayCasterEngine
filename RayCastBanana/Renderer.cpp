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
    m_wallTexture = load_img("resources/images/textures/t_wall_spaceship_01.png");

    m_pixelBuffer.create(width, height, sf::Color(255, 255, 255, 255));
    m_screenTexture.create(width, height);
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
    // Clear pixel buffer
    m_pixelBuffer.create(640, 480, sf::Color(0, 0, 0, 255));
    const float h = (float)m_window.getSize().y;
    const int textureWidth = m_wallTexture.getSize().x - 1;
    const int textureHeight = m_wallTexture.getSize().y - 1;

    int cnt = 0;
    int x = 0;
    for (auto& ray : m_camera.m_rays)
    {
        // Calculate line heights and where to draw the first pixel
        const float lineHeight = h / (float)ray.wallDist;
        const float drawStart = (h - lineHeight) / 2;

        // Draw textures
        for (int i = 0; i < (int)lineHeight; ++i)
        {
            // Get texture pixel color
        	int pixelX = (int)(ray.wallIntersectPoint * (float)textureWidth);
        	int pixelY = (int)((float)i / (lineHeight - 1) * (float)textureHeight);
        	sf::Color color = m_wallTexture.getPixel(pixelX, pixelY);
            if (ray.verticleWall)
            {
                color = sf::Color(color.r / 2, color.g / 2, color.b / 2, color.a);
            }

            // Update buffer with color from texture
            int y = int(drawStart + i);
            if (y > -1 && y < h)
            {
                m_pixelBuffer.setPixel(x, y, color);
            }
            cnt++;
        }
    	x++;
    }
    m_screenTexture.update(m_pixelBuffer);
    m_window.draw(sf::Sprite(m_screenTexture));
    std::cout << "NUMBER OF PIXELS DRAWN: " << cnt << std::endl;
}
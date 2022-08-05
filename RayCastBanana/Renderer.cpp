#include "Renderer.h"

#include "MathMethods.h"
#include "m_grid.h"
#include "texture_loader.h"

#define PI 3.14159265359

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
    m_floorTexture = load_img("resources/images/textures/texture_floor_scifi_01.png");
    m_ceilingTexture = load_img("resources/images/textures/texture_ceiling_window_01.png");


    m_pixelBuffer.create(width, height, sf::Color(255, 255, 255, 255));
    m_pixelBufferClear.create(width, height, sf::Color(0, 0, 0, 255));

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

    m_camera.update(m_window.getSize().x, m_window.getSize().y, m_model->getGrid());
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
    m_pixelBuffer = sf::Image(m_pixelBufferClear);

    const float screenHeight = (float)m_window.getSize().y;
    const int halfH = screenHeight / 2;
    const unsigned int textureWidth = m_wallTexture.getSize().x - 1;
    const unsigned int textureHeight = m_wallTexture.getSize().y - 1;


    int cnt = 0;
    int x = 0;

    // Height of the wall in units
    const float wallHeight = screenHeight * 0.75;

    for (auto& ray : m_camera.m_rays)
    {
        // Wall column height based of the distance
        const float lineHeight = wallHeight / (float)ray.wallDist;

        // First pixel in the y-axis to draw
    	float drawStart = (screenHeight - lineHeight) / 2.0;
        if (drawStart < 0) drawStart = 0.0f;
        float heightOffset = 0.0f; // draw start offset if line is higher then screen

        // Last pixel in the y-axis to draw.
        float drawEnd = lineHeight;
        if (drawEnd > screenHeight)
        {
            drawEnd = screenHeight;
            heightOffset = (lineHeight - drawEnd) / 2;
        }

        int textureX = (int)(ray.wallIntersectPoint * (float)(textureWidth));
        double textureStep = (double)textureHeight / lineHeight;
        int prevTextureY = -1; // I
		sf::Color color;

        // Draw textures
        for (int i = 0; i < (int)drawEnd; ++i)
        {
            // Get texture pixel color
            int textureY = int(((float)i + heightOffset) * textureStep);

            if (textureY > prevTextureY)
            {
                prevTextureY = textureY;

                // Get color of the texture pixel
                color = m_wallTexture.getPixel(textureX, textureY);
                if (ray.verticleWall)
                {
                    color = sf::Color(color.r / 2, color.g / 2, color.b / 2, color.a);
                }

                // Calculate fog
                double fog = calculateFog(ray.wallDist, 1, 15);
                color = sf::Color(
                    color.r * fog,
                    color.g * fog,
                    color.b * fog,
                    color.a);

            }
            int y = (int)(drawStart + i);
            m_pixelBuffer.setPixel(x, y, color);
        }


        // Draw floor & ceiling
        // Position of the camera in z-axis
        float cameraZ = wallHeight / 2.0f;
        int prevTX = -1;
        int prevTY = -1;

        for (int y = drawStart + lineHeight; y < screenHeight; y++)
        {
            // Offset from the center of the screen.
            float rayCenterOffset = y - halfH;

            // Angle from x-axis: vector = (1, 0)
            float angleXAxis = angleFromXAxis(ray.dirX, ray.dirY);

            // World distance to the point on the floor. Using cos to correct for fisheye-effect
            float pointDist = cameraZ / rayCenterOffset / cos(ray.angle);

            // Coordinates of the point on the floor
            float floorX = *m_camera.m_posX + cos(angleXAxis) * pointDist;
            float floorY = *m_camera.m_posY - sin(angleXAxis) * pointDist;

            // Texture coordinates
            int tx = int((floorX - (int)floorX) * 32);
            int ty = int((floorY - (int)floorY) * 32);

            // Draw floor
            if (tx != prevTX && ty != prevTY)
            {
                double fog = calculateFog(pointDist, 1, 15);
                color = m_floorTexture.getPixel(tx, ty);
                color = sf::Color(
                    color.r * fog,
                    color.g * fog,
                    color.b * fog,
                    color.a);
            }
            m_pixelBuffer.setPixel(x, y, color);

            // Draw ceiling
            if (tx != prevTX && ty != prevTY)
            {
                double fog = calculateFog(pointDist, 1, 15);
                color = m_ceilingTexture.getPixel(tx, ty);
                color = sf::Color(
                    color.r * fog,
                    color.g * fog,
                    color.b * fog,
                    color.a);
            }
            m_pixelBuffer.setPixel(x, screenHeight - y, color);
            cnt++;
        }
    	x++;
    }
    m_screenTexture.update(m_pixelBuffer);
    // m_screenTexture.setSmooth(true);
    m_window.draw(sf::Sprite(m_screenTexture));
    // std::cout << "NUMBER OF PIXELS DRAWN: " << cnt << std::endl;
}

// Calculate the amount of fog depending on the distance. Linear iterperted.
// returns 1 if no fog. 0 if full fog.
double Renderer::calculateFog(double dist, double minDist, double maxDist)
{
	if (dist < minDist)
	{
        return 1;
	}
	if (dist > maxDist)
	{
        return 0;
	}

    return 1 - ((dist - minDist) / (maxDist - minDist));
}

#include "Renderer.h"

#include "MathMethods.h"
#include "mediaManager.h"


Renderer::Renderer(int width, int height, Model* model) :
    m_window(sf::VideoMode(width, height), "Banana"),
    m_model(model),
    m_pixels(width* height, sf::Color::Black)
{
    m_width = 640;
    m_height = 480;
    m_camera.attachToPlayer(*m_model->getPlayer());

	m_wallTexture = load_img("resources\\images\\textures\\t_wall_spaceship_01.png");
    m_floorTexture = load_img("resources\\images\\textures\\texture_floor_scifi_01.png");
    m_ceilingTexture = load_img("resources\\images\\textures\\texture_ceiling_window_01.png");

    m_pixelBuffer.create(m_width, m_height, sf::Color(255, 255, 255, 255));
    m_pixelBufferClear.create(m_width, m_height, sf::Color(0, 0, 0, 255));

    m_screenTexture.create(m_width, m_height);
    m_screenSprite.setScale(width / m_width, height / m_height);

}

void Renderer::update(sf::Time dt)
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

    m_camera.update(m_width, m_height, m_model->getGrid());
    drawWorld();

    m_screenTexture.update(m_pixelBuffer);
    m_screenSprite.setTexture(m_screenTexture);
    
    m_window.draw(m_screenSprite);

    drawMinimap(m_model->getGrid(), m_model->getPawn());

    debug(dt);
	m_window.display();
}

void Renderer::debug(sf::Time dt) {
    m_debugInfoTimer -= dt.asMicroseconds();
    if (m_debugInfoTimer > 0)
    {
        return;
    }
    m_debugInfoTimer = 1000000;
    std::cout << "Frametime: " << dt.asMicroseconds() << " us" << std::endl;
    std::cout << "FPS: " << 1000000 / dt.asMicroseconds() << std::endl;
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
                m_color = sf::Color(0, 0, 0, 255);
                break;
            case FLOOR1:
                m_color = sf::Color(132, 126, 135, 255);
                break;
            }
            cell.setFillColor(m_color);
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
    m_color = sf::Color(0, 255, 0, 255);
    playerDot.setPosition(mapPosX - dotRadius, mapPosY - dotRadius);
    playerDot.setFillColor(m_color);
    m_window.draw(playerDot);
}

void Renderer::drawWorld()
{

    const int halfH = m_height / 2;

    int cnt = 0;
    int x = 0;

    // Height of the wall in units
    const float wallHeight = m_height * 0.75;

    for (auto& ray : m_camera.m_rays)
    {
        // Wall column height based of the distance
        const float lineHeight = wallHeight / (float)ray.wallDist;

        // First pixel in the y-axis to draw
    	float drawStart = (m_height - lineHeight) / 2.0;
        if (drawStart < 0) drawStart = 0.0f;
        float heightOffset = 0.0f; // draw start offset if line is higher then screen

        // Last pixel in the y-axis to draw.
        float drawEnd = lineHeight;
        if (drawEnd > m_height)
        {
            drawEnd = m_height;
            heightOffset = (lineHeight - drawEnd) / 2;
        }

        int textureX = (int)(ray.wallIntersectPoint * (float)(32));
        double textureStep = (double)31.0 / lineHeight;
        int prevTextureY = -1; // I
		

        // Calculate darkness of the pixel based of the distance
        // to the world point to the camera
        float shading = calculateFog(ray.wallDist, 1, 15);
        if (ray.verticleWall)
        {
            shading = 0.5f;
        }

        sf::Uint8 prevR;
        sf::Uint8 prevG;
        sf::Uint8 prevB;

        // Draw textures
        for (int i = 0; i < (int)drawEnd; ++i)
        {
            // Get texture pixel color
            int textureY = int(((float)i + heightOffset) * textureStep);

            // Only calculate new color value if neccesary
            if (textureY != prevTextureY)
            {
                prevTextureY = textureY;

                // Get color of the texture pixel
                m_color = m_wallTexture.getPixel(textureX, textureY);
                if (ray.verticleWall)
                {
                    m_color = sf::Color(m_color.r / 2, m_color.g / 2, m_color.b / 2, m_color.a);
                }

                // Calculate fog
                m_color = sf::Color(
                    m_color.r * shading,
                    m_color.g * shading,
                    m_color.b * shading,
                    m_color.a);

            }
            int y = (int)(drawStart + i);
            //m_pixels[y * m_width + x] = sf::Color(m_color.r, m_color.g, m_color.b, m_color.a);
            m_pixelBuffer.setPixel(x, y, m_color);
        }


        // Draw floor & ceiling
    	// Position of the camera in z-axis
        const float cameraZ = wallHeight / 2.0f;
        int prevTX = -1;
        int prevTY = -1;

        // Angle from x-axis: vector = (1, 0)
        const float angleXAxis = angleFromXAxis(ray.dirX, ray.dirY);

        // Store sin and cos for the angle to save calculations per pixel
        const float cosAngle = cos(angleXAxis);
        const float sinAngle = sin(angleXAxis);

        // Correct for fisheye-effect
        const float pointDistCorrection = cameraZ / cos(ray.angle);

        for (int y = drawStart + lineHeight; y < m_height; y++)
        {
            // Offset from the center of the screen.
            float rayCenterOffset = y - halfH;

            // World distance to the point on the floor
            float pointDist = pointDistCorrection / rayCenterOffset;

            // Coordinates of the point on the floor
            float floorX = *m_camera.m_posX + cosAngle * pointDist;
            float floorY = *m_camera.m_posY - sinAngle * pointDist;

            // Texture coordinates
            int tx = int((floorX - (int)floorX) * 32);
            int ty = int((floorY - (int)floorY) * 32);

            float fog = calculateFog(pointDist, 1, 15);

            // Draw floor
            if (tx != prevTX && ty != prevTY)
            {
                //float fog = calculateFog(pointDist, 1, 15);
                m_color = m_floorTexture.getPixel(tx, ty);
                m_color = sf::Color(
                    m_color.r * fog,
                    m_color.g * fog,
                    m_color.b * fog,
                    m_color.a);
            }
            m_pixelBuffer.setPixel(x, y, m_color);

            // Draw ceiling
            if (tx != prevTX && ty != prevTY)
            {
                //float fog = calculateFog(pointDist, 1, 15);
                m_color = m_ceilingTexture.getPixel(tx, ty);
                m_color = sf::Color(
                    m_color.r * fog,
                    m_color.g * fog,
                    m_color.b * fog,
                    m_color.a);
            }
            m_pixelBuffer.setPixel(x, m_height - y, m_color);
        }
    	x++;
    }
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
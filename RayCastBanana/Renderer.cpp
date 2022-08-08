#include "imgui.h"
#include "imgui-SFML.h"

#include "Renderer.h"

#include "MathMethods.h"
#include "mediaManager.h"
#include "m_grid.h"


Renderer::Renderer(int width, int height, Model* model) :
	m_window(sf::VideoMode(width, height), "Banana"),
	m_model(model)
{
    m_width = 640;
    m_height = 480;
    m_window.setFramerateLimit(60);
    m_camera.attachToPlayer(*m_model->getPlayer());
    //m_window.setMouseCursorVisible(false);

	m_wallTexture = load_img("resources\\images\\textures\\t_wall_spaceship_01.png");
    m_floorTexture = load_img("resources\\images\\textures\\texture_floor_scifi_01.png");
    m_ceilingTexture = load_img("resources\\images\\textures\\texture_ceiling_window_01.png");

    m_pixelBuffer.create(m_width, m_height, sf::Color(255, 255, 255, 255));
    m_pixelBufferClear.create(m_width, m_height, sf::Color(0, 0, 0, 255));

    m_screenTexture.create(m_width, m_height);
    m_screenSprite.setScale(width / m_width, height / m_height);
    ImGui::SFML::Init(m_window);
}

void Renderer::update(sf::Time dt)
{
    sf::Time deltaTime;


    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
        {
            
            m_window.close();
            ImGui::SFML::Shutdown();
            exit(1);
        }
    }
	ImGui::SFML::Update(m_window, dt);

	m_window.clear();
    clearDepthBuffer();

    m_camera.update(m_width, m_height, m_model->getGrid(), m_model->getProps());
    drawWorld();
	drawObjects();


    drawFromDepth();

    m_screenTexture.update(m_pixelBuffer);
    m_screenSprite.setTexture(m_screenTexture);
    m_window.draw(m_screenSprite);


    drawMinimap(m_model->getGrid(), m_model->getPawn());

    m_uiManager.update();

    ImGui::SFML::Render(m_window);
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

    int cnt = 0;

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

            cnt++;
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
        cnt++;
	}

    // Draw circle
    float dotRadius = 2.5f;
    sf::CircleShape playerDot(dotRadius, 8);
    color = sf::Color(0, 255, 0, 255);
    playerDot.setPosition(mapPosX - dotRadius, mapPosY - dotRadius);
    playerDot.setFillColor(color);
    m_window.draw(playerDot);
    cnt++;
    // std::cout << "Minimap draw calls: " << cnt << std::endl;
}

void Renderer::drawWorld()
{
    // Clear pixel buffer
    m_pixelBuffer = sf::Image(m_pixelBufferClear);

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
        double textureStep = (double)32.0 / lineHeight;
        int prevTextureY = -1; // I
		sf::Color color;


        int zDepth = int(ray.wallDist * 100);

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

            m_depthBuffer.at(zDepth).push_back(Pixel(x, y, color));
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
        cnt++;

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
            m_pixelBuffer.setPixel(x, m_height - y, color);
        }
    	x++;
    }
    // std::cout << "NUMBER OF PIXELS DRAWN: " << cnt << std::endl;
}

void Renderer::drawObjects()
{
    const double wallHeight = m_height * 0.75;
	for (auto object : m_camera.m_renderObj)
	{
        const double imgSize = wallHeight / object.dist;
        const double halfImgSize = imgSize * 0.5;

        // First pixel in the y-axis to draw
        double drawStartY = (m_height - imgSize) / 2.0;
		if (drawStartY < 0) drawStartY = 0.0;

        // draw start offset if line is higher then screen
        double offsetY = 0.0; 

        // Last pixel in the y-axis to draw.
        double drawEndY = imgSize;
        if (drawEndY > m_height)
        {
            drawEndY = m_height;
            offsetY = (imgSize - drawEndY) * 0.5;
        }

        // draw start offset if line is higher then screen
        double offsetX = 0.0;

        // Last pixel in the y-axis to draw.
        double drawEndX = imgSize;
        if (drawEndX > m_width)
        {
            drawEndX = m_width;
            offsetX = (imgSize - drawEndX) * 0.5;
        }

        int zDepth = int(object.dist * 100);

        for (int y = 0; y < drawEndY; ++y)
        {
	        for (int x = 0; x < drawEndX; ++x)
	        {
                int tx = int(((x + offsetX) / imgSize) * 32.0);
                int ty = int(((y + offsetY) / imgSize) * 32.0);

                sf::Color color = object.img.getPixel(tx, ty);
                if (color.a > 0)
                {
                    int xCoord = object.screenX + x - halfImgSize;
                    if (xCoord > -1 && xCoord < m_width) // Only draw pixels that are visable
                    {
                        m_depthBuffer.at(zDepth).push_back(Pixel(xCoord, drawStartY + y, color));
                    }
                }
	        }
        }
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

void Renderer::clearDepthBuffer()
{
    m_depthBuffer.clear();
    for (int i = 0; i < 3500; i++)
    {
        std::vector<Pixel> empty;
        m_depthBuffer.insert(std::make_pair(i, empty));
    }
}

void Renderer::drawFromDepth()
{
	for (int i = m_depthBuffer.size() - 1; i > -1; --i)
	{
		for (auto pixel : m_depthBuffer.at(i))
		{
            m_pixelBuffer.setPixel(pixel.x, pixel.y, pixel.color);
		}
	}
}
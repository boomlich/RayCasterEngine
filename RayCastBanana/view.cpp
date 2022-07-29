#include "view.h"
#include "texture_loader.h"


View::View(int width, int height) : m_window(sf::VideoMode(width, height), "Banana")
{
    m_width = width;
    m_height = height;
    m_camera = Camera();
}

void View::setGrid(Grid grid)
{
    m_mapGrid = grid;
    m_camera.setPosition((sf::Vector2f)m_mapGrid.getPlayerSpawn());
}

/**
 * \brief main rendering loop of the application
 */
void View::render_loop()
{
    const sf::Texture texture = load_texture("resources/images/maps/map_01.png");

    sf::Sprite map;
    map.setTexture(texture);
    map.setScale(10.0f, 10.0f);

    m_window.setFramerateLimit(60);

    sf::RectangleShape rect(sf::Vector2f(100.0, 10.0));
    rect.setPosition(100, 100);

    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
	            
                m_window.close();
        }

        
        

        m_window.clear();
        m_camera.update(m_window, m_mapGrid);
        m_window.draw(map);
        m_window.draw(rect);
        m_window.display();
    }
}
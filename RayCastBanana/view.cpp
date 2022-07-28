#include "view.h"
#include "texture_loader.h"

View::View(int width, int height) : m_window(sf::VideoMode(width, height), "Banana")
{
    m_width = width;
    m_height = height;
    render_loop();
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

    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
	            
                m_window.close();
				
        }

        m_window.clear();
        m_window.draw(map);
        m_window.display();
    }
}
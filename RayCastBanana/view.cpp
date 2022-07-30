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


    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
	            
                m_window.close();

            if (event.type == sf::Event::KeyPressed)
            {
	            if (event.key.code == sf::Keyboard::W)
	            {
                    m_camera.moveForward(0.1);
                    //std::cout << "W pressed" << std::endl;;
                }
                else if (event.key.code == sf::Keyboard::A)
                {
                    std::cout << "A pressed" << std::endl;
                    // m_camera.moveForward(0);
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    std::cout << "Left pressed" << std::endl;
                    m_camera.rotate(-0.1);
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    std::cout << "Left pressed" << std::endl;
                    m_camera.rotate(0.1);
                }
            }
        }
        
        

        m_window.clear();
        m_camera.update(m_window, m_mapGrid);
        m_window.draw(map);
        m_window.display();
    }
}
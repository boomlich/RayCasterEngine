#include "view.h"
#include "texture_loader.h"

View::View(int width, int height) : window(sf::VideoMode(width, height), "Banana")
{
    this->width = width;
    this->height = height;
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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
	            
                window.close();
				
        }

        window.clear();
        window.draw(map);
        window.display();
    }
}
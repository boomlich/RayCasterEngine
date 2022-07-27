#include "view.h"

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
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
	            
                window.close();
				
        }

        window.clear();
        window.display();
    }
}
#include "Controller.h"

#include <iostream>

Controller::Controller(Model* model, Renderer* renderer)
{
	m_renderer = renderer;
	m_model = model;
	m_pawn = model->getPawn();
	gameLoop();

	oldMouseX = sf::Mouse::getPosition().x;
}

void Controller::gameLoop()
{
	sf::Clock clock;
	while (true)
	{
		sf::Time time = clock.restart();
		m_dt = time.asSeconds();
		keyboardInput();
		m_renderer->update(time);
		m_model->update(m_dt);
	}
}

void Controller::keyboardInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_pawn->moveForward(1.0 * m_dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_pawn->moveForward(-1.0 * m_dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_pawn->moveSideways(-1.0 * m_dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_pawn->moveSideways(1.0 * m_dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_pawn->rotate(-1.0 * m_dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_pawn->rotate(1.0 * m_dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		exit(3);
	}

	// Lock the mouse in center of screen
	//int newMouseX = sf::Mouse::getPosition().x;
	//m_pawn->rotate((newMouseX - oldMouseX) * m_dt * 0.25);
	//sf::Mouse::setPosition(sf::Vector2i(
	//	m_renderer->getWindowPosition().x + m_renderer->getWidth() / 2,
	//	m_renderer->getWindowPosition().y + m_renderer->getHeight() / 2
	//));

	//oldMouseX = sf::Mouse::getPosition().x;
	
}


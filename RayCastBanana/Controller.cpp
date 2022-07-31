#include "Controller.h"

#include <iostream>

Controller::Controller(Model* model, Renderer* renderer)
{
	m_renderer = renderer;
	m_model = model;
	m_pawn = model->getPawn();
	gameLoop();
}

void Controller::gameLoop()
{
	sf::Clock clock;

	while (true)
	{
		const float dt = clock.restart().asSeconds();
		keyboardInput();
		m_renderer->update(dt);
		m_model->update(dt);
	}
}

void Controller::keyboardInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_pawn->moveForward(0.025);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_pawn->moveForward(-0.025);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_pawn->moveSideways(-0.025);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_pawn->moveSideways(0.025);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_pawn->rotate(-0.025);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_pawn->rotate(0.025);
	}
}


#include "Player.h"

#include <iostream>

void Player::moveForward(double x)
{
	m_posX += m_dirX * x;
	m_posY += m_dirY * x;
}

void Player::moveSideways(double x)
{
	m_posX -= m_dirY * x;
	m_posY += m_dirX * x;
}

void Player::rotate(double x)
{
	double oldDirX = m_dirX;
	m_dirX = m_dirX * cos(x) - m_dirY * sin(x);
	m_dirY = oldDirX * sin(x) + m_dirY * cos(x);
}

Player::Player()
{
	m_posX = 0.0;
	m_posY = 0.0;
	m_dirX = 0.0;
	m_dirY = -1.0;
}

void Player::setSpawn(int x, int y, double dirX, double dirY)
{
	
	m_posX = (double)x;
	m_posY = (double)y;
	m_dirX = dirX;
	m_dirY = dirY;
	std::cout << "UPDATED: " << m_posX << " :" << m_posY << std::endl;
}

sf::Vector2<double*> Player::getDir()
{
	return sf::Vector2<double*>(&m_dirX, &m_dirY);
}

sf::Vector2<double*> Player::getPos()
{
	return sf::Vector2<double*>(&m_posX, &m_posY);
}
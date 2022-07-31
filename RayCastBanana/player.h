#pragma once
#include <SFML/System/Vector2.hpp>

#include "CtrlPawn.h"

class Player : public CtrlPawn
{
private:
	double m_posX, m_posY;
	double m_dirX, m_dirY;

	void moveForward(double x) override;
	void moveSideways(double x) override;
	void rotate(double x) override;
public:
	Player();
	void setSpawn(int x, int y, double dirX, double dirY);
	sf::Vector2<double*> getPos();
	sf::Vector2<double*> getDir();
};

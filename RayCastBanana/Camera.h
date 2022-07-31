#pragma once
#include "Player.h"

class Camera
{
public:
	double* m_posX;
	double* m_posY;
	double* m_dirX;
	double* m_dirY;

public:
	void attachToPlayer(Player& player);
	void update();
};

#pragma once
#include <SFML/System/Vector2.hpp>

#include "MapParser.h"
#include "Player.h"
#include "Renderable.h"

class Model : public Renderable
{
private:
	Player m_player;
	MapParser m_mapParser;
	Grid m_grid;

private:
	void loadLevel();

public:
	CtrlPawn* getPawn();
	Model();
	Grid& getGrid();
	void update(float dt);
	Player* getPlayer();
};

#pragma once

#include "MapParser.h"
#include "Player.h"

class Model
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

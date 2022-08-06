#pragma once

#include "EntityManager.h"
#include "MapParser.h"
#include "Player.h"
#include "Renderable.h"

class Model : public Renderable
{
private:
	Player m_player;
	MapParser m_mapParser;
	Grid m_grid;
	EntityManager m_entityManager;

private:
	void loadLevel();

public:
	CtrlPawn* getPawn();
	Model();
	Grid& getGrid();
	void update(float dt);
	Player* getPlayer();
	std::vector<Prop> getProps();
};

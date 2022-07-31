#include "Model.h"

Model::Model()
{
	loadLevel();
}

Grid& Model::getGrid()
{
	return m_grid;
}

void Model::update(float dt)
{
}

Player* Model::getPlayer()
{
	return &m_player;
}

CtrlPawn* Model::getPawn()
{
	return &m_player;
}

void Model::loadLevel()
{
	m_grid = m_mapParser.parse_img("resources/images/maps/map_01.png");
	m_player.setSpawn(m_grid.getPlayerSpawn().x, m_grid.getPlayerSpawn().y, 0.0, -1.0);

}

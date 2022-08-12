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

std::vector<Prop> Model::getProps()
{
	return m_entityManager.getProps();
}

CtrlPawn* Model::getPawn()
{
	return &m_player;
}

void Model::loadLevel()
{
	m_grid = m_mapParser.parse_img("resources/images/maps/map_01.png");
	m_player.setSpawn(m_grid.getPlayerSpawn().x, m_grid.getPlayerSpawn().y, 0.0, -1.0);
	//m_entityManager.addProp(Prop("resources/images/textures/props/texture_prop_crate_01.png", 10, 10));
	//m_entityManager.addProp(Prop("resources/images/textures/props/texture_prop_crate_01.png", 10, 11));

}

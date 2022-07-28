#include "Model.h"

Model::Model() : m_grid(100, 100)
{
	m_map_parser = MapParser();
	m_grid = m_map_parser.parse_img("resources/images/maps/map_01.png");
	m_camera = Camera(
		384,
		(sf::Vector2f)m_grid.getPlayerSpawn(),
		sf::Vector2f(-1, 0),
		sf::Vector2f(0.0f, 0.6666f)
	);
}

void Model::update(double deltaTime)
{

}

Grid& Model::getGrid()
{
	return m_grid;
}

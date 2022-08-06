#include "EntityManager.h"

void EntityManager::addProp(Prop prop)
{
	m_props.push_back(prop);
}

void EntityManager::update(double dt)
{
}

std::vector<Prop> EntityManager::getProps()
{
	return m_props;
}

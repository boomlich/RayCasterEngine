#pragma once
#include <vector>

#include "Prop.h"

class EntityManager
{
private:
	std::vector<Prop> m_props;
public:
	void addProp(Prop prop);
	void update(double dt);
	std::vector<Prop> getProps();
};

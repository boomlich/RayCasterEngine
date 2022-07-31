#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>

inline double vectorLength(sf::Vector2f input)
{
	return sqrt(input.x * input.x + input.y * input.y);
}

inline sf::Vector2f vectorNormal(sf::Vector2f input)
{
	return sf::Vector2f(-input.y, input.x);
}

inline sf::Vector2f vectorUnit(sf::Vector2f input)
{
	double length = vectorLength(input);
	return sf::Vector2f(input.x / length, input.y / length);
}

inline sf::Vector2f vectorNormalUnit(sf::Vector2f input)
{
	return vectorUnit(vectorNormal(input));
}

inline sf::Vector2<double> rotateVector(sf::Vector2<double> input, double angle)
{
	double vecX = input.x * cos(angle) - input.y * sin(angle);
	double vecY = input.x * sin(angle) + input.y * cos(angle);
	return sf::Vector2<double>(vecX, vecY);
}
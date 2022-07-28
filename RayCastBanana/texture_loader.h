#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

inline sf::Texture load_texture(const std::string& path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cout << "[ERROR] File not found";
	}

	return texture;
}

inline sf::Image load_img(const std::string& path)
{
	sf::Image image;
	if (!image.loadFromFile(path))
	{
		std::cout << "[ERROR] File not found";
	}
	return image;
}
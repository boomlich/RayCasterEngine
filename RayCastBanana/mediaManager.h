#pragma once

#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>




enum TextureID
{
	TX_WALL_SCIFI_01,
	TX_FLOOR_SCIFI_01,
	TX_CEILING_SCIFI_01
};

extern std::unordered_map<TextureID, sf::Image> loadedImages;
extern std::unordered_map<TextureID, sf::Texture> loadedTextures;


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
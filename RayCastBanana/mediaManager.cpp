#include "mediaManager.h"

std::unordered_map<TextureID, sf::Texture> loadedTextures = {
        {TX_WALL_SCIFI_01, load_texture("resources\\images\\textures\\t_wall_spaceship_01.png")},
        {TX_FLOOR_SCIFI_01, load_texture("resources\\images\\textures\\texture_floor_scifi_01.png")},
        {TX_CEILING_SCIFI_01, load_texture("resources\\images\\textures\\texture_ceiling_window_01.png")},
};
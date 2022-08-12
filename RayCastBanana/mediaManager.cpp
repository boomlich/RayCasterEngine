#include "mediaManager.h"

//std::unordered_map<TextureID, sf::Texture> loadedTextures = {
//    {TX_WALL_SCIFI_01, load_texture("resources\\images\\textures\\t_wall_spaceship_01.png")},
//    {TX_FLOOR_SCIFI_01, load_texture("resources\\images\\textures\\texture_floor_scifi_01.png")},
//    {TX_CEILING_SCIFI_01, load_texture("resources\\images\\textures\\texture_ceiling_window_01.png")},
//    {TX_PR_BOX_01, load_texture("resources\\images\\textures\\props\\texture_prop_crate_01.png")},
//};

//std::unordered_map<TextureID, sf::Texture> loadedTextures = {
//    {TX_WALL_SCIFI_01, sf::Texture()},
//    {TX_FLOOR_SCIFI_01, sf::Texture()},
//    {TX_CEILING_SCIFI_01, sf::Texture()},
//    {TX_PR_BOX_01, sf::Texture()},
//};

std::unordered_map<TextureID, sf::Image> loadedImages = {
    {TX_WALL_SCIFI_01, load_img("resources\\images\\textures\\t_wall_spaceship_01.png")},
    {TX_FLOOR_SCIFI_01, load_img("resources\\images\\textures\\texture_floor_scifi_01.png")},
    {TX_CEILING_SCIFI_01, load_img("resources\\images\\textures\\texture_ceiling_window_01.png")},
    {TX_PR_BOX_01, load_img("resources\\images\\textures\\props\\texture_prop_crate_01.png")},
};
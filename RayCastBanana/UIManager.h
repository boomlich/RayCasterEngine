#pragma once
#include <string>
#include <SFML/Graphics/Texture.hpp>

#include "mediaManager.h"

enum EditorMode
{
	EDIT_WALL,
	EDIT_SELECT,
	EDIT_SELECTED,
	EDIT_OPTIONS,
	EDIT_FLOOR,
	EDIT_EYEDROPPER
};


enum ContentType
{
	C_TEXTURE,
	C_ENEMY,
	C_PROP,
	C_AUDIO
};

enum ContentCategory
{
	C_Cat_TX_WALL,
	C_CAT_TX_FLOOR,
	C_CAT_TX_CEILING,
};

struct Content
{
	std::string name;
	TextureID txID;
	ContentType type;
	ContentCategory cat;
	Content(std::string name, TextureID txID, ContentType type, ContentCategory cat)
	{
		this->name = name;
		this->txID = txID;
		this->type = type;
		this->cat = cat;
	}
};

enum UIState
{
	UI_HUD,
	UI_PAUSE,
	UI_MAIN_MENU,
	UI_LEVEL_EDITOR


};

struct Brush
{
	TextureID textures[4];
};

class UIManager
{
private:
	UIState m_uiState;
	bool showProfiler;
	std::vector<Content> contentTextures;

private:
	bool addContentWithCategory(std::vector<Content> &content, ContentCategory targetCat);
public:
	UIManager();
	void update();
	void HUD();
	void mainMenu();
	void pauseMenu();
	void levelEditor();
	void profiler();
};

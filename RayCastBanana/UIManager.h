#pragma once
#include <string>

#include "CellGrid.h"
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
	C_NONE,
	C_TEXTURE,
	C_ENEMY,
	C_PROP,
	C_AUDIO
};



enum ContentCategory
{
	C_CAT_NONE,
	C_CAT_TX_WALL,
	C_CAT_TX_FLOOR,
	C_CAT_TX_CEILING,
	C_CAT_PR_BOX
};

struct Content
{
	std::string name;
	TextureID txID;
	ContentType type;
	ContentCategory cat;
	ContentID cID;
	Content(std::string name, TextureID txID = TX_FLOOR_SCIFI_01, ContentType type = C_TEXTURE, ContentCategory cat = C_CAT_NONE, ContentID cID = C_ID_NONE)
	{
		this->name = name;
		this->txID = txID;
		this->type = type;
		this->cat = cat;
		this->cID = cID;
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
	std::vector<Content> contentProps;
	Cell* selectedCell;
	Prop* selectedProp;

	std::unordered_map<TextureID, sf::Texture> loadedTextures;


private:
	bool addContentWithCategory(std::vector<Content> &content, ContentCategory targetCat, std::string dropID);
	void addObjectToMap(CellGrid &grid, float &mapWidth, float &mapHeight, int &gridWidth, int &gridHeight, float &mapPosX, float &mapPosY, float cellSize);
	void deselect();
public:
	UIManager();
	void update();
	void HUD();
	void mainMenu();
	void pauseMenu();
	void levelEditor();
	void profiler();
};

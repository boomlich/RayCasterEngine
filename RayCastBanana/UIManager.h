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
	Brush(TextureID tx0 = TX_WALL_SCIFI_01, TextureID tx1 = TX_WALL_SCIFI_01, TextureID tx2 = TX_WALL_SCIFI_01, TextureID tx3 = TX_WALL_SCIFI_01)
	{
		textures[0] = tx0;
		textures[1] = tx1;
		textures[2] = tx2;
		textures[3] = tx3;
	}
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
	void addPropertiesEditTexture(int num, std::string names[], TextureID textures[]);
	void addContentTree(std::string name, ContentCategory cat, std::string dropID);
	void addBrushProperties(std::string title, int txCount, std::string txNames[], TextureID tx[4]);
	void addSelectedCellProperties(std::string name, std::string txTitle, int txCount, std::string txNames[]);

public:
	UIManager();
	void update();
	void HUD();
	void mainMenu();
	void pauseMenu();
	void levelEditor();
	void profiler();
};

#pragma once

enum EditorMode
{
	EDIT_WALL,
	EDIT_SELECT,
	EDIT_SELECTED,
	EDIT_OPTIONS,
	EDIT_FLOOR,
	EDIT_EYEDROPPER
};

enum UIState
{
	UI_HUD,
	UI_PAUSE,
	UI_MAIN_MENU,
	UI_LEVEL_EDITOR


};

class UIManager
{
private:
	UIState m_uiState;
	bool showProfiler;
public:
	UIManager();
	void update();
	void HUD();
	void mainMenu();
	void pauseMenu();
	void levelEditor();
	void profiler();
};

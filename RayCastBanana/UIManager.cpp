#include "UIManager.h"
#include <imgui.h>
#include <iostream>
#include <string>

UIManager::UIManager()
{
    m_uiState = UI_LEVEL_EDITOR;
}

void UIManager::update()
{
	if (m_uiState == UI_PAUSE)
	{
        pauseMenu();
	} else if (m_uiState == UI_LEVEL_EDITOR)
	{
        levelEditor();
	}

	if (showProfiler)
	{
        profiler();
	}
}

void UIManager::HUD()
{

}

void UIManager::mainMenu()
{

}

void UIManager::pauseMenu()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::Begin("My First Tool", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize
    );

    //ImGui::SetCursorPos(ImVec2(10.0f, 10.0f));
    std::string title = "Game Paused";
    float winWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(title.c_str()).x;

    ImGui::SetCursorPosX((winWidth - textWidth) * 0.5);
    ImGui::Text(title.c_str());

    if (ImGui::Button("Save game"))
    {
        std::cout << "saved" << std::endl;
    }

    if (ImGui::Button("Load game"))
    {
        std::cout << "saved" << std::endl;
    }

    if (ImGui::Button("Controls"))
    {
        std::cout << "saved" << std::endl;
    }

    if (ImGui::Button("Options"))
    {
        std::cout << "saved" << std::endl;
    }

    if (ImGui::Button("Exit level"))
    {
        std::cout << "saved" << std::endl;
    }
    ImGui::End();
}

void UIManager::levelEditor()
{
    static int gridWidth = 16;
    static int gridHeight = 16;
    int gridSizePx = 300;
    const float padding = 1.0f;

    float cellWidth = (gridSizePx - gridWidth * padding) / gridWidth;
    float cellHeight = (gridSizePx - gridHeight * padding) / gridHeight;
    float cellSize = cellWidth;
    if (cellHeight < cellSize) cellSize = cellHeight;

    static EditorMode editMode = EDIT_WALL;


    const ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(640, 480));
    ImGui::Begin("ViewPort", nullptr, windowFlags);

    int toolSize = 32;
    ImGui::BeginChild("Toolbar", ImVec2(toolSize * 2, 300), true);
    ImGui::Text("Tools");
    if (ImGui::Button("S", ImVec2(toolSize, toolSize)))
    {
        editMode = EDIT_SELECTED;
    }
    if (ImGui::Button("W", ImVec2(toolSize, toolSize)))
    {
        editMode = EDIT_WALL;
    }
	if (ImGui::Button("F", ImVec2(toolSize, toolSize)))
    {
        editMode = EDIT_FLOOR;
    }
    if (ImGui::Button("E", ImVec2(toolSize, toolSize)))
    {
        editMode = EDIT_EYEDROPPER;
    }
    if (ImGui::Button("O", ImVec2(toolSize, toolSize)))
    {
        editMode = EDIT_OPTIONS;
    }

    ImGui::EndChild();

    ImGui::SameLine();


    std::string title = "Map Viewport";
    
	ImGui::BeginChild("Map", ImVec2(gridSizePx, gridSizePx + ImGui::CalcTextSize(title.c_str()).y));
    ImGui::Text(title.c_str());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(padding, padding));
    for (int y = 0; y < gridHeight; y++)
        for (int x = 0; x < gridWidth; x++)
        {
        	if (x > 0)
                ImGui::SameLine();

            ImGui::PushID(y * gridWidth + x);

            if (ImGui::Button("-", ImVec2(cellSize, cellSize)));
            {

            }
            ImGui::PopID();
        }
    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("Properties", ImVec2(200, 300));
    ImGui::Text("Properties");

    if (editMode == EDIT_SELECTED)
    {
        ImGui::Text("Selected cell: (%d, %d)", 2, 5);
        ImGui::Text("Pulse-rifle Ammo");
    }
	else if (editMode == EDIT_WALL)
    {
        ImGui::Text("Selected cell: (%d, %d)", 10, 6);
        ImGui::Text("Wall");
    }
    else if (editMode == EDIT_OPTIONS)
    {
        ImGui::Text("Map size:");
        ImGui::SliderInt("Width", &gridWidth, 8, 32);
        ImGui::SliderInt("Height", &gridHeight, 8, 32);
    }

    ImGui::EndChild();

    ImGui::BeginChild("contentBrowser", ImVec2(460, 150));
    ImGui::Text("Content Browser");

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Textures"))
        {
            ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Doors"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Props"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Enemies"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Miscellaneous"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::EndChild();

    ImGui::End();
}

void UIManager::profiler()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::Begin("My First Tool", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize
    );




}

#include "UIManager.h"

#include <imgui.h>
#include "imgui-SFML.h"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Cell.h"
#include "CellGrid.h"
#include "Renderer.h"
#include "mediaManager.h"



UIManager::UIManager()
{
    m_uiState = UI_LEVEL_EDITOR;

    contentTextures.emplace_back(Content("Big panels - 01", TX_WALL_SCIFI_01, C_TEXTURE, C_Cat_TX_WALL));
    contentTextures.emplace_back(Content("Pathway - 01", TX_FLOOR_SCIFI_01, C_TEXTURE, C_CAT_TX_FLOOR));
    contentTextures.emplace_back(Content("Bright window - 01", TX_CEILING_SCIFI_01, C_TEXTURE, C_CAT_TX_CEILING));
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

    // Zoom
    static float zoomLevel = 1;
    float maxZoom = 4.0f;
    float minZoom = 0.5f;
    cellSize *= zoomLevel;


    static CellGrid cellGrid(gridWidth, gridHeight);
    //static Cell selectedCell(CELL_DISABLED, 0, 0);
    static Cell *selectedCell = nullptr;

    if (editMode != EDIT_SELECT)
    {
        selectedCell = nullptr;
    }

    static Brush brush;



    const ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(640, 480));
    ImGui::Begin("ViewPort", nullptr, windowFlags);

    int toolSize = 32;

    EditorMode toolModes[5] = { EDIT_SELECT, EDIT_WALL, EDIT_FLOOR, EDIT_EYEDROPPER, EDIT_OPTIONS };
    std::string toolNames[] = { "Select", "Wall brush", "Free tool", "Eyedropper", "Options"};
    ImGui::BeginChild("Toolbar", ImVec2(toolSize * 2, 300), true);
    ImGui::Text("Tools");

    for (int i = 0; i < 5; ++i)
    {
        int styleCount = 0;
        if (editMode == toolModes[i])
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 0, 0, 255));
            styleCount++;
        }
        ImGui::PushID(i);
        if (ImGui::Button("T", ImVec2(toolSize, toolSize))) { editMode = toolModes[i]; }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(toolNames[i].c_str());
        }
        ImGui::PopID();
        ImGui::PopStyleColor(styleCount);
    }

    ImGui::EndChild();

    ImGui::SameLine();


    std::string title = "Map Viewport";

    static ImVec2 offset(0.0f, 0.0f);
    
	ImGui::BeginChild("Map", ImVec2(gridSizePx, 325));
    ImGui::Text(title.c_str());

    

    

    ImGui::BeginChild("MapContainer", ImVec2(gridSizePx, gridSizePx), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset.x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset.y);
    float viewportX = cellSize * gridWidth + gridWidth * padding;
    float viewportY = cellSize * gridHeight + gridHeight * padding;

    ImGui::BeginChild("Viewport", ImVec2(viewportX, viewportY), false, ImGuiWindowFlags_NoScrollbar);
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
    {
        offset.x += ImGui::GetIO().MouseDelta.x;
        offset.y += ImGui::GetIO().MouseDelta.y;
    }

    // Viewport zooming
    zoomLevel += ImGui::GetIO().MouseWheel * 0.1;
    if (zoomLevel < minZoom)
    {
        zoomLevel = minZoom;
    } else if (zoomLevel > maxZoom)
    {
        zoomLevel = maxZoom;
    }

    // Viewport grid
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(padding, padding));
    for (int y = 0; y < gridHeight; y++)
        for (int x = 0; x < gridWidth; x++)
        {
        	if (x > 0)
                ImGui::SameLine();

            ImGui::PushID(y * gridWidth + x);
            Cell *cell = cellGrid.getCell(x, y);

            int styleCount = 0;
            if (cell == selectedCell)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255, 0, 255));
                styleCount++;
            }
        	else if (cell->m_type == CELL_WALL)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 255));
                styleCount++;
            }

            if (ImGui::Button("-", ImVec2(cellSize, cellSize)))
            {
                if (editMode == EDIT_FLOOR)
                {
                    cell->m_type = CELL_FREE;
                    cell->m_textures[0] = brush.textures[0];
                    cell->m_textures[1] = brush.textures[1];
                }
            	else if (editMode == EDIT_WALL)
                {
                    cell->m_type = CELL_WALL;
                    cell->m_textures[0] = brush.textures[0];
                    cell->m_textures[1] = brush.textures[1];
                    cell->m_textures[2] = brush.textures[2];
                    cell->m_textures[3] = brush.textures[3];
                }
            	else if (editMode == EDIT_SELECT)
                {
                    selectedCell = cell;
                }
            }
            ImGui::PopStyleColor(styleCount);
            ImGui::PopID();
        }
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("Properties", ImVec2(200, 300));
    ImGui::Text("Properties");
    ImGui::Spacing();

    if (editMode == EDIT_SELECT)
    {
	    if (selectedCell != nullptr)
	    {
            if (selectedCell->m_type == CELL_WALL)
            {
                ImGui::Text("Wall cell");
                ImGui::Text("(%d, %d)", selectedCell->getPosX(), selectedCell->getPosY());
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Text("Set textures:");

                std::string names[4] = { "North", "South", "West", "East" };

                for (int i = 0; i < 4; ++i)
                {
                    if (i > 0) ImGui::SameLine();

                    TextureID txID = selectedCell->m_textures[i];
                    sf::Sprite sprite = sf::Sprite(loadedTextures.at(txID));
                    ImGui::BeginGroup();
                    ImGui::Text(names[i].c_str());
                    ImGui::ImageButton(sprite);

                    if (ImGui::BeginDragDropSource())
                    {
                        ImGui::SetDragDropPayload("TEXTURE_DRAG", &txID, sizeof(int));
                        ImGui::Image(sprite, sf::Vector2f(16.0f, 16.0f));
                        ImGui::EndDragDropSource();
                    }

                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(int));
                            TextureID payload_n = *(const TextureID*)payload->Data;
                            selectedCell->m_textures[i] = payload_n;
                        }
                        ImGui::EndDragDropTarget();
                    }
                    ImGui::EndGroup();
                }
            }
            else if (selectedCell->m_type == CELL_FREE)
            {
                ImGui::Text("Free cell");
                ImGui::Text("(%d, %d)", selectedCell->getPosX(), selectedCell->getPosY());
            }
	    }
    }
	else if (editMode == EDIT_WALL)
    {

        ImGui::Text("Wall brush");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Set textures:");
        std::string names[4] = { "North", "South", "West", "East" };

        for (int i = 0; i < 4; ++i)
        {
            if (i > 0) ImGui::SameLine();

            TextureID txID = brush.textures[i];
            sf::Sprite sprite = sf::Sprite(loadedTextures.at(txID));
            ImGui::BeginGroup();
            ImGui::Text(names[i].c_str());
            ImGui::ImageButton(sprite);

            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("TEXTURE_DRAG", &txID, sizeof(int));
                ImGui::Image(sprite, sf::Vector2f(16.0f, 16.0f));
                ImGui::EndDragDropSource();
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(int));
                    TextureID payload_n = *(const TextureID*)payload->Data;

                    brush.textures[i] = payload_n;
                }
                ImGui::EndDragDropTarget();
            }


            ImGui::EndGroup();
        }
    }
    else if (editMode == EDIT_OPTIONS)
    {
        ImGui::Text("Map size:");
        ImGui::SliderInt("Width", &gridWidth, 8, 64);
        ImGui::SliderInt("Height", &gridHeight, 8, 64);
        // std::cout << ImGui::IsItemActive() << std::endl;

        ImGui::Spacing();
        ImGui::Text("Map zoom:");
        ImGui::SliderFloat("Level", &zoomLevel, minZoom, maxZoom);

        if (ImGui::Button("Reset zoom"))
        {
            zoomLevel = 1;
        }
        ImGui::SameLine();
    	if (ImGui::Button("Reset position"))
        {
            offset.x = 0;
            offset.y = 0;
        }
    }

    ImGui::EndChild();

    ImGui::BeginChild("contentBrowser", ImVec2(460, 150));
    ImGui::Text("Content Browser");

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Textures"))
        {
	        if (ImGui::TreeNode("C_TX_WALL", "Walls"))
	        {
                addContentWithCategory(contentTextures, C_Cat_TX_WALL);
                ImGui::TreePop();
	        }
            if (ImGui::TreeNode("C_TX_FLOOR", "Floors"))
            {
                addContentWithCategory(contentTextures, C_CAT_TX_FLOOR);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("C_TX_CEILING", "Ceilings"))
            {
                addContentWithCategory(contentTextures, C_CAT_TX_CEILING);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("C_TX_DECAL", "Decals"))
            {
                ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
                ImGui::TreePop();
            }

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


bool UIManager::addContentWithCategory(std::vector<Content> &content, ContentCategory targetCat)
{
    bool firstAdded = false;
	for (auto item : content)
	{
		if (item.cat == targetCat)
		{
			if (firstAdded)
			{
                ImGui::SameLine();
			}
            firstAdded = true;

            ImGui::BeginGroup();
            ImGui::ImageButton(loadedTextures.at(item.txID));
			if (ImGui::BeginDragDropSource())
			{
                ImGui::SetDragDropPayload("TEXTURE_DRAG", &item.txID, sizeof(int));
                ImGui::Image(sf::Sprite(loadedTextures.at(item.txID)), sf::Vector2f(16.0f, 16.0f));
                ImGui::EndDragDropSource();
			}
            ImGui::Text(item.name.c_str());
            ImGui::EndGroup();
		}
	}

    return true;
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

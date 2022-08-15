#include "UIManager.h"
#include "mediaManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <string>
#include <vector>

#include "Cell.h"
#include "CellGrid.h"
#include "Renderer.h"





UIManager::UIManager()
{
    m_uiState = UI_LEVEL_EDITOR;

    contentTextures.emplace_back(Content("Big panels - 01", TX_WALL_SCIFI_01, C_TEXTURE, C_CAT_TX_WALL));
    contentTextures.emplace_back(Content("Pathway - 01", TX_FLOOR_SCIFI_01, C_TEXTURE, C_CAT_TX_FLOOR));
    contentTextures.emplace_back(Content("Bright window - 01", TX_CEILING_SCIFI_01, C_TEXTURE, C_CAT_TX_CEILING));
    contentTextures.emplace_back(Content("Big barell - 01", TX_PR_BOX_01, C_PROP, C_CAT_PR_BOX, C_ID_PR_BOX_01));

    loadedTextures.insert(std::make_pair(TX_WALL_SCIFI_01, load_texture("resources\\images\\textures\\t_wall_spaceship_01.png")));
    loadedTextures.insert(std::make_pair(TX_FLOOR_SCIFI_01, load_texture("resources\\images\\textures\\texture_floor_scifi_01.png")));
    loadedTextures.insert(std::make_pair(TX_CEILING_SCIFI_01, load_texture("resources\\images\\textures\\texture_ceiling_window_01.png")));
    loadedTextures.insert(std::make_pair(TX_PR_BOX_01, load_texture("resources\\images\\textures\\props\\texture_prop_crate_01.png")));


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

    if (editMode != EDIT_SELECT)
    {
        deselect();
    }

    static Brush wallBrush;
    static Brush freeBrush(TX_FLOOR_SCIFI_01, TX_CEILING_SCIFI_01);

    const ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration;
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
        if (ImGui::Button("T", ImVec2(toolSize, toolSize))) editMode = toolModes[i];
        if (ImGui::IsItemHovered()) ImGui::SetTooltip(toolNames[i].c_str());
        ImGui::PopID();
        ImGui::PopStyleColor(styleCount);
    }

    ImGui::EndChild();

    ImGui::SameLine();

    std::string title = "Map Viewport";

    static ImVec2 offset(0.0f, 0.0f);
    
	ImGui::BeginChild("Map", ImVec2(gridSizePx, 325));
    ImGui::Text(title.c_str());

    ImGui::BeginChild("MapContainer", ImVec2(gridSizePx, gridSizePx), false, 
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset.x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset.y);
    float mapWidth = cellSize * gridWidth + gridWidth * padding;
    float mapHeight = cellSize * gridHeight + gridHeight * padding;

    // Screen coordinates of the top right corner of the map
    float mapPosX = ImGui::GetCursorScreenPos().x; // screen posX of the map top right corner
    float mapPosY = ImGui::GetCursorScreenPos().y; // screen posY of the map top right corner

    // Mouse position on the map gri
    float mouseGridPosX = (ImGui::GetMousePos().x - mapPosX) / mapWidth * (float)gridWidth;
    float mouseGridPosY = (ImGui::GetMousePos().y - mapPosY) / mapHeight * (float)gridHeight;

    // Display names of texture options
    std::string txNamesWall[4] = { "North", "South", "West", "East" };
    std::string txNamesFree[2] = { "Floor", "Ceiling" };

    ImGui::BeginChild("Viewport", ImVec2(mapWidth, mapHeight), false, ImGuiWindowFlags_NoScrollbar);
    

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
                    for (int i = 0; i < 2; ++i) cell->m_textures[i] = freeBrush.textures[i];
                }
            	else if (editMode == EDIT_WALL)
                {
                    cell->m_type = CELL_WALL;
                    for (int i = 0; i < 4; ++i) cell->m_textures[i] = wallBrush.textures[i];
                }
            	else if (editMode == EDIT_SELECT)
                {
                    deselect();
                    selectedCell = cell;
                }
            }

            std::string cellTitle = "Wall";
            int numOfTextures = 4;

            // Placing objects (Props, Enemies, etc.)
            if (cell->m_type == CELL_FREE)
            {
                cellTitle = "Free";
                numOfTextures = 2;
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ITEM_DRAG"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        ContentID payload_n = *(const ContentID*)payload->Data;

                    	deselect();
                        if (payload_n == C_ID_PR_BOX_01)
                        {
                            selectedProp = cellGrid.addProp(P_Box_01(mouseGridPosX, mouseGridPosY));
                        }
                        editMode = EDIT_SELECT;
                    }

                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ITEM_MOVE"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        ContentID payload_n = *(const ContentID*)payload->Data;

                        selectedProp->setPosX(mouseGridPosX);
                        selectedProp->setPosY(mouseGridPosY);
                    }
                    ImGui::EndDragDropTarget();
                }
            }

            if (ImGui::IsItemHovered())
            {
                const int cellX = static_cast<int>(mouseGridPosX);
                const int cellY = static_cast<int>(mouseGridPosY);
                const Cell* hoveredCell = cellGrid.getCell(cellX, cellY);

                ImGui::BeginTooltip();
                ImGui::Text(cellTitle.c_str());
                ImGui::Text("(%d, %d)", cellX, cellY);

                ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

                for (int i = 0; i < numOfTextures; ++i)
                {
                    if (i > 0) ImGui::SameLine();
                    ImGui::Image(sf::Sprite(loadedTextures.at(hoveredCell->m_textures[i])));
                }
                ImGui::EndTooltip();
            }
            
            ImGui::PopStyleColor(styleCount);
            ImGui::PopID();
        }
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::EndChild();

    if (ImGui::IsWindowHovered(ImGuiFocusedFlags_ChildWindows))
    {
        // Viewport zooming
        zoomLevel += ImGui::GetIO().MouseWheel * 0.1;
        if (zoomLevel < minZoom)
        {
            zoomLevel = minZoom;
        }
        else if (zoomLevel > maxZoom)
        {
            zoomLevel = maxZoom;
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
        {
            offset.x += ImGui::GetIO().MouseDelta.x;
            offset.y += ImGui::GetIO().MouseDelta.y;
        }
    }

    ImGui::EndChild();

    

    ImGui::SameLine();
    ImGui::BeginChild("Properties", ImVec2(200, 300));
    ImGui::Text("Properties");
    ImGui::Spacing();

    // Properties panel
    if (editMode == EDIT_SELECT)
    {
	    if (selectedCell != nullptr)
	    {
            // Selected cell properties
            if (selectedCell->m_type == CELL_WALL)
                addSelectedCellProperties("Wall cell", "Set wall textures:", 4, txNamesWall);
            else if (selectedCell->m_type == CELL_FREE)
                addSelectedCellProperties("Free cell", "Set textures:", 2, txNamesFree);
	    }
    }

	else if (editMode == EDIT_WALL)
        addBrushProperties("Wall cell brush", 4, txNamesWall, wallBrush.textures);
    else if (editMode == EDIT_FLOOR)
        addBrushProperties("Free cell brush", 2, txNamesFree, freeBrush.textures);

    else if (editMode == EDIT_OPTIONS)
    {
        ImGui::Text("Map size:");
        ImGui::SliderInt("Width", &gridWidth, 8, 64);
        ImGui::SliderInt("Height", &gridHeight, 8, 64);

        ImGui::Spacing();
        ImGui::Text("Map zoom:");
        ImGui::SliderFloat("Level", &zoomLevel, minZoom, maxZoom);

        ImGui::Spacing();
        ImGui::Text("Position offset:");
        ImGui::InputFloat("x", &offset.x, 1.0f, 10.0f);
        ImGui::InputFloat("y", &offset.y, 1.0f, 10.0f);

        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

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

    ContentCategory txCat[3] = { C_CAT_TX_WALL, C_CAT_TX_FLOOR, C_CAT_TX_CEILING };
    std::string txCatNames[3] = { "Walls", "Floor", "Ceiling" };

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Textures"))
        {
	        for (int i = 0; i < std::size(txCat); ++i)
                addContentTree(txCatNames[i], txCat[i], "TEXTURE_DRAG");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Props"))
        {
            addContentTree("Boxes", C_CAT_PR_BOX, "ITEM_DRAG");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();


    ImGui::SameLine();

    // Play / Save 
    ImGui::BeginChild("playLevelSection", ImVec2(100, 100));
    if (ImGui::Button("Save"))
    {
        std::cout << cellGrid.saveGrid() << std::endl;
    }
    ImGui::SameLine();
    if (ImGui::Button("Play"))
    {
        std::cout << cellGrid.saveGrid() << std::endl;
    }
    ImGui::EndChild();


    // ImGui::GetForegroundDrawList()->AddLine(ImVec2(0, 0), ImVec2(80, 100), ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
    // ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(100, 100), 20, ImGui::GetColorU32(ImVec4(0, 255, 0, 255)), 32);

    // Display all props on the grid
    int i = 0;
    for (auto item : cellGrid.getAllProps())
    {
        float x = (item.getPosX() * mapWidth) / gridWidth + mapPosX;
        float y = (item.getPosY() * mapHeight) / gridHeight + mapPosY;

        float size = cellSize / 2;
        std::string itemID = "Interact_prop" + std::to_string(i);

        ImGui::SetCursorPos(ImVec2(x - size / 2, y - size / 2));
        ImGui::BeginChild(itemID.c_str(), ImVec2(size, size));

        Prop *prop = cellGrid.getProp(i);

        if (prop == selectedProp)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255, 0, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 255, 255));


        if (ImGui::Button("-", ImVec2(size, size)))
        {
	        if (editMode == EDIT_SELECT) deselect(); selectedProp = prop;
        }

    	if (ImGui::BeginDragDropSource())
        {
            selectedProp = prop;
            ImGui::SetDragDropPayload("ITEM_MOVE", &item.getContentID(), sizeof(int));
            ImGui::Image(sf::Sprite(loadedTextures.at(item.getTextureID())), sf::Vector2f(16.0f, 16.0f));
            ImGui::EndDragDropSource();
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();
        i++;
    }

    

    ImGui::End();
}


bool UIManager::addContentWithCategory(std::vector<Content> &content, ContentCategory targetCat, std::string dropID)
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
				if (item.type == C_TEXTURE)
				{
                    ImGui::SetDragDropPayload(dropID.c_str(), &item.txID, sizeof(int));
				} else
				{
                    ImGui::SetDragDropPayload(dropID.c_str(), &item.cID, sizeof(int));
				}
                ImGui::Image(sf::Sprite(loadedTextures.at(item.txID)), sf::Vector2f(16.0f, 16.0f));
                ImGui::EndDragDropSource();
			}
            ImGui::Text(item.name.c_str());
            ImGui::EndGroup();
		}
	}

    return true;
}

void UIManager::addObjectToMap(CellGrid& grid,
    float& mapWidth, float& mapHeight,
    int& gridWidth, int& gridHeight,
    float& mapPosX, float& mapPosY,
    float cellSize
    )
{
    int i = 0;
	for (auto item : grid.getAllProps())
	{
        float x = (item.getPosX() * mapWidth) / gridWidth + mapPosX;
        float y = (item.getPosY() * mapHeight) / gridHeight + mapPosY;

        float size = cellSize / 2;

        ImGui::SetCursorPos(ImVec2(x - size / 2, y - size / 2));
        ImGui::BeginChild("Interact_prop" + char(i), ImVec2(size, size));
        if (&item == selectedProp)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255, 0, 255));
        } else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 255, 255));
        }

        if (ImGui::Button("-", ImVec2(size, size)))
        {
            selectedProp = grid.getProp(i);
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();
        i++;
	}
}

void UIManager::deselect()
{
    selectedCell = nullptr;
    selectedProp = nullptr;
}

void UIManager::addSelectedCellProperties(std::string name, std::string txTitle, int txCount, std::string txNames[])
{
    ImGui::Text(name.c_str());
    ImGui::Text("(%d, %d)", selectedCell->getPosX(), selectedCell->getPosY());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text(txTitle.c_str());
    addPropertiesEditTexture(txCount, txNames, selectedCell->m_textures);
}

void UIManager::addPropertiesEditTexture(int num, std::string names[], TextureID textures[])
{
    for (int i = 0; i < num; ++i)
    {
        if (i > 0) ImGui::SameLine();

        TextureID txID = textures[i];
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
                textures[i] = payload_n;
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::EndGroup();
    }
}

void UIManager::addContentTree(std::string name, ContentCategory cat, std::string dropID)
{
    std::string id = "C_CAT_" + name;
    if (ImGui::TreeNode(id.c_str(), name.c_str()))
    {
        addContentWithCategory(contentTextures, cat, dropID);
        ImGui::TreePop();
    }
}

void UIManager::addBrushProperties(std::string title, int txCount, std::string txNames[], TextureID tx[4])
{
    ImGui::Text(title.c_str());
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    ImGui::Text("Set brush textures:");
    addPropertiesEditTexture(txCount, txNames, tx);
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

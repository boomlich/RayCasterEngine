
#include <iostream>
#include <string>

#include "UI.h"
#include "ImGUI-1.88/imgui.h"

class UI_Pause : public UI
{
public:
	void update() override
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
};

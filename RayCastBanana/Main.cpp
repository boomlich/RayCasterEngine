#include "Controller.h"
#include "Renderer.h"



int main()
{

    sf::Texture test;

    const int resolutionX = 640;
    const int resolutionY = 480;
    Model model;
    Renderer view(resolutionX, resolutionY, &model);
    Controller controller(&model, &view);
}
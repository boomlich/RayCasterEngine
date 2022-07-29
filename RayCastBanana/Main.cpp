#include "MapParser.h"
#include "view.h"

int main()
{
    int resolutionX = 640;
    int resolutionY = 480;

    MapParser mapParser;
    View view(resolutionX, resolutionY);
    view.setGrid(mapParser.parse_img("resources/images/maps/map_01.png"));
    view.render_loop();
}
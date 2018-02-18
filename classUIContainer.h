#ifndef CLASS_UICONTAINER_H
#define CLASS_UICONTAINER_H
#include "classUIWindow.h"

class UIContainer
{
    std::vector<UIWindow> interfaceWindows;

public:
    void addWindow(sf::Vector2f position, float width, float height, bool fixedToWin, sf::Color color = {50,50,50,150});

    void renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView);

    UIWindow &getWindow(int windowIndex);
};


#endif // CLASS_UICONTAINER_H

#ifndef CLASS_UICONTAINER_H
#define CLASS_UICONTAINER_H
#include "classUIWindow.h"

class UIContainer
{
    std::vector<UIWindow> interfaceWindows;
    std::vector<bool> mouseIntersectionList;
    //std::vector<bool,int> buttonIntersectList;


public:
    void addWindow(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable = false, sf::Color color = {50,50,50,150});

    void renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView);
    void checkMouseIntersection(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView);
    std::pair<bool,int> doesMIntExist();

    UIWindow &getWindow(int windowIndex);
};


#endif // CLASS_UICONTAINER_H

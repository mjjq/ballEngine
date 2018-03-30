#ifndef CLASS_UICONTAINER_H
#define CLASS_UICONTAINER_H
#include "classUIWindow.h"

class UIContainer
{
    std::vector<UIWindow> interfaceWindows;
    std::vector<bool> mouseIntersectionList;
    std::pair<bool,int> currentIntButton;
    std::pair<bool,int> currentIntWindow;


public:
    void addWindow(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable = false, sf::Color color = {50,50,50,150});
    void renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView);
    UIWindow &getWindow(int windowIndex);

    void checkMouseIntersection(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView);
    std::pair<bool,int> doesMIntExist();

    void clickOnUI(sf::RenderWindow &window);
    void resetUIClick();

    bool isWindowDraggable();
    void dragWindow(sf::RenderWindow &window);


};


#endif // CLASS_UICONTAINER_H

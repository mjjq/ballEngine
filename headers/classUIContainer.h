#ifndef CLASS_UICONTAINER_H
#define CLASS_UICONTAINER_H
#include "classUIWindow.h"

class UIContainer
{
    static sf::RenderWindow &parentWindow;
    static sf::View &originalView;
    static sf::View &GUIView;

    std::vector<std::unique_ptr<UIWindow>> interfaceWindows;
    std::vector<int> interfaceWindowIDs;
    std::vector<bool> mouseIntersectionList;
    std::pair<bool,int> currentIntButton;
    std::pair<bool,int> currentIntWindow;


public:
    UIContainer();

    void addWindow(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable = false, sf::Color color = {50,50,50,150});
    void renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView);
    UIWindow &getWindow(unsigned int windowIndex);

    void checkMouseIntersection(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView);
    std::pair<bool,int> doesMIntExist();

    void clickOnUI(sf::RenderWindow &window);
    void resetUIClick();

    bool isWindowDraggable();
    void dragWindow(sf::RenderWindow &window);
    void destroyWindow(unsigned int index);
    void destroyAllWindows();

    static void setViewParameters(sf::RenderWindow &window, sf::View view1, sf::View view2);
};


#endif // CLASS_UICONTAINER_H

#ifndef CLASS_UICONTAINER_H
#define CLASS_UICONTAINER_H
#include "classUIWindow.h"
#include "structs.h"

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

    //void addWindow(WindowParams &params);

    //template <class T>
    void addWindow(CompleteWindow &compWindow);
    void addTextElType(TextElBaseParams &tParams);
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

#ifndef CLASS_UICONTAINER_H
#define CLASS_UICONTAINER_H
#include "classUIWindow.h"
#include "structs.h"
#include "../extern/json.hpp"

class UIContainer
{
    using json = nlohmann::json;

    static sf::RenderWindow &parentWindow;
    static sf::View &originalView;
    static sf::View &GUIView;

    std::vector<std::unique_ptr<UIWindow>> interfaceWindows;
    std::vector<int> interfaceWindowIDs;
    std::vector<bool> mouseIntersectionList;
    std::pair<bool,int> currentIntButton{false, -1};
    std::pair<bool,int> currentIntWindow{false, -1};

    bool canInteract = true;
    bool isVisible = true;
public:
    UIContainer(bool _isVisible);

    //void addWindow(WindowParams &params);

    //template <class T>
    void addWindow(CompleteWindow &compWindow);
    void addWindow(json &j,
                mapstrvoid &bFuncMap,
                mapstrvoidfloat &sFuncMap,
                std::map<std::string, std::function<std::string()> > &varMap);
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

    void setWindowIsVisible(int index, bool value);
    void setWindowCanInteract(int index, bool value);
};


#endif // CLASS_UICONTAINER_H

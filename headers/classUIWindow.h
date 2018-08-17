#ifndef CLASS_UIWIN_H
#define CLASS_UIWIN_H

#include "classTextElement.h"
//#include "classUISlider.h"
//#include "classUIButton.h"
#include "structs.h"

#include <functional>

class UIButton;
class UISlider;
class UIGroup;

class UIWindow
{
    sf::Vector2f normPosition{0.0f,0.0f};
    sf::Vector2f effOrigPos{0.0f,0.0f};

    bool canInteract = true;
    bool isVisible = true;
protected:
    std::pair<bool,int> mouseOnButtonWhenClicked{false, -1};
    std::pair<bool,int> mouseOnButton{false, -1};
    std::pair<bool,int> mouseOnGroupWhenClicked{false, -1};
    std::pair<bool,int> mouseOnGroup{false, -1};

    sf::Font currentFont;
    sf::Vector2i mouseOffset = {0,0};

    sf::Vector2f origPosition;
    float width;
    float height;
    //sf::Rect<float> origRect;//{effOrigPos, {width,height}};

    sf::Color color;
    sf::RectangleShape windowBox;

    std::vector<std::unique_ptr<UITextElement>> textArray;
    std::vector<std::unique_ptr<UIButton>> buttonArray;
    std::vector<std::unique_ptr<UIGroup>> groupArray;

    bool isButton = false;
    bool fixedToWindow = true;
    bool draggable = true;
    bool mouseIntersecting = false;

    void addGroup(WindowParams &wParams);

public:
    //static sf::Vector2u appWinSize;

    UIWindow(WindowParams &params);

    virtual ~UIWindow();

    void addElement(TextElParams &tParams);

    virtual void addButton(ButtonParams &bParams);

    virtual void addSlider(SliderParams &sParams);

    bool ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2);
    void renderWindow(sf::RenderWindow &window, sf::View &GUIView);
    void renderElements(sf::RenderWindow &window, sf::View &GUIView);

    void clickIntersectedButton(sf::RenderWindow &window);
    virtual void releaseClickedButton();

    virtual void checkMouseIntersection(sf::RenderWindow &window);
    bool getIsMouseIntersecting();
    void resetButtonPair();
    std::pair<bool,int> getClickedButton();
    virtual void changeOrigin(sf::RenderWindow &window, sf::Vector2i origin);
    void destroyAllElements();

    void moveWindow(sf::RenderWindow &window, sf::Vector2i newPosition);

    bool getFixedToWin();

    void setIsVisible(bool value);
    void setCanInteract(bool value);
};

#endif // CLASS_UIWIN_H

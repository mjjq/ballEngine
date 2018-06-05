#ifndef CLASS_UIWIN_H
#define CLASS_UIWIN_H

#include "classTextElement.h"
//#include "classUISlider.h"
//#include "classUIButton.h"

#include <functional>

class UIButton;
class UISlider;
class UIGroup;

class UIWindow
{
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
    sf::Rect<float> origRect{origPosition, {width,height}};

    sf::Color color;
    sf::RectangleShape windowBox;

    std::vector<UITextElementBase*> textArray;
    std::vector<UIButton*> buttonArray;
    std::vector<UIGroup*> groupArray;

    bool isButton = false;
    bool fixedToWindow = true;
    bool draggable = true;
    bool mouseIntersecting = false;

    void addGroup(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable);

public:
    UIWindow(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable = false, sf::Color color = {50,50,50,150});

    template<class T>
    void addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var = nullptr);

    virtual void addButton(std::string font, std::string text, int fontSize, sf::Vector2f position, sf::Vector2f bSize,
                           std::function<void()> const& func, sf::Color color = {80,80,80,150}, bool changeState = true);

    virtual void addSlider(sf::Vector2f position, float range, sf::Vector2f bSize,
                   sf::Vector2f physRange ={0,0}, std::function<void(float)> sliderFunc = nullptr, float *variable = nullptr);

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

    void moveWindow(sf::RenderWindow &window, sf::Vector2i newPosition);
};

#endif // CLASS_UIWIN_H

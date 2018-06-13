/**
    @class NonSimulationStuff
    Purpose: Handle all aspects of the program which are not involved in
    simulation. This includes event handling of user inputs, and functions
    to do with rendering.

    @author mjjq
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>
#include <numeric>

#include "../../headers/classMenuScene.h"
#include "../../headers/classTextElementBase.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"



/**
    If the mouse button is pressed, draws a line between position where
    mouse was clicked and the current position of the mouse.

    @param &initPos The click position of the mouse.
    @param mouseType Boolean which draws line if set to true.
*/
void MenuScene::checkMBPress(sf::Vector2i &initPos, bool mouseType)
{
    sf::Vector2i finalPos;
    sf::Vertex line[2];

    if((mouseType == true))
    {
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        finalPos = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));

        line[0] = sf::Vertex(static_cast<sf::Vector2f>(initPos),
                                              sf::Color::White);
        line[1] = sf::Vertex(static_cast<sf::Vector2f>(finalPos),
                                              sf::Color::White);
        window.draw(line, 2, sf::Lines);
    }
}


/**
    Resets the window view to a zoom of 1, centered on the simulation world
    centre.

    @return Void.
*/
void MenuScene::resetView()
{
    currentZoom = 1.0;
    window.setView(worldView);
}


/**
    Clicks on a User Interface window.

    @return Void.
*/
void MenuScene::clickOnUI()
{
    container.checkMouseIntersection(window, GUIView, worldView);
    mouseOnUIWhenClicked = container.doesMIntExist();
    container.clickOnUI(window);

}

/**
    Sets the current User Interface window to not be draggable.

    @return Void.
*/
void MenuScene::resetUIClick()
{
    container.resetUIClick();
    clickedWindowToDrag = false;
}


/**
    Function which handles all events to do with checking for UI interaction
    or adjusting the view.

    @param &event The event case to process.

    @return Void.
*/
void MenuScene::mouseViewEvents(sf::Event &event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        clickOnUI();
    }

    else if(event.type == sf::Event::MouseMoved)
    {
        container.checkMouseIntersection(window, GUIView, worldView);
        container.doesMIntExist();
    }

    else if(event.type == sf::Event::MouseButtonReleased)
    {
        resetUIClick();
    }

}


/**
    Function which handles all events to do with generating interactions with
    the User Interface.

    @param &event The event case to process.

    @return Void.
*/
void MenuScene::mouseUIEvents(sf::Event &event)
{
        if(event.type == sf::Event::MouseButtonPressed)
            clickedWindowToDrag = container.isWindowDraggable();
}




/**
    Function which handles general key based events under some combination of
    primary key presses e.g. ctrl, alt, shift.

    @param &newLayerKeys Vector of isKeyPressed boolean.
    @param &event The event case to process.

    @return Void.
*/
void MenuScene::newLayerEvent(std::vector<bool> &newLayerKeys, sf::Event &event)
{
    if(newLayerKeys[0]&&(!newLayerKeys[1]))
    {
    }
    else if(newLayerKeys[2])
    {
        //if(event.key.code == sf::Keyboard::Return)
        //    toggleFullScreen();
    }
}


/**
    Function which handles general key based events. Events are processed provided
    there are no key primary keys held. If these keys are held, the newLayerEvent
    events are processed instead.

    @param &event The event case to process.

    @return Void.
*/
void MenuScene::keyEvents(sf::Event &event)
{
    std::vector<bool> newLayerKeys = {sf::Keyboard::isKeyPressed(sf::Keyboard::LControl),
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::LShift),
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)};
    bool newLayerPressed = false;
    if(std::find(newLayerKeys.begin(), newLayerKeys.end(), true) != newLayerKeys.end())
        newLayerPressed = true;

    if(event.type == sf::Event::EventType::KeyPressed)
    {
        if(!newLayerPressed)
        {

        }

        newLayerEvent(newLayerKeys, event);
    }
}


/**
    Function which handles player key held cases.

    @param player The ball index which the player controls.

    @return Void.
*/
void MenuScene::playerKeysDown(int player)
{


}


/**
    Function which handles events on window resize.

    @param &event The event case to process.

    @return Void.
*/
void MenuScene::resizeEvents(sf::Event &event)
{
    if(event.type == sf::Event::Resized)
        adjustViewSize({event.size.width, event.size.height});//, currentZoom);
}


/**
    Scales the view to the window based on the current window and world sizes.
    This function ensure the world level zoom is preserved as well as accounting
    for vertical aspect ratios by checking for effective zooms in each direction.

    @param sizeX The x-direction size of the window.
    @param sizeY The y-direction size of the window.
    @param worldSizeX The x-direction size of the simulation world.
    @param worldSizeY The y-direction size of the simulation world.

    @return Void.
*/
void MenuScene::adjustViewSize(sf::Vector2u newSize)
{
    worldView.setSize(newSize.x, newSize.y);
    GUIView.setSize(newSize.x, newSize.y);

    window.setView(worldView);
}



void MenuScene::events(sf::Event &event)
{
    mouseViewEvents(event);

    if(mouseOnUIWhenClicked.first)
        mouseUIEvents(event);

    keyEvents(event);
    resizeEvents(event);
}

void MenuScene::load()
{
    std::vector<CompleteWindow> completeWindows;

    /*winPars.push_back( {{0,100}, {250, 250}, true, false} );
    winPars.push_back( {{0,0}, {250, 70}, true, false} );
    winPars.push_back( {{0,400}, {250, 50}, true, false} );
    winPars.push_back( {{0,500}, {250, 150}, true, false} );*/



    CompleteWindow window0;
    window0.wParams = {{0,200}, {250, 250}, true, false, {0,0,0,0}};
    window0.bParamsVec =  std::vector<ButtonParams>{
        {"./fonts/cour.ttf", "New Game", 12, {20,30}, {100,30}, [&]{requestScene(SceneEnum::SCENE_GAME);}},
        {"./fonts/cour.ttf", "Options", 12, {20,70}, {100,30}, [&]{}},
        {"./fonts/cour.ttf", "Exit", 12, {20,110}, {100,30}, [&]{window.close();}}
    };
    window0.sParamsVec = std::vector<SliderParams>{

    };
    window0.tParamsIntVec = std::vector<TextElParams<int>>{
        {"./fonts/cour.ttf", "ballEngine", 16, {10,00}, nullptr}
    };
    window0.tParamsFloatVec = std::vector<TextElParams<float>>{

    };
    window0.tParamsBoolVec = std::vector<TextElParams<bool>>{

    };
    completeWindows.push_back(window0);

    for(unsigned int i=0; i<completeWindows.size(); ++i)
       container.addWindow(completeWindows.at(i));

}

void MenuScene::unload()
{
    container.destroyAllWindows();
}

void MenuScene::redraw(sf::RenderWindow &window)
{
    container.renderWindows(window, GUIView, worldView);
}

void MenuScene::update(sf::RenderWindow &window)
{
    if(!mouseOnUIWhenClicked.first)
    {
    }
    if(clickedWindowToDrag)
        container.dragWindow(window);

    playerKeysDown(0);
}

MenuScene::MenuScene(sf::RenderWindow &_window,
                     sf::Time &_currentFTime, float &_currentFPS) : window{_window},
                     currentFrameTime{_currentFTime}, currentFPS{_currentFPS}
{

}



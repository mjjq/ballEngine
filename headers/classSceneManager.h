#ifndef CLASS_SCENEMAN_H
#define CLASS_SCENEMAN_H

#include "classScene.h"
#include "classSandboxScene.h"
#include "classSurvivalScene.h"
#include "classMainMenuScene.h"
#include "classPauseMenuScene.h"
#include "classUIContainer.h"
#include "classUIWindow.h"
#include "sceneEnums.h"
#include "classRenderer.h"

class SceneManager
{
    Renderer* sceneRenderer = new Renderer();

    SandboxScene sandbox{sceneRenderer->windowManager.getWindow(),
                         sceneRenderer->windowManager.getTargFrameTime(),
                         sceneRenderer->windowManager.getCurrFrameTime(),
                         sceneRenderer->windowManager.getCurrentFPS()};
    SurvivalScene survival{sceneRenderer->windowManager.getWindow(),
                           sceneRenderer->windowManager.getTargFrameTime(),
                           sceneRenderer->windowManager.getCurrFrameTime(),
                           sceneRenderer->windowManager.getCurrentFPS()};
    MainMenuScene menu{sceneRenderer->windowManager.getWindow()};
    PauseMenuScene pauseMenu{sceneRenderer->windowManager.getWindow()};
    Scene *currScene = &menu;
    Scene *gameScene = nullptr;

    SceneEnum thisSceneEnum = SceneEnum::LAST;
    SceneEnum nextSceneEnum = SceneEnum::LAST;

    void loadNextScene(SceneEnum nextScene);

public:
    SceneManager();
    void mainLoop();
};

#endif // CLASS_SCENEMAN_H

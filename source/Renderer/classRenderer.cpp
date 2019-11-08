#include "classRenderer.h"
#include <iostream>
#include <cmath>
#include "Math.h"
#include <SFML/OpenGL.hpp>

Renderer::Renderer()
{
    Renderable::renderSubject.addObserver(this);
    LightSource::renderSubject.addObserver(this);

    std::string lightingName = lightingEngine.getShaderName();
    resourceManager.loadShader(lightingName);
    lightingEngine.setShader(resourceManager.getShader(lightingName));
    lightingEngine.resizeTextures(windowManager.getWindowSize());
}

void Renderer::sortByZ(std::vector<Renderable* > & objects)
{
    for(int i=1; i<objects.size(); ++i)
        for(int j=i; j > 0 && objects[j]->getZPosition() < objects[j-1]->getZPosition(); --j)
            std::swap(objects[j], objects[j-1]);
}

void Renderer::redrawAll(sf::RenderWindow &window)
{
    sortByZ(renderObjects);
    lightingEngine.clearShadowTextures(windowManager.getWindow().getView());
    lightingEngine.generateShadowTextures(renderObjects);
    lightingEngine.displayShadowTextures();
    for(int i=0; i<(int)renderObjects.size(); ++i)
    {
        Renderable& renderObj = *renderObjects[i];

        if(renderObj.shader != nullptr)
        {
            sf::Shader* shader = renderObj.shader;
            Material& material = renderObj.material;

            float rotation = Math::PI * renderObj.getRotation() / 180.0f;
            shader->setUniform("rotCosine", (float)cos(rotation));
            shader->setUniform("rotSine", (float)sin(rotation));
            shader->setUniform("material.diffuseMap", *renderObj.diffuseMap);
            shader->setUniform("material.normalMap", *renderObj.normalMap);
            shader->setUniform("material.emissionMap", *renderObj.emissionMap);
            shader->setUniform("material.diffuseStrength", material.diffuseStrength);
            shader->setUniform("material.ambientStrength", material.ambientStrength);
            shader->setUniform("material.specularStrength", material.specularStrength);
            shader->setUniform("material.emissionStrength", material.emissionStrength);
            shader->setUniform("material.shininess", material.shininess);
            shader->setUniform("zPosition", renderObj.zPosition);
            shader->setUniform("windowSize", (sf::Vector2f)window.getSize());
            shader->setUniform("viewCentre", (sf::Vector2f)window.getView().getCenter());
        }

        window.draw(renderObj.verts, sf::RenderStates(sf::BlendMode(),
                                                              renderObj.getTransform(),
                                                              nullptr,
                                                              renderObj.shader));
    }

}

void Renderer::onNotify(Component& entity, Event event, Container* data)
{
    switch(event.type)
    {
        case(EventType::New_Renderable):
        {
            Renderable& ren = (Renderable&)entity;
            resourceManager.assignResources(ren);

            renderObjects.push_back(&ren);
            break;
        }
        case(EventType::Delete_Renderable):
        {
            Renderable* obj = (Renderable*)&entity;
            for(int i=0; i<(int)renderObjects.size(); ++i)
                if(renderObjects[i] == obj)
                    renderObjects.erase(renderObjects.begin() + i);
            break;
        }
        case(EventType::New_LightSrc):
        {
            LightSource* lSrc = (LightSource*)&entity;
            lightingEngine.addLightSource(lSrc);
            break;
        }
        case(EventType::Delete_LightSrc):
        {
            LightSource* lSrc = (LightSource*)&entity;
            lightingEngine.removeLightSource(lSrc);
            break;
        }
        default:
            break;
    }
}

void Renderer::resizeWindow(sf::Vector2u newSize)
{
    lightingEngine.resizeTextures(newSize);
}

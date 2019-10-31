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
        if(renderObjects[i]->shader != nullptr)
        {
            sf::Shader* shader = renderObjects[i]->shader;

            float rotation = Math::PI * renderObjects[i]->getRotation() / 180.0f;
            shader->setUniform("rotCosine", (float)cos(rotation));
            shader->setUniform("rotSine", (float)sin(rotation));
            shader->setUniform("material.diffuseMap", *renderObjects[i]->diffuseMap);
            shader->setUniform("material.normalMap", *renderObjects[i]->normalMap);
            shader->setUniform("material.emissionMap", *renderObjects[i]->emissionMap);
            shader->setUniform("material.diffuseStrength", renderObjects[i]->material.diffuseStrength);
            shader->setUniform("material.ambientStrength", renderObjects[i]->material.ambientStrength);
            shader->setUniform("material.specularStrength", renderObjects[i]->material.specularStrength);
            shader->setUniform("material.emissionStrength", renderObjects[i]->material.emissionStrength);
            shader->setUniform("material.shininess", renderObjects[i]->material.shininess);
        }

        window.draw(renderObjects[i]->verts, sf::RenderStates(sf::BlendMode(),
                                                              renderObjects[i]->getTransform(),
                                                              nullptr,
                                                              renderObjects[i]->shader));
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

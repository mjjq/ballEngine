#ifndef CLASS_LIGHTING_ENGINE_H
#define CLASS_LIGHTING_ENGINE_H

#include <SFML/Graphics.hpp>
#include "classRenderable.h"
#include "classLight.h"

class LightingEngine
{
    sf::Shader* umbralShader;
    const std::string umbralShaderName = "umbralgen";

    sf::RenderTexture blankShadowTexture;
    std::vector<sf::RenderTexture* > shadowTextures;
    std::vector<LightSource* > lights;
    sf::Vector2u shadowTextureSize;

    void shadowStencil(LightSource &lightSource,
                       sf::Shape &shape,
                       sf::RenderTexture &shadowTexture);
    bool isShapeWithinRange(sf::Shape &shape, LightSource& lightSource);
public:
    LightingEngine();

    void clearShadowTextures(sf::View const & view);
    void displayShadowTextures();
    void generateShadowTextures(std::vector<Renderable* > const & _renderObjects);
    void resizeTextures(sf::Vector2u const & size);

    void addLightSource(LightSource* light);
    void removeLightSource(LightSource* light);

    void setShader(sf::Shader *_shader);
    const std::string getShaderName();
    void setObjectShaderLightProps(sf::Shader& objectShader);
};

#endif // CLASS_LIGHTING_ENGINE_H

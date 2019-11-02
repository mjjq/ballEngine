#include "LevelCreator.h"
#include "Math.h"
#include "classPolygonTriangulator.h"

void LevelCreator::generateLevelAssets(std::string const & imageName,
                            lvlCreator::Parameters const & params)
{
    sf::Image image;
    image.loadFromFile(imageName + ".png");
    generateCollisionAssets(image, params);
    generateVisualAssets(imageName, params);
}

void LevelCreator::generateVisualAssets(std::string const & imageName,
                               lvlCreator::Parameters const & params)
{
    ObjectProperties props;
    props.type = ObjectType::Polygon;
    props._isStatic = true;
    props._vertices = {
        {{0.0f, 0.0f}},
        {{params.targetSize.x, 0.0f}},
        {{params.targetSize.x, params.targetSize.y}},
        {{0.0f, params.targetSize.y}}
    };
    props.material.diffuseID = imageName + "_d.png";
    props._position = params.targetSize / 2.0f;

    //GameObject* gObj = new GameObject(new Renderable(props));
}

void LevelCreator::generateCollisionAssets(sf::Image const & image,
                               lvlCreator::Parameters const & params)
{
    VertsFromBmpWrap vbmp;

    if(params.useTargetSize)
        vbmp.setScale({(float)params.targetSize.x/(float)image.getSize().x,
                       (float)params.targetSize.y/(float)image.getSize().y});

    vbmp.setAverageSpacing(params.minVertexDistance);

    std::vector<lvlCreator::Poly > polys = vbmp.generateIslands(image);

    ObjectProperties props;
    props.type = ObjectType::ConcavePoly;
    props._isStatic = true;
    props._size.x = 10.0f;
    props._size.y = 20.0f;
    props._vertices = std::vector<sf::Vertex >(2);

    for(auto const & poly : polys)
    {
        props._position = Math::average(poly);
        props._vertices = poly;
        GameObject* gObj = new GameObject(new Renderable(props),
                                          new Polygon(props));
    }
}

#include "LevelCreator.h"
#include "Math.h"

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

    GameObject* gObj = new GameObject(new Renderable(props));
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
    props.type = ObjectType::Capsule;
    props._isStatic = true;
    props._size.x = 10.0f;
    props._size.y = 20.0f;
    props._vertices = std::vector<sf::Vertex >(2);

    /*for(auto const & poly : polys)
    {
        props._position = Math::average(poly);
        props._vertices = poly;
        GameObject* gObj = new GameObject(new Renderable(props),
                                          new Polygon(props));
    }*/

    for(auto const & poly : polys)
    {
        for(int i=0; i<poly.size()-1; ++i)
        {
            props._vertices.at(0) = (poly[i]);
            props._vertices.at(1) = (poly[i+1]);
            std::cout << poly[i+1].position.x << " polyx\n";
            std::cout << poly[i+1].position.y << " polyy\n";
            sf::Vector2f direction = poly[i+1].position - poly[i].position;

            props._rotation = 90.0f + 180.0f * atan2(direction.y, direction.x) / Math::PI;
            std::cout << props._rotation << " rotation\n";
            props._size.y = sqrtf(Math::square(direction));

            props._position = Math::average(props._vertices);

            GameObject* gObj = new GameObject(nullptr,//new Renderable(props),
                                          new Polygon(props));
        }
    }
}

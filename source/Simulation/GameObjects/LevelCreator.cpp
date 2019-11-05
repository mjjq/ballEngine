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
    //props.material.diffuseID = "testLevel_d.png";
    props.useCustomTexCoords = true;

    for(auto const & poly : polys)
    {
        props._position = Math::average(poly);
        props._vertices = poly;

        ObjectProperties renderProps(props);
        renderProps.type = ObjectType::VertexArray;
        renderProps._vertices = PolygonTriangulator::convertArray(PolygonTriangulator::triangulateCCWVertexList(poly));

        for(auto & vert : renderProps._vertices)
        {
            vert.texCoords.x = vert.position.x / params.targetSize.x;
            vert.texCoords.y = vert.position.y / params.targetSize.y;
        }

        renderProps._position = Math::average(renderProps._vertices);
        renderProps.vArrayType = sf::Triangles;

        GameObject* gObj = new GameObject(new Renderable(renderProps),
                                          new Polygon(props));
    }
}

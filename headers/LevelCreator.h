#ifndef LEVELCREATOR_H
#define LEVELCREATOR_H

#include "classGameObject.h"
#include "VertFromBitmapWrap.h"
#include "classPolygon.h"

namespace lvlCreator
{
    typedef std::vector<sf::Vertex > Poly;

    struct Parameters
    {
        bool useTargetSize = false;
        sf::Vector2f targetSize = {500.0f, 500.0f};
        float minVertexDistance = 50.0f;
    };
}

class LevelCreator
{
public:

    static void createLevel(std::string const & imageName,
                            lvlCreator::Parameters const & params)
    {
        sf::Image image;
        image.loadFromFile(imageName);
        createLevel(image, params);
    }

    static void createLevel(sf::Image const & image,
                            lvlCreator::Parameters const & params);
};

#endif // LEVELCREATOR_H

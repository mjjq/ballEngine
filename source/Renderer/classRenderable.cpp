#include "classRenderable.h"

#include <cmath>
#include "sfVectorMath.h"

Renderable::Renderable(std::string _texID,
               std::vector<sf::Drawable* > _primitives) : Entity()
{
    textureID = _texID;
    //primitives = _primitives;
    renderSubject.notify(*this, Event(EventType::New_Renderable));
}

Renderable::Renderable(std::string _texID,
               ObjectProperties objProps)
{
    textureID = _texID;
    generateDrawables(objProps);
    renderSubject.notify(*this, Event(EventType::New_Renderable));
}

void Renderable::generateDrawables(ObjectProperties objProps)
{
    switch(objProps.type)
    {
        case(ObjectType::Ball):
        {
            float radius = sqrt(sfVectorMath::square(objProps._size));

            sf::CircleShape* circle = new sf::CircleShape(radius);
            circle->setPosition(objProps._position);
            circle->setOrigin(radius,radius);
            circle->setFillColor(sf::Color{180,180,180,100});
            primDrawable = std::move(circle);
            primTransformable = circle;
            /*sf::VertexArray* line = new sf::VertexArray(sf::Lines, 2);

            line->append(sf::Vertex(objProps._position));
            line->append(sf::Vertex(objProps._position +
                sfVectorMath::rotate(sf::Vector2f{0.0f, radius},
                                     objProps._rotation)));*/

            //sf::RectangleShape* rect = new sf::RectangleShape({0.0, radius});

            //primitives.push_back(std::move(drawable));
            //primitives.push_back(std::move(line));
            break;
        }
        default:
            break;
    }
}

void Renderable::updatePosition(sf::Vector2f position)
{
    if(primTransformable != nullptr)
        primTransformable->setPosition(position);
}
void Renderable::updateOrientation(float angle)
{
    if(primTransformable != nullptr)
        primTransformable->setRotation(angle);
}

Subject Renderable::renderSubject;

const std::string Renderable::NULL_TEXTURE = "";

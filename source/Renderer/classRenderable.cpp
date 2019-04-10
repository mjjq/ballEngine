#include "classRenderable.h"

#include <cmath>
#include "Math.h"

Renderable::Renderable(std::string _texID,
               std::vector<sf::Drawable* > _primitives) : Entity()
{
    material.diffuseID = _texID;
    //primitives = _primitives;
    renderSubject.notify(*this, Event(EventType::New_Renderable));
}

Renderable::Renderable(ObjectProperties objProps)
{
    material = objProps.material;
    generateDrawables(objProps);
    //primShape->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(1, 1)));
    renderSubject.notify(*this, Event(EventType::New_Renderable));
}

Renderable::~Renderable()
{
    renderSubject.notify(*this, Event(EventType::Delete_Renderable));
}

void Renderable::generateDrawables(ObjectProperties objProps)
{
    switch(objProps.type)
    {
        case(ObjectType::Ball):
        {
            float radius = sqrt(Math::square(objProps._size));

            sf::CircleShape* circle = new sf::CircleShape(radius);
            circle->setPosition(objProps._position);
            circle->setOrigin(radius,radius);
            circle->setFillColor(sf::Color{180,180,180,100});
            primDrawable = std::move(circle);
            primTransformable = circle;
            primShape = circle;
            break;
        }
        case(ObjectType::Polygon):
        {
            sf::ConvexShape* shape = new sf::ConvexShape(objProps._vertices.size());
            shape->setPosition(objProps._position);
            shape->setOutlineThickness(-2);
            shape->setOutlineColor(sf::Color::Red);
            shape->setFillColor({80,80,80,80});
            for(int i=0; i<(int)objProps._vertices.size(); ++i)
                shape->setPoint(i, objProps._vertices[i].position);
            shape->setOrigin(Math::average(objProps._vertices));

            primDrawable = std::move(shape);
            primTransformable = shape;
            primShape = shape;
            break;
        }
        case(ObjectType::Capsule):
        {
            float radius = objProps._size.x;
            float length = objProps._size.y;

            sf::RectangleShape* shape = new sf::RectangleShape({2.0f*radius, length});
            shape->setOrigin({radius, length/2.0f});
            shape->setPosition(objProps._position);

            primDrawable = std::move(shape);
            primTransformable = shape;
            primShape = shape;
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

const std::string Renderable::NULL_ID = "";

#include "classRenderable.h"

#include <cmath>
#include "Math.h"

#include "classConcavePolygonWrap.h"
#include "classPolygonTriangulator.h"

Renderable::Renderable(ObjectProperties objProps)
{
    material = objProps.material;
    if(material.diffuseID == "") material.diffuseID = "null.jpg";
    if(material.shaderID == "") material.shaderID = "default.frag";
    generateDrawables(objProps);
    //primShape->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(1, 1)));
    renderSubject.notify(*this, Event(EventType::New_Renderable));
    zPosition = objProps._zPosition;
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

            verts.append(sf::Vertex({0.0f, 0.0f}));
            int numSides = 24;
            for(int i=0; i<numSides+1; ++i)
            {
                float phase = 2.0f*Math::PI*(float)i/(float)numSides;
                sf::Vector2f relPos = radius * sf::Vector2f{cosf(phase), sinf(phase)};
                verts.append(sf::Vertex{relPos});
            }
            verts.setPrimitiveType(sf::TriangleFan);
            break;
        }
        case(ObjectType::Polygon):
        {
            for(int i=0; i<objProps._vertices.size(); ++i)
            {
                verts.append(objProps._vertices[i]);
            }

            verts.setPrimitiveType(sf::TriangleFan);
            break;
        }
        case(ObjectType::ConcavePoly):
        {
            verts = PolygonTriangulator::triangulateCCWVertexList(objProps._vertices);
            verts.setPrimitiveType(sf::Triangles);
            break;
        }
        case(ObjectType::Capsule):
        {
            float radius = objProps._size.x;
            float length = objProps._size.y;

            verts.append(sf::Vertex({0.0f, 0.0f}));
            int numSides = 24;
            for(int i=0; i<numSides+1; ++i)
            {
                sf::Vector2f offset = sf::Vector2f{0.0f, length/2.0f};
                if(i>numSides/2) offset.y *= -1.0f;

                float phase = 2.0f*Math::PI*(float)i/(float)numSides;
                sf::Vector2f relPos = offset + radius * sf::Vector2f{cosf(phase), sinf(phase)};


                verts.append(sf::Vertex{relPos});
                if(i==numSides/2)
                {
                    relPos = relPos - 2.0f*offset;
                    verts.append(sf::Vertex{relPos});
                }

            }
            verts.append(sf::Vertex({radius, length/2.0f}));
            verts.setPrimitiveType(sf::TriangleFan);
            break;
        }
        case(ObjectType::VertexArray):
        {
            verts = sf::VertexArray(objProps.vArrayType, objProps.vArrayCount);
        }
        default:
            break;
    }

    setOrigin(Math::average(objProps._vertices));

    setPosition(objProps._position);
    setRotation(objProps._rotation);
    sf::Vector2f boundingRect = {verts.getBounds().left, verts.getBounds().top};
    sf::Vector2f boundsSize = {verts.getBounds().width, verts.getBounds().height};

    for(int i=0; i<verts.getVertexCount(); ++i)
    {
        verts[i].texCoords = verts[i].position - boundingRect;
        verts[i].texCoords.x /= boundsSize.x;
        verts[i].texCoords.y /= boundsSize.y;
    }
}

Subject Renderable::renderSubject;

const std::string Renderable::NULL_ID = "";

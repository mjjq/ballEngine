#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H

#include "classConcavePolygonWrap.h"

class PolygonTriangulator
{
public:
    static std::vector<sf::Vertex > convertArray(sf::VertexArray const & input)
    {
        std::vector<sf::Vertex > returnArray;
        returnArray.reserve(input.getVertexCount());

        for(int i=0; i<input.getVertexCount(); ++i)
        {
            returnArray.emplace_back(input[i]);
        }

        return returnArray;
    }

    static sf::VertexArray triangulateConcavePolygon(ConcavePolygonWrap & concavePoly)
    {
        sf::VertexArray returnArray;
        returnArray.setPrimitiveType(sf::Triangles);

        int numConvexPolys = concavePoly.getConvexPolyCount();

        for(int i=0; i<numConvexPolys; ++i)
        {
            ConcavePolygonWrap & convexPoly = concavePoly.getConvexPoly(i);

            std::vector<sf::Vertex > convexVerts = convexPoly.getVertices(false);

            for(int j=1; j<convexVerts.size(); ++j)
            {
                returnArray.append(convexVerts[0]);
                returnArray.append(convexVerts[j]);
                returnArray.append(convexVerts[Math::modulo(j+1, convexVerts.size())]);
            }
        }

        return returnArray;
    }

    static sf::VertexArray triangulateCCWVertexList(std::vector<sf::Vertex > const & verts)
    {
        ConcavePolygonWrap poly(verts);

        return triangulateConcavePolygon(poly);
    }
};

#endif // TRIANGULATOR_H

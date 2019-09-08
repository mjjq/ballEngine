#ifndef VERTFROMBMPWRAP_H
#define VERTFROMBMPWRAP_H

#include "VerticesFromBitmap.hpp"


class VertsFromBmpWrap
{
    vfb::VerticesFromBitmap vbmp;

    typedef std::vector<sf::Vertex > PolyIsland;

    std::vector<PolyIsland > convertToWrapped(std::vector<vfb::Island > const & _island)
    {
        std::vector<PolyIsland > toReturn;

        for(vfb::Island const & island : _island)
        {
            PolyIsland tempPolyIsland;
            for(int i=0; i<island.vertexPositions.size(); ++i)
            {
                vfb::Vec2 position = island.vertexPositions[i];
                tempPolyIsland.push_back({{(float)position.x, (float)position.y}});
            }

            if(tempPolyIsland.size() > 0)
                toReturn.push_back(tempPolyIsland);
        }

        return toReturn;
    }
public:
    std::vector<PolyIsland > generateIslands(sf::Image const & bitmap)
    {
        std::vector<vfb::Island > result = vbmp.generateIslands(bitmap);

        std::vector<PolyIsland > toReturn = convertToWrapped(result);

        return toReturn;
    }

    float getAverageSpacing() { return vbmp.getAverageSpacing(); }

    void setAverageSpacing(float spacing)
    {
        vbmp.setAverageSpacing(spacing);
    }

    float getColinearThreshold() { return vbmp.getColinearThreshold(); }

    void setColinearThreshold(float CLT)
    {
        vbmp.setColinearThreshold(CLT);
    }

    float getExtremeColinearThreshold() { return vbmp.getExtremeColinearThreshold(); }

    void setExtremeColinearThreshold(float CLT)
    {
        vbmp.setExtremeColinearThreshold(CLT);
    }

    float getPixelThreshold() { return vbmp.getPixelThreshold(); }

    void setPixelThreshold(float pixelThreshold)
    {
        vbmp.setPixelThreshold(pixelThreshold);
    }

    void setScale(sf::Vector2f const & _scale)
    {
        vbmp.setScale(vfb::Vec2{_scale.x, _scale.y});
    }

    void setScaleTo(sf::Vector2f const & _scaleTo)
    {

    }
};

#endif // VERTFROMBMPWRAP_H

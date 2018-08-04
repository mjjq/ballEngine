#include <iostream>
#include "../../headers/class2DMatrix.h"
#include <tuple>
#include <algorithm>

float Matrix2d::getElementValue(int x, int y)
{
    return matrix.at(x + y*width);
}

void Matrix2d::setElementValue(int x, int y, float value)
{
    matrix.at(x + y*width) = value;
}

void Matrix2d::insertColumn(int position, float initVal)
{
    if(height == 0)
    {
        matrix.push_back(initVal);
        height++;
    }
    else
    {
        int j=0;
        for(unsigned int i=0; i<height-1; ++i)
        {
            matrix.insert(matrix.begin() + i*width + width + j, initVal);
            j++;
        }
        matrix.push_back(initVal);
        /*for(unsigned int i=0; i<height; ++i)
        {
            matrix.push_back(initVal);
        }*/
    }
    width++;
}

/*void Matrix2d::removeColumn(int position)
{
    if(position >= 0 && std::abs(position) < matrix.size())
        matrix.erase(matrix.begin() + position);
    else if(position < 0 && std::abs(position) < matrix.size())
        matrix.erase(matrix.end() + position);
    else
        matrix.erase(matrix.end() - 1);

    width--;
}*/

void Matrix2d::insertRow(int position, float initVal)
{
    if(width == 0)
    {
        matrix.push_back(initVal);
        width++;
    }
    else
    {
        for(unsigned int i=0; i<width; ++i)
            matrix.push_back(initVal);
    }

    height++;
}

/*void Matrix2d::removeRow(int position)
{
    if(position >= 0 && std::abs(position) < height)
        for(unsigned int i=0; i<width; ++i)
        {
            matrix.at(i).erase(matrix.at(i).begin() + position);
        }
    else if(position < 0 && std::abs(position) < height)
        for(unsigned int i=0; i<width; ++i)
        {
            matrix.at(i).erase(matrix.at(i).end() + position);
        }
    else
        for(unsigned int i=0; i<width; ++i)
        {
            matrix.at(i).erase(matrix.at(i).end() -1);
        }
    height--;
}*/

std::tuple<int, int, float> Matrix2d::getMatrixMin()
{
    unsigned int x=0;
    unsigned int y=0;
    float minimum = 1e+15;

    /*for(unsigned int i=0; i<height; ++i)
        for(unsigned int j=0; j<width; ++j)
        {
            float temp = matrix.at(i*width+j);
            if(temp < minimum)
            {
                x=i;
                y=j;
                minimum = temp;
            }
        }*/
    for(unsigned int i=0; i<matrix.size(); ++i)
    {
        float temp = matrix.at(i);
        if(temp < minimum)
        {
            x = i % width;
            y = (i-x)/width;
            minimum = temp;
        }
    }
    return std::make_tuple(x,y,minimum);
}

void Matrix2d::printMatrix()
{
    for(unsigned int i=0; i<height; ++i)
    {
        for(unsigned int j=0; j<width; ++j)
        {
            std::cout << matrix.at(i*width+j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "width: " << width << ", height: " << height << "\n\n";
}

void Matrix2d::addConstValue(float value)
{
    for(unsigned int i=0; i<matrix.size(); ++i)
        matrix.at(i) += value;
    //use .at(i) for debug if testing for out of range errors
}

void Matrix2d::clearMatrix()
{
    matrix.clear();
    width = 0;
    height = 0;
}


unsigned int Matrix2d::getWidth()
{
    return width;
}

unsigned int Matrix2d::getHeight()
{
    return height;
}

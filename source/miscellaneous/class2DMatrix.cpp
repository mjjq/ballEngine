#include <iostream>
#include "../../headers/class2DMatrix.h"
#include <tuple>
#include <algorithm>
#include <cmath>

float Matrix2d::getElementValue(int x, int y)
{
    return matrix[x + y*width];
}

void Matrix2d::setElementValue(int x, int y, float value)
{
    matrix[x + y*width] = value;
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
    }
    width++;
}

void Matrix2d::insertColumnQuick(float initVal)
{
    if(height!=0)
    {
        for(unsigned int i=0; i<height; ++i)
            matrix.push_back(initVal);
        fillMatrix(initVal);
    }
    width++;
}

void Matrix2d::removeColumnQuick(float initVal)
{
    if(width > 0)
    {
        for(unsigned int i=0; i<height; ++i)
            matrix.pop_back();
        fillMatrix(initVal);
        width--;
    }
}

void Matrix2d::insertRow(int position, float initVal)
{
    if(width!=0)
    {
        for(unsigned int i=0; i<width; ++i)
            matrix.push_back(initVal);
    }

    height++;
}

void Matrix2d::removeEndRow()
{
    if(height > 0)
    {
        for(unsigned int i=0; i<width; ++i)
            matrix.pop_back();
        height--;
    }
}

std::tuple<int, int, float> Matrix2d::getMatrixMin()
{
    unsigned int x=0;
    unsigned int y=0;
    float minimum = 1e+15;

    for(unsigned int i=0; i<matrix.size(); ++i)
    {
        float temp = matrix[i];
        if(!std::isnan(temp) && temp < minimum)
        {
            x = i % width;
            y = (i-x)/width;
            minimum = temp;
        }
    }
    return std::make_tuple(x,y,minimum);
}

/*std::tuple<int, int, float> Matrix2d::getMatrixMin()
{
    unsigned int x=0;
    unsigned int y=0;
    float minimum = 1e+15;
    int i=0;

    for(auto &value : matrix)
    {
        if(!std::isnan(value) && value < minimum)
        {
            x = i % width;
            y = (i-x)/width;
            minimum = value;
        }
        i++;
    }
    return std::make_tuple(x,y,minimum);
}*/

/*float Matrix2d::getMatrixMin(int &x, int &y)
{
    float minimum = 1e+15;

    for(unsigned int i=0; i<matrix.size(); ++i)
    {
        float temp = matrix[i];
        if(!std::isnan(temp) && temp < minimum)
        {
            x = i % width;
            y = (i-x)/width;
            minimum = temp;
        }
    }
    return minimum;
}*/

float Matrix2d::getMatrixMin(int &x, int &y)
{
    float minimum = 1e+15;
    int i=0;

    for(auto &value : matrix)
    {
        if(!std::isnan(value) && value < minimum)
        {
            x = i % width;
            y = (i-x)/width;
            minimum = value;
        }
        i++;
    }
    return minimum;
}

void Matrix2d::printMatrix()
{
    for(unsigned int i=0; i<height; ++i)
    {
        for(unsigned int j=0; j<width; ++j)
        {
            std::cout << matrix[i*width+j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "width: " << width << ", height: " << height << "\n\n";
}

void Matrix2d::addConstValue(float value)
{
    for(auto &element : matrix)
        if(!std::isnan(element))
            element += value;
    //use .at(i) for debug if testing for out of range errors
}

void Matrix2d::clearMatrix()
{
    matrix.clear();
    width = 0;
    height = 0;
}

void Matrix2d::fillMatrix(float value)
{
    std::fill(matrix.begin(), matrix.end(), value);
}


unsigned int Matrix2d::getWidth()
{
    return width;
}

unsigned int Matrix2d::getHeight()
{
    return height;
}

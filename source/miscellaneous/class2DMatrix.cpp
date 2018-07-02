#include <iostream>
#include "../../headers/class2DMatrix.h"
#include <tuple>

float Matrix2d::getElementValue(int x, int y)
{
    return matrix.at(x).at(y);
}

void Matrix2d::setElementValue(int x, int y, float value)
{
    matrix.at(x).at(y) = value;
}

void Matrix2d::insertColumn(int position)
{
    if(matrix.size()>0)
    {
        if(position >= 0 && std::abs(position) < matrix.size())
            matrix.insert(matrix.begin() + position, std::vector<float>(height));
        else if(position < 0 && std::abs(position) < matrix.size())
            matrix.insert(matrix.end() + position, std::vector<float>(height));
        else
            matrix.insert(matrix.end(), std::vector<float>(height));
    }
    else
        matrix.push_back(std::vector<float>(height));

    width++;
}

void Matrix2d::removeColumn(int position)
{
    if(position >= 0 && std::abs(position) < matrix.size())
        matrix.erase(matrix.begin() + position);
    else if(position < 0 && std::abs(position) < matrix.size())
        matrix.erase(matrix.end() + position);
    else
        matrix.erase(matrix.end() - 1);

    width--;
}

void Matrix2d::insertRow(int position, float initVal)
{
    if(height>0)
    {
        if(position >= 0 && std::abs(position) < height)
            for(unsigned int i=0; i<width; ++i)
            {
                matrix.at(i).insert(matrix.at(i).begin() + position, initVal);
            }
        else if(position < 0 && std::abs(position) < height)
            for(unsigned int i=0; i<width; ++i)
            {
                matrix.at(i).insert(matrix.at(i).end() + position, initVal);
            }
        else
            for(unsigned int i=0; i<width; ++i)
            {
                matrix.at(i).insert(matrix.at(i).end() -1, initVal);
            }
    }
    else
        for(unsigned int i=0; i<width; ++i)
        {
            matrix.at(i).push_back(initVal);
        }

    height++;
}

void Matrix2d::removeRow(int position)
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
}

std::tuple<int, int, float> Matrix2d::getMatrixMin()
{
    unsigned int x=0;
    unsigned int y=0;
    float minimum = 1e+15;

    for(unsigned int i=0; i<width; ++i)
        for(unsigned int j=0; j<height; ++j)
        {
            float temp = matrix.at(i).at(j);
            if(temp < minimum)
            {
                x=i;
                y=j;
                minimum = temp;
            }
        }
    return std::make_tuple(x,y,minimum);
}

void Matrix2d::printMatrix()
{
    for(unsigned int i=0; i<width; ++i)
    {
        for(unsigned int j=0; j<height; ++j)
        {
            std::cout << matrix.at(i).at(j) << " ";
        }
        std::cout << "\n";
    }
}

void Matrix2d::addConstValue(float value)
{
    for(unsigned int i=0; i<width; ++i)
    {
        for(unsigned int j=0; j<height; ++j)
            matrix.at(i).at(j) += value;
    }
}

void Matrix2d::clearMatrix()
{
    matrix.clear();
    width = 0;
    height = 0;
}

#ifndef CLASS_2DMATRIX_H
#define CLASS_2DMATRIX_H
#include <iostream>
#include <vector>

class Matrix2d
{
    std::vector< float > matrix;
    unsigned int width = 0;
    unsigned int height = 0;

public:
    float getElementValue(int x, int y);
    void setElementValue(int x, int y, float value);
    void insertColumn(int position, float initVal = 0.0f);
    void insertColumnQuick(float initVal = 0.0f);
    //void removeColumn(int position);
    void insertRow(int position, float initVal = 0.0f);
    //void removeRow(int position);
    std::tuple<int, int, float> getMatrixMin();
    float getMatrixMin(int &x, int &y);
    void clearMatrix();
    void fillMatrix(float value);
    void printMatrix();
    void addConstValue(float value);
    unsigned int getWidth();
    unsigned int getHeight();
};

#endif // CLASS_2DMATRIX_H

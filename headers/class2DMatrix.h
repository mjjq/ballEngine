#ifndef CLASS_2DMATRIX_H
#define CLASS_2DMATRIX_H
#include <iostream>
#include <vector>

class Matrix2d
{
    std::vector< std::vector<float> > matrix;
    unsigned int width = 0;
    unsigned int height = 0;

public:
    float getElementValue(int x, int y);
    void setElementValue(int x, int y, float value);
    void insertColumn(int position);
    void removeColumn(int position);
    void insertRow(int position, float initVal = 0.0f);
    void removeRow(int position);
    std::tuple<int, int, float> getMatrixMin();
    void clearMatrix();
    void printMatrix();
    void addConstValue(float value);
};

#endif // CLASS_2DMATRIX_H

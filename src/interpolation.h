#pragma once
#include <algorithm>
#include <vector>

#include "modelclass.h"

typedef double (*fptr)(double);

class Interpolation
{
public:
    Interpolation();
    ~Interpolation();

    void AddPoint(float x, float y, float z);
    void DeletePoint(int);
    void Calculate();
    double F(double);

    DirectX::XMFLOAT3 GetPoint(int) const;
    int GetListCount() const;

    Interpolation* Get();

private:
    int Factorial(int);

private:
    int m_n;
    double* m_f;

    std::vector<VertexType*> m_vertexList;
};

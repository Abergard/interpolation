#pragma once

#include "interpolation.h"
#include "modelclass.h"

class PolygonClass : public ModelClass
{
public:
    PolygonClass();
    PolygonClass(const PolygonClass&);
    ~PolygonClass();

    void SetInterpolation(Interpolation*);
    void SetProperty(int, int, int);

private:
    virtual bool InitializeBuffers(ID3D11Device*);
    virtual void RenderBuffers(ID3D11DeviceContext*);

    void DrawFunction(VertexType*, unsigned long*);

private:
    int m_size;
    Interpolation* m_Interpolation;
};

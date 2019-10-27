#pragma once

#include <algorithm>
#include <vector>

#include "modelclass.h"

class PointClass : public ModelClass
{
public:
    PointClass();
    PointClass(const PointClass&);
    ~PointClass();

    void AddPoint(float, float, float);
    void DeletePoint(int);

    bool IsRendered();
    bool ReInitialize(ID3D11Device*);

    void GetVertex(std::vector<VertexType*>&);

private:
    virtual bool InitializeBuffers(ID3D11Device*);
    virtual void RenderBuffers(ID3D11DeviceContext*);

private:
    std::vector<VertexType*> m_vertexList;

    bool m_isRendered;
    int m_countIgnored;
};

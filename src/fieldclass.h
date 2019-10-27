#pragma once

#include "modelclass.h"

class FieldClass : public ModelClass
{
public:
    FieldClass();
    FieldClass(const FieldClass&);
    ~FieldClass();

private:
    virtual bool InitializeBuffers(ID3D11Device*);
    virtual void RenderBuffers(ID3D11DeviceContext*);
};

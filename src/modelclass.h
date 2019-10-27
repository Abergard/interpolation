////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
// #include <d3dx10math.h>
#include <DirectXMath.h>

struct VertexType
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
public:
    ModelClass();
    ModelClass(const ModelClass&);
    ~ModelClass();

    bool Initialize(ID3D11Device*);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    int GetIndexCount();

protected:
    virtual bool InitializeBuffers(ID3D11Device*) = 0;
    void ShutdownBuffers();
    virtual void RenderBuffers(ID3D11DeviceContext*);

protected:
    ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
    int m_vertexCount, m_indexCount;

    bool m_field;
    int m_left;
    int m_right;
};

#endif

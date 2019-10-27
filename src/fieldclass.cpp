#include "fieldclass.h"

FieldClass::FieldClass()
{
}

FieldClass::FieldClass(const FieldClass&)
{
}

FieldClass::~FieldClass()
{
}

bool FieldClass::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_vertexCount = 6;

    // Set the number of indices in the index array.
    m_indexCount = m_vertexCount;

    // Create the vertex array.
    vertices = new VertexType[m_vertexCount];
    if (!vertices)
    {
        return false;
    }

    // Create the index array.
    indices = new unsigned long[m_indexCount];
    if (!indices)
    {
        return false;
    }

    vertices[0].pos = DirectX::XMFLOAT3(-6, 0, 0.0f);
    vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    vertices[1].pos = DirectX::XMFLOAT3(+16, 0, 0.0f);
    vertices[1].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

    vertices[2].pos = DirectX::XMFLOAT3(0, 16, 0.0f);
    vertices[2].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    vertices[3].pos = DirectX::XMFLOAT3(0, -6, 0.0f);
    vertices[3].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    vertices[4].pos = DirectX::XMFLOAT3(0, 0, 16.0f);
    vertices[4].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
    vertices[5].pos = DirectX::XMFLOAT3(0, 0, -6.0f);
    vertices[5].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

    for (int i = 0; i < m_indexCount; ++i)
    {
        indices[i] = i;
    }

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    result =
        device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result =
        device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Release the arrays now that the vertex and index buffers have been
    // created and loaded.
    delete[] vertices;
    vertices = 0;

    delete[] indices;
    indices = 0;

    return true;
}

void FieldClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    ModelClass::RenderBuffers(deviceContext);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    return;
}

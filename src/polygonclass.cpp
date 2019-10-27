#include "polygonclass.h"

PolygonClass::PolygonClass() : m_size(200), m_Interpolation(0)
{

    m_left = -6;
    m_right = 6;
}

PolygonClass::PolygonClass(const PolygonClass&)
{
}

PolygonClass::~PolygonClass()
{
}

bool PolygonClass::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_vertexCount = m_size;

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

    DrawFunction(vertices, indices);

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

void PolygonClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    ModelClass::RenderBuffers(deviceContext);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    return;
}

void PolygonClass::DrawFunction(VertexType* vertices, unsigned long* indices)
{
    float y;
    float x = (float)m_left;
    float next = ((float)m_right - x) / (m_indexCount - 1);

    if (m_Interpolation != NULL && m_Interpolation->GetListCount() > 0)
    {
        for (int i = 0; i < m_indexCount; ++i) //-4,-2,0,2, 4
        {
            if (m_Interpolation)
            {
                y = m_Interpolation->F(x);
            }
            else
            {
                y = 0;
            }

            vertices[i].pos = DirectX::XMFLOAT3(x, y, 0.0f);
            vertices[i].color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
            x += next;
        }
    }
}

void PolygonClass::SetProperty(int left, int right, int size)
{
    m_left = left;
    m_right = right;
    m_size = size;
}

void PolygonClass::SetInterpolation(Interpolation* pnInterpolation)
{
    m_Interpolation = pnInterpolation;
}

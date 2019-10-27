#include "pointclass.h"

PointClass::PointClass() : m_isRendered(true), m_countIgnored(0)
{
}

PointClass::PointClass(const PointClass&)
{
}

PointClass::~PointClass()
{
}

bool PointClass::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_vertexCount = (m_vertexList.size() - m_countIgnored) * 6;
    if (m_vertexCount == 0)
    {
        m_isRendered = false;
        return true;
    }
    else
    {
        m_isRendered = true;
    }

    // Set the number of indices in the index array.
    m_indexCount = m_vertexCount;

    // Create the vertex array.
    vertices = new VertexType[m_vertexCount];
    if (!vertices)
    {
        return false;
    }

    int nr = 0;
    float size = 0.3f;

    float before = -9999999;
    DirectX::XMFLOAT4 color;

    for (auto obj : m_vertexList)
    {
        if (obj->pos.z == 0)
        {
            color = DirectX::XMFLOAT4(
                obj->color.x, obj->color.y, obj->color.z, obj->color.w);

            vertices[nr].pos =
                DirectX::XMFLOAT3(obj->pos.x, obj->pos.y + size, obj->pos.z);
            vertices[nr].color = color;
            ++nr;

            vertices[nr].pos =
                DirectX::XMFLOAT3(obj->pos.x + size, obj->pos.y, obj->pos.z);
            vertices[nr].color = color;
            ++nr;

            vertices[nr].pos =
                DirectX::XMFLOAT3(obj->pos.x, obj->pos.y - size, obj->pos.z);
            vertices[nr].color = color;
            ++nr;
            //

            vertices[nr].pos =
                DirectX::XMFLOAT3(obj->pos.x, obj->pos.y - size, obj->pos.z);
            vertices[nr].color = color;
            ++nr;

            vertices[nr].pos =
                DirectX::XMFLOAT3(obj->pos.x - size, obj->pos.y, obj->pos.z);
            vertices[nr].color = color;
            ++nr;

            vertices[nr].pos =
                DirectX::XMFLOAT3(obj->pos.x, obj->pos.y + size, obj->pos.z);
            vertices[nr].color = color;
            ++nr;
        }
    }

    // Create the index array.
    indices = new unsigned long[m_indexCount];
    if (!indices)
    {
        return false;
    }

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

void PointClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    if (m_isRendered)
    {
        ModelClass::RenderBuffers(deviceContext);
        deviceContext->IASetPrimitiveTopology(
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
    return;
}

bool PointClass::ReInitialize(ID3D11Device* device)
{
    bool result;
    this->ShutdownBuffers();
    result = this->InitializeBuffers(device);
    if (!result)
    {
        return false;
    }

    return true;
}

void PointClass::AddPoint(float x, float y, float z)
{
    VertexType* vertex = new VertexType;
    vertex->pos.x = x;
    vertex->pos.y = y;
    vertex->pos.z = z;

    if (z > 0)
    {
        ++m_countIgnored;
    }

    vertex->color.x = 1.0f;
    vertex->color.y = 0.0f;
    vertex->color.z = 1.0f;
    vertex->color.w = 1.0f;

    m_vertexList.push_back(vertex);

    std::sort(m_vertexList.begin(),
              m_vertexList.end(),
              [](VertexType* v1, VertexType* v2) -> bool {
                  return (v1->pos.x < v2->pos.x);
              });
}

void PointClass::DeletePoint(int nr)
{
    if (m_vertexList[nr]->pos.z > 0)
    {
        --m_countIgnored;
    }

    m_vertexList.erase(m_vertexList.begin() + nr);
}

bool PointClass::IsRendered()
{
    return m_isRendered;
}

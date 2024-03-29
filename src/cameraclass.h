////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

//////////////
// INCLUDES //
//////////////
// #include <d3dx10math.h>
#include <DirectXMath.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
    CameraClass();

    void SetPosition(float, float, float);
    void SetRotation(float, float, float);

    DirectX::XMFLOAT3 GetPosition();
    DirectX::XMFLOAT3 GetRotation();

    void Render();
    void GetViewMatrix(DirectX::XMMATRIX&);
    void GetViewMatrix(DirectX::XMFLOAT3X3&);

private:
    float m_positionX, m_positionY, m_positionZ;
    float m_rotationX, m_rotationY, m_rotationZ;
    DirectX::XMFLOAT3X3 m_viewMatrix;
    DirectX::XMMATRIX _viewMatrix;
};

#endif

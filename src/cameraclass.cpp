////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

CameraClass::CameraClass()
{
    m_positionX = 0.0f;
    m_positionY = 0.0f;
    m_positionZ = 0.0f;

    m_rotationX = 0.0f;
    m_rotationY = 0.0f;
    m_rotationZ = 0.0f;
}

void CameraClass::SetPosition(float x, float y, float z)
{
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
    return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
    m_rotationX = x;
    m_rotationY = y;
    m_rotationZ = z;
    return;
}

DirectX::XMFLOAT3 CameraClass::GetPosition()
{
    return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 CameraClass::GetRotation()
{
    return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
    DirectX::XMFLOAT3 up, position, lookAt;
    DirectX::XMVECTOR upVector, positionVector, lookAtVector;
    float yaw, pitch, roll;
    // DirectX::XMFLOAT3X3 rotationMatrix;

    // Setup the vector that points upwards.
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    upVector = DirectX::XMLoadFloat3(&up);

    // Setup the position of the camera in the world.
    position.x = m_positionX;
    position.y = m_positionY;
    position.z = m_positionZ;

    positionVector = DirectX::XMLoadFloat3(&position);

    // Setup where the camera is looking by default.
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    lookAtVector = DirectX::XMLoadFloat3(&lookAt);

    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in
    // radians.
    pitch = m_rotationX * 0.0174532925f;
    yaw = m_rotationY * 0.0174532925f;
    roll = m_rotationZ * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch, and roll values.
    auto rotationMatrix =
        DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
    // D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

    // Transform the lookAt and up vector by the rotation matrix so the view is
    // correctly rotated at the origin.
    lookAtVector =
        DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);
    upVector = DirectX::XMVector3TransformCoord(upVector, rotationMatrix);
    // D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
    // D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);
    // lookAt = position + lookAt;

    // Finally create the view matrix from the three updated vectors.
    // D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
    _viewMatrix =
        DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

    return;
}

void CameraClass::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
    viewMatrix = _viewMatrix;
}

void CameraClass::GetViewMatrix(DirectX::XMFLOAT3X3& viewMatrix)
{
    viewMatrix = m_viewMatrix;
}

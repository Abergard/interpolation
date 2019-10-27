////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

GraphicsClass::GraphicsClass()
{
    m_D3D = 0;
    m_Camera = 0;
    m_Model = 0;
    m_ColorShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth,
                               int screenHeight,
                               HWND hwnd,
                               InputClass* input)
{
    bool result;
    m_Input = input;

    // Create the Direct3D object.
    m_D3D = new D3DClass;
    if (!m_D3D)
    {
        return false;
    }

    // Initialize the Direct3D object.
    result = m_D3D->Initialize(screenWidth,
                               screenHeight,
                               VSYNC_ENABLED,
                               hwnd,
                               FULL_SCREEN,
                               SCREEN_DEPTH,
                               SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
        return false;
    }

    // Create the camera object.
    m_Camera = new CameraClass;
    if (!m_Camera)
    {
        return false;
    }

    // Set the initial position of the camera.
    m_Camera->SetPosition(0.0f, 0.0f, -50.0f);

    m_Points = new PointClass;
    if (!m_Points)
    {
        return false;
    }

    result = m_Points->Initialize(m_D3D->GetDevice());
    if (!result)
    {
        MessageBox(hwnd,
                   "Could not initialize the m_Points object.",
                   "Error",
                   MB_OK);
        return false;
    }

    // Create the model object.
    m_Field = new FieldClass;
    if (!m_Field)
    {
        return false;
    }

    // Initialize the model object.
    result = m_Field->Initialize(m_D3D->GetDevice());
    if (!result)
    {
        MessageBox(hwnd,
                   "Could not initialize the m_Field object.",
                   "Error",
                   MB_OK);
        return false;
    }

    // Create the model object.
    m_Model = new PolygonClass;
    if (!m_Model)
    {
        return false;
    }

    // Initialize the model object.
    result = m_Model->Initialize(m_D3D->GetDevice());
    if (!result)
    {
        MessageBox(
            hwnd, "Could not initialize the model object.", "Error", MB_OK);
        return false;
    }

    // Create the color shader object.
    m_ColorShader = new ColorShaderClass;
    if (!m_ColorShader)
    {
        return false;
    }

    // Initialize the color shader object.
    result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd,
                   "Could not initialize the color shader object.",
                   "Error",
                   MB_OK);
        return false;
    }

    return true;
}

bool GraphicsClass::ReInitializeModel(int size,
                                      int left,
                                      int right,
                                      Interpolation* pnInterpolation)
{
    bool result;

    result = m_Points->ReInitialize(m_D3D->GetDevice());
    if (!result)
    {
        MessageBox(
            NULL, "Could not initialize the model object.", "Error", MB_OK);
        return false;
    }

    m_Model->Shutdown();
    m_Model->SetProperty(left, right, size);
    m_Model->SetInterpolation(pnInterpolation);

    // Initialize the model object.
    result = m_Model->Initialize(m_D3D->GetDevice());
    if (!result)
    {
        MessageBox(
            NULL, "Could not initialize the model object.", "Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::AddPoint(float x, float y, float z)
{
    m_Points->AddPoint(x, y, z);
}

void GraphicsClass::DeletePoint(int nr)
{
    m_Points->DeletePoint(nr);
}

void GraphicsClass::Shutdown()
{
    // Release the color shader object.
    if (m_ColorShader)
    {
        m_ColorShader->Shutdown();
        delete m_ColorShader;
        m_ColorShader = 0;
    }

    if (m_Field)
    {
        m_Field->Shutdown();
        delete m_Field;
        m_Field = 0;
    }

    if (m_Points)
    {
        m_Points->Shutdown();
        delete m_Points;
        m_Points = 0;
    }

    // Release the model object.
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }

    // Release the camera object.
    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = 0;
    }

    // Release the D3D object.
    if (m_D3D)
    {
        m_D3D->Shutdown();
        delete m_D3D;
        m_D3D = 0;
    }

    return;
}

bool GraphicsClass::Frame()
{
    bool result;

    // Render the graphics scene.
    result = Render();
    if (!result)
    {
        return false;
    }

    return true;
}

bool GraphicsClass::Render()
{
    DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;

    static float rotationX = 0.0f;
    static float rotationY = 0.0f;
    static float hight = 0.0f;
    static float zoom = -50.0f;

    const float speed = 0.02f;

    if (m_Input->IsKeyDown(VK_RIGHT))
    {
        // Update the rotation variable each frame.
        rotationX += (float)DirectX::XM_PI * speed;
        if (rotationX > 360.0f)
        {
            rotationX -= 360.0f;
        }
    }

    if (m_Input->IsKeyDown(VK_LEFT))
    {
        // Update the rotation variable each frame.
        rotationX -= (float)DirectX::XM_PI * speed;
        if (rotationX < -360.0f)
        {
            rotationX += 360.0;
            ;
        }
    }

    if (m_Input->IsKeyDown(VK_UP))
    {
        // Update the rotation variable each frame.
        rotationY += (float)DirectX::XM_PI * speed;
        if (rotationY > 360.0f)
        {
            rotationY -= 360.0f;
        }

        hight += (float)DirectX::XM_PI * speed * 2;
    }

    if (m_Input->IsKeyDown(VK_DOWN))
    {
        // Update the rotation variable each frame.
        rotationY -= (float)DirectX::XM_PI * speed;
        if (rotationY < -360.0f)
        {
            rotationY += 360.0;
            ;
        }

        hight -= (float)DirectX::XM_PI * speed * 2;
    }

    if (m_Input->IsKeyDown(VK_SUBTRACT))
    {
        zoom -= (float)DirectX::XM_PI * speed * 6;
    }

    if (m_Input->IsKeyDown(VK_ADD))
    {
        zoom += (float)DirectX::XM_PI * speed * 6;
    }

    // Clear the buffers to begin the scene.
    m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate the view matrix based on the camera's position.
    m_Camera->Render();
    m_Camera->SetPosition(0.0f, hight, zoom);
    // Get the world, view, and projection matrices from the camera and d3d
    // objects.
    m_Camera->GetViewMatrix(viewMatrix);
    m_D3D->GetWorldMatrix(worldMatrix);
    m_D3D->GetProjectionMatrix(projectionMatrix);

    // Rotate the world matrix by the rotation value so that the triangle will
    // spin.
    // D3DXMatrixRotationY(&worldMatrix, rotationY);
    // D3DXMatrixRotationX(&worldMatrix, rotationX);
    // D3DXMatrixRotationYawPitchRoll(&worldMatrix, rotationX, 0, 0);
    worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(0, rotationX, 0);

    // Put the model vertex and index buffers on the graphics pipeline to
    // prepare them for drawing.

    if ((m_Points)->IsRendered())
    {
        m_Points->Render(m_D3D->GetDeviceContext());

        // Render the model using the color shader.
        result = m_ColorShader->Render(m_D3D->GetDeviceContext(),
                                       m_Points->GetIndexCount(),
                                       worldMatrix,
                                       viewMatrix,
                                       projectionMatrix);
        if (!result)
        {
            return false;
        }
    }

    m_Field->Render(m_D3D->GetDeviceContext());

    m_Model->Render(m_D3D->GetDeviceContext());

    // Render the model using the color shader.
    result = m_ColorShader->Render(m_D3D->GetDeviceContext(),
                                   m_Model->GetIndexCount(),
                                   worldMatrix,
                                   viewMatrix,
                                   projectionMatrix);
    if (!result)
    {
        return false;
    }

    m_Field->Render(m_D3D->GetDeviceContext());

    result = m_ColorShader->Render(m_D3D->GetDeviceContext(),
                                   m_Field->GetIndexCount(),
                                   worldMatrix,
                                   viewMatrix,
                                   projectionMatrix);
    if (!result)
    {
        return false;
    }

    // Present the rendered scene to the screen.
    m_D3D->EndScene();

    return true;
}

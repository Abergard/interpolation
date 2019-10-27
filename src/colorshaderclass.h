////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
// #include <d3dx10math.h>
#include <DirectXMath.h>
// #include <d3dx11async.h>
#include <filesystem>
#include <fstream>

// MY INCLUDES
#include "defines.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShaderClass
////////////////////////////////////////////////////////////////////////////////
class ColorShaderClass
{
private:
    struct MatrixBufferType
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

public:
    ColorShaderClass(std::filesystem::path app_path);
    ColorShaderClass(const ColorShaderClass&);
    ~ColorShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*,
                int,
                DirectX::XMMATRIX,
                DirectX::XMMATRIX,
                DirectX::XMMATRIX);

private:
    bool InitializeShader(ID3D11Device*, HWND, std::wstring, std::wstring);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*,
                             DirectX::XMMATRIX,
                             DirectX::XMMATRIX,
                             DirectX::XMMATRIX);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    std::filesystem::path m_shader_path;

    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_matrixBuffer;
};

#endif

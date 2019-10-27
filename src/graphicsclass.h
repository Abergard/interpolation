////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "cameraclass.h"
#include "colorshaderclass.h"
#include "d3dclass.h"
#include "fieldclass.h"
#include "inputclass.h"
#include "pointclass.h"
#include "polygonclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
    GraphicsClass();
    GraphicsClass(const GraphicsClass&);
    ~GraphicsClass();

    bool Initialize(int, int, HWND, InputClass*);
    void Shutdown();
    bool Frame();

    bool ReInitializeModel(int, int, int, Interpolation*);
    void AddPoint(float, float, float);
    void DeletePoint(int);

private:
    bool Render();

private:
    D3DClass* m_D3D;
    CameraClass* m_Camera;
    FieldClass* m_Field;
    PolygonClass* m_Model;
    PointClass* m_Points;
    ColorShaderClass* m_ColorShader;
    InputClass* m_Input;
};

#endif

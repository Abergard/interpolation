////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

#include "boost/lexical_cast.hpp"

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "graphicsclass.h"
#include "inputclass.h"
#include "interpolation.h"
////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
    SystemClass(std::filesystem::path app_path);
    SystemClass(const SystemClass&);
    ~SystemClass();

    bool Initialize();
    void Shutdown();
    void Run();

    HWND GetHandle();
    HWND GetGraphicHandle();

    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
    bool Frame();
    void InitializeWindows(int&, int&);
    void InitializeGraphicWindows(int&, int&);
    void ShutdownWindows();
    void ReadFromFile(char*);

    void DrawGUI();

private:
    std::filesystem::path m_app_path;

    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;

    HWND m_graphic_hwnd;

    HWND m_hbutton;
    HWND m_hsize;
    HWND m_htxt;
    HWND m_hleft;
    HWND m_hright;

    HWND m_hx;
    HWND m_hy;
    HWND m_hz;
    HWND m_hnewbutton;
    HWND m_hdeletebutton;
    HWND m_hListBox;

    InputClass* m_Input;
    GraphicsClass* m_Graphics;
    Interpolation* m_Interpolation;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;

#endif

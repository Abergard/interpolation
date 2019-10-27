////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
    m_Input = 0;
    m_Graphics = 0;
    m_Interpolation = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
    int screenWidth, screenHeight;
    bool result;

    // Initialize the width and height of the screen to zero before sending the
    // variables into the function.
    screenWidth = 0;
    screenHeight = 0;

    // Initialize the windows api.
    InitializeWindows(screenWidth, screenHeight);
    InitializeGraphicWindows(screenWidth, screenHeight);

    // Create the input object.  This object will be used to handle reading the
    // keyboard input from the user.
    m_Input = new InputClass;
    if (!m_Input)
    {
        return false;
    }

    // Initialize the input object.
    m_Input->Initialize();

    // Create the graphics object.  This object will handle rendering all the
    // graphics for this application.
    m_Graphics = new GraphicsClass;
    if (!m_Graphics)
    {
        return false;
    }

    // Initialize the graphics object.
    result = m_Graphics->Initialize(
        screenWidth, screenHeight, m_graphic_hwnd, m_Input);
    if (!result)
    {
        return false;
    }

    m_Interpolation = new Interpolation;
    if (!m_Interpolation)
    {
        return false;
    }

    this->ReadFromFile("input");

    return true;
}

void SystemClass::Shutdown()
{
    // Release the graphics object.
    if (m_Graphics)
    {
        m_Graphics->Shutdown();
        delete m_Graphics;
        m_Graphics = 0;
    }

    // Release the input object.
    if (m_Input)
    {
        delete m_Input;
        m_Input = 0;
    }

    if (m_Interpolation)
    {
        delete m_Interpolation;
        m_Interpolation = 0;
    }

    // Shutdown the window.
    ShutdownWindows();

    return;
}

void SystemClass::Run()
{
    MSG msg;
    bool done, result;

    // Initialize the message structure.
    ZeroMemory(&msg, sizeof(MSG));

    // Loop until there is a quit message from the window or the user.
    done = false;
    while (!done)
    {
        // Handle the windows messages.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // If windows signals to end the application then exit out.
        if (msg.message == WM_QUIT)
        {
            done = true;
        }
        else
        {
            // Otherwise do the frame processing.
            result = Frame();
            if (!result)
            {
                done = true;
            }
        }
    }

    return;
}

bool SystemClass::Frame()
{
    bool result;

    // Check if the user pressed escape and wants to exit the application.
    if (m_Input->IsKeyDown(VK_ESCAPE))
    {
        return false;
    }

    // Do the frame processing for the graphics object.
    result = m_Graphics->Frame();
    if (!result)
    {
        return false;
    }

    return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd,
                                             UINT umsg,
                                             WPARAM wparam,
                                             LPARAM lparam)
{
    bool click = false;

    switch (umsg)
    {
    // Check if a key has been pressed on the keyboard.
    case WM_KEYDOWN:
    {
        // If a key is pressed send it to the input object so it can record
        // that state.
        m_Input->KeyDown((unsigned int)wparam);
        return 0;
    }

    // Check if a key has been released on the keyboard.
    case WM_KEYUP:
    {
        // If a key is released then send it to the input object so it can
        // unset the state for that key.
        m_Input->KeyUp((unsigned int)wparam);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        if (ApplicationHandle->GetHandle() == hwnd ||
            ApplicationHandle->GetGraphicHandle() == hwnd)
        {
            SetFocus(ApplicationHandle->GetHandle());
        }
    }

    case WM_COMMAND:
    {
        click = true;
        int wmId, wmEvent;

        wmId = LOWORD(wparam);
        wmEvent = HIWORD(wparam);

        switch (wparam)
        {
        case 502:
        {

            DWORD dlugosc = GetWindowTextLength(m_hsize);
            LPWSTR Bufor = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
            GetWindowTextW(m_hsize, Bufor, dlugosc + 1);
            int size = _wtoi(Bufor);

            dlugosc = GetWindowTextLength(m_hleft);
            Bufor = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
            GetWindowTextW(m_hleft, Bufor, dlugosc + 1);
            int left = _wtoi(Bufor);

            dlugosc = GetWindowTextLength(m_hright);
            Bufor = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
            GetWindowTextW(m_hright, Bufor, dlugosc + 1);
            int right = _wtoi(Bufor);

            m_Interpolation->Calculate();
            m_Graphics->ReInitializeModel(
                size, left, right, m_Interpolation->Get());
        }
        break;

        case 503:
        {
            DWORD dlugosc = GetWindowTextLength(m_hx);
            LPWSTR Bufor = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
            GetWindowTextW(m_hx, Bufor, dlugosc + 1);
            float x = boost::lexical_cast<float>(Bufor);
            // std::wstring txt = std::wstring(L"x: ") + Bufor ;

            dlugosc = GetWindowTextLength(m_hy);
            Bufor = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
            GetWindowTextW(m_hy, Bufor, dlugosc + 1);
            float y = boost::lexical_cast<float>(Bufor);
            // txt += std::wstring(L"  y: ") + Bufor;

            dlugosc = GetWindowTextLength(m_hz);
            Bufor = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
            GetWindowTextW(m_hz, Bufor, dlugosc + 1);
            float z = boost::lexical_cast<float>(Bufor);
            // txt += std::wstring(L"  !: ") + Bufor;

            SendMessage(m_hListBox, LB_RESETCONTENT, 0, 0);
            m_Graphics->AddPoint(x, y, z);
            m_Interpolation->AddPoint(x, y, z);

            int size = m_Interpolation->GetListCount();
            DirectX::XMFLOAT3 position;
            std::wstring txt;

            for (int i = 0; i < size; ++i)
            {
                position = m_Interpolation->GetPoint(i);

                txt = std::wstring(L"x: ");
                txt += std::to_wstring(position.x);
                txt += std::wstring(L"  y: ");
                txt += std::to_wstring(position.y);
                txt += std::wstring(L"  !: ");
                txt += std::to_wstring(int(position.z));
                // txt += std::wstring(L"\n");

                SendMessage(m_hListBox, LB_ADDSTRING, 0, (LPARAM)txt.c_str());
            }
        }
        break;

        case 504:
        {
            int iCurSel = (int)SendMessage(m_hListBox, LB_GETCURSEL, 0, 0);
            SendMessage(m_hListBox, LB_DELETESTRING, (WPARAM)iCurSel, 0);

            if (iCurSel >= 0)
            {
                m_Graphics->DeletePoint(iCurSel);
                m_Interpolation->DeletePoint(iCurSel);
            }
        }
        break;
        }

        return 0;
    }

    // Any other messages send to the default message handler as our
    // application won't make use of them.
    default:
    {
        return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
    }
}

void SystemClass::DrawGUI()
{
    int leftBorder = 810;

    CreateWindowExW(NULL,
                   L"STATIC",
                   L"Size:",
                   WS_CHILD | WS_VISIBLE,
                   leftBorder,
                   50,
                   80,
                   20,
                   m_hwnd,
                   NULL,
                   m_hinstance,
                   NULL);

    m_hsize = CreateWindowExW(NULL,
                             L"EDIT",
                             L"200",
                             WS_CHILD | WS_VISIBLE,
                             leftBorder + 40,
                             50,
                             80,
                             20,
                             m_hwnd,
                             NULL,
                             m_hinstance,
                             NULL);

    CreateWindowExW(NULL,
                   L"STATIC",
                   L"Start Point:",
                   WS_CHILD | WS_VISIBLE,
                   leftBorder,
                   20,
                   100,
                   20,
                   m_hwnd,
                   NULL,
                   m_hinstance,
                   NULL);

    m_hleft = CreateWindowExW(NULL,
                             L"EDIT",
                             L"-6",
                             WS_CHILD | WS_VISIBLE,
                             leftBorder + 80,
                             20,
                             60,
                             20,
                             m_hwnd,
                             NULL,
                             m_hinstance,
                             NULL);

    CreateWindowExW(NULL,
                   L"STATIC",
                   L"End Point:",
                   WS_CHILD | WS_VISIBLE,
                   leftBorder + 150,
                   20,
                   80,
                   20,
                   m_hwnd,
                   NULL,
                   m_hinstance,
                   NULL);

    m_hright = CreateWindowExW(NULL,
                              L"EDIT",
                              L"6",
                              WS_CHILD | WS_VISIBLE,
                              leftBorder + 220,
                              20,
                              80,
                              20,
                              m_hwnd,
                              NULL,
                              m_hinstance,
                              NULL);

    m_hbutton = CreateWindowExW(NULL,
                               L"BUTTON",
                               L"GENERATE",
                               WS_CHILD | WS_VISIBLE,
                               leftBorder,
                               80,
                               80,
                               20,
                               m_hwnd,
                               (HMENU)502,
                               m_hinstance,
                               NULL);

    CreateWindowExW(NULL,
                   L"STATIC",
                   L"x:",
                   WS_CHILD | WS_VISIBLE,
                   leftBorder,
                   120,
                   20,
                   20,
                   m_hwnd,
                   NULL,
                   m_hinstance,
                   NULL);

    m_hx = CreateWindowExW(NULL,
                          L"EDIT",
                          L"0",
                          WS_CHILD | WS_VISIBLE,
                          leftBorder + 20,
                          120,
                          40,
                          20,
                          m_hwnd,
                          NULL,
                          m_hinstance,
                          NULL);

    CreateWindowExW(NULL,
                   L"STATIC",
                   L"y:",
                   WS_CHILD | WS_VISIBLE,
                   leftBorder + 80,
                   120,
                   20,
                   20,
                   m_hwnd,
                   NULL,
                   m_hinstance,
                   NULL);

    m_hy = CreateWindowExW(NULL,
                          L"EDIT",
                          L"0",
                          WS_CHILD | WS_VISIBLE,
                          leftBorder + 100,
                          120,
                          40,
                          20,
                          m_hwnd,
                          NULL,
                          m_hinstance,
                          NULL);

    CreateWindowExW(NULL,
                   L"STATIC",
                   L"!:",
                   WS_CHILD | WS_VISIBLE,
                   leftBorder + 160,
                   120,
                   20,
                   20,
                   m_hwnd,
                   NULL,
                   m_hinstance,
                   NULL);

    m_hz = CreateWindowExW(NULL,
                          L"EDIT",
                          L"0",
                          WS_CHILD | WS_VISIBLE,
                          leftBorder + 180,
                          120,
                          40,
                          20,
                          m_hwnd,
                          NULL,
                          m_hinstance,
                          NULL);

    m_hnewbutton = CreateWindowExW(NULL,
                                  L"BUTTON",
                                  L"ADD",
                                  WS_CHILD | WS_VISIBLE,
                                  leftBorder + 240,
                                  120,
                                  40,
                                  20,
                                  m_hwnd,
                                  (HMENU)503,
                                  m_hinstance,
                                  NULL);

    m_hdeletebutton = CreateWindowExW(NULL,
                                     L"BUTTON",
                                     L"DELETE",
                                     WS_CHILD | WS_VISIBLE,
                                     leftBorder + 300,
                                     120,
                                     80,
                                     20,
                                     m_hwnd,
                                     (HMENU)504,
                                     m_hinstance,
                                     NULL);

    m_hListBox = CreateWindowExW(WS_EX_CLIENTEDGE,
                                L"LISTBOX",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
                                leftBorder,
                                160,
                                300,
                                200,
                                m_hwnd,
                                NULL,
                                m_hinstance,
                                NULL);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
    WNDCLASSEXW wc;
    DEVMODE dmScreenSettings;
    int posX, posY;
    int widthGUI = 1300;
    int heightGUI = 600;

    // Get an external pointer to this object.
    ApplicationHandle = this;

    // Get the instance of this application.
    m_hinstance = GetModuleHandle(NULL);

    // Give the application a name.
    m_applicationName = L"Engine";

    // Setup the windows class with default settings.
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassExW(&wc);

    // Determine the resolution of the clients desktop screen.
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings depending on whether it is running in full
    // screen or in windowed mode.
    if (FULL_SCREEN)
    {
        // If full screen set the screen to maximum size of the users desktop
        // and 32bit.
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields =
            DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner.
        posX = posY = 0;
    }
    else
    {
        // If windowed then set it to 800x600 resolution.
        screenWidth = 800;
        screenHeight = 600;

        // Place the window in the middle of the screen.
        posX = (GetSystemMetrics(SM_CXSCREEN) - widthGUI) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - heightGUI) / 2;
    }

    // Create the window with the screen settings and get the handle to it.
    m_hwnd = CreateWindowExW(
        WS_EX_APPWINDOW | WS_EX_RIGHTSCROLLBAR | WS_EX_CONTROLPARENT,
        m_applicationName,
        m_applicationName,
        WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX,
        posY,
        widthGUI,
        heightGUI,
        NULL,
        NULL,
        m_hinstance,
        NULL);

    DrawGUI();

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    // SetFocus(m_hwnd);

    // Hide the mouse cursor.
    // ShowCursor(false);

    return;
}

void SystemClass::InitializeGraphicWindows(int& screenWidth, int& screenHeight)
{
    WNDCLASSEXW wc;
    DEVMODE dmScreenSettings;
    int posX, posY;

    // Setup the windows class with default settings.
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassExW(&wc);

    // Determine the resolution of the clients desktop screen.
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings depending on whether it is running in full
    // screen or in windowed mode.
    if (FULL_SCREEN)
    {
        // If full screen set the screen to maximum size of the users desktop
        // and 32bit.
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields =
            DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner.
        posX = posY = 0;
    }
    else
    {
        // If windowed then set it to 800x600 resolution.
        screenWidth = 800;
        screenHeight = 600;

        // Place the window in the middle of the screen.
        posX = 0;
        posY = 0;
    }

    // Create the window with the screen settings and get the handle to it.
    m_graphic_hwnd = CreateWindowExW(WS_EX_APPWINDOW,
                                    m_applicationName,
                                    m_applicationName,
                                    WS_CHILD,
                                    posX,
                                    posY,
                                    screenWidth,
                                    screenHeight,
                                    m_hwnd,
                                    NULL,
                                    m_hinstance,
                                    NULL);

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(m_graphic_hwnd, SW_SHOW);
    SetForegroundWindow(m_graphic_hwnd);
    SetFocus(m_graphic_hwnd);

    // Hide the mouse cursor.
    // ShowCursor(false);

    return;
}

void SystemClass::ShutdownWindows()
{
    // Show the mouse cursor.
    // ShowCursor(true);

    // Fix the display settings if leaving full screen mode.
    if (FULL_SCREEN)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    // Remove the window.

    DestroyWindow(m_htxt);
    m_htxt = NULL;

    DestroyWindow(m_hleft);
    m_hleft = NULL;

    DestroyWindow(m_hright);
    m_hright = NULL;

    DestroyWindow(m_hsize);
    m_hsize = NULL;

    DestroyWindow(m_hbutton);
    m_hbutton = NULL;

    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    DestroyWindow(m_graphic_hwnd);
    m_graphic_hwnd = NULL;

    DestroyWindow(m_hx);
    DestroyWindow(m_hy);
    DestroyWindow(m_hz);
    DestroyWindow(m_hnewbutton);
    DestroyWindow(m_hListBox);
    DestroyWindow(m_hdeletebutton);
    m_hdeletebutton = NULL;
    m_hx = NULL;
    m_hy = NULL;
    m_hz = NULL;
    m_hnewbutton = NULL;
    m_hListBox = NULL;

    // Remove the application instance.
    // UnregisterClassW
    UnregisterClassW(m_applicationName, m_hinstance);
    m_hinstance = NULL;

    // Release the pointer to this class.
    ApplicationHandle = NULL;

    return;
}

LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT umessage,
                         WPARAM wparam,
                         LPARAM lparam)
{
    switch (umessage)
    {
    // Check if the window is being destroyed.
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    // Check if the window is being closed.
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }

    // All other messages pass to the message handler in the system class.
    default:
    {
        return ApplicationHandle->MessageHandler(
            hwnd, umessage, wparam, lparam);
    }
    }
}

HWND SystemClass::GetHandle()
{
    return m_hwnd;
}

HWND SystemClass::GetGraphicHandle()
{
    return m_graphic_hwnd;
}

void SystemClass::ReadFromFile(char* filename)
{
    float x, y, z;

    std::ifstream file(filename, std::ifstream::in);

    while (file >> x >> y >> z)
    {
        m_Interpolation->AddPoint(x, y, z);
        m_Graphics->AddPoint(x, y, z);
    }

    file.close();

    int size = m_Interpolation->GetListCount();
    DirectX::XMFLOAT3 position;
    std::wstring txt;

    for (int i = 0; i < size; ++i)
    {
        position = m_Interpolation->GetPoint(i);

        txt = std::wstring(L"x: ");
        txt += std::to_wstring(position.x);
        txt += std::wstring(L"  y: ");
        txt += std::to_wstring(position.y);
        txt += std::wstring(L"  !: ");
        txt += std::to_wstring(int(position.z));

        SendMessage(m_hListBox, LB_ADDSTRING, 0, (LPARAM)txt.c_str());
    }
}

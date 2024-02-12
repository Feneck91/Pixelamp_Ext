// pixelampextsimu.cpp : Définit le point d'entrée de l'application.
//

#include "../include/pixellampchipset.h"
#include "../simu_include/framework.h"
#include "../simu_include/pixelampextsimu.h"
#include "../pixelampext.ino"
#include <commctrl.h>

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

/*
#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
        #define DEBUG_NEW   new(THIS_FILE, __LINE__)
        #undef THIS_FILE
        static char THIS_FILE[] = __FILE__;
    #endif
#endif
*/

#define     ID_TRACKBAR_ANIMATION                   (WM_USER + 100)
#define     ID_TRACKBAR_BRIGHTNESS                  (WM_USER + 101)
#define     ID_CHECKBOX_DISPLAY_LED_INDEX           (WM_USER + 102)

// CreateTrackbar - creates and initializes a trackbar. 
// 
// Global variable
//     g_hinst - instance handle
//
HWND WINAPI CreateTrackbar(
    HINSTANCE hInst,
    HWND hwndDlg,  // handle of dialog box (parent window)
    uint16_t IdCtrl,
    int iValue,
    int x,
    int y,
    UINT iMin,     // minimum value in trackbar range 
    UINT iMax,     // maximum value in trackbar range 
    UINT iSelMin,  // minimum value in trackbar selection 
    UINT iSelMax)  // maximum value in trackbar selection 
{
    HWND hwndTrack = CreateWindowEx(
        0,                                                              // no extended styles 
        TRACKBAR_CLASS,                                                 // class name 
        L"Trackbar Control",                                            // title (caption) 
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,     // style 
        x, y,                                                           // position 
        200, 40,                                                        // size 
        hwndDlg,                                                        // parent window 
        (HMENU) IdCtrl,                                                 // control identifier 
        hInst,                                                          // instance 
        NULL                                                            // no WM_CREATE parameter 
    );

    SendMessage(hwndTrack, TBM_SETRANGE, 
        (WPARAM)TRUE,                   // redraw flag 
        (LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions

    SendMessage(hwndTrack, TBM_SETPAGESIZE,
        0, (LPARAM)4);                  // new page size 

    SendMessage(hwndTrack, TBM_SETSEL,
        (WPARAM)FALSE,                  // redraw flag 
        (LPARAM)MAKELONG(iSelMin, iSelMax));

    SendMessage(hwndTrack, TBM_SETPOS,
        (WPARAM)TRUE,                   // redraw flag 
        (LPARAM)iValue);

    SetFocus(hwndTrack);

    return hwndTrack;
}

// Used to record pot values
#define EEPROM_ADDRESS_CURRENT_BRIGHTNESS               100 // Be careful : use 2 ADRESS !!
#define EEPROM_ADDRESS_CURRENT_ANIMATION                102 // Be careful : use 2 ADRESS !!

ISimulatorController<DEFAULT_COLOR_ORDER>* ISimulatorController<DEFAULT_COLOR_ORDER>::m_pSimulator = nullptr;

class ArduinoSimulator : public ISimulatorController<DEFAULT_COLOR_ORDER>
{
private:
    /// <summary>
    /// Handle of the window.
    /// </summary>
    HWND                        m_hWnd;

    /// <summary>
    /// Singleton of this ArduinoSimulator.
    /// </summary>
    static ArduinoSimulator *   m_pInstance;

    /// <summary>
    // Must quit thread?
    /// </summary>
    bool                        m_bMustQuit;

    /// <summary>
    /// Thread handle
    /// </summary>
    HANDLE                      m_hThread;

    /// <summary>
    /// Brightness.
    /// </summary>
    int                         m_iValueBrightness;

    /// <summary>
    /// Animation value.
    /// </summary>
    int                         m_iValueAnimation;

    /// <summary>
    /// To manage thread.
    /// 
    /// Protect data to read / write.
    /// </summary>
    CRITICAL_SECTION            m_CriticalSection;

    /// <summary>
    /// Handle to brightness window.
    /// </summary>
    HWND                        m_hWndBrightness;

    /// <summary>
    /// Handle to animation window.
    /// </summary>
    HWND                        m_hWndAnimation;

    /// <summary>
    /// Handle to check box to display led number.
    /// </summary>
    HWND                        m_hWndDisplayLedsNumber;

    /// <summary>
    /// Should display led number?
    /// </summary>
    bool                        m_bIsDisplayLedsNumber;

    /// <summary>
    /// Pointer on LEDs array value used to displayed.
    /// </summary>
    CRGB *                      m_pLeds;

private:
    /// <summary>
    /// virtual destructor.
    /// </summary>
    virtual ~ArduinoSimulator();

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    /// <param name="_hInstance">Instance of application</param>
    /// <param name="_hwnd">Handle of the parent window</param>
    ArduinoSimulator(HINSTANCE _hInstance, HWND _hwnd);

    /// <summary>
    /// Pointer on LEDs array value used to displayed.
    /// </summary>
    CRGB*                       GetLeds();

    /// <summary>
    /// Should display led number?
    /// </summary>
    /// <returns>true if must display led number, false else.</returns>
    bool                        IsDisplayLedsNumber() const;

    /// <summary>
    /// Get the critical section.
    /// </summary>
    /// <returns></returns>
    CRITICAL_SECTION &          GetCriticalSection();

    /// <summary>
    /// Initialize the class.
    /// 
    /// Called by the led chipset when it is initialized.
    /// </summary>
    virtual void                init() override;

    /// <summary>
    /// Called by the framework simulation when pixels are displayed.
    /// </summary>
    /// <param name="_rPpixels">Range of leds to display</param>
    virtual void                onShowPixels(PixelController<DEFAULT_COLOR_ORDER>& _rPpixels) override;

    /// <summary>
    /// Indicate if the thread that manage the engine must be quit.
    /// </summary>
    /// <returns>true if should quit, false else.</returns>
    bool                        shouldQuit() const;

    /// <summary>
    /// Compute infos for RECT leds.
    /// </summary>
    /// <param name="_piLeft">Get the left position if pointer is pass into this parameter.</param>
    /// <param name="_piTop">Get the top position if pointer is pass into this parameter.</param>
    /// <param name="_piWidth">Get the width if pointer is pass into this parameter.</param>
    /// <param name="_piHeight">Get the height if pointer is pass into this parameter.</param>
    /// <returns></returns>
    RECT                        ComputeRectLeds(int* _piLeft, int* _piTop, int* _piWidth, int* _piHeight) const;

    /// <summary>
    /// Get the analog value.
    /// 
    /// Callback.
    /// </summary>
    /// <param name="_iPin">Pin (A0 to A15)</param>
    /// <returns>The value</returns>
    int                         GetAnalogValue(uint8_t _iPin) const;

    /// <summary>
    /// Delete the instance
    /// </summary>
    static void                 KillInstance();

    /// <summary>
    /// Get the instance
    /// </summary>
    static ArduinoSimulator *   Instance();

    /// <summary>
    /// Get the analog value.
    /// 
    /// Callback.
    /// </summary>
    /// <param name="_pContext">it is a pointer on ArduinoSimulator class.</param>
    /// <param name="_iPin">Pin (A0 to A15)</param>
    /// <returns>The value</returns>
    static int                  StaticGetAnalogValue(void * _pContext, uint8_t _iPin);

    /// <summary>
    /// Manage button notifications.
    /// </summary>
    /// <param name="wParam">Notification (BN_CLICKED).</param>
    /// <param name="hwnd">Window's handle.</param>
    VOID WINAPI                 ButtonNotification(WPARAM wParam, HWND hwnd);
        
    /// <summary>
    /// TBNotifications - handles trackbar notifications received 
    //  in the wParam parameter of WM_HSCROLL. This function simply  ensures that the slider remains within the selection range.
    /// </summary>
    /// <param name="wParam">wParam of WM_HSCROLL message</param>
    /// <param name="hwndTrack">handle of trackbar window</param>
    /// <param name="iSelMin">minimum value of trackbar selection</param>
    /// <param name="iSelMax">maximum value of trackbar selection</param>
    VOID WINAPI                 TBNotifications(WPARAM wParam, HWND hwndTrack, UINT iSelMin, UINT iSelMax);

    /// <summary>
    /// Simultate EEPROM read.
    /// 
    /// Read one bit only (8 bits).
    /// </summary>
    /// <param name="idx">Index of adress to read.</param>
    /// <returns>The value of this EEPROM value at this address.</returns>
    static int                  ReadByteEEPROM(int idx);

    /// <summary>
    /// Simultate EEPROM read.
    /// 
    /// Read two bits only (16 bits).
    /// </summary>
    /// <param name="idx">Index of adress to read.</param>
    /// <returns>The value of this EEPROM value at this address & address + 1.</returns>
    static int                  ReadIntEEPROM(int idx);

    /// <summary>
    /// Simultate EEPROM write.
    /// 
    /// Write two bits only (16 bits).
    /// </summary>
    /// <param name="idx">Index of adress to write.</param>
    /// <_iValue>The value to write in this EEPROM.</returns>
    static void                 WriteIntEEPROM(int idx, int _iValue);
};

RECT ArduinoSimulator::ComputeRectLeds(int * _piLeft, int * _piTop, int * _piWidth, int * _piHeight) const
{
    int iLeft = 0;
    int iTop = 0;

    const int WIDTH_LEFT_PANE = 210;

    int iNbLedsWidth = CEngine::Instance().GetRealMatrixWidth();
    int iNbLedsHeight = CEngine::Instance().GetRealMatrixHeight();

    RECT rect;
    GetClientRect(m_hWnd, &rect);


    int iWidth = (rect.right - rect.left - WIDTH_LEFT_PANE) / (iNbLedsWidth);
    int iHeight = (rect.bottom - rect.top) / (iNbLedsHeight);

    if (iWidth > iHeight)
    {
        iWidth = iHeight = min(iWidth, iHeight);
        iLeft = WIDTH_LEFT_PANE + ((rect.right - rect.left - WIDTH_LEFT_PANE) - iWidth * iNbLedsWidth) / 2;
    }
    else
    {
        iWidth = iHeight = min(iWidth, iHeight);
        iLeft = WIDTH_LEFT_PANE;
        iTop = ((rect.bottom - rect.top) - iHeight * iNbLedsHeight) / 2;
    }

    if (_piLeft != nullptr)
    {
        *_piLeft = iLeft;
    }
    if (_piTop != nullptr)
    {
        *_piTop = iTop;
    }
    if (_piWidth != nullptr)
    {
        *_piWidth = iWidth;
    }
    if (_piHeight != nullptr)
    {
        *_piHeight = iHeight;
    }

    RECT rectDisplay = { iLeft, iTop, iLeft + iWidth * iNbLedsWidth, iTop + iNbLedsHeight * iHeight };
    return rectDisplay;
}


int ArduinoSimulator::ReadByteEEPROM(int idx)
{
    return EEPROM.read(idx);
}

int ArduinoSimulator::ReadIntEEPROM(int idx)
{
    return EEPROM.read(idx) + (EEPROM.read(idx + 1) << 8);
}

void ArduinoSimulator::WriteIntEEPROM(int idx, int _iValue)
{
    EEPROM.write(idx,       _iValue        & 0xFF);
    EEPROM.write(idx + 1,   (_iValue >> 8) & 0xFF);
}

ArduinoSimulator * ArduinoSimulator::m_pInstance = nullptr;

int ArduinoSimulator::GetAnalogValue(uint8_t _iPin) const
{
    int iValue = 0;
    if (_iPin == CEngine::Instance().GetPinBrightness())
    {
        iValue = m_iValueBrightness;
    }
    else if (_iPin == CEngine::Instance().GetPinAnimation())
    {
        iValue = m_iValueAnimation;
    }

    return iValue;
}

int ArduinoSimulator::StaticGetAnalogValue(void * _pContext, uint8_t _iPin)
{
    return ((const ArduinoSimulator*)_pContext)->GetAnalogValue(_iPin);
}

DWORD WINAPI RunEngineThread(LPVOID _lpParameter)
{
    // The new thread will start here
    ArduinoSimulator* pSimulator = (ArduinoSimulator *) _lpParameter;
    while (!pSimulator->shouldQuit())
    {
        loop();
    }
    return 0;
}

ArduinoSimulator::ArduinoSimulator(HINSTANCE _hInstance, HWND _hWnd)
    : m_hWnd(_hWnd)
{
    m_pInstance = this;
    m_pLeds = nullptr;
    m_bIsDisplayLedsNumber = false;

    // Create engine instance.
    ISimulatorController<DEFAULT_COLOR_ORDER>::SetSimulator(this);
    setAnalogReadFunction(this, ArduinoSimulator::StaticGetAnalogValue);

    m_iValueBrightness = ArduinoSimulator::ReadIntEEPROM(EEPROM_ADDRESS_CURRENT_BRIGHTNESS);
    m_iValueAnimation = ArduinoSimulator::ReadIntEEPROM(EEPROM_ADDRESS_CURRENT_ANIMATION);

    m_hWndBrightness        = CreateTrackbar(_hInstance, _hWnd, ID_TRACKBAR_BRIGHTNESS, m_iValueBrightness, 0, 20, 0, 1023, 0, 0);
    m_hWndAnimation         = CreateTrackbar(_hInstance, _hWnd, ID_TRACKBAR_ANIMATION, m_iValueAnimation,   0, 100, 0, 1023, 0, 0);
    m_hWndDisplayLedsNumber = CreateWindowEx(0,                                         // no extended styles 
                                             L"button",                                 // class name 
                                             L"Display leds number",                    // title (caption) 
                                             WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,   // style 
                                             0, 180,                                    // position 
                                             200, 40,                                   // size 
                                             _hWnd,                                     // parent window 
                                             (HMENU) ID_CHECKBOX_DISPLAY_LED_INDEX,     // control identifier
                                             _hInstance,                                // instance 
                                             NULL);                                     // no WM_CREATE parameter 
    setup();

    // Initialize the critical section
    InitializeCriticalSection(&m_CriticalSection);

    // Arduino Setup
    // Create a new thread which will start at the DoStuff function
    m_hThread = CreateThread(NULL,              // Thread attributes
                             0,                 // Stack size (0 = use default)
                             RunEngineThread,   // Thread start address
                             this,              // Parameter to pass to the thread
                             0,                 // Creation flags
                             NULL);             // Thread id
    m_bMustQuit = (m_hThread == NULL);
}

ArduinoSimulator::~ArduinoSimulator()
{
    m_bMustQuit = true;
    if (m_hThread != NULL)
    {
        // Wait for thread to finish execution
        WaitForSingleObject(m_hThread, INFINITE);

        // Thread handle must be closed when no longer needed
        CloseHandle(m_hThread);
    }
    DeleteCriticalSection(&m_CriticalSection);
    CEngine::KillInstance();
    if (m_pLeds != nullptr)
    {
        delete[]m_pLeds;
    }
    if (m_pInstance == this)
    {
        m_pInstance = nullptr;
    }
}

ArduinoSimulator* ArduinoSimulator::Instance()
{
    return m_pInstance;
}

void ArduinoSimulator::KillInstance()
{
    if (m_pInstance != nullptr)
    {
        delete m_pInstance;
    }
}

CRGB* ArduinoSimulator::GetLeds()
{
    return m_pLeds;
}

bool ArduinoSimulator::IsDisplayLedsNumber() const
{
    return m_bIsDisplayLedsNumber;
}

CRITICAL_SECTION & ArduinoSimulator::GetCriticalSection()
{
    return m_CriticalSection;
}

// Handles button notifications received 
VOID WINAPI ArduinoSimulator::ButtonNotification(WPARAM wParam,  // Notification (BN_CLICKED)
                                                 HWND hwnd)      // handle of window 
{
    if (HIWORD(wParam) == BN_CLICKED)
    {
        if (m_hWndDisplayLedsNumber == hwnd)
        {
            switch (IsDlgButtonChecked(m_hWnd, ID_CHECKBOX_DISPLAY_LED_INDEX))
            {
                case BST_CHECKED:
                {
                    m_bIsDisplayLedsNumber = true;
                    break;
                }
                case BST_UNCHECKED:
                {
                    m_bIsDisplayLedsNumber = false;
                    break;
                }
            }
        }
    }
}

// TBNotifications - handles trackbar notifications received 
// in the wParam parameter of WM_HSCROLL. This function simply 
// ensures that the slider remains within the selection range. 
VOID WINAPI ArduinoSimulator::TBNotifications(WPARAM wParam,  // wParam of WM_HSCROLL message 
                                              HWND hwndTrack, // handle of trackbar window 
                                              UINT iSelMin,   // minimum value of trackbar selection 
                                              UINT iSelMax)   // maximum value of trackbar selection 
{
    DWORD dwPos;    // current position of slider 

    switch (LOWORD(wParam))
    {
        case TB_THUMBTRACK:
        case TB_ENDTRACK:
        {
            if (m_hWndBrightness == hwndTrack || m_hWndAnimation == hwndTrack)
            {
                dwPos = (DWORD) SendMessage(hwndTrack, TBM_GETPOS, 0, 0);
                if (m_hWndBrightness == hwndTrack)
                {
                    ArduinoSimulator::WriteIntEEPROM(EEPROM_ADDRESS_CURRENT_BRIGHTNESS, dwPos);
                    m_iValueBrightness = dwPos;
                }
                else if (m_hWndAnimation == hwndTrack)
                {
                    ArduinoSimulator::WriteIntEEPROM(EEPROM_ADDRESS_CURRENT_ANIMATION, dwPos);
                    m_iValueAnimation = dwPos;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void ArduinoSimulator::init()
{
    int iNumLeds = CEngine::Instance().GetRealNumLeds();
    m_pLeds = new CRGB[iNumLeds];
    memset(m_pLeds, 0, iNumLeds * sizeof(CRGB)); // Set all to 0 (black)
    RECT rect = ComputeRectLeds(nullptr, nullptr, nullptr, nullptr);
    InvalidateRect(m_hWnd, &rect, false);
}

bool ArduinoSimulator::shouldQuit() const
{
    return m_bMustQuit;
}

void ArduinoSimulator::onShowPixels(PixelController<GRB>& _rPpixels)
{
    EnterCriticalSection(&m_CriticalSection);
    memmove8(m_pLeds, _rPpixels.mData, sizeof(CRGB) * _rPpixels.mLen);
    LeaveCriticalSection(&m_CriticalSection);

    RECT rect = ComputeRectLeds(nullptr, nullptr, nullptr, nullptr);
    InvalidateRect(m_hWnd, &rect, false);
}

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PIXELAMPEXTSIMU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIXELAMPEXTSIMU));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PIXELAMPEXTSIMU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PIXELAMPEXTSIMU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   new ArduinoSimulator(hInstance, hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case  WM_HSCROLL:
        {
            if (ArduinoSimulator::Instance() != nullptr)
            {
                // LPRAM is the handle of the slider
                ArduinoSimulator::Instance()->TBNotifications(wParam, (HWND) lParam, 0, 0);
            }
        }
        case WM_COMMAND:
        {
            if (lParam != 0)
            {
                int wmId = LOWORD(wParam);

                if (HIWORD(wParam) == BN_CLICKED)
                {
                    ArduinoSimulator::Instance()->ButtonNotification(wParam, (HWND) lParam);
                }

                // Analyse les sélections de menu :
                switch (wmId)
                {
                    case IDM_ABOUT:
                    {
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                        break;
                    }
                    case IDM_EXIT:
                    {
                        DestroyWindow(hWnd);
                        break;
                    }
                    default:
                    {
                        return DefWindowProc(hWnd, message, wParam, lParam);
                    }
                }
                break;
            }
            break;

        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // From 0 to 255
            double dBrightness = (FastLED.getBrightness() / (double) 255.0);
            const int WIDTH_LEFT_PANE = 210;

            int iNbLedsWidth  = CEngine::Instance().GetRealMatrixWidth();
            int iNbLedsHeight = CEngine::Instance().GetRealMatrixHeight();
            if (   ArduinoSimulator::Instance() != nullptr
                && ArduinoSimulator::Instance()->GetLeds() != nullptr)
            {
                EnterCriticalSection(&ArduinoSimulator::Instance()->GetCriticalSection());

                int iLeft = 0;
                int iTop = 0;
                int iWidth = 0;
                int iHeight = 0;
                RECT rectDisplay = ArduinoSimulator::Instance()->ComputeRectLeds(&iLeft, &iTop, &iWidth, &iHeight);
                RECT rect = {0, 0, 0, 0};
                for (int x = 0; x < iNbLedsWidth; ++x)
                {
                    for (int y = 0; y < iNbLedsHeight; ++y)
                    {
                        rect.left   = iLeft + (x * iWidth);
                        rect.right  = rect.left + iWidth - 1;
                        rect.top    = iTop + y * iHeight;
                        rect.bottom = rect.top + iHeight - 1;

                        int iLedIndex = CEngine::Instance().ComputePositionFromRealXY(x, y);
                        CEngine::eLedStatus status = CEngine::Instance().GetLedStatusFromRealPosition(x, y);

                        CRGB led(0);
                        switch (status)
                        {
                            case CEngine::eLedStatusNormal:
                            {
                                led = ArduinoSimulator::Instance()->GetLeds()[iLedIndex];
                                break;
                            }
                            case CEngine::eLedStatusIgnored:
                            {
                                led = ArduinoSimulator::Instance()->GetLeds()[iLedIndex];
                                if (x != 0 || y == 0)
                                {
                                    rect.left  += (long) (iWidth / 2.0);
                                    rect.right += (long) (iWidth / 2.0);
                                }
                                break;
                            }
                            case CEngine::eLedStatusNotExist:
                            {
                                continue;
                            }
                        }
                        HBRUSH brush = CreateSolidBrush(RGB(led.r * dBrightness + 0.5, led.g * dBrightness + 0.5, led.b * dBrightness + 0.5));
                        FillRect(hdc, &rect, brush);
                        if (status == CEngine::eLedStatusIgnored)
                        {
                            HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
                            HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                            MoveToEx(hdc, rect.left, rect.top, NULL);
                            LineTo(hdc, rect.right, rect.bottom);
                            MoveToEx(hdc, rect.right, rect.top, NULL);
                            LineTo(hdc, rect.left, rect.bottom);
                            SelectObject(hdc, oldPen);
                            DeleteObject(pen);
                        }
                        if (ArduinoSimulator::Instance()->IsDisplayLedsNumber())
                        {
                            // Display text
                            auto oldColor = SetTextColor(hdc, COLORREF(RGB(0, 255, 0)));
                            auto oldBkMode = SetBkMode(hdc, TRANSPARENT);
                            char szText[4];
                            szText[3] = 0;
                            _itoa_s(iLedIndex, szText, 10);
                            DrawTextA(hdc, szText, (int)strlen(szText), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            SetBkMode(hdc, oldBkMode);
                            SetTextColor(hdc, oldColor);
                        }

                        DeleteObject(brush);
                    }
                }

/* Code to verify where is the computed rect used to display leds
                HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
                HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                MoveToEx(hdc, rectDisplay.left, rectDisplay.top, NULL);
                LineTo(hdc, rectDisplay.left, rectDisplay.bottom);
                LineTo(hdc, rectDisplay.right, rectDisplay.bottom);
                LineTo(hdc, rectDisplay.right, rectDisplay.top);
                LineTo(hdc, rectDisplay.left, rectDisplay.top);
                SelectObject(hdc, oldPen);
                DeleteObject(pen);
*/
                LeaveCriticalSection(&ArduinoSimulator::Instance()->GetCriticalSection());
            }

            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY:
        {
            ArduinoSimulator::KillInstance();
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


/* Vector tests
class A
{
public:
    int m_iToto;

    A(int _iToto);
    A(const A& _rA);
    A(A&& _rA);
    ~A();

    A& operator=(const A& _rA);
};

A::A(int _iToto)
{
    m_iToto = _iToto;
}

A::A(const A& _rA)
    : m_iToto(_rA.m_iToto)
{
    int h = 0;
    ++h;
}

A::A(A&& _rA)
    : m_iToto(_rA.m_iToto)
{
    int h = 0;
    ++h;
}

A::~A()
{
    m_iToto = 0;
}

A& A::operator=(const A& _rA)
{
    m_iToto = _rA.m_iToto;
    return *this;
}

{
    vector<shared_ptr<A>> vectoData;
    vectoData.push_back(new A(10));
    vectoData.push_back(new A(20));
    vectoData.push_back(new A(30));
    vectoData.push_back(new A(40));
    vectoData.pop_back();
    vectoData.push_back(new A(50));
    vectoData.pop_back();
    vectoData.pop_back();
    vectoData.pop_back();
}
*/

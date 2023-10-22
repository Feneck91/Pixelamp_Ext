// pixelampextsimu.cpp : Définit le point d'entrée de l'application.
//

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

#define     ID_TRACKBAR     (WM_USER + 100)

// CreateTrackbar - creates and initializes a trackbar. 
// 
// Global variable
//     g_hinst - instance handle
//
HWND WINAPI CreateTrackbar(
    HINSTANCE hInst,
    HWND hwndDlg,  // handle of dialog box (parent window) 
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
        NULL,                                                           // control identifier 
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

class ArduinoSimulator : public ISimulatorController<GRB>
{
private:
    HWND                        m_hWnd;
    static ArduinoSimulator *   m_pInstance;
    unsigned __int64            m_nIDTimerEvent;
    HWND                        m_hWndBrightness;
    HWND                        m_hWndAnimation;
    int                         m_iValueBrightness;
    int                         m_iValueAnimation;

private:
    virtual ~ArduinoSimulator();
public:
    CRGB *                      m_pLeds;

    /// <summary>
    /// Constructor.
    /// </summary>
    /// <param name="_hInstance">Instance of application</param>
    /// <param name="_hwnd">Handle of the parent window</param>
    ArduinoSimulator(HINSTANCE _hInstance, HWND _hwnd);

    virtual void                init() override;
    virtual void                showPixels(PixelController<GRB>& pixels) override;
    void                        OnTimer();

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
    /// TBNotifications - handles trackbar notifications received 
    //  in the wParam parameter of WM_HSCROLL. This function simply  ensures that the slider remains within the selection range.
    /// </summary>
    /// <param name="wParam">wParam of WM_HSCROLL message</param>
    /// <param name="hwndTrack">handle of trackbar window</param>
    /// <param name="iSelMin">minimum value of trackbar selection</param>
    /// <param name="iSelMax">maximum value of trackbar selection</param>
    VOID WINAPI                 TBNotifications(WPARAM wParam, HWND hwndTrack, UINT iSelMin, UINT iSelMax);

    static int                  ReadByteEEPROM(int idx); // 1 address
    static int                  ReadIntEEPROM(int idx);  // 2 address
    static void                 WriteIntEEPROM(int idx, int _iValue);
};

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
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
    ArduinoSimulator::Instance()->OnTimer();
}

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

ArduinoSimulator::ArduinoSimulator(HINSTANCE _hInstance, HWND _hWnd)
    : m_hWnd(_hWnd)
{
    m_pInstance = this;
    m_pLeds = nullptr;

    // Create engine instance.
    DEFAULT_CHIPSET<DEFAULT_LED_DATA_PIN, DEFAULT_COLOR_ORDER>::SetSimulator(this);
    setAnalogReadFunction(this, ArduinoSimulator::StaticGetAnalogValue);

    int iValueBrightness  = ArduinoSimulator::ReadIntEEPROM(EEPROM_ADDRESS_CURRENT_BRIGHTNESS);
    m_iValueBrightness = (ArduinoSimulator::ReadByteEEPROM(EEPROM_ADDRESS_INVERT_POT_BRIGHTNESS) == 1)
        ? 1023 - iValueBrightness
        : iValueBrightness;

    int iValueAnimation  = ArduinoSimulator::ReadIntEEPROM(EEPROM_ADDRESS_CURRENT_ANIMATION);
    m_iValueAnimation = (ArduinoSimulator::ReadByteEEPROM(EEPROM_ADDRESS_INVERT_POT_ANIMATION) == 1)
        ? 1023 - iValueAnimation
        : iValueAnimation;

    m_hWndBrightness = CreateTrackbar(_hInstance, _hWnd, iValueBrightness, 0, 20, 0, 1023, 0, 0);
    m_hWndAnimation  = CreateTrackbar(_hInstance, _hWnd, iValueAnimation,  0, 100, 0, 1023, 0, 0);

    // Arduino Setup
    setup();

    m_nIDTimerEvent = 0;
    if (0 == SetTimer(m_hWnd, m_nIDTimerEvent, 10, (TIMERPROC)&TimerProc))
    {
        m_nIDTimerEvent = 0;
    }
}

ArduinoSimulator::~ArduinoSimulator()
{
    if (m_nIDTimerEvent != 0)
    {
        KillTimer(m_hWnd, m_nIDTimerEvent);
    }
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

void ArduinoSimulator::OnTimer()
{
    loop();
}

void ArduinoSimulator::init()
{
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
                    m_iValueBrightness = CEngine::Instance().IsInvertPotBrightness()
                        ? 1023 - dwPos
                        : dwPos;
                }
                else if (m_hWndAnimation == hwndTrack)
                {
                    ArduinoSimulator::WriteIntEEPROM(EEPROM_ADDRESS_CURRENT_ANIMATION, dwPos);
                    m_iValueAnimation = CEngine::Instance().IsInvertPotAnimation()
                        ? 1023 - dwPos
                        : dwPos;
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

void ArduinoSimulator::showPixels(PixelController<GRB>& pixels)
{
    if (m_pLeds != nullptr)
    {
        delete[]m_pLeds;
    }
    m_pLeds = new CRGB[CEngine::Instance().GetNumLeds()];
    memmove8(m_pLeds, CEngine::Instance().GetLeds(), sizeof(CRGB) * CEngine::Instance().GetNumLeds());
    InvalidateRect(m_hWnd, nullptr, false);
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
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // Dessiner les leds
            RECT rect;
            GetClientRect(hWnd, &rect);

            int iLeft = 0;

            // From 0 to 255
            double dBrightness = (FastLED.getBrightness() / (double) 255.0);

            if (   ArduinoSimulator::Instance() != nullptr
                && ArduinoSimulator::Instance()->m_pLeds != nullptr)
            {
                int iWidth  = (rect.right - rect.left - 200) / 17;
                int iHeight = (rect.bottom - rect.top) / 9;

                if (iWidth > iHeight)
                {
                    iLeft = iWidth - iHeight + 200;
                }
                else
                {
                    iLeft = 200;
                }
                iWidth = iHeight = min(iWidth, iHeight);

                RECT rect = {0, 0, 0, 0};
                for (int x = 0; x < 16; ++x)
                {
                    for (int y = 0; y < 8; ++y)
                    {
                        rect.left   = iLeft + (x * iWidth);
                        rect.right  = rect.left + iWidth - 1;
                        rect.top    = (7 - y) * iHeight;
                        rect.bottom = rect.top + iHeight - 1;

                        int iYIndex = y;
                        if (x % 2 == 1)
                        {
                            iYIndex = 7 - iYIndex;
                        }
                        auto led = ArduinoSimulator::Instance()->m_pLeds[iYIndex + x * 8];
                        HBRUSH brush = CreateSolidBrush(RGB(led.r * dBrightness + 0.5, led.g * dBrightness + 0.5, led.b * dBrightness + 0.5));
                        FillRect(hdc, &rect, brush);
                        DeleteObject(brush);
                    }
                }
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


/* Test sur les vecteurs
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
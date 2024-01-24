#include "../include/animationfx.h"
#include "../include/engine.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                                CAnimationFx
// 
//========================================================================

CAnimationFx::CAnimationFx()
    : m_ui8CurrentAnimation(0)
{
}

CAnimationFx::~CAnimationFx()
{
    m_pFxHueDatas = nullptr;
}

bool CAnimationFx::Loop()
{
    m_pFxHueDatas->Animate();
    return false; // Always false
}

void CAnimationFx::Enter()
{
    m_pFxHueDatas = new CFxHueRotationDatas(m_ui8CurrentAnimation);
}

void CAnimationFx::Leave()
{
    m_pFxHueDatas = nullptr;
}
 
uint16_t CAnimationFx::GetMillisecondWait()
{
    return 10;
}

uint16_t CAnimationFx::GetNumberAnimations()
{
    return 2;
}

void CAnimationFx::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
    m_ui8CurrentAnimation = (uint8_t) _uiCurrentAnimation; // No more 256 differents animations
    if (m_pFxHueDatas.IsNotNull())
    {
        m_pFxHueDatas->SetCurrentAnimation(m_ui8CurrentAnimation);
    }
}
//========================================================================
// 
//                        CAnimationFx::CFxHueRotationDatas
// 
//========================================================================
CAnimationFx::CFxHueRotationDatas::CFxHueRotationDatas(uint8_t _ui8CurrentAnimation)
{
    Initialize();
    SetCurrentAnimation(_ui8CurrentAnimation);
}

CAnimationFx::CFxHueRotationDatas::~CFxHueRotationDatas()
{
}

void CAnimationFx::CFxHueRotationDatas::Initialize()
{
    m_uiHue = 0;
    m_funcAnimation = nullptr;
}

void CAnimationFx::CFxHueRotationDatas::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    switch (_uiCurrentAnimation)
    {
        case 0:
        {
            m_uiHue = 0;
            m_funcAnimation = &CAnimationFx::CFxHueRotationDatas::AnimateHueRotation;
            break;
        }
        case 1:
        {
            m_funcAnimation = &CAnimationFx::CFxHueRotationDatas::AnimateHue;
            break;
        }
    }
}

void CAnimationFx::CFxHueRotationDatas::Animate()
{
    if (m_funcAnimation != nullptr)
    {
        (this->*m_funcAnimation)();
    }

    // Done. Bring it on!
    FastLED.show();
}

void CAnimationFx::CFxHueRotationDatas::AnimateHue()
{
    uint8_t kMatrixHeight = CEngine::Instance().GetMatrixHeight();
    uint8_t kMatrixWidth = CEngine::Instance().GetMatrixWidth();

    for (uint8_t x = 0; x < kMatrixWidth; x++)
    {
        uint16_t uiLedPosition = CEngine::Instance().ComputePositionFromXY(true, x, x % 2 == 0 ? kMatrixHeight - 1 : 0, false, false);
        //fill_solid(&CEngine::Instance().GetLeds()[x * kMatrixHeight], kMatrixHeight, CHSV(m_uiHue + (kMatrixHeight * x), 255, 255));
        fill_solid(&CEngine::Instance().GetLeds()[uiLedPosition], kMatrixHeight, CHSV(m_uiHue + (kMatrixHeight * x), 255, 255));
    }
    ++m_uiHue;
}

void CAnimationFx::CFxHueRotationDatas::AnimateHueRotation()
{
    uint32_t ms = millis() % UINT32_MAX;
    int32_t yHueDelta32 = ((int32_t) cos16(ms * (27 / 1)) * (350 / CEngine::Instance().GetMatrixWidth()));
    int32_t xHueDelta32 = ((int32_t) cos16(ms * (39 / 1)) * (310 / CEngine::Instance().GetMatrixHeight()));
    DrawOneFrameHueRotation(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
}

void CAnimationFx::CFxHueRotationDatas::DrawOneFrameHueRotation(byte _byStartHue8, int8_t _iYHueDelta8, int8_t _iXHueDelta8)
{
    uint8_t kMatrixHeight = CEngine::Instance().GetMatrixHeight();
    uint8_t kMatrixWidth  = CEngine::Instance().GetMatrixWidth();
    byte lineStartHue = _byStartHue8;
    for (led_coordinate y = 0; y < kMatrixHeight; ++y)
    {
        lineStartHue += _iYHueDelta8;
        byte pixelHue = lineStartHue;
        for (led_coordinate x = 0; x < kMatrixWidth; ++x)
        {
            pixelHue += _iXHueDelta8;
            CEngine::Instance().SetLedColor(true, x, y, true, true, CHSV(pixelHue, 255, 255));
        }
    }
}

#include "../include/animationlight.h"
#include "../include/engine.h"

#define MAX_NUMBER_ANIMATION    1020.0

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                              CAnimationLight
// 
//========================================================================
CAnimationLight::CAnimationLight(bool _bIsCheckerBoard)
    : m_color(255, 255, 255)
    , m_bIsCheckerBoard(_bIsCheckerBoard)
{
}

CAnimationLight::~CAnimationLight()
{
}

bool CAnimationLight::Loop()
{
    if (m_bIsCheckerBoard)
    {
        auto rgbBlack = CRGB(0, 0, 0);
        for (uint8_t uiX = 0; uiX < CEngine::Instance().GetMatrixWidth(); ++uiX)
        {
            for (uint8_t uiY = 0; uiY < CEngine::Instance().GetMatrixWidth(); ++uiY)
            {
                CEngine::Instance().SetLedColor(true, uiX, uiY, true, true, (uiX + uiY) % 2 == 0 ? m_color : rgbBlack);
            }
        }
    }
    else
    {
        CEngine::Instance().SetAllLedsColor(m_color);
    }
    FastLED.show();
    return false; // Always false
}

void CAnimationLight::Enter()
{
}

void CAnimationLight::Leave()
{
}

uint16_t CAnimationLight::GetNumberAnimations()
{
    return (uint16_t) (MAX_NUMBER_ANIMATION + 0.5);
}

void CAnimationLight::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    double dCurrentRatioAnimation = ((double)_uiCurrentAnimation) / MAX_NUMBER_ANIMATION;

    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
    m_color.setRGB((uint8_t)((uint32_t)((256.0 * dCurrentRatioAnimation))) % 256,
                   (uint8_t)((uint32_t)((3 * 256.0 * dCurrentRatioAnimation))) % 256,
                   (uint8_t)((uint32_t)((9 * 256.0 * dCurrentRatioAnimation))) % 256
                  );

    // To don't have break in colors, go from 0 -> 255 and then 255 -> 0 (and not 0 -> 255 again) each 3 (for green) / 9 (for blue)
    if ((((uint32_t)(3.0 * dCurrentRatioAnimation)) % 2) == 1)
    {
        m_color.g = 255 - m_color.g;
    }
    if ((((uint32_t)(9.0 * dCurrentRatioAnimation)) % 2) == 1)
    {
        m_color.b = 255 - m_color.b;
    }
}

uint16_t CAnimationLight::GetMillisecondWait()
{
    return 100;
}

bool CAnimationLight::ShouldEraseBetweenAnimations() const
{
    return false;
}

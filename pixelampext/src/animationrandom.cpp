#include "../include/animationrandom.h"
#include "../include/engine.h"

#define MAX_NUMBER_WAIT_TIME    1020.0  // Max different times
#define RANDOM_MIN_TIME              2  // In second
#define RANDOM_MAX_TIME            600  // In second

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                              CAnimationRandom
// 
//========================================================================
CAnimationRandom::CAnimationRandom(String _strModeText, CRGB _crTextColor, CAnimationBase* _pAnimation, ...)
    : CAnimationMode(_strModeText, _crTextColor)
    , m_ui32AnimationDuration(10)
    , m_lastTimeStartAnimation(millis())
    , m_uiCurrentAnimation(0)
{
    va_list parametersInfos;
    // Initialize the va_list structure
    va_start(parametersInfos, _crTextColor);
    InitAnimations(_pAnimation, parametersInfos);
    va_end(parametersInfos);
}

CAnimationRandom::~CAnimationRandom()
{
}

bool CAnimationRandom::Loop()
{
    if (!IsDisplayingMode() && millis() - m_lastTimeStartAnimation > m_ui32AnimationDuration)
    {
        ChangeAnimation();
    }
    return CAnimationMode::Loop();
}

uint16_t CAnimationRandom::GetNumberAnimations()
{
    return (uint16_t) (MAX_NUMBER_WAIT_TIME + 0.5);
}

/// <summary>
/// Called when text animation ended.
/// 
/// Does nothing but it's better to call it on derived class.
/// </summary>
void CAnimationRandom::OnEndDisplayModeAnimation()
{
    m_uiCurrentAnimation = UINT16_MAX; // Force changing
    ChangeAnimation();
}

void CAnimationRandom::ChangeAnimation()
{
    uint16_t ui16NumMaxAnimation = CAnimationMode::GetNumberAnimations();
    uint16_t uiAnimationToChoose;
    do
    {
        uiAnimationToChoose = random16(ui16NumMaxAnimation);
    } while (m_uiCurrentAnimation == uiAnimationToChoose && ui16NumMaxAnimation > 1); // Never take twice the same animation (if not more 2 animation, leave the iteration else it will block here)
    m_uiCurrentAnimation = uiAnimationToChoose;
    CAnimationMode::SetCurrentAnimation(m_uiCurrentAnimation);
    m_lastTimeStartAnimation = millis();
}

void CAnimationRandom::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    m_ui32AnimationDuration = (uint32_t) (1000.0 * (RANDOM_MIN_TIME + (RANDOM_MAX_TIME - RANDOM_MIN_TIME) * (_uiCurrentAnimation / MAX_NUMBER_WAIT_TIME)) + 0.5);
}

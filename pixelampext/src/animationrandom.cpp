#include "../include/animationrandom.h"
#include "../include/engine.h"

#define MAX_NUMBER_WAIT_TIME      20.0  // Max different times
#define RANDOM_MIN_TIME             10  // In second
#define RANDOM_MAX_TIME            600  // In second
#define RANDOM_STEP_MIN             10  // In second

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
    if (!IsDisplayingMode())
    {
        if (m_pAnimationDisplayDuration.IsNotNull())
        {
            if (m_pAnimationDisplayDuration->Loop())
            {
                m_pAnimationDisplayDuration = nullptr; // If m_pAnimationDisplayDuration is not null, it will be deleted, no need to call Leave()
                ChangeAnimation();
            }
            else
            {
                return false;
            }
        }
        else if (millis() - m_lastTimeStartAnimation > m_ui32AnimationDuration)
        {
            ChangeAnimation();
        }
    }

    return CAnimationMode::Loop();
}

void CAnimationRandom::Leave()
{
    // Call base class
    CAnimationMode::Leave();
    m_pAnimationDisplayDuration = nullptr; // If m_pAnimationDisplayDuration is not null, it will be deleted, no need to call Leave()
}

uint16_t CAnimationRandom::GetMillisecondWait()
{
    if (!IsDisplayingMode() && m_pAnimationDisplayDuration.IsNotNull())
    {
        return m_pAnimationDisplayDuration->GetMillisecondWait();
    }
    return CAnimationMode::GetMillisecondWait();
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

uint32_t GetClosestNumber(uint32_t _ui32Value, uint32_t _ui32Step)
{
    // Determing the numbers on either side of value
    uint32_t ui32Low = _ui32Value - (_ui32Value % _ui32Step);
    uint32_t ui32High = ui32Low + _ui32Step;

    // Return the closest one
    return  _ui32Value - ui32Low < ui32High - _ui32Value ? ui32Low : ui32High;
}

String FromValue(uint32_t _ui32Value)
{
#ifdef SIMULATION
    // From std, this function does not exists in Arduino
    return std::to_string(_ui32Value);
#else
    // From std, this function does not exists in Arduino
    return String(_ui32Value);
#endif
}

void CAnimationRandom::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    uint32_t ui32AnimationDuration = GetClosestNumber((uint32_t)(1000.0 * (RANDOM_MIN_TIME + (RANDOM_MAX_TIME - RANDOM_MIN_TIME) * (MAX_NUMBER_WAIT_TIME * (_uiCurrentAnimation / (MAX_NUMBER_WAIT_TIME - 1.0))/ MAX_NUMBER_WAIT_TIME)) + 0.5), RANDOM_STEP_MIN * 1000);
    if (m_ui32AnimationDuration != ui32AnimationDuration)
    {
        m_ui32AnimationDuration = ui32AnimationDuration;
        if (!IsDisplayingMode())
        {   // If animation displaying mode is running, don't display duration
            m_pAnimationDisplayDuration = new CAnimationText(FromValue(m_ui32AnimationDuration / 1000), CRGB::MediumSlateBlue);
            m_pAnimationDisplayDuration->Enter();
        }
    }
}

bool CAnimationRandom::ShouldEraseBetweenAnimations() const
{
    return !IsDisplayingMode() && m_pAnimationDisplayDuration.IsNotNull()
        ? m_pAnimationDisplayDuration->ShouldEraseBetweenAnimations()
        : CAnimationMode::ShouldEraseBetweenAnimations();
}

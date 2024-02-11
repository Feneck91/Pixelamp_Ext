#include "../include/animationmode.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                          CAnimationMode
// 
//========================================================================
CAnimationMode::CAnimationMode(String _strModeText, CRGB _crTextColor)
    : m_strModeText(_strModeText)
    , m_crTextColor(_crTextColor)
    , m_bDisplayMode(true)
{
}

CAnimationMode::CAnimationMode(String _strModeText, CRGB _crTextColor, CAnimationBase * _pAnimation, ...)
    : CAnimationMode(_strModeText, _crTextColor)
{
    va_list parametersInfos;
    // Initialize the va_list structure
    va_start(parametersInfos, _crTextColor);
    InitAnimations(parametersInfos);
    va_end(parametersInfos);
}

CAnimationMode::CAnimationMode(const CAnimationMode& _rAnimationMode)
    : m_strModeText(_rAnimationMode.m_strModeText)
    , m_crTextColor(_rAnimationMode.m_crTextColor)
    , m_bDisplayMode(_rAnimationMode.m_bDisplayMode)
    , m_pAnimationDisplayMode(_rAnimationMode.m_pAnimationDisplayMode)
    , m_arrAnimations(_rAnimationMode.m_arrAnimations)
{
}

CAnimationMode::~CAnimationMode()
{
    m_pAnimationDisplayMode = nullptr;
}

bool CAnimationMode::IsDisplayingMode() const
{
    return m_bDisplayMode;
}

void CAnimationMode::InitAnimations(va_list _argsAnimations)
{
    CAnimationBase* pAnimation = nullptr;
    do
    {
        pAnimation = (CAnimationBase*) va_arg(_argsAnimations, CAnimationBase*);
        if (pAnimation != nullptr)
        {
            m_arrAnimations.push_back(shared_ptr<CAnimationBase>(pAnimation));
        }
    }
    while (pAnimation != nullptr);
}

bool CAnimationMode::Loop()
{
    if (IsDisplayingMode())
    {
        if (m_pAnimationDisplayMode->Loop())
        {
            m_bDisplayMode = false;
            m_pAnimationDisplayMode->Leave();
            m_pAnimationDisplayMode = nullptr;
            OnEndDisplayModeAnimation();
            if (m_pCurrentAnimation.IsNotNull())
            {
                m_pCurrentAnimation->Enter();
            }
        }
    }
    else if (m_pCurrentAnimation.IsNotNull())
    {
        m_pCurrentAnimation->Loop();
    }

    return false; // Always false
}

void CAnimationMode::Enter()
{
    // Call base class
    CAnimationBase::Enter();
    m_pAnimationDisplayMode = new CAnimationText(m_strModeText, m_crTextColor);
    m_pAnimationDisplayMode->Enter();
    m_bDisplayMode = true;
}

void CAnimationMode::Leave()
{
    // Call base class
    CAnimationBase::Leave();

    if (m_pAnimationDisplayMode.IsNotNull())
    {
        m_pAnimationDisplayMode->Leave();
    }
    if (m_pCurrentAnimation.IsNotNull())
    {
        m_pCurrentAnimation->Leave();
    }

    m_pAnimationDisplayMode = nullptr;
    m_pCurrentAnimation = nullptr;
}

uint16_t CAnimationMode::GetMillisecondWait()
{
    if (IsDisplayingMode())
    {
        return m_pAnimationDisplayMode->GetMillisecondWait();
    }
    else if (m_pCurrentAnimation.IsNotNull())
    {
        return m_pCurrentAnimation->GetMillisecondWait();
    }
    return 1000;
}

uint16_t CAnimationMode::GetNumberAnimations()
{
    uint16_t uiNumber = CAnimationBase::GetNumberAnimations();

    for (auto it = m_arrAnimations.begin(); it < m_arrAnimations.end(); ++it)
    {
        uiNumber += (*it)->GetNumberAnimations();
    }

    return uiNumber;
}

void CAnimationMode::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);

    for (auto it = m_arrAnimations.begin(); it < m_arrAnimations.end(); ++it)
    {
        auto uiNumberAnimation = (*it)->GetNumberAnimations();
        if (uiNumberAnimation <= _uiCurrentAnimation)
        {   // Find the correct index animation when several animations are used
            _uiCurrentAnimation -= uiNumberAnimation;
        }
        else
        {
            if (!IsDisplayingMode() && m_pCurrentAnimation.IsNotNull())
            {
                m_pCurrentAnimation->Leave();
            }
            m_pCurrentAnimation = *it;
            if (!IsDisplayingMode() && m_pCurrentAnimation.IsNotNull())
            {
                m_pCurrentAnimation->Enter();
            }
            m_pCurrentAnimation->SetCurrentAnimation(_uiCurrentAnimation);
            break;
        }
    }
}

bool CAnimationMode::ShouldEraseBetweenAnimations()
{
    return IsDisplayingMode()
        ? false
        : (!IsDisplayingMode() && m_pCurrentAnimation.IsNotNull()
            ? m_pCurrentAnimation->ShouldEraseBetweenAnimations()
            : CAnimationBase::ShouldEraseBetweenAnimations()
          );
}

void CAnimationMode::OnEndDisplayModeAnimation()
{
}

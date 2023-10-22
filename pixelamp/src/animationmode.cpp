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
CAnimationMode::CAnimationMode(String _strModeText, CRGB _crTextColor, CAnimationBase * _pAnimation, ...)
    : m_strModeText(_strModeText)
    , m_crTextColor(_crTextColor)
    , m_bDisplayMode(true)
{
    va_list parametersInfos;
    // Initialize the va_list structure
    va_start(parametersInfos, _pAnimation);
    while (_pAnimation != nullptr)
    {
        m_arrAnimations.push_back(shared_ptr<CAnimationBase>(_pAnimation));
        _pAnimation = (CAnimationBase *) va_arg(parametersInfos, CAnimationBase *);
    }
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

bool CAnimationMode::Loop()
{
    if (m_bDisplayMode)
    {
        if (m_pAnimationDisplayMode->Loop())
        {
            m_bDisplayMode = false;
            m_pAnimationDisplayMode->Leave();
            m_pAnimationDisplayMode = nullptr;
            if (m_pCurrentAnimation.IsNotNull())
            {
                m_pCurrentAnimation->Enter();
            }
        }
    }
    else
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
    if (m_bDisplayMode)
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
        {
            _uiCurrentAnimation -= uiNumberAnimation;
        }
        else
        {
            if (!m_bDisplayMode && m_pCurrentAnimation.IsNotNull())
            {
                m_pCurrentAnimation->Leave();
            }
            m_pCurrentAnimation = *it;
            if (!m_bDisplayMode && m_pCurrentAnimation.IsNotNull())
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
    return m_bDisplayMode
        ? false
        : (!m_bDisplayMode && m_pCurrentAnimation.IsNotNull()
            ? m_pCurrentAnimation->ShouldEraseBetweenAnimations()
            : CAnimationBase::ShouldEraseBetweenAnimations()
          );
}

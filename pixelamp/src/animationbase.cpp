#include "../include/animationbase.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

// Constructor.
CAnimationBase::CAnimationBase()
{
}

CAnimationBase::~CAnimationBase()
{
}

bool CAnimationBase::Loop()
{
    return false;
}

void CAnimationBase::Enter()
{
}

void CAnimationBase::Leave()
{
}

uint16_t CAnimationBase::GetMillisecondWait()
{
    return 0;
}

uint16_t CAnimationBase::GetNumberAnimations()
{
    // For the moment, nothing to do, return 0
    return 0;
}

void CAnimationBase::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    // For the moment, nothing to do
}

bool CAnimationBase::ShouldEraseBetweenAnimations()
{
    return true;
}

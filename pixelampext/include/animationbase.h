#pragma once

#ifdef SIMULATION
    #include "../simu_include/FastLEDSimu.h"
    #include <cstdarg>
#else
    #include <FastLED.h>
    #include <WString.h>
    #include <EEPROM.h>
#endif

// Check we are using GDB Debugging and include the additional code
#ifdef VM_DEBUG_GDB
    #include "avr8-stub.h"
#endif

#include "shared_ptr.h"

/// <summary>
/// Base class for all animations.
/// </summary>
class CAnimationBase : public IRefCountImpl<CAnimationBase>
{
public:
    CAnimationBase();
    virtual ~CAnimationBase();

    /// <summary>
    /// Make the animation.
    /// 
    /// You must call the base class (do nothing but may be if the engine change, it can be used).
    /// </summary>
    /// <returns>true if done, false else.</returns>
    virtual bool                            Loop() = 0;

    /// <summary>
    /// Run into animation.
    /// 
    /// Prepare animation.
    /// You must call the base class (do nothing but may be if the engine change, it can be used).
    /// </summary>
    virtual void                            Enter() = 0;

    /// <summary>
    /// Quit animation.
    /// 
    /// Used to release and delete all memory not in use.
    /// You must call the base class (do nothing but may be if the engine change, it can be used).
    /// </summary>
    virtual void                            Leave() = 0;

    /// <summary>
    /// Get the time between 2 frames.
    /// 
    /// You must call the base class (do nothing but may be if the engine change, it can be used).
    /// </summary>
    /// <returns>The number of millisecond between 2 frames (2 calls to Loop function).</returns>
    virtual uint16_t                        GetMillisecondWait() = 0;

    /// <summary>
    /// Get the number of animations.
    /// 
    /// You must call the base class (do nothing but may be if the engine change, it can be used).
    /// </summary>
    /// <returns>The number of differents animation supported by this class.</returns>
    virtual uint16_t                        GetNumberAnimations() = 0;

    /// <summary>
    /// Set the current animation.
    /// 
    /// You must call the base class (do nothing but may be if the engine change, it can be used).
    /// </summary>
    /// <param name="_uiCurrentAnimation">The animation to set.</param>
    virtual void                            SetCurrentAnimation(uint16_t _uiCurrentAnimation) = 0;

    /// <summary>
    /// Should erade all led when animation change?
    /// </summary>
    /// <returns>Return true by default.</returns>
    virtual bool                            ShouldEraseBetweenAnimations();
};

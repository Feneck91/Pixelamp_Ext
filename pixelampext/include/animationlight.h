#pragma once
#include "animationbase.h"

/// <summary>
/// Class that manage lights.
/// </summary>
class CAnimationLight : public CAnimationBase
{
private:
    /// <summary>
    /// Current color.
    /// </summary>
    CRGB                                    m_color;

    /// <summary>
    /// Indicate if is checkboard, used for test.
    /// </summary>
    bool                                    m_bIsCheckerBoard;

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    /// <param name="_bIsCheckerBoard">Indicate if is a checker board or not.</param>
    CAnimationLight(bool _bIsCheckerBoard = false);

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationLight() override;

    /// <summary>
    /// Manage current animation mode.
    /// </summary>
    /// <returns>true if done, false else.</returns>
    virtual bool                            Loop() override;

    /// <summary>
    /// Run into animation.
    /// 
    /// Prepare animation.
    /// </summary>
    virtual void                            Enter() override;

    /// <summary>
    /// Quit animation.
    /// 
    /// Used to release and delete all memory not in use.
    /// </summary>
    virtual void                            Leave() override;

    /// <summary>
    /// Get the time between 2 frames.
    /// </summary>
    /// <returns>The number of millisecond between 2 frames (2 calls to Loop function).</returns>
    virtual uint16_t                        GetMillisecondWait() override;

    /// <summary>
    /// Get the number of animations.
    /// </summary>
    /// <returns>The number of differents animation supported by this class.</returns>
    virtual uint16_t                        GetNumberAnimations() override;

    /// <summary>
    /// Set the current animation.
    /// 
    /// You must call the base class (do nothing but may be if the engine change, it can be used).
    /// </summary>
    /// <param name="_uiCurrentAnimation">The animation to set.</param>
    virtual void                            SetCurrentAnimation(uint16_t _uiCurrentAnimation) override;

    /// <summary>
    /// Should erade all led when animation change?
    /// </summary>
    /// <returns>Return true by default.</returns>
    virtual bool                            ShouldEraseBetweenAnimations() const override;
};

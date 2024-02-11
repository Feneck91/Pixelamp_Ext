#pragma once
#include "animationmode.h"

/// <summary>
/// Class that manage random animations.
/// </summary>
class CAnimationRandom : public CAnimationMode
{
private:
    /// <summary>
    /// The duration of each random animation.
    /// </summary>
    uint32_t                                m_ui32AnimationDuration;

    /// <summary>
    /// Record last time the animation start, used to know when to change animation.
    /// </summary>
    unsigned long                           m_lastTimeStartAnimation;

    /// <summary>
    /// Current animation running.
    /// 
    /// Used to force change animation after a while independantly of random number generator.
    /// Because random number generator can give several time the same number.
    /// </summary>
    uint16_t                                m_uiCurrentAnimation;
public:
    /// <summary>
    /// Constructor.
    /// </summary>
    /// <param name="_strModeText">Text displayed for this mode.</param>
    /// <param name="_crTextColor">Text color</param>
    /// <param name="_pAnimation">All animations for this mode.</param>
    /// <param name="">More animations</param>
    CAnimationRandom(String _strModeText, CRGB _crTextColor, CAnimationBase* _pAnimation, ...);

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationRandom() override;

    /// <summary>
    /// Manage current animation mode.
    /// </summary>
    /// <returns>true if done, false else.</returns>
    virtual bool                            Loop() override;

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
protected:
    /// <summary>
    /// Called when text animation ended (title of animation mode).
    /// 
    /// Compute new animation to start.
    /// </summary>
    virtual void                            OnEndDisplayModeAnimation() override;

    /// <summary>
    /// Change the current animation.
    /// </summary>
    void                                    ChangeAnimation();
};

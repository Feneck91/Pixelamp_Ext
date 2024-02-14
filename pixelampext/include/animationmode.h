#pragma once
#include "animationtext.h"

/// <summary>
/// Class that manage all animations and modes.
/// </summary>
class CAnimationMode : public CAnimationBase
{
private:
    /// <summary>
    /// Text mode.
    /// </summary>
    String                                  m_strModeText;

    /// <summary>
    /// Text color.
    /// </summary>
    CRGB                                    m_crTextColor;

    /// <summary>
    /// Animations array.
    /// </summary>
    vector<shared_ptr<CAnimationBase>>      m_arrAnimations;

    /// <summary>
    /// Is mode is currently displayed?
    /// 
    /// Else it's animation that's running.
    /// </summary>
    bool                                    m_bDisplayMode;

    /// <summary>
    /// Used to display text.
    /// </summary>
    shared_ptr<CAnimationBase>              m_pAnimationDisplayMode;

    /// <summary>
    /// Current animation.
    /// </summary>
    shared_ptr<CAnimationBase>              m_pCurrentAnimation;

protected:
    /// <summary>
    /// Constructor.
    /// 
    /// Protected, reserved for this clas and derived one.
    /// Never call it directly because it doesn't contains animation.
    /// </summary>
    /// <param name="_strModeText">Text displayed for this mode.</param>
    /// <param name="_crTextColor">Text color</param>
    CAnimationMode(String _strModeText, CRGB _crTextColor);

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    /// <param name="_strModeText">Text displayed for this mode.</param>
    /// <param name="_crTextColor">Text color</param>
    /// <param name="_pAnimation">All animations for this mode.</param>
    /// <param name="">More animations</param>
    CAnimationMode(String _strModeText, CRGB _crTextColor, CAnimationBase* _pAnimation, ...);

    /// <summary>
    /// Copy constructor.
    /// </summary>
    /// <param name="_rAnimationMode">Animation mode to copy into this.</param>
    CAnimationMode(const CAnimationMode& _rAnimationMode);

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationMode() override;

protected:
    /// <summary>
    /// Initialize the animation from va_list argument.
    /// </summary>
    /// <param name="_pFirstAnimation">First animation (mandatory at least one).</param>
    /// <param name="_argsAnimations">va_list argument to get all animations.</param>
    void                                    InitAnimations(CAnimationBase* _pFirstAnimation, va_list _argsAnimations);

    /// <summary>
    /// Called when text animation ended.
    /// 
    /// Does nothing but it's better to call it on derived class.
    /// </summary>
    virtual void                            OnEndDisplayModeAnimation();

    /// <summary>
    /// Is mode is currently displayed?
    /// 
    /// Else it's animation that's running.
    /// </summary>
    bool                                    IsDisplayingMode() const;

public:
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
    /// Should erase all led when animation change?
    /// </summary>
    /// <returns>Return true by default.</returns>
    virtual bool                            ShouldEraseBetweenAnimations() override;
};

#include "animationbase.h"
#include "../include/sprite.h"

#pragma once

class CAnimationText : public CAnimationBase
{
private:

    /// <summary>
    /// Text to display.
    /// </summary>
    String                                  m_strText;

    /// <summary>
    /// Text to display with all sprites.
    /// 
    /// Is empty when the animation is inactive.
    /// </summary>
    vector<shared_ptr<CSprite>>             m_arrSpritesText;

    /// <summary>
    /// Text color.
    /// </summary>
    CRGB                                    m_crTextColor;

    /// <summary>
    /// Move the sprite?
    /// </summary>
    bool                                    m_bMove;

    /// <summary>
    /// Duration of animation, 0 if no duration, stop when sprite leave the screen.
    /// </summary>
    uint16_t                                m_ui16Duration;

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    /// <param name="_strText">Text to display</param>
    /// <param name="_crTextColor">Text color</param>
    /// <param name="_bMove">Move text or not</param>
    CAnimationText(String _strText, CRGB _crTextColor, bool _bMove = true, uint16_t _ui16Duration = 0);

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationText() override;

    /// <summary>
    /// Make the animation.
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
};

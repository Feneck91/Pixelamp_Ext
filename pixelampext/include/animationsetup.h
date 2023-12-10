#pragma once
#include "animationbase.h"
#include "sprite.h"

#define SETUP_COLOR_TEXT                    CRGB::BlueViolet
#define SETUP_COLOR_CURRENT_VALUE           CRGB::BlueViolet
#define SETUP_COLOR_VALUE_CHANGING          CRGB::Blue
#define SETUP_COLOR_CURRENT_VALUE_BLINK     CRGB::GreenYellow
#define SETUP_COLOR_VALUE_REGISTERED        CRGB::Green
#define SETUP_COLOR_VALUE_CANCELLED         CRGB::Red

#define SETUP_TIME_BEFORE_BLINK              4000
#define SETUP_TIME_BLINKING                  8000
#define SETUP_TIME_RETURNING_TO_VALUE       12000

/// <summary>
/// Class used to manage options.
/// </summary>
class CAnimationSetup : public CAnimationBase
{
    /// <summary>
    /// The current animation setup.
    /// </summary>
    uint8_t                                 m_ui8CurrentAnimation;

    /// <summary>
    /// Sprite used for the current animation.
    /// </summary>
    shared_ptr<CSprite>                     m_Sprite;

    /// <summary>
    /// Sprite used for the current animation just before record (blinking sprite).
    /// </summary>
    shared_ptr<CSprite>                     m_SpriteBlinking;

    /// <summary>
    /// Sprite used to display OK / NOK.
    /// </summary>
    shared_ptr<CSprite>                     m_SpriteValidation;

    /// <summary>
    /// Value at the beginning (before modifying by setup).
    /// </summary>
    uint16_t                                m_ui16CurrentRecordedValue;

    /// <summary>
    /// Current Value.
    /// </summary>
    uint16_t                                m_ui16Value;

    /// <summary>
    /// List time the user change the light.
    /// </summary>
    unsigned long                           m_lastTimeBrightnessUpdate;

    /// <summary>
    /// Original or last change of brithness value.
    /// 
    /// Display value of setup item without taking current brightness into account for this value or last change to know when was the last change of value
    /// until the user change the brightness position.
    /// </summary>
    unsigned long                           m_originBrightnessValue;
public:
    /// <summary>
    /// Constructor.
    /// </summary>
    CAnimationSetup();

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationSetup() override;

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

private:
    /// <summary>
    /// Record values at the begenning of the process to be able to know if the value has changed.
    /// </summary>
    void                                    RecordValues();
};


#include "../include/animationsetup.h"
#include "../include/engine.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

const SpriteDefinition<40> arrow PROGMEM
{
  5, 8,
  2, 0,
  0, 0,
  {
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0,
    1, 0, 1, 0, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 0, 1, 0, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
  }
};

const SpriteDefinition<56> recordOK PROGMEM
{
  7, 8,
  3, 0,
  0, 0,
  {
    0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 1, 0,
    1, 1, 0, 0, 1, 1, 0,
    0, 0, 1, 0, 1, 0, 0,
    0, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
  }
};

const SpriteDefinition<40> recordNOK PROGMEM
{
  5, 8,
  2, 0,
  0, 0,
  {
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1,
  }
};

const SpriteDefinition<64> pot_light_normal PROGMEM
{
  8, 8,
  2, 0,
  0, 0,
  {
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 1, 2, 2, 4, 2, 1, 0,
    1, 2, 3, 3, 3, 4, 2, 1,
    1, 2, 4, 4, 4, 4, 4, 1,
    1, 2, 3, 3, 3, 4, 2, 1,
    1, 2, 3, 3, 4, 3, 2, 1,
    0, 1, 2, 2, 2, 2, 1, 0,
    0, 0, 1, 1, 1, 1, 0, 0,
  }
};

const SpriteDefinition<64> pot_light_inverted PROGMEM
{
  8, 8,
  2, 0,
  0, 0,
  {
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 1, 2, 4, 2, 2, 1, 0,
    1, 2, 4, 3, 3, 3, 2, 1,
    1, 4, 4, 4, 4, 4, 2, 1,
    1, 2, 4, 3, 3, 3, 2, 1,
    1, 2, 3, 4, 3, 3, 2, 1,
    0, 1, 2, 2, 2, 2, 1, 0,
    0, 0, 1, 1, 1, 1, 0, 0,
  }
};

const SpriteDefinition<64> pot_animation_normal PROGMEM
{
  8, 8,
  4, 0,
  0, 0,
  {
    0, 0, 1, 1, 1, 1, 3, 0,
    0, 1, 1, 1, 1, 1, 1, 3,
    2, 2, 1, 1, 2, 2, 1, 1,
    3, 2, 2, 1, 3, 2, 2, 1,
    2, 2, 1, 1, 2, 2, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 3, 0, 1, 3, 0, 3, 1,
  }
};

const SpriteDefinition<64> pot_animation_inverted PROGMEM
{
  8, 8,
  4, 0,
  0, 0,
  {
    0, 3, 1, 1, 1, 1, 0, 0,
    3, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 2, 2, 1, 1, 2, 2,
    1, 2, 2, 3, 1, 2, 2, 3,
    1, 1, 2, 2, 1, 1, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 3, 0, 3, 1, 0, 3, 1,
  }
};

//========================================================================
// 
//                              CAnimationSetup
// 
//========================================================================
CAnimationSetup::CAnimationSetup()
    : m_ui8CurrentAnimation(0)
{
}

CAnimationSetup::~CAnimationSetup()
{
}

bool CAnimationSetup::Loop()
{
    bool bIgnoreMatrixDelta = false;
    uint16_t ui16Value = 0;
    uint8_t xPosition  = 0;
    int8_t  xPositionRelative = 0;

    switch (m_ui8CurrentAnimation)
    {
        case 0: // Relative position
        {
            unsigned long ulNow = millis();
            ui16Value = (m_lastTimeBrightnessUpdate == 0)
                ? (uint16_t) CEngine::Instance().GetCurrentMatrixDeltaX()
                : (uint16_t) constrain(map(CEngine::Instance().GetCurrentBrightness(), MIN_BRIGHTNESS, MAX_BRIGHTNESS, 0, CEngine::Instance().GetMatrixWidth()), 0, CEngine::Instance().GetMatrixWidth() - 1);
            xPosition = m_lastTimeBrightnessUpdate != 0 && ulNow - m_lastTimeBrightnessUpdate > SETUP_TIME_BEFORE_BLINK
                ? (uint8_t) m_ui16Value
                : (uint8_t) ui16Value;
            xPositionRelative = -2;
            bIgnoreMatrixDelta = true; // For this setup, the MatrixDelta that we are setup must be ignored
            break;
        }
        case 1: // Invert brightness POT
        {
            unsigned long ulNow = millis();
            ui16Value = (m_lastTimeBrightnessUpdate == 0)
                ? (uint16_t) (CEngine::Instance().IsInvertPotAnimation() ? 1 : 0)
                : (uint16_t)  constrain(map(CEngine::Instance().GetCurrentBrightness(), MIN_BRIGHTNESS, MAX_BRIGHTNESS, 0, 2), 0, 1);
            xPositionRelative = -3;

            if (m_Sprite.IsNull() || m_ui16Value != ui16Value)
            {   // Load correct sprites
                CRGB colorShowOrChanging = m_lastTimeBrightnessUpdate == 0 && m_originBrightnessValue == CEngine::Instance().GetCurrentBrightness()
                    ? SETUP_COLOR_CURRENT_VALUE
                    : SETUP_COLOR_VALUE_CHANGING;

                // Display current values to the user
                ColorReplacement cr;
                cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) CRGB::Red,    });
                cr.AddColorReplacement(ColorReplacement::stColors{ 2, (uint32_t) CRGB::Orange, });
                cr.AddColorReplacement(ColorReplacement::stColors{ 3, (uint32_t) CRGB::Yellow, });
                cr.AddColorReplacement(ColorReplacement::stColors{ 4, (uint32_t) colorShowOrChanging, });
                m_Sprite = (ui16Value == 0)
                    ? pot_light_normal.ToSprite(cr)
                    : pot_light_inverted.ToSprite(cr);
                // Sprite move blink
                cr.Clear();
                cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) CRGB::Red,    });
                cr.AddColorReplacement(ColorReplacement::stColors{ 2, (uint32_t) CRGB::Orange, });
                cr.AddColorReplacement(ColorReplacement::stColors{ 3, (uint32_t) CRGB::Yellow, });
                cr.AddColorReplacement(ColorReplacement::stColors{ 4, (uint32_t) SETUP_COLOR_CURRENT_VALUE_BLINK, });
                m_SpriteBlinking = (ui16Value == 0)
                    ? pot_light_normal.ToSprite(cr)
                    : pot_light_inverted.ToSprite(cr);
            }
            break;
        }
        case 2: // Invert animation POT
        {
            unsigned long ulNow = millis();
            ui16Value = (m_lastTimeBrightnessUpdate == 0)
                ? (uint16_t) (CEngine::Instance().IsInvertPotAnimation() ? 1 : 0)
                : (uint16_t)constrain(map(CEngine::Instance().GetCurrentBrightness(), MIN_BRIGHTNESS, MAX_BRIGHTNESS, 0, 2), 0, 1);
            xPositionRelative = -3;

            if (m_Sprite.IsNull() || m_ui16Value != ui16Value)
            {   // Load correct sprites
                CRGB colorShowOrChanging = m_lastTimeBrightnessUpdate == 0 && m_originBrightnessValue == CEngine::Instance().GetCurrentBrightness()
                    ? SETUP_COLOR_CURRENT_VALUE
                    : SETUP_COLOR_VALUE_CHANGING;

                // Display current values to the user
                ColorReplacement cr;
                cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) colorShowOrChanging });
                cr.AddColorReplacement(ColorReplacement::stColors{ 2, (uint32_t) CRGB::White,       });
                cr.AddColorReplacement(ColorReplacement::stColors{ 3, (uint32_t) CRGB::DarkGray,    });
                m_Sprite = (ui16Value == 0)
                    ? pot_animation_normal.ToSprite(cr)
                    : pot_animation_inverted.ToSprite(cr);
                // Sprite move blink
                cr.Clear();
                cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) SETUP_COLOR_CURRENT_VALUE_BLINK });
                cr.AddColorReplacement(ColorReplacement::stColors{ 2, (uint32_t) CRGB::White,                    });
                cr.AddColorReplacement(ColorReplacement::stColors{ 3, (uint32_t) CRGB::DarkGray,                 });
                m_SpriteBlinking = (ui16Value == 0)
                    ? pot_animation_normal.ToSprite(cr)
                    : pot_animation_inverted.ToSprite(cr);
            }
            break;
        }
    }


    // Detect the brightness changes
    if (m_lastTimeBrightnessUpdate == 0 && m_originBrightnessValue != CEngine::Instance().GetCurrentBrightness())
    {
        m_lastTimeBrightnessUpdate = millis();
        // Loading new sprite color => color of changing value
        switch (m_ui8CurrentAnimation)
        {
            case 0: // Relative position
            {
                ColorReplacement cr;
                cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) SETUP_COLOR_VALUE_CHANGING, });
                m_Sprite = arrow.ToSprite(cr);
                break;
            }
            case 1: // Invert brightness POT
            {
                // Already done just before
                break;
            }
            case 2: // Invert animation POT
            {
                // Already done just before
                break;
            }
        }
    }

    unsigned long ulNow = millis();
    if (m_lastTimeBrightnessUpdate == 0)
    {   // Display current value to the user
        m_Sprite->m_X = xPosition + xPositionRelative;
        CEngine::Instance().ClearAllMatrix();
        m_Sprite->ShowSprite(bIgnoreMatrixDelta, true, false);
        FastLED.show();
    }
    else if (m_ui16Value != ui16Value && m_SpriteValidation.IsNull())
    {   // The value change
        if (ulNow - m_lastTimeBrightnessUpdate > SETUP_TIME_BEFORE_BLINK)
        {   // Canceled
            // Sprite record NOK
            ColorReplacement cr;
            cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) SETUP_COLOR_VALUE_CANCELLED, });
            m_SpriteValidation         = recordNOK.ToSprite(cr);
            m_SpriteValidation->m_X = xPosition - 2;
            m_lastTimeBrightnessUpdate = ulNow - SETUP_TIME_BLINKING;
            m_ui16CurrentRecordedValue = m_ui16Value; // Don't write the value !
        }
        else
        {
            m_Sprite->m_X = xPosition + xPositionRelative;
            CEngine::Instance().ClearAllMatrix();
            m_Sprite->ShowSprite(bIgnoreMatrixDelta, true, false);
            m_lastTimeBrightnessUpdate = ulNow;
        }
        FastLED.show();
    }
    else if (ulNow - m_lastTimeBrightnessUpdate > SETUP_TIME_BEFORE_BLINK && ulNow - m_lastTimeBrightnessUpdate < SETUP_TIME_BLINKING)
    {   // Blink : the value no changed
        m_SpriteBlinking->m_X = xPosition + xPositionRelative;
        CEngine::Instance().ClearAllMatrix();
        if (((ulNow - m_lastTimeBrightnessUpdate - SETUP_TIME_BEFORE_BLINK) / 500) % 2 == 1)
        {
            m_SpriteBlinking->ShowSprite(bIgnoreMatrixDelta, true, false);
        }
        FastLED.show();
    }
    else if (ulNow - m_lastTimeBrightnessUpdate > SETUP_TIME_BLINKING && ulNow - m_lastTimeBrightnessUpdate < SETUP_TIME_RETURNING_TO_VALUE)
    {   // Validate
        if (m_SpriteValidation.IsNull())
        {
            // The cancel sprite was not created: valition
            // Sprite record OK
            ColorReplacement cr;
            cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) SETUP_COLOR_VALUE_REGISTERED, });
            m_SpriteValidation = recordOK.ToSprite(cr);
            m_SpriteValidation->m_X = xPosition - 3;
        }
        CEngine::Instance().ClearAllMatrix();
        m_SpriteValidation->ShowSprite(bIgnoreMatrixDelta, true, false);

        if (m_ui16CurrentRecordedValue != m_ui16Value)
        {   // Record
            switch (m_ui8CurrentAnimation)
            {
                case 0:
                {
                    CEngine::Instance().SetCurrentMatrixDeltaX((uint8_t) m_ui16Value, true);
                    break;
                }
                case 1:
                {
                    CEngine::Instance().SetInvertPotBrightness(m_ui16Value == 0 ? false : true, true);
                    break;
                }
                case 2:
                {
                    CEngine::Instance().SetInvertPotAnimation(m_ui16Value == 0 ? false : true, true);
                    break;
                }
            }
            m_ui16CurrentRecordedValue = m_ui16Value;
        }
        FastLED.show();
    }
    else if (ulNow - m_lastTimeBrightnessUpdate > SETUP_TIME_RETURNING_TO_VALUE)
    {   // Done : back to registered value
        SetCurrentAnimation(m_ui8CurrentAnimation);
    }

    if (m_lastTimeBrightnessUpdate != 0 && ulNow - m_lastTimeBrightnessUpdate < SETUP_TIME_BEFORE_BLINK)
    {   // After the value is never changed, if changed, the operation is cancelled
        m_ui16Value = ui16Value;
    }

    return false; // Always false
}

void CAnimationSetup::RecordValues()
{
    m_SpriteValidation          = nullptr;
    m_lastTimeBrightnessUpdate  = 0; // Display current value
    m_originBrightnessValue     = CEngine::Instance().GetCurrentBrightness();
    switch (m_ui8CurrentAnimation)
    {
        case 0:
        {
            m_ui16CurrentRecordedValue = CEngine::Instance().GetCurrentMatrixDeltaX();
            break;
        }
        case 1:
        {
            m_ui16CurrentRecordedValue = CEngine::Instance().IsInvertPotBrightness() ? 1 : 0;
            break;
        }
        case 2:
        {
            m_ui16CurrentRecordedValue = CEngine::Instance().IsInvertPotAnimation() ? 1 : 0;
            break;
        }
    }
}

void CAnimationSetup::Enter()
{
    RecordValues();
}

void CAnimationSetup::Leave()
{
    m_Sprite            = nullptr;
    m_SpriteValidation  = nullptr;
}

uint16_t CAnimationSetup::GetMillisecondWait()
{
    return 100;
}

uint16_t CAnimationSetup::GetNumberAnimations()
{
    return 3;
}

void CAnimationSetup::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
    m_ui8CurrentAnimation = (uint8_t)_uiCurrentAnimation;
    switch (m_ui8CurrentAnimation)
    {
        case 0:
        {   // Add delta X for display placement. Used to record the real center of the lamp
            ColorReplacement cr;
            cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) SETUP_COLOR_CURRENT_VALUE, });
            m_Sprite = arrow.ToSprite(cr);
            // Sprite move blink
            cr.Clear();
            cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) SETUP_COLOR_CURRENT_VALUE_BLINK, });
            m_SpriteBlinking = arrow.ToSprite(cr);
            break;
        }
        case 1:
        {   // Invert Light POT values
            m_Sprite         =  nullptr;
            m_SpriteBlinking = nullptr;
            break;
        }
        case 2:
        {   // Invert Animation POT values
            m_Sprite         =  nullptr;
            m_SpriteBlinking = nullptr;
            break;
        }
    }
    RecordValues();
}


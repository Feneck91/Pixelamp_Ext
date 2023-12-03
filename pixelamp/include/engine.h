#include "animationmode.h"

#pragma once

// Default values (used in Init function)
#define DEFAULT_LED_DATA_PIN                    6
#define DEFAULT_COLOR_ORDER                     GRB
#define DEFAULT_CHIPSET                         WS2812

// Width / Height defaults matrix leds
#define DEFAULT_MATRIX_WIDTH                    16
#define DEFAULT_MATRIX_HEIGHT                   8

// Port used for brightness and animation
#define DEFAULT_POTENTIOMETER_BRIGHTNESS        A0
#define DEFAULT_POTENTIOMETER_ANIMATION         A1

// The leds are arrange in zigzab by default
#define DEFAULT_ARRANGE_IN_ZIGZAG               true

// Used into ComputePositionFromXY
#define INVALID_INDEX                           (uint16_t (-1))

#define POT_BRIGHTNESS_MIN                      0
#define POT_BRIGHTNESS_MAX                      1023

#define MIN_BRIGHTNESS                          0
#define MAX_BRIGHTNESS                          150 // Can be changed to 255 but ONLY if 8A power supply is possible (not just 4A)

// Indicate the current mode index is not set
#define INDEX_MODE_NOMODE                       255

// To save current mode into EEPROM
#define EEPROM_ADDRESS_CURRENT_MODE             0
#define EEPROM_ADDRESS_INVERT_POT_BRIGHTNESS    1
#define EEPROM_ADDRESS_INVERT_POT_ANIMATION     2
#define EEPROM_ADDRESS_MATRIX_DELTA_X           3

// Max durantion to be able to change mode
#define MAX_TIME_CHANGE_MODE_MS                 2000
#define VALUE_DELTA_CHANGE_MODE_MAX             70
#define VALUE_DELTA_CHANGE_MODE_MIN             10

// Animations
#define POT_ANIMATION_MIN                       0
#define POT_ANIMATION_MAX                       1023

/// <summary>
/// Engine class that manage all sprites.
/// </summary>
class CEngine
{
protected:
    /// <summary>
    /// Instance (singleton).
    /// </summary>
    static CEngine *                        m_pEngine;

private:
    /// <summary>
    /// To be able to make wait for each mode.
    /// </summary>
    unsigned long                           m_lastTime;

    /// <summary>
    /// Delta X for screen display.
    /// </summary>
    uint8_t                                 m_ui8MatrixDeltaX; 
    
    /// <summary>
    /// Used to manage the mode changing to up or down.
    /// </summary>
    struct ChangeModeBrightness
    {
        /// <summary>
        /// To be able to change mode, record time of last brightness changes.
        /// </summary>
        unsigned long                           m_lastTimeBrightness;

        /// <summary>
        /// Last update time.
        /// </summary>
        unsigned long                           m_lastTimeBrightnessUpdate;

        /// <summary>
        /// Brightness changing to up or down.
        /// </summary>
        bool                                    m_bChangeModeUp;

        /// <summary>
        /// Begin of start value.
        /// </summary>
        uint8_t                                 m_ui8StartValue;

        /// <summary>
        /// To know how many up/down is actually done (3 = mode / 5 = setup).
        /// </summary>
        uint8_t                                 m_ui8ModeCount;
    }                                       m_ModeChanging;

    /// <summary>
    /// Contains all animations / mode.
    /// </summary>
    vector<shared_ptr<CAnimationMode>>      m_arrAnimationsMode;

    /// <summary>
    /// Used when enter SETUP mode.
    /// </summary>
    shared_ptr<CAnimationMode>              m_setupMode;

    /// <summary>
    /// Index of on current animation mode (254 max).
    /// 
    /// INDEX_MODE_NOMODE if none.
    /// 
    /// It is stored into EEPROM (to remember value) at EEPROM_ADDRESS_CURRENT_MODE address.
    /// </summary>
    uint8_t                                 m_ui8CurrentAnimationsMode;

    /// <summary>
    /// Number of led into the matrix (width).
    /// </summary>
    uint8_t                                 m_ui8MatrixWidth;

    /// <summary>
    /// Number of led into the matrix (height).
    /// </summary>
    uint8_t                                 m_ui8MatrixHeight;

    /// <summary>
    /// Port used to read potentiometer for brightness.
    /// </summary>
    uint8_t                                 m_uiPinBrightness;

    /// <summary>
    /// Port used to read potentiometer for animation.
    /// </summary>
    uint8_t                                 m_uiPinAnimation;

    /// <summary>
    /// Current displayed leds.
    /// </summary>
    CRGB *                                  m_pLeds;

    /// <summary>
    /// Indicate if the LED are arrange in zigzag or not.
    /// 
    /// For example, if 8 leds, from bottom (0) to up (7), the 8'th led is in up (in zigzag) or in bottom (not in zigzag).
    /// </summary>
    bool                                    m_bArrangeInZigZag;

    /// <summary>
    /// Current brightness.
    /// </summary>
    uint8_t                                 m_iCurrentBrightness;

    /// <summary>
    /// Current animation.
    /// </summary>
    uint16_t                                m_i16CurrentAnimation;

    /// <summary>
    /// Number of animation for this mode.
    /// </summary>
    uint32_t                                m_iNumberAnimation;

    /// <summary>
    /// Should invert brightness potentiometer.
    /// </summary>
    bool                                    m_bInvertPotBrightness;

    /// <summary>
    /// Should invert animation potentiometer.
    /// </summary>
    bool                                    m_bInvertPotAnimation;
protected:
    /// <summary>
    /// Protected constructor, not allowed outside of this class or derived.
    /// </summary>
    CEngine();

    /// <summary>
    /// Private destructor, not allowed outside of this class.
    /// </summary>
    virtual ~CEngine();

    /// <summary>
    /// Initialize the engine.
    /// 
    /// Don't call any functions for leds, all is not currently initialized.
    /// Do it into OnInit function.
    /// </summary>
    /// <param name="_ui8MatrixWidth">Led number in width.</param>
    /// <param name="_ui8MatrixHeight">Led number in height.</param>
    /// <param name="_uiPinBrightness">Port used to read potentiometer for brightness.</param>
    /// <param name="_uiPinAnimation">Port used to read potentiometer for animation.</param>
    /// <param name="_bArrangeInZigZag">Indicate if led are arrange in zigzag or not, used to compute each led position..</param>
    void                        Init(uint8_t _ui8MatrixWidth, uint8_t _ui8MatrixHeight, uint8_t _uiPinBrightness, uint8_t _uiPinAnimation, bool _bArrangeInZigZag);

    /// <summary>
    /// Called when all is correctly initialized.
    /// 
    /// Called only one time.
    /// </summary>
    void                        OnInit();

    /// <summary>
    /// Init FastLed library.
    /// </summary>
    virtual void                InitFastLeds() = 0;
private:
    /// <summary>
    /// Private copy constructor, not allowed outside of this class.
    /// </summary>
    CEngine(const CEngine &);

public:
    /// <summary>
    /// Get the number of led into the matrix (width).
    /// </summary>
    /// <returns>The number of led into the matrix (width).</returns>
    uint8_t                     GetMatrixWidth() const;

    /// <summary>
    /// Get the number of led into the matrix (height).
    /// </summary>
    /// <returns>The number of led into the matrix (height).</returns>
    uint8_t                     GetMatrixHeight() const;

    /// <summary>
    /// Port used to read potentiometer for brightness.
    /// </summary>
    /// <returns>The brightness pin number.</returns>
    uint8_t                     GetPinBrightness() const;

    /// <summary>
    /// Port used to read potentiometer for animation.
    /// </summary>
    /// <returns>The animation pin number.</returns>
    uint8_t                     GetPinAnimation() const;

    /// <summary>
    /// Get the number of animation.
    /// 
    /// Return 1 when setup is running.
    /// </summary>
    /// <returns></returns>
    unsigned int                GetAnimationsModeSize() const;

    /// <summary>
    /// Get the animation at the index.
    /// 
    /// Return the setup animation when setup is running.
    /// </summary>
    /// <param name="_ui8AnimationIndex">Index of animation.</param>
    /// <returns></returns>
    shared_ptr<CAnimationMode> GetAnimationsAtIndex(uint8_t _ui8AnimationIndex);

    /// <summary>
    /// Get the instance engine.
    /// </summary>
    /// <returns>The unique instance engine.</returns>
    static CEngine &            Instance();

    /// <summary>
    /// Kill (delete) the instance engine.
    /// </summary>
    static void                 KillInstance();

    /// <summary>
    /// Make the engine work on each loop of Arduino.
    /// </summary>
    void                        Loop();

    /// <summary>
    /// Delta X fos screen display (to have the message at the center of the leds)
    /// </summary>
    /// <returns></returns>
    uint8_t                     GetCurrentMatrixDeltaX() const;

    /// <summary>
    /// Set the delta X fos screen display (to have the message at the center of the leds).
    /// </summary>
    /// <returns>The new value of the delta X matrix Led</returns>
    /// <param name="_bWriteInEEPROM">If true write the value into the EEPROM. Only if the value is changed!</param>
    void                        SetCurrentMatrixDeltaX(uint8_t _ui8MatrixDeltaX, bool _bWriteInEEPROM);

    /// <summary>
    /// Change the current animation mode.
    /// 
    /// It call Enter and Leave functions on animation.
    /// </summary>
    /// <param name="_ui8CurrentAnimationsMode">Current index of animation mode</param>
    /// <param name="_bWriteInEEPROM">If true write the value into the EEPROM. Only if the value is changed!</param>
    void                        ChangeCurrentAnimationMode(uint8_t _ui8CurrentAnimationsMode, bool _bWriteInEEPROM);

    /// <summary>
    /// Initialize the animations contents.
    /// </summary>
    /// <param name="_pAnimationMode">Animation mode.</param>
    void                        AddAnimationMode(shared_ptr<CAnimationMode> _pAnimationMode); 

    /// <summary>
    /// Get the number of leds.
    /// </summary>
    /// <returns>The width x height leds.</returns>
    uint16_t                    GetNumLeds() const;
    
    /// <summary>
    /// Get the content of the leds.
    /// </summary>
    /// <returns>An array of CRGB elements, of m_ui8MatrixHeight x m_ui8MatrixHeight elements.
    ///          Be careful, the m_ui8MatrixDeltaX is not taken into account when accessing directly to this leds buffer. </returns>
    CRGB *                      GetLeds();

    /// <summary>
    /// Set the led color, computed the led address from X,Y.
    /// </summary>
    /// <param name="_bIgnoreMatrixDelta">Ignore matrix delta or not.</param>
    /// <param name="_X">X position.</param>
    /// <param name="_Y">Y position.</param>
    /// <param name="_bWrapX">Wrap X?</param>
    /// <param name="_bWrapY">Wrap Y?</param>
    /// <param name="_rgbColor">Color of the led</param>
    /// <param name="_bTransparent">Don't display black pixel.</param>
    /// <returns>true if the pixel is visible, false else.</returns>
    bool                        SetLedColor(bool _bIgnoreMatrixDelta, led_coordinate _X, led_coordinate _Y, bool _bWrapX, bool _bWrapY, CRGB _rgbColor, bool _bTransparent = false);

    /// <summary>
    /// Add color to current led color, computed the led address from X,Y.
    /// </summary>
    /// <param name="_bIgnoreMatrixDelta">Ignore matrix delta or not.</param>
    /// <param name="_X">X position.</param>
    /// <param name="_Y">Y position.</param>
    /// <param name="_bWrapX"></param>
    /// <param name="_bWrapY"></param>
    /// <param name="_rgbColor">Color of the led</param>
    /// <returns>true if the pixel is visible, false else.</returns>
    bool                        AddLedColor(bool _bIgnoreMatrixDelta, led_coordinate _X, led_coordinate _Y, bool _bWrapX, bool _bWrapY, CRGB _rgbColor);

    /// <summary>
    /// Set all led to same color.
    /// </summary>
    /// <param name="_rgbColor">Color</param>
    void                        SetAllLedsColor(CRGB _rgbColor);

    /// <summary>
    /// Compute the led address from X,Y.
    /// 
    /// [0;0] is on top-left-hand corner
    /// </summary>
    /// <param name="_bIgnoreMatrixDelta">Ignore matrix delta or not.</param>
    /// <param name="_X">X position.</param>
    /// <param name="_Y">Y position.</param>
    /// <param name="_bWrapX">Wrap X?</param>
    /// <param name="_bWrapY">Wrap Y?</param>
    /// <returns>The led address. INVALID_INDEX if no wrap and index out of led min / max indexes.</returns>
    uint16_t                    ComputePositionFromXY(bool _bIgnoreMatrixDelta, led_coordinate _X, led_coordinate _Y, bool _bWrapX, bool _bWrapY);

    /// <summary>
    /// Clear all led.
    /// 
    /// Put all led to 0 color but don't transfert to display (show).
    /// </summary>
    void                        ClearAllMatrix();

    /// <summary>
    /// Invert brightness direction potentiometer .
    /// 
    /// Used if the wiring is bad.
    /// </summary>
    /// <param name="_bInvert">true if inverted, false else.</param>
    /// <param name="_bWriteInEEPROM">If true write the value into the EEPROM. Only if the value is changed!</param>
    void                        SetInvertPotBrightness(bool _bInvert, bool _bWriteInEEPROM);

    /// <summary>
    /// Is the brightness direction potentiometer inverted?
    /// </summary>
    /// <returns>true if inverted, false else.</returns>
    bool                        IsInvertPotBrightness() const;

    /// <summary>
    /// Invert animation direction potentiometer .
    /// 
    /// Used if the wiring is bad.
    /// </summary>
    /// <param name="_bInvert">true if inverted, false else.</param>
    /// <param name="_bWriteInEEPROM">If true write the value into the EEPROM. Only if the value is changed!</param>
    void                        SetInvertPotAnimation(bool _bInvert, bool _bWriteInEEPROM);

    /// <summary>
    /// Is the brightness direction potentiometer inverted?
    /// </summary>
    /// <returns>true if inverted, false else.</returns>
    bool                        IsInvertPotAnimation() const;

    /// <summary>
    /// Get current brightness.
    /// </summary>
    /// <returns>The value of the current brightness.</returns>
    uint8_t                     GetCurrentBrightness() const;

protected:
    /// <summary>
    /// Read and set brightness.
    /// 
    /// Read user light changes and detect mode change up or down; or setup enter / leave.
    /// </summary>
    /// <returns>true if the mode has changed, false else.</returns>
    bool                        ReadBrightness();

    /// <summary>
    /// Read and set the animation.
    /// </summary>
    /// <param name="_bForceNotification">If true, notify by calling SetCurrentAnimation.
    ///                                   Used when mode is changed.</param>
    void                        ReadAnimation(bool _bForceNotification);

    template<template<uint8_t DataLedPin, EOrder ColorOrder> class Chipset, uint8_t DataLedPin, EOrder ColorOrder> friend class CEngineImpl;
};

template<template<uint8_t DataLedPin, EOrder ColorOrder> class Chipset, uint8_t DataLedPin, EOrder ColorOrder>
class CEngineImpl : CEngine
{
public:
    /// <summary>
    /// Initialize the engine.
    /// </summary>
    /// <param name="_ui8MatrixWidth">Led number in width.</param>
    /// <param name="_ui8MatrixHeight">Led number in height.</param>
    /// <param name="_uiPinBrightness">Port used to read potentiometer for brightness.</param>
    /// <param name="_uiPinAnimation">Port used to read potentiometer for animation.</param>
    /// <param name="_bArrangeInZigZag">Indicate if led are arrange in zigzag or not, used to compute each led position..</param>
    static void                 Init(uint8_t _ui8MatrixWidth, uint8_t _ui8MatrixHeight, uint8_t _uiPinBrightness, uint8_t _uiPinAnimation, bool _bArrangeInZigZag)
    {
        // Create singleton instance
        m_pEngine = new CEngineImpl<Chipset, DataLedPin, ColorOrder>();

        // Init engine singleton
        CEngine::Instance().Init(_ui8MatrixWidth, _ui8MatrixHeight, _uiPinBrightness, _uiPinAnimation, _bArrangeInZigZag);
        CEngine::Instance().OnInit();
    }

    /// <summary>
    /// Init FastLed library.
    /// </summary>
    virtual void                InitFastLeds() override
    {
        // Initialize FastLed chipset
        FastLED.addLeds<Chipset, DataLedPin, ColorOrder>(CEngine::Instance().GetLeds(), CEngine::Instance().GetNumLeds()).setCorrection(Halogen);
    }
};

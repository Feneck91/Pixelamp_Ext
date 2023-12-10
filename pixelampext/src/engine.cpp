#include "../include/engine.h"
#include "../include/animationsetup.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                             CEngine
// 
//========================================================================
// Unique instance
CEngine * CEngine::m_pEngine = nullptr;

// Constructor
CEngine::CEngine()
    : m_lastTime(millis())
    , m_ui8CurrentAnimationsMode(INDEX_MODE_NOMODE)
    , m_ui8MatrixWidth(0)
    , m_ui8MatrixHeight(0)
    , m_uiPinBrightness(0)
    , m_uiPinAnimation(0)
    , m_iCurrentBrightness(150)
    , m_i16CurrentAnimation(0)
    , m_iNumberAnimation(0)
    , m_bArrangeInZigZag(false)
    , m_pLeds(nullptr)
    , m_bInvertPotBrightness(false)
    , m_bInvertPotAnimation(false)
{
}

// Destructor
CEngine::~CEngine()
{
    m_pEngine = nullptr;
    if (m_pLeds != nullptr)
    {
        delete []m_pLeds;
    }
}

// Get the unique instance.
CEngine & CEngine::Instance()
{
    return *m_pEngine;
}

// Kill the unique instance.
void CEngine::KillInstance()
{
    if (m_pEngine != nullptr)
    {
        delete m_pEngine;
    }
}

uint8_t CEngine::GetMatrixWidth() const
{
    return m_ui8MatrixWidth;
}

uint8_t CEngine::GetMatrixHeight() const
{
    return m_ui8MatrixHeight;
}

uint8_t CEngine::GetPinBrightness() const
{
    return m_uiPinBrightness;
}

uint8_t CEngine::GetPinAnimation() const
{
    return m_uiPinAnimation;
}

unsigned int CEngine::GetAnimationsModeSize() const
{
    return m_setupMode.IsNotNull()
        ? 1
        : (unsigned int) m_arrAnimationsMode.size();
}

shared_ptr<CAnimationMode> CEngine::GetAnimationsAtIndex(uint8_t _ui8AnimationIndex)
{
    return m_setupMode.IsNotNull()
        ? m_setupMode
        : (_ui8AnimationIndex < (uint8_t) m_arrAnimationsMode.size()
            ? m_arrAnimationsMode[_ui8AnimationIndex]
            : shared_ptr<CAnimationMode>(nullptr)
          );
}

void CEngine::Loop()
{
    // Read and set the brightness
    bool bModeChanged = ReadBrightness();
    ReadAnimation(false);

    if (m_ui8CurrentAnimationsMode == INDEX_MODE_NOMODE)
    {
        ChangeCurrentAnimationMode(EEPROM.read(EEPROM_ADDRESS_CURRENT_MODE), false);
    }

    shared_ptr<CAnimationMode> pAnimationMode = GetAnimationsAtIndex(m_ui8CurrentAnimationsMode);
    if (pAnimationMode.IsNotNull())
    {
        unsigned long nbMsWait = pAnimationMode->GetMillisecondWait();
        unsigned long ulDelay = millis() - m_lastTime;
        if (!bModeChanged && ulDelay < nbMsWait)
        {
            FastLED.delay(min(nbMsWait - ulDelay, 100));
        }
        else
        {
            pAnimationMode->Loop();
            m_lastTime = millis();
        }
    }
}

uint8_t CEngine::GetCurrentMatrixDeltaX() const
{
    return m_ui8MatrixDeltaX;
}

void CEngine::SetCurrentMatrixDeltaX(uint8_t _ui8MatrixDeltaX, bool _bWriteInEEPROM)
{
    if (_ui8MatrixDeltaX >= 0 && _ui8MatrixDeltaX < GetMatrixWidth())
    {
        m_ui8MatrixDeltaX = _ui8MatrixDeltaX;
    }
    else
    {
        m_ui8MatrixDeltaX = 0;
    }
    if (_bWriteInEEPROM && EEPROM.read(EEPROM_ADDRESS_MATRIX_DELTA_X) != m_ui8MatrixDeltaX)
    {
        EEPROM.write(EEPROM_ADDRESS_MATRIX_DELTA_X, m_ui8MatrixDeltaX);
    }
}

void CEngine::ChangeCurrentAnimationMode(uint8_t _ui8CurrentAnimationsMode, bool _bWriteInEEPROM)
{
    if (m_ui8CurrentAnimationsMode != _ui8CurrentAnimationsMode)
    {
        shared_ptr<CAnimationMode> pAnimationMode = GetAnimationsAtIndex(m_ui8CurrentAnimationsMode);

        if (pAnimationMode.IsNotNull())
        {
            pAnimationMode->Leave();
            ClearAllMatrix();
            m_ui8CurrentAnimationsMode = INDEX_MODE_NOMODE;
        }
        
        pAnimationMode = GetAnimationsAtIndex(_ui8CurrentAnimationsMode);
        if (pAnimationMode.IsNotNull())
        {
            m_ui8CurrentAnimationsMode = _ui8CurrentAnimationsMode;
            if (_bWriteInEEPROM && EEPROM.read(EEPROM_ADDRESS_CURRENT_MODE) != m_ui8CurrentAnimationsMode)
            {
                EEPROM.write(EEPROM_ADDRESS_CURRENT_MODE, m_ui8CurrentAnimationsMode);
            }
            ClearAllMatrix();
            pAnimationMode->Enter();
            m_iNumberAnimation = pAnimationMode->GetNumberAnimations();
        }
        else
        {
            m_iNumberAnimation = 0;
        }
        ReadAnimation(true);
    }
}

// Initialize informations about the engine.
void CEngine::Init(uint8_t _ui8MatrixWidth, uint8_t _ui8MatrixHeight, uint8_t _uiPinBrightness, uint8_t _uiPinAnimation, bool _bArrangeInZigZag)
{
    // Internal class initialization
    m_ui8CurrentAnimationsMode          = INDEX_MODE_NOMODE;
    m_ui8MatrixWidth                    = _ui8MatrixWidth;
    m_ui8MatrixHeight                   = _ui8MatrixHeight;
    m_uiPinBrightness                   = _uiPinBrightness;
    m_uiPinAnimation                    = _uiPinAnimation;
    m_bArrangeInZigZag                  = _bArrangeInZigZag;
    m_pLeds                             = new CRGB[GetNumLeds()];
    m_ModeChanging.m_lastTimeBrightness = 0;
    m_ModeChanging.m_bChangeModeUp      = true;
    m_ModeChanging.m_ui8StartValue      = 0;
    m_ModeChanging.m_ui8ModeCount       = 0;

    // Init Brightness
    SetInvertPotBrightness(EEPROM.read(EEPROM_ADDRESS_INVERT_POT_BRIGHTNESS) == 1, false);
    SetInvertPotAnimation(EEPROM.read(EEPROM_ADDRESS_INVERT_POT_ANIMATION) == 1, false);

    // Init Delta X Matrix
    SetCurrentMatrixDeltaX(EEPROM.read(EEPROM_ADDRESS_MATRIX_DELTA_X), false);

    // Initialize Serial port
    Serial.begin(115200);

    // Initialize FastLed infos
    InitFastLeds();

    FastLED.setBrightness(m_iCurrentBrightness);
    FastLED.setDither(DISABLE_DITHER);
}

void CEngine::OnInit()
{
    // Clear all leds
    ClearAllMatrix();
    // Display
    FastLED.show();
}

void CEngine::AddAnimationMode(shared_ptr<CAnimationMode> _pAnimationMode)
{
    m_arrAnimationsMode.push_back(_pAnimationMode);
}

uint16_t CEngine::GetNumLeds() const
{
    return m_ui8MatrixWidth * m_ui8MatrixHeight;
}

CRGB * CEngine::GetLeds()
{
    return m_pLeds;
}

// Set led color from x, y
bool CEngine::SetLedColor(bool _bIgnoreMatrixDelta, led_coordinate _X, led_coordinate _Y, bool _bWrapX, bool _bWrapY, CRGB _rgbColor, bool _bTransparent)
{
    bool bRet = false;
    auto uiIndex = ComputePositionFromXY(_bIgnoreMatrixDelta, _X, _Y, _bWrapX, _bWrapY);
    if (uiIndex != INVALID_INDEX)
    {
        if (!_bTransparent || _rgbColor != CRGB::Black)
        {
            GetLeds()[uiIndex] = _rgbColor;
        }
        bRet = true;
    }

    return bRet;
}

// Add value to Set led color from x, y
bool CEngine::AddLedColor(bool _bIgnoreMatrixDelta, led_coordinate _X, led_coordinate _Y, bool _bWrapX, bool _bWrapY, CRGB _rgbColor)
{
    bool bRet = false;
    auto uiIndex = ComputePositionFromXY(_bIgnoreMatrixDelta, _X, _Y, _bWrapX, _bWrapY);
    if (uiIndex != INVALID_INDEX)
    {
        GetLeds()[uiIndex] += _rgbColor;
        bRet = true;
    }

    return bRet;
}

void CEngine::SetAllLedsColor(CRGB _rgbColor)
{
    for (uint16_t uiIndex = 0; uiIndex < GetNumLeds(); ++uiIndex)
    {
        GetLeds()[uiIndex] = _rgbColor;
    }
}

// Compute led position
uint16_t CEngine::ComputePositionFromXY(bool _bIgnoreMatrixDelta, led_coordinate _X, led_coordinate _Y, bool _bWrapX, bool _bWrapY)
{
    const uint8_t matrixMaxX = m_ui8MatrixWidth - 1;
    const uint8_t matrixMaxY = m_ui8MatrixHeight - 1;

    //comment this line to put [0;0] on bottom-left-hand corner.
    //y = (kMatrixHeight-1) - y;
    if (!_bIgnoreMatrixDelta)
    {
        _X += GetCurrentMatrixDeltaX();
    }

    if (_bWrapX)
    {
        if (_X > matrixMaxX)
        {
            _X = _X % m_ui8MatrixWidth;
        }
        else if (_X < 0)
        {
            _X = _X % m_ui8MatrixWidth;
            _X += m_ui8MatrixWidth;
        }
    }
    if (_bWrapY)
    {
        if (_Y > matrixMaxY)
        {
            _Y = _Y % m_ui8MatrixHeight;
        }
        else if (_Y < 0)
        {
            _Y = _Y % m_ui8MatrixHeight;
            _Y += m_ui8MatrixHeight;
        }
    }

    // If LEDs are arranged in zigzag
    if (m_bArrangeInZigZag && _X % 2 == 0)
    {
        _Y = (m_ui8MatrixHeight - 1) - _Y;
    }

    if (!_bIgnoreMatrixDelta && !_bWrapX)
    {
        if (_X > matrixMaxX && _X <= matrixMaxX / 2 + GetCurrentMatrixDeltaX() + 1)
        {   // If cut at the right of the grid, it can wrap to left because the matrix centered, not completly to the left
            _X -= matrixMaxX + 1;
        }
        else if (_X > GetCurrentMatrixDeltaX() - matrixMaxX / 2 - 1 && _X < 0)
        {
            _X += matrixMaxX + 1;
        }
        else if (_X <= GetCurrentMatrixDeltaX() - matrixMaxX / 2 - 1 || _X > matrixMaxX / 2 + GetCurrentMatrixDeltaX() + 1)
        {
            return INVALID_INDEX;
        }
    }

    return (   (!_bWrapX && (_X > matrixMaxX || _X < 0))
            || (!_bWrapY && (_Y > matrixMaxY || _Y < 0))
           )
        ? INVALID_INDEX
        : (_X * m_ui8MatrixHeight) + _Y;
}

// Clear all led.
void CEngine::ClearAllMatrix()
{
    uint16_t iNum;

    for (iNum = 0; iNum < GetNumLeds(); ++iNum)
    {
        GetLeds()[iNum] = CRGB(0, 0, 0);
    }
}

void CEngine::SetInvertPotBrightness(bool _bInvert, bool _bWriteInEEPROM)
{
    if (m_bInvertPotBrightness != _bInvert)
    {
        m_bInvertPotBrightness = _bInvert;
        uint8_t ui8InvertPotBrightness = m_bInvertPotBrightness ? uint8_t{ 1 } : uint8_t{ 0 };
        if (_bWriteInEEPROM && EEPROM.read(EEPROM_ADDRESS_INVERT_POT_BRIGHTNESS) != ui8InvertPotBrightness)
        {
            EEPROM.write(EEPROM_ADDRESS_INVERT_POT_BRIGHTNESS, ui8InvertPotBrightness);
        }
        ReadBrightness();
    }
}

bool CEngine::IsInvertPotBrightness() const
{
    return m_bInvertPotBrightness;
}

void CEngine::SetInvertPotAnimation(bool _bInvert, bool _bWriteInEEPROM)
{
    if (m_bInvertPotAnimation != _bInvert)
    {
        m_bInvertPotAnimation = _bInvert;
        uint8_t ui8InvertPotAnimation = m_bInvertPotAnimation ? uint8_t{ 1 } : uint8_t{ 0 };
        if (_bWriteInEEPROM && EEPROM.read(EEPROM_ADDRESS_INVERT_POT_ANIMATION) != ui8InvertPotAnimation)
        {
            EEPROM.write(EEPROM_ADDRESS_INVERT_POT_ANIMATION, ui8InvertPotAnimation);
        }
        ReadAnimation(false);
    }
}

bool CEngine::IsInvertPotAnimation() const
{
    return m_bInvertPotAnimation;
}

uint8_t CEngine::GetCurrentBrightness() const
{
    return m_iCurrentBrightness;
}

bool CEngine::ReadBrightness()
{
    bool bHasChangedMode = false;
    // Read brightness from potentiometer
    int iPotBrightnessValue = analogRead(m_uiPinBrightness);
    // Convert to Min / Max read value from potentiometer
    iPotBrightnessValue = constrain(iPotBrightnessValue, POT_BRIGHTNESS_MIN, POT_BRIGHTNESS_MAX);
    if (IsInvertPotBrightness())
    {
        iPotBrightnessValue = POT_BRIGHTNESS_MAX - iPotBrightnessValue + POT_BRIGHTNESS_MIN;
    }

    // Convert to MIN / MAX brightness allowed values
    uint8_t iCurrentBrightness = (uint8_t) map(iPotBrightnessValue, POT_BRIGHTNESS_MIN, POT_BRIGHTNESS_MAX, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

    auto ulCurrentTime = millis();
    if (m_iCurrentBrightness != iCurrentBrightness)
    {
        if (m_ModeChanging.m_lastTimeBrightness == 0)
        {   // START
            m_ModeChanging.m_lastTimeBrightness         = ulCurrentTime;
            m_ModeChanging.m_lastTimeBrightnessUpdate   = ulCurrentTime;
            m_ModeChanging.m_bChangeModeUp              = iCurrentBrightness > m_iCurrentBrightness;
            m_ModeChanging.m_ui8StartValue              = m_iCurrentBrightness; // Not iCurrentBrightness, take m_iCurrentBrightness
            m_ModeChanging.m_ui8ModeCount               = 1;
        }
        else
        {
            m_ModeChanging.m_lastTimeBrightnessUpdate = ulCurrentTime;
            bool bChangeModeUpOrDown = m_ModeChanging.m_bChangeModeUp;
            if (m_ModeChanging.m_ui8ModeCount % 2 == 0)
            {
                bChangeModeUpOrDown = !bChangeModeUpOrDown;
            }
            double dDeltaValueComputation = (double) (bChangeModeUpOrDown ? 1.0 : -1.0);
            if (   (bChangeModeUpOrDown  && m_iCurrentBrightness > iCurrentBrightness)  // => UP but POT go down
                || (!bChangeModeUpOrDown && m_iCurrentBrightness < iCurrentBrightness)) // => DOWN but POT go up
            {   // Changing direction
                // m_ui8ModeCount = 1 / 3 / 5 => Up (or down) from m_ui8StartValue, last value MUST be more than VALUE_DELTA_CHANGE_MODE to be taken into account
                if (    m_ModeChanging.m_ui8ModeCount % 2 == 1
                     && (dDeltaValueComputation * (((double)m_iCurrentBrightness) - ((double) m_ModeChanging.m_ui8StartValue)) < VALUE_DELTA_CHANGE_MODE_MAX)
                   )
                {
                    // Too small changes : stop it
                    m_ModeChanging.m_lastTimeBrightness = 0;
                }
                else
                {   // Validate the change
                    ++m_ModeChanging.m_ui8ModeCount;
                }
            }
        }

        m_iCurrentBrightness = iCurrentBrightness;
        if (m_setupMode.IsNull())
        {
            FastLED.setBrightness(m_iCurrentBrightness);
        }
    }
    else
    {
        if (m_ModeChanging.m_lastTimeBrightness != 0 && (ulCurrentTime - m_ModeChanging.m_lastTimeBrightness > MAX_TIME_CHANGE_MODE_MS || ulCurrentTime - m_ModeChanging.m_lastTimeBrightnessUpdate > 400))
        {
            if (m_ModeChanging.m_ui8ModeCount == 3 && m_setupMode.IsNull())
            {
                if (m_ModeChanging.m_bChangeModeUp)
                {
                    if (m_ui8CurrentAnimationsMode == (uint8_t) GetAnimationsModeSize() - 1)
                    {
                        ChangeCurrentAnimationMode(0, true);
                    }
                    else
                    {
                        ChangeCurrentAnimationMode(m_ui8CurrentAnimationsMode + 1, true);
                    }
                }
                else
                {
                    if (m_ui8CurrentAnimationsMode == 0)
                    {
                        ChangeCurrentAnimationMode(((uint8_t) GetAnimationsModeSize()) - 1, true);
                    }
                    else
                    {
                        ChangeCurrentAnimationMode(m_ui8CurrentAnimationsMode - 1, true);
                    }
                }
                bHasChangedMode = true;
            }
            else if (m_ModeChanging.m_ui8ModeCount == 5)
            {
                if (m_setupMode.IsNotNull())
                {
                    ChangeCurrentAnimationMode(INDEX_MODE_NOMODE, false); // No more mode
                    m_setupMode = nullptr;
                    ChangeCurrentAnimationMode(EEPROM.read(EEPROM_ADDRESS_CURRENT_MODE), false); // Go out of setup
                    FastLED.setBrightness(m_iCurrentBrightness);
                }
                else
                {   // Enter SETUP
                    ChangeCurrentAnimationMode(INDEX_MODE_NOMODE, false); // No more mode
                    m_setupMode = new CAnimationMode(String("Setup"), SETUP_COLOR_TEXT, new CAnimationSetup(), nullptr);
                    FastLED.setBrightness(200); // Fix brightness => light pot will change value
                    ChangeCurrentAnimationMode(0, false); // Go for setup
                }
                bHasChangedMode = true;
            }

            m_ModeChanging.m_lastTimeBrightness = 0;
        }
    }
    return bHasChangedMode;

}

void CEngine::ReadAnimation(bool _bForceNotification)
{
    // Read animation from potentiometer
    int iPotAnimationValue = analogRead(m_uiPinAnimation);
    // Convert to Min / Max read value from potentiometer
    iPotAnimationValue = constrain(iPotAnimationValue, POT_ANIMATION_MIN, POT_ANIMATION_MAX);
    if (IsInvertPotAnimation())
    {
        iPotAnimationValue = POT_ANIMATION_MAX - iPotAnimationValue + POT_ANIMATION_MIN;
    }

    // Convert to MIN / MAX brightness allowed values
    uint16_t i16CurrentAnimation = (uint16_t) map(iPotAnimationValue, POT_ANIMATION_MIN, POT_ANIMATION_MAX, 0, m_iNumberAnimation);
    i16CurrentAnimation = constrain(i16CurrentAnimation, 0, m_iNumberAnimation - 1);

    if (m_i16CurrentAnimation != i16CurrentAnimation || _bForceNotification)
    {
        m_i16CurrentAnimation = i16CurrentAnimation;
        if (m_ui8CurrentAnimationsMode < (uint8_t) m_arrAnimationsMode.size())
        {
            GetAnimationsAtIndex(m_ui8CurrentAnimationsMode)->SetCurrentAnimation(m_i16CurrentAnimation);
            if (GetAnimationsAtIndex(m_ui8CurrentAnimationsMode)->ShouldEraseBetweenAnimations())
            {
                ClearAllMatrix();
            }
        }
    }
}

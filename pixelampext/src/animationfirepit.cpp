#include "../include/animationfirepit.h"
#include "../include/engine.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                              CAnimationFirepit
// 
//========================================================================
// Play with this value. Range: from 0 (almost no fire) to 10 (biiig flames) or even more.
#define FIRE_HEIGHT 0.5

#define COMPUTEXY(x,y)  (x * kMatrixHeight + y)

CAnimationFirepit::CAnimationFirepit()
    : m_ui8CurrentAnimation(0)
{
}

CAnimationFirepit::~CAnimationFirepit()
{
    m_pFirepitEffectDatas = nullptr;
}

bool CAnimationFirepit::Loop()
{
    m_pFirepitEffectDatas->Animate();
    return false; // Always false
}

void CAnimationFirepit::Enter()
{
    m_pFirepitEffectDatas = new CFirepitEffectDatas(m_ui8CurrentAnimation);
}

void CAnimationFirepit::Leave()
{
    m_pFirepitEffectDatas = nullptr;
}
 
uint16_t CAnimationFirepit::GetMillisecondWait()
{
    return 20;
}

uint16_t CAnimationFirepit::GetNumberAnimations()
{
    return 5;
}

void CAnimationFirepit::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
    m_ui8CurrentAnimation = (uint8_t) _uiCurrentAnimation; // No more 256 differents animations
    if (m_pFirepitEffectDatas.IsNotNull())
    {
        m_pFirepitEffectDatas->SetCurrentAnimation(m_ui8CurrentAnimation);
    }
}
//========================================================================
// 
//                  CAnimationFirepit::CFirepitEffectDatas
// 
//========================================================================
CAnimationFirepit::CFirepitEffectDatas::CFirepitEffectDatas(uint8_t _ui8CurrentAnimation)
{
    Initialize();
    SetCurrentAnimation(_ui8CurrentAnimation);
}

void CAnimationFirepit::CFirepitEffectDatas::Initialize()
{
    m_pArrNoise = new uint8_t[CEngine::Instance().GetNumLeds()];
    m_pArrHeat  = new uint8_t[CEngine::Instance().GetNumLeds()];
    m_Pal = LavaColors_p;
}

void CAnimationFirepit::CFirepitEffectDatas::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    switch (_uiCurrentAnimation)
    {
        case 0:
        {
            m_Pal = LavaColors_p;
            break;
        }
        case 1:
        {
            m_Pal = OceanColors_p;
            break;
        }
        case 2:
        {
            m_Pal = CloudColors_p;
            break;
        }
        case 3:
        {
            m_Pal = ForestColors_p;
            break;
        }
        case 4:
        {
            m_Pal = HeatColors_p;
            break;
        }
    }
}

void CAnimationFirepit::CFirepitEffectDatas::Animate()
{
    auto kMatrixWidth = CEngine::Instance().GetMatrixWidth();
    auto kMatrixHeight = CEngine::Instance().GetMatrixHeight();

    uint8_t CentreX = (kMatrixWidth / 2) - 1;
    uint8_t CentreY = (kMatrixHeight / 2) - 1;

    // get one noise value out of a moving noise space
    uint16_t ctrl1 = inoise16(11 * millis(), 0, 0);
    // get another one
    uint16_t ctrl2 = inoise16(13 * millis(), 100000, 100000);
    // average of both to get a more unpredictable curve
    uint16_t  ctrl = ((ctrl1 + ctrl2) / 2);

    // this factor defines the general speed of the heatmap movement
    // high value = high speed
    uint8_t speed = 27;

    // here we define the impact of the wind
    // high factor = a lot of movement to the sides
    m_uiXPosition = 3 * ctrl * speed;

    // this is the speed of the upstream itself
    // high factor = fast movement
    m_uiYPosition = 15 * millis() * speed;

    // just for ever changing patterns we move through z as well
    m_uiZPosition = 3 * millis() * speed;

    // ...and dynamically scale the complete heatmap for some changes in the
    // size of the heatspots.
    // The speed of change is influenced by the factors in the calculation of ctrl1 & 2 above.
    // The divisor sets the impact of the size-scaling.
    m_uiScaleX = ctrl1 / 2;
    m_uiScaleY = ctrl2 / 2;

    // Calculate the noise array based on the control parameters.
    uint8_t layer = 0;
    for (uint8_t i = 0; i < kMatrixWidth; i++)
    {
        uint32_t ioffset = m_uiScaleX * (i - CentreX);
        for (uint8_t j = 0; j < kMatrixHeight; j++)
        {
            uint32_t joffset = m_uiScaleY * (j - CentreY);
            uint16_t data = ((inoise16(m_uiXPosition + ioffset, m_uiYPosition + joffset, m_uiZPosition)) + 1);
            m_pArrNoise[COMPUTEXY(i, j)] = data >> 8;
        }
    }

    // Draw the first (lowest) line - seed the fire.
    // It could be random pixels or anything else as well.
    for (uint8_t x = 0; x < kMatrixWidth; x++)
    {
        // draw
        CEngine::Instance().SetLedColor(true, x, kMatrixHeight - 1, false, false, ColorFromPalette(m_Pal, m_pArrNoise[COMPUTEXY(x, 0)]));
        // and fill the lowest line of the heatmap, too
        m_pArrHeat[CEngine::Instance().ComputePositionFromXY(true, x, kMatrixHeight - 1, false, false, true)] = m_pArrNoise[COMPUTEXY(x, 0)];
    }

    // Copy the heatmap one line up for the scrolling.
    for (uint8_t y = 0; y < kMatrixHeight - 1; y++)
    {
        for (uint8_t x = 0; x < kMatrixWidth; x++)
        {
            m_pArrHeat[CEngine::Instance().ComputePositionFromXY(true, x, y, false, false, true)] = m_pArrHeat[CEngine::Instance().ComputePositionFromXY(true, x, y + 1, false, false, true)];
        }
    }

    // Scale the heatmap values down based on the independent scrolling noise array.
    for (uint8_t y = 0; y < kMatrixHeight - 1; y++)
    {
        for (uint8_t x = 0; x < kMatrixWidth; x++)
        {
            // get data from the calculated noise field
            uint8_t dim = m_pArrNoise[COMPUTEXY(x, y)];

            // This number is critical
            // If it´s to low (like 1.1) the fire dosn´t go up far enough.
            // If it´s to high (like 3) the fire goes up too high.
            // It depends on the framerate which number is best.
            // If the number is not right you loose the uplifting fire clouds
            // which seperate themself while rising up.
            dim = (uint8_t)(dim / FIRE_HEIGHT);

            dim = 255 - dim;

            // here happens the scaling of the heatmap
            uint16_t uiLedPosition = CEngine::Instance().ComputePositionFromXY(true, x, y, false, false, true);
            assert(uiLedPosition < CEngine::Instance().GetNumLeds());
            m_pArrHeat[uiLedPosition] = scale8(m_pArrHeat[uiLedPosition], dim);
        }
    }

    // Now just map the colors based on the heatmap.
    for (uint8_t y = 0; y < kMatrixHeight - 1; y++)
    {
        for (uint8_t x = 0; x < kMatrixWidth; x++)
        {
            CEngine::Instance().SetLedColor(true, x, y, false, false, ColorFromPalette(m_Pal, m_pArrHeat[CEngine::Instance().ComputePositionFromXY(true, x, y, false, false, true)]));
        }
    }

    // Done. Bring it on!
    FastLED.show();

    // I hate this delay but with 8 bit scaling there is no way arround.
    // If the framerate gets too high the frame by frame scaling doesn´s work anymore.
    // Basically it does but it´s impossible to see then...

    // If you change the framerate here you need to adjust the
    // y speed and the dim divisor, too.
}

CAnimationFirepit::CFirepitEffectDatas::~CFirepitEffectDatas()
{
    delete []m_pArrNoise;
    delete []m_pArrHeat;
}

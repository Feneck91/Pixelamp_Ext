#include "../include/animationfirework.h"
#include "../include/engine.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                              CAnimationFirework
// 
//========================================================================
CAnimationFirework::CAnimationFirework()
{
}

CAnimationFirework::~CAnimationFirework()
{
}

bool CAnimationFirework::Loop()
{
    AnimateFireworks();
    return false; // Always false
}

void CAnimationFirework::Enter()
{
    m_pFireworkEffectDatas = new CFireworkEffectDatas();
}

void CAnimationFirework::Leave()
{
    m_pFireworkEffectDatas = nullptr;
}
 
uint16_t CAnimationFirework::GetNumberAnimations()
{
    return 1;
}

void CAnimationFirework::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
}

void CAnimationFirework::AnimateFireworks()
{
    random16_add_entropy(random());
    CRGB sky1(0, 0, 2);
    CRGB sky2(2, 0, 2);

    CEngine::Instance().SetAllLedsColor(sky1);
    CEngine::Instance().GetLeds()[random8((uint8_t) CEngine::Instance().GetNumLeds())] = sky2;

    m_pFireworkEffectDatas->m_Dot.Move(*m_pFireworkEffectDatas);
    m_pFireworkEffectDatas->m_Dot.Draw();
    for (byte b = 0; b < NUM_SPARKS; b++)
    {
        m_pFireworkEffectDatas->m_Sparks[b].Move(*m_pFireworkEffectDatas);
        m_pFireworkEffectDatas->m_Sparks[b].Draw();
    }

    FastLED.show();
    if (!m_pFireworkEffectDatas->m_Dot.m_bIsShow)
    {
        if (m_pFireworkEffectDatas->m_ui16LaunchCountDown == 0)
        {
            m_pFireworkEffectDatas->m_Dot.GroundLaunch();
            m_pFireworkEffectDatas->m_ui16LaunchCountDown = random16(350) + 1;
        }
        else
        {
            --m_pFireworkEffectDatas->m_ui16LaunchCountDown;
        }
    }

    if (m_pFireworkEffectDatas->m_bIsInSkyburst)
    {
        byte nsparks = random8(NUM_SPARKS / 2, NUM_SPARKS + 1);
        for (byte b = 0; b < nsparks; b++)
        {
            m_pFireworkEffectDatas->m_Sparks[b].Skyburst(m_pFireworkEffectDatas->m_ui8BurstX, m_pFireworkEffectDatas->m_ui8BurstY, m_pFireworkEffectDatas->m_ui8BurstXv, m_pFireworkEffectDatas->m_rgbBurstColor);
            m_pFireworkEffectDatas->m_bIsInSkyburst = false;
        }
    }
}

uint16_t CAnimationFirework::GetMillisecondWait()
{
    return 10;
}

//========================================================================
// 
//                  CAnimationFirework::CFireworkEffectDatas
// 
//========================================================================
CAnimationFirework::CFireworkEffectDatas::CFireworkEffectDatas()
{
    Initialize();
}

CAnimationFirework::CFireworkEffectDatas::~CFireworkEffectDatas()
{
}

void CAnimationFirework::CFireworkEffectDatas::Initialize()
{
    m_ui8Gravity          = FIREWORK_GRAVITY;
    m_ui8Bounce           = 200;
    m_ui8Drag             = 255;
    m_bIsInSkyburst       = false;
    m_ui16LaunchCountDown = 0;
    m_Dot.Initialize();
    for (int iIndex = 0; iIndex < NUM_SPARKS; ++iIndex)
    {
        m_Sparks[iIndex].Initialize();
    }
}

//========================================================================
// 
//                          CAnimationFirework::Dot
// 
//========================================================================
//used by fireworks animation
#define MODEL_BORDER    1
#define MODEL_WIDTH     (MODEL_BORDER + CEngine::Instance().GetMatrixWidth()  + MODEL_BORDER)
#define MODEL_HEIGHT    (MODEL_BORDER + CEngine::Instance().GetMatrixHeight() + MODEL_BORDER)

CAnimationFirework::CFireworkEffectDatas::Dot::Dot()
{
    Initialize();
}

CAnimationFirework::CFireworkEffectDatas::Dot::~Dot()
{
    Initialize();
}

void CAnimationFirework::CFireworkEffectDatas::Dot::Initialize()
{
    m_bIsShow   = false;
    m_DotType   = eTypeNone;
    x           = 0;
    y           = 0;
    xv          = 0;
    yv          = 0;
    r           = 0;
    color.setRGB(0, 0, 0);
}

void CAnimationFirework::CFireworkEffectDatas::Dot::Draw()
{
    if (m_bIsShow)
    {
        byte ix, xe, xc;
        byte iy, ye, yc;
        ScreenScale(x, MODEL_WIDTH,  ix, xe);
        ScreenScale(y, MODEL_HEIGHT, iy, ye);
        yc = 255 - ye;
        xc = 255 - xe;

        CRGB c00 = CRGB(dim8_video(scale8(scale8(color.r, yc), xc)),
                        dim8_video(scale8(scale8(color.g, yc), xc)),
                        dim8_video(scale8(scale8(color.b, yc), xc))
                       );
        CRGB c01 = CRGB(dim8_video(scale8(scale8(color.r, ye), xc)),
                        dim8_video(scale8(scale8(color.g, ye), xc)),
                        dim8_video(scale8(scale8(color.b, ye), xc))
                       );
        CRGB c10 = CRGB(dim8_video(scale8(scale8(color.r, yc), xe)),
                        dim8_video(scale8(scale8(color.g, yc), xe)),
                        dim8_video(scale8(scale8(color.b, yc), xe))
                       );
        CRGB c11 = CRGB(dim8_video(scale8(scale8(color.r, ye), xe)),
                        dim8_video(scale8(scale8(color.g, ye), xe)),
                        dim8_video(scale8(scale8(color.b, ye), xe))

                       );
        CEngine::Instance().AddLedColor(false, ix,     iy,     true, false, c00);
        CEngine::Instance().AddLedColor(false, ix,     iy + 1, true, false, c01);
        CEngine::Instance().AddLedColor(false, ix + 1, iy,     true, false, c10);
        CEngine::Instance().SetLedColor(false, ix + 1, iy + 1, true, false, c11);
    }
}

int16_t CAnimationFirework::CFireworkEffectDatas::Dot::Scale15by8Local(int16_t _i16Value, fract8 _ui8Scale)
{
    return (int16_t) (((int32_t)((int32_t)_i16Value * _ui8Scale)) / 256);
}

void CAnimationFirework::CFireworkEffectDatas::Dot::Move(CAnimationFirework::CFireworkEffectDatas& _rDataEffects)
{
    saccum78 oyv = yv;

    if (m_bIsShow)
    {
        yv -= _rDataEffects.m_ui8Gravity;
        xv = CAnimationFirework::CFireworkEffectDatas::Dot::Scale15by8Local(xv, _rDataEffects.m_ui8Drag);
        yv = CAnimationFirework::CFireworkEffectDatas::Dot::Scale15by8Local(yv, _rDataEffects.m_ui8Drag);

        if (m_DotType == eTypeSpark)
        {
            xv = CAnimationFirework::CFireworkEffectDatas::Dot::Scale15by8Local(xv, _rDataEffects.m_ui8Drag);
            yv = CAnimationFirework::CFireworkEffectDatas::Dot::Scale15by8Local(yv, _rDataEffects.m_ui8Drag);
            color.nscale8(255);
            if (!color)
            {
                m_bIsShow = false;
            }
        }

        // If we'd hit the ground, bounce
        if (yv < 0 && (y < (-yv)))
        {
            if (m_DotType == eTypeSpark)
            {
                m_bIsShow = false;
            }
            else
            {
                yv = -yv;
                yv = CAnimationFirework::CFireworkEffectDatas::Dot::Scale15by8Local(yv, _rDataEffects.m_ui8Bounce);
                if (yv < 500)
                {
                    m_bIsShow = false;
                }
            }
        }

        if ((yv < -300) /* && (!(oyv < 0))*/)
        {
            // pinnacle
            if (m_DotType == CAnimationFirework::CFireworkEffectDatas::Dot::eTypeShell)
            {
                if ((y > (uint16_t)(0x8000)) /*&& (random8() < 64)*/)
                {
                    // boom
                    FastLED.showColor(CRGB::White);
                    //FastLED.delay(1);
                    FastLED.showColor(CRGB::Black);
                }

                m_bIsShow = false;
                _rDataEffects.m_bIsInSkyburst = true;
                _rDataEffects.m_ui8BurstX     = x;
                _rDataEffects.m_ui8BurstY     = y;
                _rDataEffects.m_ui8BurstXv    = xv;
                _rDataEffects.m_ui8BurstYv    = yv;
                _rDataEffects.m_rgbBurstColor = color;
            }
        }

        if (m_DotType == eTypeSpark)
        {
            if (((xv > 0) && (x > xv)) || ((xv < 0) && (x < (0xFFFF + xv))))
            {
                x += xv;
            }
            else
            {
                m_bIsShow = false;
            }
        }
        else
        {
            x += xv;
        }
        y += yv;
    }
}

void CAnimationFirework::CFireworkEffectDatas::Dot::GroundLaunch()
{
    m_DotType   = CAnimationFirework::CFireworkEffectDatas::Dot::eTypeShell;
    yv          = 600 + random16(300 + (25 * CEngine::Instance().GetMatrixHeight()));
    xv          = (int16_t) random16(600) - (int16_t) 300;
    y           = 0;
    x           = 0x8000;
    m_bIsShow   = true;
    hsv2rgb_rainbow(CHSV(random8(), 240, 200), color);
}

void CAnimationFirework::CFireworkEffectDatas::Dot::Skyburst(accum88 basex, accum88 basey, saccum78 basedv, CRGB& basecolor)
{
    yv          = (int16_t)0 + (int16_t)random16(1500) - (int16_t)500;
    xv          = basedv + (int16_t)random16(2000) - (int16_t)1000;
    y           = basey;
    x           = basex;
    color       = basecolor;
    color       *= 4;
    m_DotType   = eTypeSpark;
    m_bIsShow   = true;
}

//fireworks
void CAnimationFirework::CFireworkEffectDatas::Dot::ScreenScale(accum88 a, byte N, byte& screen, byte& screenerr)
{
    byte ia     = a >> 8;
    screen      = scale8(ia, N);
    byte m      = screen * (256 / N);
    screenerr   = (ia - m) * scale8(255, N);
}

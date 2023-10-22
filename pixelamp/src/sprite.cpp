#include "../include/engine.h"
#include "../include/sprite.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                               CSprite
// 
//========================================================================
CSprite::CSprite()
    : m_ui8Width(0)
    , m_ui8Height(0)
    , m_X(0)
    , m_Y(0)
    , m_i8MxdX(0)
    , m_i8MxdY(0)
{
}

CSprite::CSprite(uint8_t _ui8Width, uint8_t _ui8Height, uint8_t _io8X, uint8_t _io8Y, int8_t _i8MxdX, int8_t _i8MxdY, const vector<uint32_t> & _rDatas)
    : m_ui8Width(_ui8Width)
    , m_ui8Height(_ui8Height)
    , m_X(_io8X)
    , m_Y(_io8Y)
    , m_i8MxdX(_i8MxdX)
    , m_i8MxdY(_i8MxdY)
    , m_datas(_rDatas)
{
}

/// <summary>
/// Copy constructor.
/// </summary>
CSprite::CSprite(const CSprite & _ref)
    : m_ui8Width(_ref.m_ui8Width)
    , m_ui8Height(_ref.m_ui8Height)
    , m_X(_ref.m_X)
    , m_Y(_ref.m_Y)
    , m_i8MxdX(_ref.m_i8MxdX)
    , m_i8MxdY(_ref.m_i8MxdY)
    , m_datas(_ref.m_datas)
{
}

bool CSprite::ShowSprite(bool _bIgnoreMatrixDelta, bool _bWrapX, bool _bWrapY)
{
    CEngine &rEngine = CEngine::Instance();
    bool bAteLeastOnePixelVisible = false;

    for (led_coordinate Y = 0; Y < m_ui8Height; ++Y)
    {
        for (led_coordinate X = 0; X < m_ui8Width; ++X)
        {
            led_coordinate nx = m_X + X;
            led_coordinate ny = m_Y + Y;
            if (rEngine.SetLedColor(_bIgnoreMatrixDelta, nx, ny, _bWrapX, _bWrapY, CRGB(m_datas[X + (Y * m_ui8Width)])))
            {
                bAteLeastOnePixelVisible = true;
            }
        }
    }
    return bAteLeastOnePixelVisible;
}

/// Sprite fusion.
shared_ptr<CSprite> CSprite::Fusion(const CSprite & _rSprite) const
{
/*
    m_io8X
    CSprite<N>
*/
    return shared_ptr<CSprite>(new CSprite());
}

/// Sprite fusion.
void CSprite::Fusion(const CSprite& _rSprite)
{
/*

    uint8_t ui8width  = m_ui8Width + _rSprite.m_ui8Width;
    uint8_t ui8Height = max(m_ui8Height, _rSprite.m_ui8Height + 1);
    m_ui8Height = ;
    m_io8X;
    m_io8Y;
*/
}

void CSprite::MoveSprite()
{
    m_X += m_i8MxdX;
    m_Y += m_i8MxdY;
}

void CSprite::MoveSprite(uint8_t _i8MxdX, uint8_t _i8MxdY)
{
    m_X += _i8MxdX;
    m_Y += _i8MxdY;
}

//========================================================================
// 
//               All functions / structures for progmem sprites
// 
//========================================================================
const ColorReplacement ColorReplacement::m_EmptyColorReplacement;

ColorReplacement::ColorReplacement()
{
}

void ColorReplacement::AddColorReplacement(const stColors & _rstColorInfos)
{
    m_arrColorsReplacement.push_back(_rstColorInfos);
}

uint32_t ColorReplacement::GetColor(uint8_t _uiColorNum) const
{
    for (auto it = m_arrColorsReplacement.begin(); it != m_arrColorsReplacement.end(); ++it)
    {
        if ((*it).m_iColorNum == _uiColorNum)
        {
            return (*it).m_iColorReplacement;
        }
    }

    return 0;
}

void ColorReplacement::Clear()
{
    m_arrColorsReplacement.clear();
}

const ColorReplacement & ColorReplacement::EmptyColorReplacement()
{
    return m_EmptyColorReplacement;
}

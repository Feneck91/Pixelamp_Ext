#pragma once

#include <stdint.h>
#include "vector.h"
#include "shared_ptr.h"

/// <summary>
/// Used for X / Y led position.
/// </summary>
typedef int32_t                                 led_coordinate;

/// <summary>
/// Class for sprites managment.
/// </summary>
class CSprite : public IRefCountImpl<CSprite>
{
public:
    /// <summary>
    /// Width of the sprite.
    ///
    /// It's the number of colors of each row into m_datas.
    /// </summary>
    uint8_t             m_ui8Width;

    /// <summary>
    /// Height of the sprite.
    ///
    /// It's the number rows into m_datas.
    /// </summary>
    uint8_t             m_ui8Height;

    /// <summary>
    /// Original X position of the sprite.
    /// </summary>
    led_coordinate      m_X;

    /// <summary>
    /// Original Y position of the sprite.
    /// </summary>
    led_coordinate      m_Y;

    /// <summary>
    /// Move offset for X.
    /// 
    /// Used into move function.
    /// </summary>
    int8_t              m_i8MxdX;

    /// <summary>
    /// Move offset for Y.
    /// 
    /// Used into move function.
    /// </summary>
    int8_t              m_i8MxdY;

    /// <summary>
    /// Sprite colors.
    /// </summary>
    vector<uint32_t>    m_datas;

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    CSprite();

    /// <summary>
    /// Constructor.
    /// </summary>
    CSprite(uint8_t _ui8Width, uint8_t _ui8Height, led_coordinate _X, led_coordinate _Y, int8_t _i8MxdX, int8_t _i8MxdY, const vector<uint32_t> & _rDatas);

    /// <summary>
    /// Copy constructor.
    /// </summary>
    CSprite(const CSprite &_ref);

    /// <summary>
    /// Display the sprite.
    /// </summary>
    /// <param name="_bIgnoreMatrixDelta">Ignore matrix delta or not.</param>
    /// <param name="_bWrapX">Wrap X or not</param>
    /// <param name="_bWrapY">Wrap Y or not</param>
    /// <param name="_bTransparent">Don't display black pixel.</param>
    /// <returns>true if at least one pixel is visible, false else.</returns>
    bool                        ShowSprite(bool _bIgnoreMatrixDelta, bool _bWrapX, bool _bWrapY, bool _bTransparent = false);

    /// <summary>
    /// Revert the sprite.
    /// </summary>
    void                        RevertSprite();

    /// <summary>
    /// Move the sprite.
    /// 
    /// The sprite is modified.
    /// </summary>
    void                        MoveSprite();

    /// <summary>
    /// Move the sprite.
    /// 
    /// The sprite is modified.
    /// </summary>
    /// <param name="_i8MxdX">X move.</param>
    /// <param name="_i8MxdY">Y Move.</param>
    void                        MoveSprite(int8_t _i8MxdX, int8_t _i8MxdY);
};

/// <summary>
/// Class used to replace color into sprite.
/// </summary>
class ColorReplacement
{
public:
    struct stColors
    {
        uint8_t     m_iColorNum;
        uint32_t    m_iColorReplacement;
    };
private:
    /// <summary>
    /// Array of color replacement informations.
    /// </summary>
    vector<stColors>                m_arrColorsReplacement;

    /// <summary>
    /// Empty color remplacement.
    /// </summary>
    static const ColorReplacement   m_EmptyColorReplacement;

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    ColorReplacement();

    /// <summary>
    /// Add new color replacement.
    /// </summary>
    /// <param name="_rstColorInfos">Infos about the color to replace.</param>
    void                            AddColorReplacement(const stColors & _rstColorInfos);

    /// <summary>
    /// Get the color for this num color.
    /// </summary>
    /// <param name="_uiColorNum">The color if found else return 0.</param>
    /// <returns></returns>
    uint32_t                        GetColor(uint8_t _uiColorNum) const;

    /// <summary>
    /// Clear all colors.
    /// </summary>
    void                            Clear();

    /// <summary>
    /// Get empty color remplacement.
    /// </summary>
    /// <returns>An empty color replacement (replace 0 color).</returns>
    static const ColorReplacement & EmptyColorReplacement();
};

/// <summary>
/// Sprite structure.
/// 
/// Used to write sprite definition into prog memory.
/// Only 8 bits so, only 256 colors.
/// </summary>
/// <typeparam name="N">Number on lines in datas</typeparam>
template <uint32_t N> struct SpriteDefinition
{
    /// <summary>
    /// Width of the sprite.
    ///
    /// It's the number of colors of each row into m_datas.
    /// </summary>
    uint8_t             m_ui8Width;

    /// <summary>
    /// Height of the sprite.
    ///
    /// It's the number rows into m_datas.
    /// </summary>
    uint8_t             m_ui8Height;

    /// <summary>
    /// Original X position of the sprite.
    /// </summary>
    led_coordinate      m_X;

    /// <summary>
    /// Original Y position of the sprite.
    /// </summary>
    led_coordinate      m_Y;

    /// <summary>
    /// Move offset for X.
    /// 
    /// Used into move function.
    /// </summary>
    int8_t              m_i8MxdX;

    /// <summary>
    /// Move offset for Y.
    /// 
    /// Used into move function.
    /// </summary>
    int8_t              m_i8MxdY;

    /// <summary>
    /// Sprite colors (indexes only, not 32 bit, only 8 bits so 256 colors max).
    /// </summary>
    uint8_t             m_datas[N];

    /// <summary>
    /// Copy this into new Sprite class.
    /// 
    /// Must called on sprite that is not store into prog memory.
    /// </summary>
    /// <param name="_rColorReplacement">To replace colors.</param>
    /// <returns>A pointer on sprite class.</returns>
    shared_ptr<CSprite> ToSpriteNoProgMem(const ColorReplacement& _rColorReplacement) const
    {
        vector<uint32_t> arrDatas;
        for (int iIndex = 0; iIndex < N; ++iIndex)
        {
            arrDatas.push_back(_rColorReplacement.GetColor(m_datas[iIndex]));
        }
        return shared_ptr<CSprite>(new CSprite(m_ui8Width, m_ui8Height, m_X, m_Y, m_i8MxdX, m_i8MxdY, arrDatas));
    }

    /// <summary>
    /// Copy this into new Sprite class.
    /// 
    /// Must be called on sprite that is store into prog memory.
    /// </summary>
    /// <param name="_rColorReplacement">To replace colors.</param>
    /// <returns>A pointer on sprite class.</returns>
    shared_ptr<CSprite> ToSprite(const ColorReplacement& _rColorReplacement) const
    {
        SpriteDefinition<N> value;
        memcpy_P(&value, this, sizeof(*this));
        return value.ToSpriteNoProgMem(_rColorReplacement);
    }
};

//========================================================================
// 
//               All functions / structures for progmem sprites
// 
//========================================================================

/// <summary>
/// Class used to have pointer on sprite into prog memomy.
/// </summary>
class SpriteDefinitionPtr : public IRefCountImpl<SpriteDefinitionPtr>
{
public:
    /// <summary>
    /// Copy this into new Sprite class.
    /// </summary>
    /// <param name="_rColorReplacement">To replace colors.</param>
    /// <returns>A pointer on sprite class.</returns>
    virtual shared_ptr<CSprite> ToSprite(const ColorReplacement &_rColorReplacement) const = 0;
};

template<uint32_t N> class SpriteDefinitionPtrTemlate : public SpriteDefinitionPtr
{
public:
    /// <summary>
    /// Pointer on sprite.
    /// </summary>
    const SpriteDefinition<N>& m_rSpriteDefinition;

    SpriteDefinitionPtrTemlate(const SpriteDefinition<N>& _rSpriteDefinition)
        : m_rSpriteDefinition(_rSpriteDefinition)
    {
    }

    /// <summary>
    /// Copy this into new Sprite class.
    /// </summary>
    /// <param name="_rColorReplacement">To replace colors.</param>
    /// <returns>A pointer on sprite class.</returns>
    virtual shared_ptr<CSprite> ToSprite(const ColorReplacement &_rColorReplacement) const override
    {
        return m_rSpriteDefinition.ToSprite(_rColorReplacement);
    }
};

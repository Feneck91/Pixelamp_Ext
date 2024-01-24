#include "..\include\pixellampchipset.h"
#include "..\include\engine.h"

//========================================================================
// 
//                            PixelLampChipsetBase
// 
//========================================================================
PixelLampChipsetBase::PixelLampChipsetBase()
{
#ifdef NO_SOLDERING_LED
    // Compute all led to ingore (force to black all the time)
    if (m_arrIndexesSetToBlack.size() == 0)
    {
        for (led_coordinate x = 0; x < CEngine::Instance().GetRealMatrixWidth(); ++x)
        {
            for (led_coordinate y = 0; y < CEngine::Instance().GetRealMatrixHeight(); ++y)
            {
                if (CEngine::Instance().GetLedStatusFromRealPosition(x, y) == CEngine::eLedStatusIgnored)
                {
                    m_arrIndexesSetToBlack.push_back(CEngine::Instance().ComputePositionFromRealXY(x, y));
                }
            }
        }
    }
#endif
}

PixelLampChipsetBase::~PixelLampChipsetBase()
{
}

void PixelLampChipsetBase::ManagePixels(CRGB* _pDatas)
{
    // Must clear all LEDs that are not in scope (set to black)
#ifdef NO_SOLDERING_LED
    auto rgbBlack = CRGB(0, 0, 0);
    for (uint16_t ui16LedIndex : m_arrIndexesSetToBlack)
    {
        _pDatas[ui16LedIndex] = rgbBlack;
    }
#endif
}

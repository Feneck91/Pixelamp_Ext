#include "enginedefines.h"
#include "animationbase.h"
#include "vector.h"

#pragma once

#ifdef SIMULATION
/// <summary>
/// Class used to notify simulator when leds changed or when pixels are shown.
/// 
/// Be careful, when instanciating this class, you must add this line in code to prevent link error.
/// 
/// ISimulatorController<the_color_order>* ISimulatorController<the_color_order>::m_pSimulator = nullptr;
/// </summary>
/// <typeparam name="ColorOrder">The color order.</typeparam>
template <EOrder ColorOrder>
class ISimulatorController
{
private:
    /// <summary>
    /// Singleton instance of simulator.
    /// </summary>
    static ISimulatorController<ColorOrder>* m_pSimulator;

public:
    /// <summary>
    /// Set the simulator.
    /// </summary>
    /// <param name="_pSimulator">The simulator</param>
    void SetSimulator(ISimulatorController<ColorOrder>* _pSimulator)
    {
        m_pSimulator = _pSimulator;
    }

    /// <summary>
    /// Get the static pointer on simulator.
    /// </summary>
    /// <returns>A reference on simulator.</returns>
    ISimulatorController<ColorOrder>* GetSimulator()
    {
        return m_pSimulator;
    }

    virtual void init()
    {
    }

    virtual void onShowPixels(PixelController<ColorOrder>& _rPixels)
    {
    }
};
#endif 

/// <summary>
/// Main class that manage the chipset.
/// </summary>
class PixelLampChipsetBase
{
#ifdef NO_SOLDERING_LED
    /// <summary>
    /// Vector of pixels to set to black.
    /// </summary>
    vector<uint16_t> m_arrIndexesSetToBlack;
#endif

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    PixelLampChipsetBase();
        
    /// <summary>
    /// Virtual destructor.
    /// </summary>
    virtual ~PixelLampChipsetBase();

    /// <summary>
    /// Used to manage pixels.
    /// </summary>
    void ManagePixels(CRGB *_pDatas);
};

/// <summary>
/// Class that manage the chipset and the simulator.
/// </summary>
template<template<uint8_t DataLedPin, EOrder ColorOrder> class Chipset, uint8_t DataLedPin, EOrder ColorOrder>
class PixelLampChipset : public PixelLampChipsetBase
                       , public Chipset<DataLedPin, ColorOrder>
#ifdef SIMULATION
                       , ISimulatorController<ColorOrder>
#endif
{
    /// <summary>
    /// Get number of leds.
    /// </summary>
    uint16_t    m_uiNumLeds;

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    PixelLampChipset(CRGB *_pLeds, uint16_t _uiNumLeds)
    {
        // Initialize FastLed chipset
        m_uiNumLeds = _uiNumLeds;
        FastLED.addLeds(this, _pLeds, _uiNumLeds).setCorrection(Halogen);
    }

    /// <summary>
    /// Constructor.
    /// </summary>
    virtual ~PixelLampChipset() override
    {
    }

protected:
#ifdef SIMULATION
    /// Initialize the LED controller
    virtual void init() override
    {
        if (ISimulatorController<ColorOrder>::GetSimulator() != nullptr)
        {
            ISimulatorController<ColorOrder>::GetSimulator()->init();
        }
    }
#endif

    virtual void showPixels(PixelController<ColorOrder>& _rPixels) override
    {
        PixelController<ColorOrder> * pPixelController = &_rPixels;
        shared_ptr<CRGB> pTmpLedData = nullptr;
        shared_ptr<PixelController<ColorOrder, 1, 0xFFFFFFFF>> pTmpPixelController = nullptr;

        if (_rPixels.mAdvance == 0)
        {
            CRGB* pLeds = new CRGB[m_uiNumLeds];
            pTmpLedData = pLeds;
            pTmpPixelController = pPixelController = new PixelController<ColorOrder, 1, 0xFFFFFFFF>(pTmpLedData.GetPtr(), m_uiNumLeds, _rPixels.mScale, BINARY_DITHER);
            pTmpPixelController->mAdvance = 3;
            uint16_t uiNumLeds(m_uiNumLeds);
            while (uiNumLeds--)
            {
                memmove8(&pLeds[uiNumLeds], _rPixels.mData, sizeof(CRGB));
            }
            uiNumLeds = 0;
        }

        ManagePixels((CRGB*) pPixelController->mData);
        Chipset<DataLedPin, ColorOrder>::showPixels(*pPixelController);

#ifdef SIMULATION
        if (ISimulatorController<ColorOrder>::GetSimulator() != nullptr)
        {
            ISimulatorController<ColorOrder>::GetSimulator()->onShowPixels(*pPixelController);
        }
#endif 
    }
};
#pragma once
#include "animationbase.h"
#include "shared_ptr.h"

/// <summary>
/// Class that manage firewpit animations.
/// </summary>
class CAnimationFirepit : public CAnimationBase
{
private:
    //=========================
    //
    // Firepit Effect data
    //
    //=========================
    class CFirepitEffectDatas : public IRefCountImpl<CFirepitEffectDatas>
    {
    public:
        uint32_t                m_uiXPosition;
        uint32_t                m_uiYPosition;
        uint32_t                m_uiZPosition;
        uint32_t                m_uiScaleX;
        uint32_t                m_uiScaleY;
        uint8_t                 *m_pArrNoise;
        
        /// <summary>
        /// Heatmap data with the size matrix kMatrixWidth * kMatrixHeight
        /// </summary>
        uint8_t                 *m_pArrHeat;

        /// <summary>
        /// RGB Palette.
        /// </summary>
        CRGBPalette16           m_Pal;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="_ui8CurrentAnimation">Current animation.</param>
        CFirepitEffectDatas(uint8_t _ui8CurrentAnimation);

        /// <summary>
        /// Destructor.
        /// </summary>
        ~CFirepitEffectDatas();

        /// <summary>
        /// Initialize internal datas.
        /// </summary>
        void Initialize();

        /// <summary>
        /// Set the current animation.
        /// 
        /// The differents animations are palettes colors.
        /// </summary>
        /// <param name="_uiCurrentAnimation">The animation to set.</param>
        void SetCurrentAnimation(uint16_t _uiCurrentAnimation);

        /// <summary>
        /// Animate Firepit.
        /// </summary>
        void Animate();
    };
    
    /// <summary>
    /// Class that manage fireworks effect.
    /// </summary>
    shared_ptr<CFirepitEffectDatas>         m_pFirepitEffectDatas;

    /// <summary>
    /// The current animation.
    /// </summary>
    uint8_t                                 m_ui8CurrentAnimation;

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    CAnimationFirepit();

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationFirepit() override;

    /// <summary>
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
};

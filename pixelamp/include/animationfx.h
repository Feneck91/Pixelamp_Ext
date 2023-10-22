#pragma once
#include "animationbase.h"

/// <summary>
/// Class that manage Fx animations.
/// </summary>
class CAnimationFx : public CAnimationBase
{
private:
    //=========================
    //
    // Hue / Rotation hue
    //
    //=========================
    class CFxHueRotationDatas : public IRefCountImpl<CFxHueRotationDatas>
    {
        /// <summary>
        /// Pointer on function on animation.
        /// </summary>
        void (CFxHueRotationDatas::* m_funcAnimation)();

        /// <summary>
        /// Hue index.
        /// </summary>
        uint8_t m_uiHue;
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="_ui8CurrentAnimation">Current animation.</param>
        CFxHueRotationDatas(uint8_t _ui8CurrentAnimation);

        /// <summary>
        /// Destructor.
        /// </summary>
        ~CFxHueRotationDatas();

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
        /// Animate FxHue.
        /// </summary>
        void Animate();
    private:
        /// <summary>
        /// Animate FxHue.
        /// </summary>
        void AnimateHue();

        /// <summary>
        /// Animate FxHue.
        /// </summary>
        void AnimateHueRotation();

        /// <summary>
        /// Draw one frame of fx for Hue rotation.
        /// </summary>
        /// <param name="_byStartHue8">Start</param>
        /// <param name="_iYHueDelta8">Delta Y</param>
        /// <param name="_iXHueDelta8">Delta X</param>
        void DrawOneFrameHueRotation(byte _byStartHue8, int8_t _iYHueDelta8, int8_t _iXHueDelta8);
    };
    
    /// <summary>
    /// Class that manage Hue effect.
    /// </summary>
    shared_ptr<CFxHueRotationDatas>         m_pFxHueDatas;

    /// <summary>
    /// The current animation.
    /// </summary>
    uint8_t                                 m_ui8CurrentAnimation;

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    CAnimationFx();

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationFx() override;

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

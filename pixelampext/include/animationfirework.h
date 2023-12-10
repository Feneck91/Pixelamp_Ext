#pragma once
#include "animationbase.h"

#define NUM_SPARKS                      30
#define FIREWORK_GRAVITY                10

/// <summary>
/// Class that manage firework animations.
/// </summary>
class CAnimationFirework : public CAnimationBase
{
private:
    //=========================
    //
    // Firework Effect data
    //
    //=========================
    class CFireworkEffectDatas : public IRefCountImpl<CFireworkEffectDatas>
    {
        /// <summary>
        /// Dot.
        /// </summary>
        class Dot
        {
        public:
            /// <summary>
            /// Dot type.
            /// </summary>
            enum eType
            {
                eTypeNone,
                eTypeShell,
                eTypeSpark,
            }                   m_DotType;

            /// <summary>
            /// Is displayed?
            /// </summary>
            bool                m_bIsShow;

            accum88             x;
            accum88             y;
            saccum78            xv;
            saccum78            yv;
            accum88             r;
            CRGB                color;

            /// <summary>
            /// Constructor.
            /// </summary>
            Dot();

            /// <summary>
            /// Destructor.
            /// </summary>
            ~Dot();

            /// <summary>
            /// Initialize the class.
            /// </summary>
            void                Initialize();

            void                Draw();
            void                Move(CFireworkEffectDatas& _rDataEffects);
            void                GroundLaunch();
            void                Skyburst(accum88 basex, accum88 basey, saccum78 basedv, CRGB& basecolor);
            void                ScreenScale(accum88 a, byte N, byte& screen, byte& screenerr);
            static int16_t      Scale15by8Local(int16_t i, fract8 scale);
        };

    public:
        uint16_t                m_ui16LaunchCountDown;
        CRGB                    m_rgbOverrun;
        saccum78                m_ui8Gravity;
        fract8                  m_ui8Bounce;
        fract8                  m_ui8Drag;

        /// <summary>
        /// Is Skyburst ?
        /// </summary>
        bool                    m_bIsInSkyburst;

        accum88                 m_ui8BurstX;
        accum88                 m_ui8BurstY;
        saccum78                m_ui8BurstXv;
        saccum78                m_ui8BurstYv;
        CRGB                    m_rgbBurstColor;

        Dot                     m_Dot;
        Dot                     m_Sparks[NUM_SPARKS];

        /// <summary>
        /// Constructor.
        /// </summary>
        CFireworkEffectDatas();

        /// <summary>
        /// Destructor.
        /// </summary>
        ~CFireworkEffectDatas();

        /// <summary>
        /// Initialize internal datas.
        /// </summary>
        void Initialize();

    };
    
    /// <summary>
    /// Class that manage fireworks effect.
    /// </summary>
    shared_ptr<CFireworkEffectDatas>        m_pFireworkEffectDatas;

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    CAnimationFirework();

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationFirework() override;

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

private:
    /// <summary>
    /// Animate Fireworks.
    /// </summary>
    void                                    AnimateFireworks();
};

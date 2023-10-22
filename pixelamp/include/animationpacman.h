#pragma once
#include "animationtext.h"

/// <summary>
/// Class that manage PacMan animations.
/// </summary>
class CAnimationPacMan : public CAnimationBase
{
private:
    //=========================
    //
    // Hue / Rotation hue
    //
    //=========================
    class CAnimationPacManDatas : public IRefCountImpl<CAnimationPacManDatas>
    {
        /// <summary>
        /// Pointer on function on animation.
        /// </summary>
        void (CAnimationPacManDatas::* m_funcAnimation)();

        /// <summary>
        /// Animations array.
        /// </summary>
        vector<shared_ptr<CAnimationBase>>      m_arrAnimations;

        /// <summary>
        /// Pacman has opened mouth?
        /// </summary>
        bool                                    m_bIsMouthIsOpened;

        /// <summary>
        /// Sprite Pacman with opened mouth.
        /// </summary>
        shared_ptr<CSprite>                     m_PacmanOpenMouth;

        /// <summary>
        /// Sprite Pacman with closed mouth.
        /// </summary>
        shared_ptr<CSprite>                     m_PacmanClosedMouth;

        /// <summary>
        /// Sprite ghost.
        /// </summary>
        shared_ptr<CSprite>                     m_Ghost;

        /// <summary>
        /// Pacgum.
        /// </summary>
        shared_ptr<CSprite>                     m_PacGum1;

        /// <summary>
        /// Pacgum.
        /// </summary>
        shared_ptr<CSprite>                     m_PacGum2;

        /// <summary>
        /// Pacgum.
        /// </summary>
        shared_ptr<CSprite>                     m_PacGum3;

        /// <summary>
        /// Pacgum.
        /// </summary>
        shared_ptr<CSprite>                     m_PacGum4;
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="_ui8CurrentAnimation">Current animation.</param>
        CAnimationPacManDatas(uint8_t _ui8CurrentAnimation);

        /// <summary>
        /// Destructor.
        /// </summary>
        ~CAnimationPacManDatas();

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
        /// Animate packman only.
        /// </summary>
        void AnimatePacman();

        /// <summary>
        /// Animate Pacman chase.
        /// </summary>
        void AnimatePacChase();
    };

    /// <summary>
    /// The current animation.
    /// </summary>
    uint8_t                                 m_ui8CurrentAnimation;

    /// <summary>
    /// Class that manage pacman animations.
    /// </summary>
    shared_ptr<CAnimationPacManDatas>       m_pPacmanDatas;
public:
    /// <summary>
    /// Constructor.
    /// </summary>
    CAnimationPacMan();

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~CAnimationPacMan() override;

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

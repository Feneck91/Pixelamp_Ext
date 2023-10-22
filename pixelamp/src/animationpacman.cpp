#include "../include/animationpacman.h"
#include "../include/sprite.h"
#include "../include/engine.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//                          CAnimationPacMan
// 
//========================================================================
CAnimationPacMan::CAnimationPacMan()
{
    m_ui8CurrentAnimation = 0;
}

CAnimationPacMan::~CAnimationPacMan()
{
    m_pPacmanDatas = nullptr;
}

bool CAnimationPacMan::Loop()
{
    if (m_pPacmanDatas.IsNotNull())
    {
        m_pPacmanDatas->Animate();
    }
    return false; // Always false
}

void CAnimationPacMan::Enter()
{
    m_pPacmanDatas = new CAnimationPacManDatas(m_ui8CurrentAnimation);
}

void CAnimationPacMan::Leave()
{
    m_pPacmanDatas = nullptr;
}

uint16_t CAnimationPacMan::GetMillisecondWait()
{
    return 150;
}

uint16_t CAnimationPacMan::GetNumberAnimations()
{
    return CAnimationBase::GetNumberAnimations() + 2;
}

void CAnimationPacMan::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
    m_ui8CurrentAnimation = _uiCurrentAnimation;
    if (m_pPacmanDatas.IsNotNull())
    {
        m_pPacmanDatas->SetCurrentAnimation(m_ui8CurrentAnimation);
    }
}

//========================================================================
// 
//                          Pacman Sprites
// 
//========================================================================

// 1 = CRGB::Gold
// 2 = CRGB::Blue
// 3 = CRGB::HotPink
// 4 = CRGB::White
// 5 = CRGB::DeepSkyBlue
// 6 = CRGB::Red
// 7 = CRGB::Orange
const SpriteDefinition<72> pacmanOpenMouth PROGMEM
{
  9, 8,
  8, 0,
  1, 0,
  {
    0, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 2, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0,
  }
};

const SpriteDefinition<72> pacmanClosedMouth PROGMEM
{
  9, 8,
  8, 0,
  1, 0,
  {
    0, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 2, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0,
  }
};

const SpriteDefinition<72> pinkGhost PROGMEM
{
  9, 8,
  0, 0,
  1, 0,
  {
    0, 0, 0, 3, 3, 3, 0, 0, 0,
    0, 0, 3, 3, 3, 3, 3, 0, 0,
    0, 3, 4 , 4, 3, 4, 4, 3, 0,
    0, 3, 2 , 4, 3, 2, 4, 3, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 0,
    0, 3, 0, 3, 0, 3, 0, 3, 0
  }
};

const SpriteDefinition<72> blueGhost PROGMEM
{
  9, 8,
  0, 0,
  1, 0,
  {
    0, 0, 0, 5, 5, 5, 0, 0, 0,
    0, 0, 5, 5, 5, 5, 5, 0, 0,
    0, 5, 4, 4, 5, 4, 4, 5, 0,
    0, 5, 3, 4, 5, 3, 4, 5, 0,
    0, 5, 5, 5, 5, 5, 5, 5, 0,
    0, 5, 5, 5, 5, 5, 5, 5, 0,
    0, 5, 5, 5, 5, 5, 5, 5, 0,
    0, 5, 0, 5, 0, 5, 0, 5, 0
  }
};

const SpriteDefinition<72> orangeGhost PROGMEM
{
  9, 8,
  0, 0,
  1, 0,
  {
    0, 0, 0, 7, 7, 7, 0, 0, 0,
    0, 0, 7, 7, 7, 7, 7, 0, 0,
    0, 7, 4, 4, 7, 4, 4, 7, 0,
    0, 7, 2, 4, 7, 2, 4, 7, 0,
    0, 7, 7, 7, 7, 7, 7, 7, 0,
    0, 7, 7, 7, 7, 7, 7, 7, 0,
    0, 7, 7, 7, 7, 7, 7, 7, 0,
    0, 7, 0, 7, 0, 7, 0, 7, 0
  }
};

const SpriteDefinition<72> redGhost PROGMEM
{
  9, 8,
  0, 0,
  1, 0,
  {
    0, 0, 0, 6, 6, 6, 0, 0, 0,
    0, 0, 6, 6, 6, 6, 6, 0, 0,
    0, 6, 4, 4, 6, 4, 4, 6, 0,
    0, 6, 2, 4, 6, 2, 4, 6, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 6, 0, 6, 0, 6, 0, 6, 0
  }
};

const SpriteDefinition<4> pacGum PROGMEM
{
  2, 2 ,
  0, 3,
  0, 0,
  {
    4, 4,
    4, 4,
  }
};

//========================================================================
// 
//              CAnimationPacMan::CAnimationPacManDatas
// 
//========================================================================
CAnimationPacMan::CAnimationPacManDatas::CAnimationPacManDatas(uint8_t _ui8CurrentAnimation)
{
    Initialize();
    SetCurrentAnimation(_ui8CurrentAnimation);
}

CAnimationPacMan::CAnimationPacManDatas::~CAnimationPacManDatas()
{
}

void CAnimationPacMan::CAnimationPacManDatas::Initialize()
{
    m_funcAnimation = nullptr;
}

void CAnimationPacMan::CAnimationPacManDatas::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    m_PacmanOpenMouth   = nullptr;
    m_PacmanClosedMouth = nullptr;
    m_PacGum1           = nullptr;
    m_PacGum2           = nullptr;
    m_PacGum3           = nullptr;
    m_PacGum4           = nullptr;
    m_Ghost             = nullptr;

    ColorReplacement cr;
    cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t)CRGB::Gold, });
    cr.AddColorReplacement(ColorReplacement::stColors{ 2, (uint32_t)CRGB::Blue });
    cr.AddColorReplacement(ColorReplacement::stColors{ 3, (uint32_t)CRGB::HotPink });
    cr.AddColorReplacement(ColorReplacement::stColors{ 4, (uint32_t)CRGB::White });
    cr.AddColorReplacement(ColorReplacement::stColors{ 5, (uint32_t)CRGB::DeepSkyBlue });
    cr.AddColorReplacement(ColorReplacement::stColors{ 6, (uint32_t)CRGB::Red, });
    cr.AddColorReplacement(ColorReplacement::stColors{ 7, (uint32_t)CRGB::Orange });

    switch (_uiCurrentAnimation)
    {
        case 0:
        {
            m_bIsMouthIsOpened = true;

            m_PacmanOpenMouth = pacmanOpenMouth.ToSprite(cr);
            m_PacmanClosedMouth = pacmanClosedMouth.ToSprite(cr);
            m_PacGum1 = pacGum.ToSprite(cr);
            m_PacGum2 = pacGum.ToSprite(cr);
            m_PacGum3 = pacGum.ToSprite(cr);
            m_PacGum4 = pacGum.ToSprite(cr);
            m_PacGum1->m_X = 1;
            m_PacGum2->m_X = 5;
            m_PacGum3->m_X = 9;
            m_PacGum4->m_X = 13;

            m_funcAnimation = &CAnimationPacMan::CAnimationPacManDatas::AnimatePacman;
            break;
        }
        case 1:
        {
            m_bIsMouthIsOpened = true;

            m_PacmanOpenMouth   = pacmanOpenMouth.ToSprite(cr);
            m_PacmanClosedMouth = pacmanClosedMouth.ToSprite(cr);
            m_Ghost             = pinkGhost.ToSprite(cr);

            m_funcAnimation = &CAnimationPacMan::CAnimationPacManDatas::AnimatePacChase;
            break;
        }
    }
}

void CAnimationPacMan::CAnimationPacManDatas::Animate()
{
    CEngine::Instance().ClearAllMatrix();

    if (m_funcAnimation != nullptr)
    {
        (this->*m_funcAnimation)();
    }

    // Done. Bring it on!
    FastLED.show();
}

void CAnimationPacMan::CAnimationPacManDatas::AnimatePacman()
{
    (m_bIsMouthIsOpened ? m_PacmanOpenMouth : m_PacmanClosedMouth)->ShowSprite(false, true, false);

    int iPacmanXPosition = m_PacmanOpenMouth->m_X % 16;

    // Pacgum dissapearance handling
    if (iPacmanXPosition > 2 && iPacmanXPosition < 11)
    {
        m_PacGum1->ShowSprite(false, true, false);
    }
    if (iPacmanXPosition > 6 && iPacmanXPosition < 15)
    {
        m_PacGum2->ShowSprite(false, true, false);
    }
    if (iPacmanXPosition > 10 || iPacmanXPosition < 3)
    {
        m_PacGum3->ShowSprite(false, true, false);
    }
    if (iPacmanXPosition > 14 || iPacmanXPosition < 7)
    {
        m_PacGum4->ShowSprite(false, true, false);
    }

    m_bIsMouthIsOpened = !m_bIsMouthIsOpened;

    m_PacmanOpenMouth->MoveSprite();
    m_PacmanClosedMouth->MoveSprite();
}

void CAnimationPacMan::CAnimationPacManDatas::AnimatePacChase()
{
    m_Ghost->ShowSprite(false, true, false);
    (m_bIsMouthIsOpened ? m_PacmanOpenMouth : m_PacmanClosedMouth)->ShowSprite(false, true, false);

    m_bIsMouthIsOpened = !m_bIsMouthIsOpened;

    m_PacmanOpenMouth->MoveSprite();
    m_PacmanClosedMouth->MoveSprite();
    m_Ghost->m_X = m_PacmanOpenMouth->m_X + 8;
}

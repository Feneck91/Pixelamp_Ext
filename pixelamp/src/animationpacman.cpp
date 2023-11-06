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
    return m_pPacmanDatas.IsNotNull()
        ? m_pPacmanDatas->GetMillisecondWait()
        : 150;
}

uint16_t CAnimationPacMan::GetNumberAnimations()
{
    return CAnimationBase::GetNumberAnimations() + 3;
}

void CAnimationPacMan::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
    m_ui8CurrentAnimation = (uint8_t) _uiCurrentAnimation;
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
// 8 = CRGB::Blue (for scared)
// 9 = CRGB::Violet (for scared)
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
    0, 3, 4, 4, 3, 4, 4, 3, 0,
    0, 3, 4, 2, 3, 4, 2, 3, 0,
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
    0, 5, 4, 3, 5, 4, 3, 5, 0,
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
    0, 7, 4, 2, 7, 4, 2, 7, 0,
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
    0, 6, 4, 2, 6, 4, 2, 6, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 6, 0, 6, 0, 6, 0, 6, 0
  }
};

const SpriteDefinition<72> scaredGhost PROGMEM
{
  9, 8,
  0, 0,
  1, 0,
  {
    0, 0, 0, 9, 9, 9, 0, 0, 0,
    0, 0, 9, 9, 9, 9, 9, 0, 0,
    0, 9, 9, 9, 9, 9, 9, 9, 0,
    0, 9, 9, 6, 9, 6, 9, 9, 0,
    0, 9, 9, 9, 9, 9, 9, 9, 0,
    0, 9, 6, 6, 9, 6, 6, 9, 0,
    0, 6, 9, 9, 6, 9, 9, 6, 0,
    0, 9, 0, 9, 0, 9, 0, 9, 0
  }
};

const SpriteDefinition<72> eyesGhost PROGMEM
{
  9, 8,
  0, 0,
  1, 0,
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 9, 9, 9, 9, 9, 0, 0,
    0, 9, 4, 4, 9, 4, 4, 9, 0,
    0, 9, 4, 2, 9, 4, 2, 9, 0,
    0, 0, 9, 9, 9, 9, 9, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
  }
};

const SpriteDefinition<72> smallGhost PROGMEM
{
  9, 8,
  0, 0,
  1, 0,
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 4, 4, 1, 4, 4, 1, 0,
    0, 1, 4, 2, 1, 1, 2, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
  }
};

const SpriteDefinition<88> ghostHouse PROGMEM
{
  11, 8,
  -5, 0,
  0, 0,
  {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  }
};

const SpriteDefinition<88> ghostOpenedHouse PROGMEM
{
  11, 8,
  -5, 0,
  0, 0,
  {
    2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
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
    m_iNumDisplay   = 0;
}

uint16_t CAnimationPacMan::CAnimationPacManDatas::GetMillisecondWait() const
{
    return (m_iNumDisplay >= 4 && m_iNumDisplay <= 6)
        ? 600
        : 150;
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
    m_GhostHouse        = nullptr;

    ColorReplacement cr;
    InitColorReplacement(cr);

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
        case 2:
        {
            m_iNumDisplay = 0;
            InitInitialPacEat();
            m_funcAnimation = &CAnimationPacMan::CAnimationPacManDatas::AnimatePacEat;
            break;
        }
    }
}

void CAnimationPacMan::CAnimationPacManDatas::InitColorReplacement(ColorReplacement &_cr)
{
    _cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t)CRGB::Gold         });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 2, (uint32_t)CRGB::Blue         });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 3, (uint32_t)CRGB::HotPink      });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 4, (uint32_t)CRGB::White        });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 5, (uint32_t)CRGB::DeepSkyBlue  });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 6, (uint32_t)CRGB::Red          });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 7, (uint32_t)CRGB::Orange       });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 8, (uint32_t)CRGB::Blue         });
    _cr.AddColorReplacement(ColorReplacement::stColors{ 9, (uint32_t)CRGB::DarkViolet   });
}

void CAnimationPacMan::CAnimationPacManDatas::InitInitialPacEat()
{
    ColorReplacement cr;
    InitColorReplacement(cr);

    if (m_iNumDisplay == 0)
    {   // Start par man eat
        m_bIsMouthIsOpened = true;
        m_PacmanOpenMouth = pacmanOpenMouth.ToSprite(cr);
        m_PacmanClosedMouth = pacmanClosedMouth.ToSprite(cr);

        int iRandom = random8(4);
        const SpriteDefinition<72> * arr[] =
        {
            &pinkGhost,
            &blueGhost,
            &orangeGhost,
            &redGhost,
        };

        m_Ghost                     = arr[iRandom]->ToSprite(cr);
        m_PacmanOpenMouth->m_X      = -13;
        m_PacmanClosedMouth->m_X    = -13;
        m_PacGum1                   = pacGum.ToSprite(cr);
        m_PacGum1->m_X              = 3;
    }
    else if (m_iNumDisplay < 24)
    {   // On each pass, restart pacman position
        m_PacmanOpenMouth->m_X = -13;
        m_PacmanClosedMouth->m_X = -13;
    }
    else if (m_iNumDisplay >= 24 && m_iNumDisplay <= 30)
    {   // 
        m_PacmanOpenMouth->m_X   = CEngine::Instance().GetMatrixWidth() + 3 - (m_iNumDisplay - 24 + ((m_iNumDisplay == 30) ? 1 : 0));
        m_PacmanClosedMouth->m_X = CEngine::Instance().GetMatrixWidth() + 3 - (m_iNumDisplay - 24 + ((m_iNumDisplay == 30) ? 1 : 0));
    }
    else if (m_iNumDisplay == 33)
    {
        m_Ghost->m_X = -15;
    }
    else if (m_iNumDisplay == 34)
    {   // Display ghost house 
        m_GhostHouse = ghostHouse.ToSprite(cr);
        m_Ghost->m_X = -15;
    }
    else if (m_iNumDisplay == 35)
    {   // Display the opened ghost house 
        m_GhostHouse = ghostOpenedHouse.ToSprite(cr);
        m_Ghost = smallGhost.ToSprite(cr);
        m_Ghost->m_X = CEngine::Instance().GetMatrixWidth() / 2 - 12;
        m_Ghost->m_i8MxdX = 0;
        m_Ghost->m_i8MxdY = -1;
    }
    else if (m_iNumDisplay == 36)
    {   // Start at the Beginning
        SetCurrentAnimation(2);
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

void CAnimationPacMan::CAnimationPacManDatas::AnimatePacEat()
{
    bool bDisplayed = false;
    if (m_iNumDisplay == 35)
    {
        m_Ghost->MoveSprite();
    }
    else if (m_iNumDisplay == 34)
    {   // Display house, pacman out of scope
        m_PacmanOpenMouth = nullptr;
        m_PacmanClosedMouth = nullptr;
        if (m_Ghost->m_X == -CEngine::Instance().GetMatrixWidth() / 2 + 4)
        {   // Eyes is now into the house!
            ++m_iNumDisplay;
            InitInitialPacEat();
        }
        else
        {
            m_Ghost->MoveSprite();
        }
    }
    else if (m_iNumDisplay >= 32 && m_iNumDisplay <= 33)
    {   // Eyes go to the right, pacman to the left
        m_Ghost->MoveSprite();
    }
    else if (m_iNumDisplay == 31)
    {
        if (m_Ghost->m_X == -CEngine::Instance().GetMatrixWidth() / 2)
        {   // Ghost is eat !
            ColorReplacement cr;
            InitColorReplacement(cr);
            auto x = m_Ghost->m_X ;
            m_Ghost = eyesGhost.ToSprite(cr);
            m_Ghost->m_X = x; // Keep the same position
            m_Ghost->m_i8MxdX = 1;
            ++m_iNumDisplay;
        }
        else
        {
            m_Ghost->m_X = m_PacmanOpenMouth->m_X - 12 + (m_iNumDisplay - 24);
        }
    }
    else if (m_iNumDisplay >= 24 && m_iNumDisplay <= 30)
    {   // Let pacman & ghost to the right, pacman will eat phantom
        if (m_Ghost->m_X == -6)
        {   // Accelerate Pacman
            ++m_iNumDisplay;
            m_PacmanOpenMouth->MoveSprite();
            m_PacmanClosedMouth->MoveSprite();
        }
        m_Ghost->m_X = m_PacmanOpenMouth->m_X - 12 + (m_iNumDisplay - 24);
    }
    else if (m_iNumDisplay == 23)
    {   // Let pacman & ghost to the right
        m_Ghost->m_X = m_PacmanOpenMouth->m_X - 12;
    }
    else if (m_iNumDisplay == 22)
    {   // Revert pacman, it's the beginning of pac chase
        m_PacmanOpenMouth->m_i8MxdX = -1;
        m_PacmanClosedMouth->m_i8MxdX = -1;
        m_PacmanOpenMouth->RevertSprite();
        m_PacmanOpenMouth->MoveSprite();
        m_PacmanClosedMouth->RevertSprite();
        m_PacmanClosedMouth->MoveSprite();
        m_Ghost->m_X = m_PacmanOpenMouth->m_X - 11;
        ++m_iNumDisplay;
    }
    else if (m_iNumDisplay >= 20 && m_iNumDisplay <= 21)
    {   // Let pacman continue to advance, stop the ghost: pacman goes faster than ghost
        m_Ghost->m_X = m_PacmanOpenMouth->m_X - 8 - 2 * (m_iNumDisplay - 20);
        ++m_iNumDisplay;
    }
    else if (m_iNumDisplay == 19)
    {   // Pacman eat the pacgum, ghost frightened
        ColorReplacement cr;
        InitColorReplacement(cr);
        m_Ghost = scaredGhost.ToSprite(cr);
        m_Ghost->m_X = m_PacmanOpenMouth->m_X - 8;
        ++m_iNumDisplay;
    }
    else if (m_iNumDisplay >= 7)
    {   // Run ghost & pacman to eat pacgum
        m_PacGum1->ShowSprite(false, false, false);
        m_Ghost->m_X = m_PacmanOpenMouth->m_X - 14 + 2 * 3;
        ++m_iNumDisplay;
    }
    else if (m_iNumDisplay >= 4 && m_iNumDisplay <= 6)
    {   // Blink pacgum
        if (m_iNumDisplay % 2 == 0)
        {
            m_PacGum1->ShowSprite(false, false, false);
        }
        ++m_iNumDisplay;
        return;
    }
    else
    {   // Run ghost & Pacman
        m_Ghost->m_X = m_PacmanOpenMouth->m_X - 14 + 2 * m_iNumDisplay;
    }

    if (m_iNumDisplay == 34 || m_iNumDisplay == 35)
    {   // Display ghost house
        m_GhostHouse->ShowSprite(false, false, false, true);
    }

    // Display Ghost & Pacman
    if (m_iNumDisplay == 32 && m_PacmanOpenMouth.IsNotNull())
    {
        bDisplayed = (m_bIsMouthIsOpened ? m_PacmanOpenMouth : m_PacmanClosedMouth)->ShowSprite(false, false, false, true) || bDisplayed;
    }
    bDisplayed = m_Ghost->ShowSprite(false, false, false, true) || bDisplayed;
    if (m_iNumDisplay < 32 && m_PacmanOpenMouth.IsNotNull())
    {
        bDisplayed = (m_bIsMouthIsOpened ? m_PacmanOpenMouth : m_PacmanClosedMouth)->ShowSprite(false, false, false, true) || bDisplayed;
    }

    m_bIsMouthIsOpened = !m_bIsMouthIsOpened;

    if (m_PacmanOpenMouth.IsNotNull())
    {
        m_PacmanOpenMouth->MoveSprite();
        m_PacmanClosedMouth->MoveSprite();
    }

    if (!bDisplayed)
    {
        ++m_iNumDisplay;
        InitInitialPacEat();
    }
}

#include "../include/animationtext.h"
#include "../include/engine.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

//========================================================================
// 
//               All functions / structures for text sprites
// 
//========================================================================
/// <summary>
/// Structure that contains all sprites.
/// </summary>
struct tTextChar
{
    const char                  Character;
    const SpriteDefinitionPtr&  rSprite;
};

// Forward reference
const tTextChar* GetCharSpriteInfo(char _cChar);

//========================================================================
// 
//                       Class CAnimationText
// 
//========================================================================

CAnimationText::CAnimationText(String _strText, CRGB _crTextColor)
    : m_strText(_strText)
    , m_crTextColor(_crTextColor)
{
}

CAnimationText::~CAnimationText()
{
}

// Make the animation
bool CAnimationText::Loop()
{
    CEngine::Instance().ClearAllMatrix();
    bool bOnePixelBisible = false;
    // Display all sprites
    for (auto it = m_arrSpritesText.begin(); it != m_arrSpritesText.end(); ++it)
    {
        if ((*it)->ShowSprite(false, false, false))
        {
            bOnePixelBisible = true;
        }
        (*it)->MoveSprite(); // Move to right
    }
    FastLED.show();

    return !bOnePixelBisible;
}

void CAnimationText::Enter()
{
    // Call base class
    CAnimationBase::Enter();

    uint8_t io8X = 0;

    for (int iIndex = 0; iIndex < m_strText.length(); ++iIndex)
    {
        const tTextChar* pInfo = GetCharSpriteInfo(m_strText[iIndex]);
        if (pInfo != nullptr)
        {
            ColorReplacement cr;
            cr.AddColorReplacement(ColorReplacement::stColors{ 1, (uint32_t) m_crTextColor });
            m_arrSpritesText.push_back(pInfo->rSprite.ToSprite(cr));
            m_arrSpritesText.back()->m_X += io8X;
            m_arrSpritesText.back()->m_Y += 1;
            io8X += 1 + m_arrSpritesText.back()->m_ui8Width;
            m_arrSpritesText.back()->m_i8MxdX = -1;
            m_arrSpritesText.back()->m_i8MxdY = 0;
        }
    }
}

void CAnimationText::Leave()
{
    // Call base class
    CAnimationBase::Leave();
    m_arrSpritesText.clear(); // No more need
}

uint16_t CAnimationText::GetMillisecondWait()
{
    return 250;
}

uint16_t CAnimationText::GetNumberAnimations()
{
    return 1;
}

void CAnimationText::SetCurrentAnimation(uint16_t _uiCurrentAnimation)
{
    CAnimationBase::SetCurrentAnimation(_uiCurrentAnimation);
}

//========================================================================
// 
//                     All Text sprites
// 
//      https://www.gamergeeks.net/apps/pixel/letter-text-generator
// 
//========================================================================
const SpriteDefinition<20> sprite_0 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        1, 0, 1, 1,
        1, 1, 0, 1,
        0, 1, 1, 0
    }
};

const SpriteDefinition<15> sprite_1 PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        0, 1, 0,
        1, 1, 0,
        0, 1, 0,
        0, 1, 0,
        1, 1, 1,
    }
};

const SpriteDefinition<20> sprite_2 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        0, 0, 1, 0,
        0, 1, 0, 0,
        1, 1, 1, 1,
    }
};

const SpriteDefinition<20> sprite_3 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        0, 0, 1, 0,
        1, 0, 0, 1,
        0, 1, 1, 0,
    }
};


const SpriteDefinition<20> sprite_4 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 1, 1,
        0, 0, 1, 0,
        0, 0, 1, 0,
    }
};

const SpriteDefinition<20> sprite_5 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 1,
        1, 0, 0, 0,
        1, 1, 1, 0,
        0, 0, 0, 1,
        1, 1, 1, 0,
    }
};

const SpriteDefinition<20> sprite_6 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 0,
        1, 1, 1, 0,
        0, 0, 0, 1,
        0, 1, 1, 0,
    }
};

const SpriteDefinition<20> sprite_7 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 1,
        0, 0, 0, 1,
        0, 0, 1, 0,
        0, 1, 0, 0,
        0, 1, 0, 0,
    }
};

const SpriteDefinition<20> sprite_8 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        0, 1, 1, 0,
        1, 0, 0, 1,
        0, 1, 1, 0,
    }
};

const SpriteDefinition<20> sprite_9 PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        0, 1, 1, 1,
        0, 0, 0, 1,
        0, 1, 1, 0,
    }
};

const SpriteDefinition<18> sprite_e_grave PROGMEM
{
    3, 6,
    0, 0,
    1, 0,
    {
        0, 1, 0,
        0, 0, 1,
        0, 1, 1,
        1, 0, 1,
        1, 1, 0,
        0, 1, 1,
    }
};

const SpriteDefinition<18> sprite_e_aigu PROGMEM
{
    3, 6,
    0, 0,
    1, 0,
    {
        0, 0, 1,
        0, 1, 0,
        0, 1, 1,
        1, 0, 1,
        1, 1, 0,
        0, 1, 1,
    }
};

const SpriteDefinition<18> sprite_e_circon PROGMEM
{
    3, 6,
    0, 0,
    1, 0,
    {
        0, 1, 0,
        1, 0, 1,
        0, 1, 1,
        1, 0, 1,
        1, 1, 0,
        0, 1, 1,
    }
};

const SpriteDefinition<20> sprite_A PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 0, 0, 1
    }
};

const SpriteDefinition<12> sprite_a PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        0, 1, 1,
        1, 0, 1,
        1, 0, 1,
        0, 1, 1,
    }
};

const SpriteDefinition<20> sprite_B PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 0,
        1, 0, 0, 1,
        1, 1, 1, 0,
        1, 0, 0, 1,
        1, 1, 1, 0,
    }
};

const SpriteDefinition<15> sprite_b PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0,
        1, 0, 0,
        1, 1, 0,
        1, 0, 1,
        1, 1, 0,
    }
};

const SpriteDefinition<20> sprite_C PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 1,
        1, 0, 0, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
        0, 1, 1, 1,
    }
};

const SpriteDefinition<12> sprite_c PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        0, 1, 1,
        1, 0, 0,
        1, 0, 0,
        0, 1, 1,
    }
};

const SpriteDefinition<20> sprite_D PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 0,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 1, 1, 0,
    }
};

const SpriteDefinition<15> sprite_d PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        0, 0, 1,
        0, 0, 1,
        0, 1, 1,
        1, 0, 1,
        0, 1, 1,
    }
};

const SpriteDefinition<20> sprite_E PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 1,
        1, 0, 0, 0,
        1, 1, 1, 0,
        1, 0, 0, 0,
        1, 1, 1, 1,
    }
};

const SpriteDefinition<12> sprite_e PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        0, 1, 1,
        1, 0, 1,
        1, 1, 0,
        0, 1, 1,
    }
};

const SpriteDefinition<20> sprite_F PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 1,
        1, 0, 0, 0,
        1, 1, 1, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
    }
};

const SpriteDefinition<15> sprite_f PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1,
        0, 1, 0,
        1, 1, 1,
        0, 1, 0,
        0, 1, 0,
    }
};

const SpriteDefinition<20> sprite_G PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 1,
        1, 0, 0, 0,
        1, 0, 1, 1,
        1, 0, 0, 1,
        0, 1, 1, 1,
    }
};

const SpriteDefinition<15> sprite_g PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1,
        1, 0, 1,
        0, 1, 1,
        0, 0, 1,
        1, 1, 0,
    }
};

const SpriteDefinition<20> sprite_H PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
    }
};

const SpriteDefinition<15> sprite_h PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0,
        1, 0, 0,
        1, 1, 1,
        1, 0, 1,
        1, 0, 1,
    }
};

const SpriteDefinition<5> sprite_I PROGMEM
{
    1, 5,
    0, 1,
    1, 0,
    {
        1,
        1,
        1,
        1,
        1,
    }
};

const SpriteDefinition<5> sprite_i PROGMEM
{
    1, 5,
    0, 1,
    1, 0,
    {
        1,
        0,
        1,
        1,
        1,
    }
};

const SpriteDefinition<20> sprite_J PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1,
        1, 0, 0, 1,
        0, 1, 1, 0,
    }
};

const SpriteDefinition<10> sprite_j PROGMEM
{
    2, 5,
    0, 1,
    1, 0,
    {
        0, 1,
        0, 0,
        0, 1,
        0, 1,
        1, 0,
    }
};

const SpriteDefinition<20> sprite_K PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0, 1,
        1, 0, 1, 0,
        1, 1, 0, 0,
        1, 0, 1, 0,
        1, 0, 0, 1,
    }
};

const SpriteDefinition<15> sprite_k PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0,
        1, 0, 0,
        1, 0, 1,
        1, 1, 0,
        1, 0, 1,
    }
};

const SpriteDefinition<15> sprite_L PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 1, 1,
    }
};

const SpriteDefinition<10> sprite_l PROGMEM
{
    2, 5,
    0, 1,
    1, 0,
    {
        1, 0,
        1, 0,
        1, 0,
        1, 0,
        1, 1,
    }
};

const SpriteDefinition<25> sprite_M PROGMEM
{
    5, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0, 0, 1,
        1, 1, 0, 1, 1,
        1, 0, 1, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
    }
};

const SpriteDefinition<20> sprite_m PROGMEM
{
    5, 4,
    0, 2,
    1, 0,
    {
        1, 1, 0, 1, 1,
        1, 0, 1, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
    }
};

const SpriteDefinition<20> sprite_N PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0, 1,
        1, 1, 0, 1,
        1, 0, 1, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
    }
};

const SpriteDefinition<16> sprite_n PROGMEM
{
    4, 4,
    0, 2,
    1, 0,
    {
        1, 1, 1, 0,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
    }
};

const SpriteDefinition<20> sprite_O PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
        0, 1, 1, 0,
    }
};

const SpriteDefinition<16> sprite_o PROGMEM
{
    4, 4,
    0, 2,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        1, 0, 0, 1,
        0, 1, 1, 0,
    }
};

const SpriteDefinition<20> sprite_P PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 0,
        1, 0, 0, 1,
        1, 1, 1, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
    }
};

const SpriteDefinition<12> sprite_p PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        1, 1, 0,
        1, 0, 1,
        1, 1, 0,
        1, 0, 0,
    }
};

const SpriteDefinition<20> sprite_Q PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 0,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 0, 1, 1,
        0, 1, 1, 1,
    }
};

const SpriteDefinition<12> sprite_q PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        0, 1, 1,
        1, 0, 1,
        0, 1, 1,
        0, 0, 1,
    }
};

const SpriteDefinition<20> sprite_R PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 0,
        1, 0, 0, 1,
        1, 1, 1, 0,
        1, 0, 1, 0,
        1, 0, 0, 1,
    }
};

const SpriteDefinition<12> sprite_r PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        1, 0, 1,
        1, 1, 0,
        1, 0, 0,
        1, 0, 0,
    }
};

const SpriteDefinition<20> sprite_S PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        0, 1, 1, 1,
        1, 0, 0, 0,
        0, 1, 1, 0,
        0, 0, 0, 1,
        1, 1, 1, 0,
    }
};

const SpriteDefinition<12> sprite_s PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        0, 1, 1,
        1, 1, 0,
        0, 0, 1,
        1, 1, 0,
    }
};

const SpriteDefinition<15> sprite_T PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
    }
};

const SpriteDefinition<15> sprite_t PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        0, 1, 0,
        1, 1, 1,
        0, 1, 0,
        0, 1, 0,
        0, 1, 1,
    }
};

const SpriteDefinition<20> sprite_U PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 1, 1, 1,
    }
};

const SpriteDefinition<12> sprite_u PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        1, 0, 1,
        1, 0, 1,
        1, 0, 1,
        0, 1, 1,
    }
};

const SpriteDefinition<15> sprite_V PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 0, 1,
        1, 0, 1,
        1, 0, 1,
        1, 0, 1,
        0, 1, 0,
    }
};

const SpriteDefinition<12> sprite_v PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        1, 0, 1,
        1, 0, 1,
        1, 0, 1,
        0, 1, 0,
    }
};

const SpriteDefinition<25> sprite_W PROGMEM
{
    5, 5,
    0, 1,
    1, 0,
    {
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 1, 0, 1,
        0, 1, 0, 1, 0,
    }
};

const SpriteDefinition<20> sprite_w PROGMEM
{
    5, 4,
    0, 2,
    1, 0,
    {
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 1, 0, 1,
        0, 1, 0, 1, 0,
    }
};

const SpriteDefinition<15> sprite_X PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 0, 1,
        1, 0, 1,
        0, 1, 0,
        1, 0, 1,
        1, 0, 1,
    }
};

const SpriteDefinition<12> sprite_x PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        1, 0, 1,
        0, 1, 0,
        1, 0, 1,
        1, 0, 1,
    }
};

const SpriteDefinition<15> sprite_Y PROGMEM
{
    3, 5,
    0, 1,
    1, 0,
    {
        1, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 0,
        0, 1, 0,
    }
};

const SpriteDefinition<12> sprite_y PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        1, 0, 1,
        1, 0, 1,
        0, 1, 0,
        1, 0, 1,
    }
};

const SpriteDefinition<20> sprite_Z PROGMEM
{
    4, 5,
    0, 1,
    1, 0,
    {
        1, 1, 1, 1,
        0, 0, 0, 1,
        0, 0, 1, 0,
        0, 1, 0, 0,
        1, 1, 1, 1,
    }
};

const SpriteDefinition<12> sprite_z PROGMEM
{
    3, 4,
    0, 2,
    1, 0,
    {
        1, 1, 1,
        0, 0, 1,
        0, 1, 0,
        1, 1, 1,
    }
};


const struct tTextChar g_AllText[] =
{
    {   '0', SpriteDefinitionPtrTemlate<20>(sprite_0)
    },
    {   '1', SpriteDefinitionPtrTemlate<15>(sprite_1)
    },
    {   '2', SpriteDefinitionPtrTemlate<20>(sprite_2)
    },
    {   '3', SpriteDefinitionPtrTemlate<20>(sprite_3)
    },
    {   '4', SpriteDefinitionPtrTemlate<20>(sprite_4)
    },
    {   '5', SpriteDefinitionPtrTemlate<20>(sprite_5)
    },
    {   '6', SpriteDefinitionPtrTemlate<20>(sprite_6)
    },
    {   '7', SpriteDefinitionPtrTemlate<20>(sprite_7)
    },
    {   '8', SpriteDefinitionPtrTemlate<20>(sprite_8)
    },
    {   '9', SpriteDefinitionPtrTemlate<20>(sprite_9)
    },
    {   'è', SpriteDefinitionPtrTemlate<18>(sprite_e_grave)
    },
    {   'é', SpriteDefinitionPtrTemlate<18>(sprite_e_aigu)
    },
    {   'ê', SpriteDefinitionPtrTemlate<18>(sprite_e_circon)
    },
    {   'A', SpriteDefinitionPtrTemlate<20>(sprite_A)
    },
    {   'a', SpriteDefinitionPtrTemlate<12>(sprite_a)
    },
    {   'B', SpriteDefinitionPtrTemlate<20>(sprite_B)
    },
    {   'b', SpriteDefinitionPtrTemlate<15>(sprite_b)
    },
    {   'C', SpriteDefinitionPtrTemlate<20>(sprite_C)
    },
    {   'c', SpriteDefinitionPtrTemlate<12>(sprite_c)
    },
    {   'D', SpriteDefinitionPtrTemlate<20>(sprite_D)
    },
    {   'd', SpriteDefinitionPtrTemlate<15>(sprite_d)
    },
    {   'E', SpriteDefinitionPtrTemlate<20>(sprite_E)
    },
    {   'e', SpriteDefinitionPtrTemlate<12>(sprite_e)
    },
    {   'F', SpriteDefinitionPtrTemlate<20>(sprite_F)
    },
    {   'f', SpriteDefinitionPtrTemlate<15>(sprite_f)
    },
    {   'G', SpriteDefinitionPtrTemlate<20>(sprite_G)
    },
    {   'g', SpriteDefinitionPtrTemlate<15>(sprite_g)
    },
    {   'H', SpriteDefinitionPtrTemlate<20>(sprite_H)
    },
    {   'h', SpriteDefinitionPtrTemlate<15>(sprite_h)
    },
    {   'I', SpriteDefinitionPtrTemlate<5>(sprite_I)
    },
    {   'i', SpriteDefinitionPtrTemlate<5>(sprite_i)
    },
    {   'J', SpriteDefinitionPtrTemlate<20>(sprite_J)
    },
    {   'j', SpriteDefinitionPtrTemlate<10>(sprite_j)
    },
    {   'K', SpriteDefinitionPtrTemlate<20>(sprite_K)
    },
    {   'k', SpriteDefinitionPtrTemlate<15>(sprite_k)
    },
    {   'L', SpriteDefinitionPtrTemlate<15>(sprite_L)
    },
    {   'l', SpriteDefinitionPtrTemlate<10>(sprite_l)
    },
    {   'M', SpriteDefinitionPtrTemlate<25>(sprite_M)
    },
    {   'm', SpriteDefinitionPtrTemlate<20>(sprite_m)
    },
    {   'N', SpriteDefinitionPtrTemlate<20>(sprite_N)
    },
    {   'n', SpriteDefinitionPtrTemlate<16>(sprite_n)
    },
    {   'O', SpriteDefinitionPtrTemlate<20>(sprite_O)
    },
    {   'o', SpriteDefinitionPtrTemlate<16>(sprite_o)
    },
    {   'P', SpriteDefinitionPtrTemlate<20>(sprite_P)
    },
    {   'p', SpriteDefinitionPtrTemlate<12>(sprite_p)
    },
    {   'Q', SpriteDefinitionPtrTemlate<20>(sprite_Q)
    },
    {   'q', SpriteDefinitionPtrTemlate<12>(sprite_q)
    },
    {   'R', SpriteDefinitionPtrTemlate<20>(sprite_R)
    },
    {   'r', SpriteDefinitionPtrTemlate<12>(sprite_r)
    },
    {   'S', SpriteDefinitionPtrTemlate<20>(sprite_S)
    },
    {   's', SpriteDefinitionPtrTemlate<12>(sprite_s)
    },
    {   'T', SpriteDefinitionPtrTemlate<15>(sprite_T)
    },
    {   't', SpriteDefinitionPtrTemlate<15>(sprite_t)
    },
    {   'U', SpriteDefinitionPtrTemlate<20>(sprite_U)
    },
    {   'u', SpriteDefinitionPtrTemlate<12>(sprite_u)
    },
    {   'V', SpriteDefinitionPtrTemlate<15>(sprite_V)
    },
    {   'v', SpriteDefinitionPtrTemlate<12>(sprite_v)
    },
    {   'W', SpriteDefinitionPtrTemlate<25>(sprite_W)
    },
    {   'w', SpriteDefinitionPtrTemlate<20>(sprite_w)
    },
    {   'X', SpriteDefinitionPtrTemlate<15>(sprite_X)
    },
    {   'x', SpriteDefinitionPtrTemlate<12>(sprite_x)
    },
    {   'Y', SpriteDefinitionPtrTemlate<15>(sprite_Y)
    },
    {   'y', SpriteDefinitionPtrTemlate<12>(sprite_y)
    },
    {   'Z', SpriteDefinitionPtrTemlate<20>(sprite_Z)
    },
    {   'z', SpriteDefinitionPtrTemlate<12>(sprite_z)
    },
};

/// <summary>
/// Get the sprite info.
/// 
/// Not very fast but it don't take too much memory.
/// </summary>
/// <param name="_cChar">Char to find.</param>
/// <returns>The sprite info if found, nullptr if not found.</returns>
const tTextChar* GetCharSpriteInfo(char _cChar)
{
    const tTextChar* pTextCharRet = nullptr;
    for (int iIndex = 0; iIndex < sizeof(g_AllText) / sizeof(g_AllText[0]); ++iIndex)
    {
        if (g_AllText[iIndex].Character == _cChar)
        {
            pTextCharRet = &g_AllText[iIndex];
            break;
        }
    }

    return pTextCharRet;
}

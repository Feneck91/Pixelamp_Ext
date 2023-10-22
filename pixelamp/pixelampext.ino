/**
Name:       pixelampext.ino
Created:    22/09/2023 11:05:13
Author:     Feneck91
*/
#include "include/engine.h"
#include "include/animationpacman.h"
#include "include/animationfirework.h"
#include "include/animationfirepit.h"
#include "include/animationfx.h"
#include "include/animationlight.h"

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "simu_include/TrackAllocation.h"
    #endif
#endif

/**
 * The setup function runs once when you press reset or power the board.
 */
void setup()
{
    // Create engine instance.
    CEngineImpl<DEFAULT_CHIPSET, DEFAULT_LED_DATA_PIN, DEFAULT_COLOR_ORDER>::Init(DEFAULT_MATRIX_WIDTH, DEFAULT_MATRIX_HEIGHT, DEFAULT_POTENTIOMETER_BRIGHTNESS, DEFAULT_POTENTIOMETER_ANIMATION, DEFAULT_ARRANGE_IN_ZIGZAG);

    // Init Modes & Animations
    CEngine::Instance().AddAnimationMode(shared_ptr<CAnimationMode>(new CAnimationMode(String("Pacman"),   CRGB::Yellow,        new CAnimationPacMan(), nullptr)));
    CEngine::Instance().AddAnimationMode(shared_ptr<CAnimationMode>(new CAnimationMode(String("Feux"),     CRGB::Red,           new CAnimationFirework(), new CAnimationFirepit(), nullptr)));
    CEngine::Instance().AddAnimationMode(shared_ptr<CAnimationMode>(new CAnimationMode(String("Fx"),       CRGB::Aquamarine,    new CAnimationFx(), nullptr)));
    CEngine::Instance().AddAnimationMode(shared_ptr<CAnimationMode>(new CAnimationMode(String("Lumières"), CRGB::White,         new CAnimationLight(), nullptr)));

    Serial.println("");
    Serial.println("===========================================");
    Serial.println("=                                         =");
    Serial.println("=        PixelLamp Extended Program       =");
    Serial.println("=                                         =");
    Serial.println("===========================================");
}

/**
 * The loop function runs over and over again until power down or reset.
 */
void loop()
{
    CEngine::Instance().Loop();
}

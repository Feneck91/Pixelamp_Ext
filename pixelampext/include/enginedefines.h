#pragma once

// Default values (used in Init function)
#define DEFAULT_LED_DATA_PIN                    6
#define DEFAULT_COLOR_ORDER                     GRB
#define DEFAULT_CHIPSET                         WS2812

// Width / Height defaults matrix leds
#define DEFAULT_MATRIX_WIDTH                    16
#define DEFAULT_MATRIX_HEIGHT                   8

// Port used for brightness and animation
#define DEFAULT_POTENTIOMETER_BRIGHTNESS        A0
#define DEFAULT_POTENTIOMETER_ANIMATION         A1

// The leds are arrange in zigzab by default
#define DEFAULT_ARRANGE_IN_ZIGZAG               true

// Used into ComputePositionFromXY
#define INVALID_INDEX                           (uint16_t (-1))

#define POT_BRIGHTNESS_MIN                      0
#define POT_BRIGHTNESS_MAX                      1023

#define MIN_BRIGHTNESS                          0
#define MAX_BRIGHTNESS                          150 // Can be changed to 255 but ONLY if 8A power supply is possible (not just 4A)

// Defines that indicate that the the cable into the lamp of each column is done whitout soldering led (on each row)
#define NO_SOLDERING_LED
// Defines that indicate that the first led is not used (it was the second of the led ribbon), the first is the number 1
// It can let features in the future for using these unsused led
//#define NO_SOLDERING_FIRST_LED_IS_NOT_USED
// Indicate that the led ribbon is cut AFTER the last used led of the ribbon.
// If it is not defined, it can let features in the future for using these unsused led
//#define NO_SOLDERING_LAST_LED_DOES_NOT_EXIST

// Indicate the current mode index is not set
#define INDEX_MODE_NOMODE                       255

// To save current mode into EEPROM
#define EEPROM_ADDRESS_CURRENT_MODE             0
#define EEPROM_ADDRESS_INVERT_POT_BRIGHTNESS    1
#define EEPROM_ADDRESS_INVERT_POT_ANIMATION     2
#define EEPROM_ADDRESS_MATRIX_DELTA_X           3

// Max duration to be able to change mode
#define MAX_TIME_CHANGE_MODE_MS                 2000
#define VALUE_DELTA_CHANGE_MODE_MAX             70
#define VALUE_DELTA_CHANGE_MODE_MIN             10

// Animations
#define POT_ANIMATION_MIN                       0
#define POT_ANIMATION_MAX                       1023

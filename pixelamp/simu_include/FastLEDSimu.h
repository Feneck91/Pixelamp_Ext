#ifndef __INC_FASTLED_H
#define __INC_FASTLED_H

#include <stdint.h>
uint8_t qadd8(uint8_t i, uint8_t j);
uint8_t qmul8(uint8_t i, uint8_t j);
uint8_t qsub8(uint8_t i, uint8_t j);

#include "colorutilsSimu.h"
#include "scale8Simu.h"
#include "lib8tionSimu.h"
#include "colorpalettesSimu.h"

/// Fast 16-bit approximation of cos(x). This approximation never varies more than
/// 0.69% from the floating point value you'd get by doing
///    @code{.cpp}
///    float s = cos(x) * 32767.0;
///    @endcode
///
/// @param theta input angle from 0-65535
/// @returns cos of theta, value between -32767 to 32767.
int16_t cos16(uint16_t theta);

inline void* memset8(void* ptr, uint8_t value, uint16_t num)
{
    uint8_t* pValue = static_cast<uint8_t*>(ptr);
    while (num--)
    {
        *(pValue++) = value;
    }

    return ptr;
}

inline void* memmove8(void* dst, const void* src, uint16_t num)
{
    uint8_t* pDest = static_cast<uint8_t*>(dst);
    const uint8_t* pSrc = static_cast<const uint8_t*>(src);
    while (num--)
    {
        *(pDest++) = *(pSrc++);
    }

    return dst;
}

inline void* memcpy_P(void* dst, const void* src, uint16_t num)
{
    return memmove8(dst, src, num);
}

#include "colorSimu.h"
#include "pixeltypesSimu.h"

#include <string>
typedef std::string     String;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#pragma warning( push )
#pragma warning( disable : 4267)

#define DISABLE_DITHER 0x00
/// Gets the assigned color channel for a byte's position in the output,
/// using the color order (EOrder) template parameter from the
/// LED controller
/// @param X the byte's position in the output (0-2)
/// @returns the color channel for that byte (0 = red, 1 = green, 2 = blue)
/// @see EOrder
#define RO(X) RGB_BYTE(RGB_ORDER, X)

/// Gets the assigned color channel for a byte's position in the output,
/// using a passed RGB color order
/// @param RO the RGB color order
/// @param X the byte's position in the output (0-2)
/// @returns the color channel for that byte (0 = red, 1 = green, 2 = blue)
/// @see EOrder
#define RGB_BYTE(RO,X) (((RO)>>(3*(2-(X)))) & 0x3)

/// Gets the color channel for byte 0.
/// @see RGB_BYTE(RO,X)
#define RGB_BYTE0(RO) ((RO>>6) & 0x3)
/// Gets the color channel for byte 1.
/// @see RGB_BYTE(RO,X)
#define RGB_BYTE1(RO) ((RO>>3) & 0x3)
/// Gets the color channel for byte 2.
/// @see RGB_BYTE(RO,X)
#define RGB_BYTE2(RO) ((RO) & 0x3)

#define PIN_A0   (54)
#define PIN_A1   (55)
#define PIN_A2   (56)
#define PIN_A3   (57)
#define PIN_A4   (58)
#define PIN_A5   (59)
#define PIN_A6   (60)
#define PIN_A7   (61)
#define PIN_A8   (62)
#define PIN_A9   (63)
#define PIN_A10  (64)
#define PIN_A11  (65)
#define PIN_A12  (66)
#define PIN_A13  (67)
#define PIN_A14  (68)
#define PIN_A15  (69)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
static const uint8_t A8 = PIN_A8;
static const uint8_t A9 = PIN_A9;
static const uint8_t A10 = PIN_A10;
static const uint8_t A11 = PIN_A11;
static const uint8_t A12 = PIN_A12;
static const uint8_t A13 = PIN_A13;
static const uint8_t A14 = PIN_A14;
static const uint8_t A15 = PIN_A15;

typedef int (*funcGetAnalog)(void*, uint8_t); 

void setAnalogReadFunction(void* _pContext, funcGetAnalog _function);
int analogRead(uint8_t pin);

inline long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// @copydoc inoise16(uint32_t, uint32_t)
/// @param z z-axis coordinate on noise map (3D)
uint16_t inoise16(uint32_t x, uint32_t y, uint32_t z);

#define accum88     uint16_t
#define saccum78    int16_t
#define byte        uint8_t

// Don't care about Frequency
#ifndef F_CPU
#define F_CPU 16000000
#endif

#if !defined(CLOCKLESS_FREQUENCY)
#define CLOCKLESS_FREQUENCY F_CPU
#endif

/// Frequency multiplier for each clockless data interval.
/// @see Notes in @ref ClocklessChipsets
#define FMUL (CLOCKLESS_FREQUENCY/8000000)

/// Disable dithering
#define DISABLE_DITHER 0x00
/// Enable dithering using binary dithering (only option)
#define BINARY_DITHER 0x01
/// The dither setting, either DISABLE_DITHER or BINARY_DITHER
typedef uint8_t EDitherMode;

unsigned long millis();

/// Convert an HSV value to RGB using a visually balanced rainbow. 
/// This "rainbow" yields better yellow and orange than a straight
/// mathematical "spectrum".
///
/// ![FastLED 'Rainbow' Hue Chart](https://raw.githubusercontent.com/FastLED/FastLED/gh-pages/images/HSV-rainbow-with-desc.jpg)
///
/// @param hsv CHSV struct to convert to RGB. Max hue supported is HUE_MAX_RAINBOW
/// @param rgb CRGB struct to store the result of the conversion (will be modified)
void hsv2rgb_rainbow(const struct CHSV& hsv, struct CRGB& rgb);

/// Convert an HSV value to RGB using a visually balanced rainbow. 
/// This "rainbow" yields better yellow and orange than a straight
/// mathematical "spectrum".
///
/// ![FastLED 'Rainbow' Hue Chart](https://raw.githubusercontent.com/FastLED/FastLED/gh-pages/images/HSV-rainbow-with-desc.jpg)
///
/// @param hsv CHSV struct to convert to RGB. Max hue supported is HUE_MAX_RAINBOW
/// @param rgb CRGB struct to store the result of the conversion (will be modified)
void hsv2rgb_rainbow(const struct CHSV& hsv, struct CRGB& rgb); 

class CLEDController;

/// Pixel controller class.  This is the class that we use to centralize pixel access in a block of data, including
/// support for things like RGB reordering, scaling, dithering, skipping (for ARGB data), and eventually, we will
/// centralize 8/12/16 conversions here as well.
/// @tparam RGB_ORDER the rgb ordering for the LEDs (e.g. what order red, green, and blue data is written out in)
/// @tparam LANES how many parallel lanes of output to write
/// @tparam MASK bitmask for the output lanes
template<EOrder RGB_ORDER, int LANES = 1, uint32_t MASK = 0xFFFFFFFF>
struct PixelController {
    const uint8_t* mData;    ///< pointer to the underlying LED data
    int mLen;                ///< number of LEDs in the data for one lane
    int mLenRemaining;       ///< counter for the number of LEDs left to process
    uint8_t d[3];            ///< values for the scaled dither signal @see init_binary_dithering()
    uint8_t e[3];            ///< values for the scaled dither signal @see init_binary_dithering()
    CRGB mScale;             ///< the per-channel scale values, provided by a color correction function such as CLEDController::computeAdjustment()
    int8_t mAdvance;         ///< how many bytes to advance the pointer by each time. For CRGB this is 3.
    int mOffsets[LANES];     ///< the number of bytes to offset each lane from the starting pointer @see initOffsets()

    /// Copy constructor
    /// @param other the object to copy 
    PixelController(const PixelController& other) {
        d[0] = other.d[0];
        d[1] = other.d[1];
        d[2] = other.d[2];
        e[0] = other.e[0];
        e[1] = other.e[1];
        e[2] = other.e[2];
        mData = other.mData;
        mScale = other.mScale;
        mAdvance = other.mAdvance;
        mLenRemaining = mLen = other.mLen;
        for (int i = 0; i < LANES; ++i) { mOffsets[i] = other.mOffsets[i]; }
    }

    /// Initialize the PixelController::mOffsets array based on the length of the strip
    /// @param len the number of LEDs in one lane of the strip
    void initOffsets(int len) {
        int nOffset = 0;
        for (int i = 0; i < LANES; ++i) {
            mOffsets[i] = nOffset;
            if ((1 << i) & MASK) { nOffset += (len * mAdvance); }
        }
    }

    /// Constructor
    /// @param d pointer to LED data
    /// @param len length of the LED data
    /// @param s LED scale values, as CRGB struct
    /// @param dither dither setting for the LEDs
    /// @param advance whether the pointer (d) should advance per LED
    /// @param skip if the pointer is advancing, how many bytes to skip in addition to 3
    PixelController(const uint8_t* d, int len, CRGB& s, EDitherMode dither = BINARY_DITHER, bool advance = true, uint8_t skip = 0) : mData(d), mLen(len), mLenRemaining(len), mScale(s) {
        enable_dithering(dither);
        mData += skip;
        mAdvance = (advance) ? 3 + skip : 0;
        initOffsets(len);
    }

    /// Constructor
    /// @param d pointer to LED data
    /// @param len length of the LED data
    /// @param s LED scale values, as CRGB struct
    /// @param dither dither setting for the LEDs
    PixelController(const CRGB* d, int len, CRGB& s, EDitherMode dither = BINARY_DITHER) : mData((const uint8_t*)d), mLen(len), mLenRemaining(len), mScale(s) {
        enable_dithering(dither);
        mAdvance = 3;
        initOffsets(len);
    }

    /// Constructor
    /// @param d pointer to LED data
    /// @param len length of the LED data
    /// @param s LED scale values, as CRGB struct
    /// @param dither dither setting for the LEDs
    PixelController(const CRGB& d, int len, CRGB& s, EDitherMode dither = BINARY_DITHER) : mData((const uint8_t*)&d), mLen(len), mLenRemaining(len), mScale(s) {
        enable_dithering(dither);
        mAdvance = 0;
        initOffsets(len);
    }


#if !defined(NO_DITHERING) || (NO_DITHERING != 1)

    /// Predicted max update rate, in Hertz
#define MAX_LIKELY_UPDATE_RATE_HZ     400

/// Minimum acceptable dithering rate, in Hertz
#define MIN_ACCEPTABLE_DITHER_RATE_HZ  50

/// The number of updates in a single dither cycle
#define UPDATES_PER_FULL_DITHER_CYCLE (MAX_LIKELY_UPDATE_RATE_HZ / MIN_ACCEPTABLE_DITHER_RATE_HZ)

/// Set "virtual bits" of dithering to the highest level
/// that is not likely to cause excessive flickering at
/// low brightness levels + low update rates. 
/// These pre-set values are a little ambitious, since
/// a 400Hz update rate for WS2811-family LEDs is only
/// possible with 85 pixels or fewer.
/// Once we have a "number of milliseconds since last update"
/// value available here, we can quickly calculate the correct
/// number of "virtual bits" on the fly with a couple of "if"
/// statements -- no division required.  At this point,
/// the division is done at compile time, so there's no runtime
/// cost, but the values are still hard-coded.
/// @todo Can these macros be replaced with constants scoped to PixelController::init_binary_dithering()?
#define RECOMMENDED_VIRTUAL_BITS ((UPDATES_PER_FULL_DITHER_CYCLE>1) + \
                                  (UPDATES_PER_FULL_DITHER_CYCLE>2) + \
                                  (UPDATES_PER_FULL_DITHER_CYCLE>4) + \
                                  (UPDATES_PER_FULL_DITHER_CYCLE>8) + \
                                  (UPDATES_PER_FULL_DITHER_CYCLE>16) + \
                                  (UPDATES_PER_FULL_DITHER_CYCLE>32) + \
                                  (UPDATES_PER_FULL_DITHER_CYCLE>64) + \
                                  (UPDATES_PER_FULL_DITHER_CYCLE>128) )

/// Alias for RECOMMENDED_VIRTUAL_BITS
#define VIRTUAL_BITS RECOMMENDED_VIRTUAL_BITS

#endif


        /// Set up the values for binary dithering
    void init_binary_dithering() {
#if !defined(NO_DITHERING) || (NO_DITHERING != 1)
        // R is the digther signal 'counter'.
        static uint8_t R = 0;
        ++R;

        // R is wrapped around at 2^ditherBits,
        // so if ditherBits is 2, R will cycle through (0,1,2,3)
        uint8_t ditherBits = VIRTUAL_BITS;
        R &= (0x01 << ditherBits) - 1;

        // Q is the "unscaled dither signal" itself.
        // It's initialized to the reversed bits of R.
        // If 'ditherBits' is 2, Q here will cycle through (0,128,64,192)
        uint8_t Q = 0;

        // Reverse bits in a byte
        {
            if (R & 0x01) { Q |= 0x80; }
            if (R & 0x02) { Q |= 0x40; }
            if (R & 0x04) { Q |= 0x20; }
            if (R & 0x08) { Q |= 0x10; }
            if (R & 0x10) { Q |= 0x08; }
            if (R & 0x20) { Q |= 0x04; }
            if (R & 0x40) { Q |= 0x02; }
            if (R & 0x80) { Q |= 0x01; }
        }

        // Now we adjust Q to fall in the center of each range,
        // instead of at the start of the range.
        // If ditherBits is 2, Q will be (0, 128, 64, 192) at first,
        // and this adjustment makes it (31, 159, 95, 223).
        if (ditherBits < 8) {
            Q += 0x01 << (7 - ditherBits);
        }

        // D and E form the "scaled dither signal"
        // which is added to pixel values to affect the
        // actual dithering.

        // Setup the initial D and E values
        for (int i = 0; i < 3; ++i) {
            uint8_t s = mScale.raw[i];
            e[i] = s ? (256 / s) + 1 : 0;
            d[i] = scale8(Q, e[i]);
#if (FASTLED_SCALE8_FIXED == 1)
            if (d[i]) (--d[i]);
#endif
            if (e[i]) --e[i];
        }
#endif
    }

    /// Do we have n pixels left to process?
    /// @param n the number to check against
    /// @returns 'true' if there are more than n pixels left to process
    __attribute__((always_inline)) inline bool has(int n) {
        return mLenRemaining >= n;
    }

    /// Toggle dithering enable
    /// If dithering is set to enabled, this will re-init the dithering values
    /// (init_binary_dithering()). Otherwise it will clear the stored dithering
    /// data.
    /// @param dither the dither setting
    void enable_dithering(EDitherMode dither) {
        switch (dither) {
        case BINARY_DITHER: init_binary_dithering(); break;
        default: d[0] = d[1] = d[2] = e[0] = e[1] = e[2] = 0; break;
        }
    }

    /// Get the length of the LED strip
    /// @returns PixelController::mLen
    __attribute__((always_inline)) inline int size() { return mLen; }

    /// Get the number of lanes of the Controller
    /// @returns LANES from template
    __attribute__((always_inline)) inline int lanes() { return LANES; }

    /// Get the amount to advance the pointer by
    /// @returns PixelController::mAdvance
    __attribute__((always_inline)) inline int advanceBy() { return mAdvance; }

    /// Advance the data pointer forward, adjust position counter
    __attribute__((always_inline)) inline void advanceData() { mData += mAdvance; --mLenRemaining; }

    /// Step the dithering forward
    /// @note If updating here, be sure to update the asm version in clockless_trinket.h!
    __attribute__((always_inline)) inline void stepDithering() {
        // IF UPDATING HERE, BE SURE TO UPDATE THE ASM VERSION IN
        // clockless_trinket.h!
        d[0] = e[0] - d[0];
        d[1] = e[1] - d[1];
        d[2] = e[2] - d[2];
    }

    /// Some chipsets pre-cycle the first byte, which means we want to cycle byte 0's dithering separately
    __attribute__((always_inline)) inline void preStepFirstByteDithering() {
        d[RO(0)] = e[RO(0)] - d[RO(0)];
    }

    /// @name Template'd static functions for output
    /// These functions are used for retrieving LED data for the LED chipset output controllers.
    /// @{

    /// Read a byte of LED data
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t loadByte(PixelController& pc) { return pc.mData[RO(SLOT)]; }
    /// Read a byte of LED data for parallel output
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @param lane the parallel output lane to read the byte for
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t loadByte(PixelController& pc, int lane) { return pc.mData[pc.mOffsets[lane] + RO(SLOT)]; }

    /// Calculate a dither value using the per-channel dither data
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @param b the color byte to dither
    /// @see PixelController::d
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t dither(PixelController& pc, uint8_t b) { return b ? qadd8(b, pc.d[RO(SLOT)]) : 0; }
    /// Calculate a dither value
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param b the color byte to dither
    /// @param d dither data
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t dither(PixelController&, uint8_t b, uint8_t d) { return b ? qadd8(b, d) : 0; }

    /// Scale a value using the per-channel scale data
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @param b the color byte to scale
    /// @see PixelController::mScale
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t scale(PixelController& pc, uint8_t b) { return scale8(b, pc.mScale.raw[RO(SLOT)]); }
    /// Scale a value
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param b the byte to scale
    /// @param scale the scale value
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t scale(PixelController&, uint8_t b, uint8_t scale) { return scale8(b, scale); }

    /// @name Composite shortcut functions for loading, dithering, and scaling
    /// These composite functions will load color data, dither it, and scale it
    /// all at once so that it's ready for the output controller to send to the
    /// LEDs.
    /// @{


    /// Loads, dithers, and scales a single byte for a given output slot, using class dither and scale values
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t loadAndScale(PixelController& pc) { return scale<SLOT>(pc, pc.dither<SLOT>(pc, pc.loadByte<SLOT>(pc))); }

    /// Loads, dithers, and scales a single byte for a given output slot and lane, using class dither and scale values
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @param lane the parallel output lane to read the byte for
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t loadAndScale(PixelController& pc, int lane) { return scale<SLOT>(pc, pc.dither<SLOT>(pc, pc.loadByte<SLOT>(pc, lane))); }

    /// Loads, dithers, and scales a single byte for a given output slot and lane
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @param lane the parallel output lane to read the byte for
    /// @param d the dither data for the byte
    /// @param scale the scale data for the byte
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t loadAndScale(PixelController& pc, int lane, uint8_t d, uint8_t scale) { return scale8(pc.dither<SLOT>(pc, pc.loadByte<SLOT>(pc, lane), d), scale); }

    /// Loads and scales a single byte for a given output slot and lane
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @param lane the parallel output lane to read the byte for
    /// @param scale the scale data for the byte
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t loadAndScale(PixelController& pc, int lane, uint8_t scale) { return scale8(pc.loadByte<SLOT>(pc, lane), scale); }


    /// A version of loadAndScale() that advances the output data pointer
    /// @param pc reference to the pixel controller
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t advanceAndLoadAndScale(PixelController& pc) { pc.advanceData(); return pc.loadAndScale<SLOT>(pc); }

    /// A version of loadAndScale() that advances the output data pointer
    /// @param pc reference to the pixel controller
    /// @param lane the parallel output lane to read the byte for
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t advanceAndLoadAndScale(PixelController& pc, int lane) { pc.advanceData(); return pc.loadAndScale<SLOT>(pc, lane); }

    /// A version of loadAndScale() that advances the output data pointer without dithering
    /// @param pc reference to the pixel controller
    /// @param lane the parallel output lane to read the byte for
    /// @param scale the scale data for the byte
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t advanceAndLoadAndScale(PixelController& pc, int lane, uint8_t scale) { pc.advanceData(); return pc.loadAndScale<SLOT>(pc, lane, scale); }

    /// @} Composite shortcut functions


    /// @name Data retrieval functions
    /// These functions retrieve channel-specific data from the class,
    /// arranged in output order.
    /// @{

    /// Gets the dithering data for the provided output slot
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @returns dithering data for the given channel
    /// @see PixelController::d
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t getd(PixelController& pc) { return pc.d[RO(SLOT)]; }

    /// Gets the scale data for the provided output slot
    /// @tparam SLOT The data slot in the output stream. This is used to select which byte of the output stream is being processed.
    /// @param pc reference to the pixel controller
    /// @returns scale data for the given channel
    /// @see PixelController::mScale
    template<int SLOT>  __attribute__((always_inline)) inline static uint8_t getscale(PixelController& pc) { return pc.mScale.raw[RO(SLOT)]; }

    /// @} Data retrieval functions


    /// @} Template'd static functions for output

    // Helper functions to get around gcc stupidities
    __attribute__((always_inline)) inline uint8_t loadAndScale0(int lane, uint8_t scale) { return loadAndScale<0>(*this, lane, scale); }  ///< non-template alias of loadAndScale<0>()
    __attribute__((always_inline)) inline uint8_t loadAndScale1(int lane, uint8_t scale) { return loadAndScale<1>(*this, lane, scale); }  ///< non-template alias of loadAndScale<1>()
    __attribute__((always_inline)) inline uint8_t loadAndScale2(int lane, uint8_t scale) { return loadAndScale<2>(*this, lane, scale); }  ///< non-template alias of loadAndScale<2>()
    __attribute__((always_inline)) inline uint8_t advanceAndLoadAndScale0(int lane, uint8_t scale) { return advanceAndLoadAndScale<0>(*this, lane, scale); }  ///< non-template alias of advanceAndLoadAndScale<0>()
    __attribute__((always_inline)) inline uint8_t stepAdvanceAndLoadAndScale0(int lane, uint8_t scale) { stepDithering(); return advanceAndLoadAndScale<0>(*this, lane, scale); }  ///< stepDithering() and advanceAndLoadAndScale0()

    __attribute__((always_inline)) inline uint8_t loadAndScale0(int lane) { return loadAndScale<0>(*this, lane); }  ///< @copydoc loadAndScale0(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t loadAndScale1(int lane) { return loadAndScale<1>(*this, lane); }  ///< @copydoc loadAndScale1(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t loadAndScale2(int lane) { return loadAndScale<2>(*this, lane); }  ///< @copydoc loadAndScale2(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t advanceAndLoadAndScale0(int lane) { return advanceAndLoadAndScale<0>(*this, lane); }  ///< @copydoc advanceAndLoadAndScale0(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t stepAdvanceAndLoadAndScale0(int lane) { stepDithering(); return advanceAndLoadAndScale<0>(*this, lane); }  ///< @copydoc stepAdvanceAndLoadAndScale0(int, uint8_t)

    __attribute__((always_inline)) inline uint8_t loadAndScale0() { return loadAndScale<0>(*this); }  ///< @copydoc loadAndScale0(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t loadAndScale1() { return loadAndScale<1>(*this); }  ///< @copydoc loadAndScale1(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t loadAndScale2() { return loadAndScale<2>(*this); }  ///< @copydoc loadAndScale2(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t advanceAndLoadAndScale0() { return advanceAndLoadAndScale<0>(*this); }  ///< @copydoc advanceAndLoadAndScale0(int, uint8_t)
    __attribute__((always_inline)) inline uint8_t stepAdvanceAndLoadAndScale0() { stepDithering(); return advanceAndLoadAndScale<0>(*this); }  ///< @copydoc stepAdvanceAndLoadAndScale0(int, uint8_t)

    __attribute__((always_inline)) inline uint8_t getScale0() { return getscale<0>(*this); }  ///< non-template alias of getscale<0>()
    __attribute__((always_inline)) inline uint8_t getScale1() { return getscale<1>(*this); }  ///< non-template alias of getscale<1>()
    __attribute__((always_inline)) inline uint8_t getScale2() { return getscale<2>(*this); }  ///< non-template alias of getscale<2>()
};


/// Typedef for a power consumption calculation function. Used within
/// CFastLED for rescaling brightness before sending the LED data to
/// the strip with CFastLED::show().
/// @param scale the initial brightness scale value
/// @param data max power data, in milliwatts
/// @returns the brightness scale, limited to max power
typedef uint8_t(*power_func)(uint8_t scale, uint32_t data);

/// High level controller interface for FastLED.
/// This class manages controllers, global settings, and trackings such as brightness
/// and refresh rates, and provides access functions for driving led data to controllers
/// via the show() / showColor() / clear() methods.
/// This is instantiated as a global object with the name FastLED.
/// @nosubgrouping
class CFastLED {
    // int m_nControllers;
    uint8_t  m_Scale;         ///< the current global brightness scale setting
    uint16_t m_nFPS;          ///< tracking for current frames per second (FPS) value
    uint32_t m_nMinMicros;    ///< minimum µs between frames, used for capping frame rates
    uint32_t m_nPowerData;    ///< max power use parameter
    power_func m_pPowerFunc;  ///< function for overriding brightness when using FastLED.show();

public:
    CFastLED();

    /// Add a CLEDController instance to the world.  Exposed to the public to allow people to implement their own
    /// CLEDController objects or instances.  There are two ways to call this method (as well as the other addLeds()
    /// variations). The first is with 3 arguments, in which case the arguments are the controller, a pointer to
    /// led data, and the number of leds used by this controller.  The second is with 4 arguments, in which case
    /// the first two arguments are the same, the third argument is an offset into the CRGB data where this controller's
    /// CRGB data begins, and the fourth argument is the number of leds for this controller object.
    /// @param pLed the led controller being added
    /// @param data base pointer to an array of CRGB data structures
    /// @param nLedsOrOffset number of leds (3 argument version) or offset into the data array
    /// @param nLedsIfOffset number of leds (4 argument version)
    /// @returns a reference to the added controller
    static CLEDController& addLeds(CLEDController* pLed, struct CRGB* data, int nLedsOrOffset, int nLedsIfOffset = 0);

    /// Add a clockless based CLEDController instance to the world.
    template<template<uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, uint8_t DATA_PIN, EOrder RGB_ORDER>
    static CLEDController& addLeds(struct CRGB* data, int nLedsOrOffset, int nLedsIfOffset = 0) {
        static CHIPSET<DATA_PIN, RGB_ORDER> c;
        return addLeds(&c, data, nLedsOrOffset, nLedsIfOffset);
    }

private:
    template<class CHIPSET> static CLEDController& addLeds(struct CRGB* data, int nLedsOrOffset, int nLedsIfOffset = 0) {
        return addLeds<CHIPSET, RGB>(data, nLedsOrOffset, nLedsIfOffset);
    }

    template<class CHIPSET, EOrder RGB_ORDER> static CLEDController& addLeds(struct CRGB* data, int nLedsOrOffset, int nLedsIfOffset = 0) {
        return addLeds<CHIPSET, RGB_ORDER>(data, nLedsOrOffset, nLedsIfOffset);
    }

    template<class CHIPSET, EOrder RGB_ORDER, uint32_t SPI_DATA_RATE> static CLEDController& addLeds(struct CRGB* data, int nLedsOrOffset, int nLedsIfOffset = 0) {
        return addLeds<CHIPSET, RGB_ORDER, SPI_DATA_RATE>(data, nLedsOrOffset, nLedsIfOffset);
    }
public:

    /// Set the global brightness scaling
    /// @param scale a 0-255 value for how much to scale all leds before writing them out
    void setBrightness(uint8_t scale) { m_Scale = scale; }

    /// Get the current global brightness setting
    /// @returns the current global brightness value
    uint8_t getBrightness() { return m_Scale; }

    /// Update all our controllers with the current led colors, using the passed in brightness
    /// @param scale the brightness value to use in place of the stored value
    void show(uint8_t scale);

    /// Update all our controllers with the current led colors
    void show() { show(m_Scale); }

    /// Clear the leds, wiping the local array of data. Optionally you can also
    /// send the cleared data to the LEDs.
    /// @param writeData whether or not to write out to the leds as well
    void clear(bool writeData = false);

    /// Clear out the local data array
    void clearData();

    /// Set all leds on all controllers to the given color/scale.
    /// @param color what color to set the leds to
    /// @param scale what brightness scale to show at
    void showColor(const struct CRGB& color, uint8_t scale);

    /// Set all leds on all controllers to the given color
    /// @param color what color to set the leds to
    void showColor(const struct CRGB& color) { showColor(color, m_Scale); }

    /// Delay for the given number of milliseconds.  Provided to allow the library to be used on platforms
    /// that don't have a delay function (to allow code to be more portable). 
    /// @note This will call show() constantly to drive the dithering engine (and will call show() at least once).
    /// @param ms the number of milliseconds to pause for
    void delay(unsigned long ms);

    /// Set a global color temperature.  Sets the color temperature for all added led strips,
    /// overriding whatever previous color temperature those controllers may have had.
    /// @param temp A CRGB structure describing the color temperature
    void setTemperature(const struct CRGB& temp);

    /// Set a global color correction.  Sets the color correction for all added led strips,
    /// overriding whatever previous color correction those controllers may have had.
    /// @param correction A CRGB structure describin the color correction.
    void setCorrection(const struct CRGB& correction);

    /// Set the dithering mode.  Sets the dithering mode for all added led strips, overriding
    /// whatever previous dithering option those controllers may have had.
    /// @param ditherMode what type of dithering to use, either BINARY_DITHER or DISABLE_DITHER
    void setDither(uint8_t ditherMode = BINARY_DITHER);

    /// Set the maximum refresh rate.  This is global for all leds.  Attempts to
    /// call show() faster than this rate will simply wait.
    /// @note The refresh rate defaults to the slowest refresh rate of all the leds added through addLeds().
    /// If you wish to set/override this rate, be sure to call setMaxRefreshRate() _after_
    /// adding all of your leds.
    /// @param refresh maximum refresh rate in hz
    /// @param constrain constrain refresh rate to the slowest speed yet set
    void setMaxRefreshRate(uint16_t refresh, bool constrain = false);

    /// For debugging, this will keep track of time between calls to countFPS(). Every
    /// `nFrames` calls, it will update an internal counter for the current FPS.
    /// @todo Make this a rolling counter
    /// @param nFrames how many frames to time for determining FPS
    void countFPS(int nFrames = 25);

    /// Get the number of frames/second being written out
    /// @returns the most recently computed FPS value
    uint16_t getFPS() { return m_nFPS; }

    /// Get how many controllers have been registered
    /// @returns the number of controllers (strips) that have been added with addLeds()
    int count();

    /// Get a reference to a registered controller
    /// @returns a reference to the Nth controller
    CLEDController& operator[](int x);

    /// Get the number of leds in the first controller
    /// @returns the number of LEDs in the first controller
    int size();

    /// Get a pointer to led data for the first controller
    /// @returns pointer to the CRGB buffer for the first controller
    CRGB* leds();
};

/** \ingroup avr_eeprom
    Read one byte from EEPROM address \a __p.
 */
inline uint8_t eeprom_read_byte(const uint8_t* __p)
{
    return 0;
}

/** \ingroup avr_eeprom
    Write a byte \a __value to EEPROM address \a __p.
 */
inline void eeprom_write_byte(uint8_t* __p, uint8_t __value)
{
}

#define DEC 10
#define HEX 16
#define OCT 8

class Print
{
private:
    int write_error;
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);
protected:
    void setWriteError(int err = 1) { write_error = err; }
public:
    Print() : write_error(0) {}

    int getWriteError() { return write_error; }
    void clearWriteError() { setWriteError(0); }

    virtual size_t write(uint8_t) = 0;
    size_t write(const char* str)
    {
        if (str == NULL) return 0;
        return write((const uint8_t*)str, strlen(str));
    }
    
    virtual size_t write(const uint8_t* buffer, size_t size);

    size_t write(const char* buffer, size_t size)
    {
        return write((const uint8_t*)buffer, size);
    }

    // default to zero, meaning "a single write may block"
    // should be overridden by subclasses with buffering
    virtual int availableForWrite() { return 0; }

    size_t print(const String&);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

    size_t println(const String& s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);

    virtual void flush() { /* Empty implementation for backward compatibility */ }
};

class CSerial : public Print
{
public:
    void begin(uint32_t);
    virtual size_t write(uint8_t) override;
    virtual size_t write(const uint8_t* buffer, size_t size) override;
    virtual void flush() override;
};

struct EEPROMClass {
    uint8_t read(int idx);
    void write(int idx, uint8_t val);
};


extern CSerial Serial;
extern CFastLED FastLED;
extern EEPROMClass EEPROM;
extern inline void SetColorPalette(CRGB);

/// Base definition for an LED controller.  Pretty much the methods that every LED controller object will make available.
/// If you want to pass LED controllers around to methods, make them references to this type, keeps your code saner. However,
/// most people won't be seeing/using these objects directly at all.
/// @note That the methods for eventual checking of background writing of data (I'm looking at you, Teensy 3.0 DMA controller!)
/// are not yet implemented.
class CLEDController {
protected:
    friend class CFastLED;
    CRGB* m_Data;              ///< pointer to the LED data used by this controller
    CLEDController* m_pNext;   ///< pointer to the next LED controller in the linked list
    CRGB m_ColorCorrection;    ///< CRGB object representing the color correction to apply to the strip on show()  @see setCorrection
    CRGB m_ColorTemperature;   ///< CRGB object representing the color temperature to apply to the strip on show() @see setTemperature
    EDitherMode m_DitherMode;  ///< the current dither mode of the controller
    int m_nLeds;               ///< the number of LEDs in the LED data array
    static CLEDController* m_pHead;  ///< pointer to the first LED controller in the linked list
    static CLEDController* m_pTail;  ///< pointer to the last LED controller in the linked list

    /// Set all the LEDs to a given color. 
    /// @param data the CRGB color to set the LEDs to
    /// @param nLeds the number of LEDs to set to this color
    /// @param scale the rgb scaling value for outputting color
    virtual void showColor(const struct CRGB& data, int nLeds, CRGB scale) = 0;

    /// Write the passed in RGB data out to the LEDs managed by this controller. 
    /// @param data the rgb data to write out to the strip
    /// @param nLeds the number of LEDs being written out
    /// @param scale the rgb scaling to apply to each led before writing it out
    virtual void show(const struct CRGB* data, int nLeds, CRGB scale) = 0;

public:
    /// Create an led controller object, add it to the chain of controllers
    CLEDController() : m_Data(NULL), m_ColorCorrection(UncorrectedColor), m_ColorTemperature(UncorrectedTemperature), m_DitherMode(BINARY_DITHER), m_nLeds(0) {
        m_pNext = NULL;
        if (m_pHead == NULL) { m_pHead = this; }
        if (m_pTail != NULL) { m_pTail->m_pNext = this; }
        m_pTail = this;
    }

    /// Initialize the LED controller
    virtual void init() = 0;

    /// Clear out/zero out the given number of LEDs.
    /// @param nLeds the number of LEDs to clear
    virtual void clearLeds(int nLeds) { showColor(CRGB::Black, nLeds, CRGB::Black); }

    /// @copybrief show(const struct CRGB*, int, CRGB)
    ///
    /// Will scale for color correction and temperature. Can accept LED data not attached to this controller.
    /// @param data the LED data to write to the strip
    /// @param nLeds the number of LEDs in the data array
    /// @param brightness the brightness of the LEDs
    /// @see show(const struct CRGB*, int, CRGB)
    void show(const struct CRGB* data, int nLeds, uint8_t brightness) {
        show(data, nLeds, getAdjustment(brightness));
    }

    /// @copybrief showColor(const struct CRGB&, int, CRGB)
    ///
    /// Will scale for color correction and temperature. Can accept LED data not attached to this controller.
    /// @param data the CRGB color to set the LEDs to
    /// @param nLeds the number of LEDs in the data array
    /// @param brightness the brightness of the LEDs
    /// @see showColor(const struct CRGB&, int, CRGB)
    void showColor(const struct CRGB& data, int nLeds, uint8_t brightness) {
        showColor(data, nLeds, getAdjustment(brightness));
    }

    /// Write the data to the LEDs managed by this controller
    /// @param brightness the brightness of the LEDs
    /// @see show(const struct CRGB*, int, uint8_t)
    void showLeds(uint8_t brightness = 255) {
        show(m_Data, m_nLeds, getAdjustment(brightness));
    }

    /// @copybrief showColor(const struct CRGB&, int, CRGB)
    ///
    /// @param data the CRGB color to set the LEDs to
    /// @param brightness the brightness of the LEDs
    /// @see showColor(const struct CRGB&, int, CRGB)
    void showColor(const struct CRGB& data, uint8_t brightness = 255) {
        showColor(data, m_nLeds, getAdjustment(brightness));
    }

    /// Get the first LED controller in the linked list of controllers
    /// @returns CLEDController::m_pHead
    static CLEDController* head() { return m_pHead; }

    /// Get the next controller in the linked list after this one.  Will return NULL at the end of the linked list.
    /// @returns CLEDController::m_pNext
    CLEDController* next() { return m_pNext; }

    /// Set the default array of LEDs to be used by this controller
    /// @param data pointer to the LED data
    /// @param nLeds the number of LEDs in the LED data
    CLEDController& setLeds(CRGB* data, int nLeds) {
        m_Data = data;
        m_nLeds = nLeds;
        return *this;
    }

    /// Zero out the LED data managed by this controller
    void clearLedData() {
        if (m_Data) {
            memset8((void*)m_Data, 0, sizeof(struct CRGB) * m_nLeds);
        }
    }

    /// How many LEDs does this controller manage?
    /// @returns CLEDController::m_nLeds
    virtual int size() { return m_nLeds; }

    /// How many Lanes does this controller manage?
    /// @returns 1 for a non-Parallel controller
    virtual int lanes() { return 1; }

    /// Pointer to the CRGB array for this controller
    /// @returns CLEDController::m_Data
    CRGB* leds() { return m_Data; }

    /// Reference to the n'th LED managed by the controller
    /// @param x the LED number to retrieve
    /// @returns reference to CLEDController::m_Data[x]
    CRGB& operator[](int x) { return m_Data[x]; }

    /// Set the dithering mode for this controller to use
    /// @param ditherMode the dithering mode to set
    /// @returns a reference to the controller
    inline CLEDController& setDither(uint8_t ditherMode = BINARY_DITHER) { m_DitherMode = ditherMode; return *this; }

    /// Get the dithering option currently set for this controller
    /// @return the currently set dithering option (CLEDController::m_DitherMode)
    inline uint8_t getDither() { return m_DitherMode; }

    /// The color corrction to use for this controller, expressed as a CRGB object
    /// @param correction the color correction to set
    /// @returns a reference to the controller
    CLEDController& setCorrection(CRGB correction) { m_ColorCorrection = correction; return *this; }

    /// @copydoc setCorrection()
    CLEDController& setCorrection(LEDColorCorrection correction) { m_ColorCorrection = correction; return *this; }

    /// Get the correction value used by this controller
    /// @returns the current color correction (CLEDController::m_ColorCorrection)
    CRGB getCorrection() { return m_ColorCorrection; }

    /// Set the color temperature, aka white point, for this controller
    /// @param temperature the color temperature to set
    /// @returns a reference to the controller
    CLEDController& setTemperature(CRGB temperature) { m_ColorTemperature = temperature; return *this; }

    /// @copydoc setTemperature()
    CLEDController& setTemperature(ColorTemperature temperature) { m_ColorTemperature = temperature; return *this; }

    /// Get the color temperature, aka whipe point, for this controller
    /// @returns the current color temperature (CLEDController::m_ColorTemperature)
    CRGB getTemperature() { return m_ColorTemperature; }

    /// Get the combined brightness/color adjustment for this controller
    /// @param scale the brightness scale to get the correction for
    /// @returns a CRGB object representing the total adjustment, including color correction and color temperature
    CRGB getAdjustment(uint8_t scale) {
        return computeAdjustment(scale, m_ColorCorrection, m_ColorTemperature);
    }

    /// Calculates the combined color adjustment to the LEDs at a given scale, color correction, and color temperature
    /// @param scale the scale value for the RGB data (i.e. brightness)
    /// @param colorCorrection color correction to apply
    /// @param colorTemperature color temperature to apply
    /// @returns a CRGB object representing the adjustment, including color correction and color temperature
    static CRGB computeAdjustment(uint8_t scale, const CRGB& colorCorrection, const CRGB& colorTemperature) {
#if defined(NO_CORRECTION) && (NO_CORRECTION==1)
        return CRGB(scale, scale, scale);
#else
        CRGB adj(0, 0, 0);

        if (scale > 0) {
            for (uint8_t i = 0; i < 3; ++i) {
                uint8_t cc = colorCorrection.raw[i];
                uint8_t ct = colorTemperature.raw[i];
                if (cc > 0 && ct > 0) {
                    uint32_t work = (((uint32_t)cc) + 1) * (((uint32_t)ct) + 1) * scale;
                    work /= 0x10000L;
                    adj.raw[i] = work & 0xFF;
                }
            }
        }

        return adj;
#endif
    }

    /// Gets the maximum possible refresh rate of the strip
    /// @returns the maximum refresh rate, in frames per second (FPS)
    virtual uint16_t getMaxRefreshRate() const { return 0; }
};

/// Template extension of the CLEDController class
/// @tparam RGB_ORDER the rgb ordering for the LEDs (e.g. what order red, green, and blue data is written out in)
/// @tparam LANES how many parallel lanes of output to write
/// @tparam MASK bitmask for the output lanes
template<EOrder RGB_ORDER, int LANES=1, uint32_t MASK=0xFFFFFFFF> class CPixelLEDController : public CLEDController {
protected:
    /// Send the LED data to the strip
    /// @param pixels the PixelController object for the LED data
    virtual void showPixels(PixelController<RGB_ORDER,LANES,MASK> & pixels) = 0;

    /// Set all the LEDs on the controller to a given color
    /// @param data the CRGB color to set the LEDs to
    /// @param nLeds the number of LEDs to set to this color
    /// @param scale the RGB scaling value for outputting color
    virtual void showColor(const struct CRGB & data, int nLeds, CRGB scale) {
        PixelController<RGB_ORDER, LANES, MASK> pixels(data, nLeds, scale, getDither());
        showPixels(pixels);
    }

    /// Write the passed in RGB data out to the LEDs managed by this controller
    /// @param data the RGB data to write out to the strip
    /// @param nLeds the number of LEDs being written out
    /// @param scale the RGB scaling to apply to each LED before writing it out
    virtual void show(const struct CRGB *data, int nLeds, CRGB scale) {
        PixelController<RGB_ORDER, LANES, MASK> pixels(data, nLeds < 0 ? -nLeds : nLeds, scale, getDither());
        if(nLeds < 0) {
            // nLeds < 0 implies that we want to show them in reverse
            pixels.mAdvance = -pixels.mAdvance;
        }
        showPixels(pixels);
    }

public:
    CPixelLEDController() : CLEDController() {}

    /// Get the number of lanes of the Controller
    /// @returns LANES from template
    int lanes() { return LANES; }
};

template <uint8_t DATA_PIN, int T1, int T2, int T3, EOrder RGB_ORDER = RGB, int XTRA0 = 0, bool FLIP = false, int WAIT_TIME = 10>
class ClocklessController : public CPixelLEDController<RGB_ORDER>
{
};

/// WS2812 controller class @ 800 KHz.
/// @tparam DATA_PIN the data pin for these LEDs
/// @tparam RGB_ORDER the RGB ordering for these LEDs
template <uint8_t DATA_PIN, EOrder RGB_ORDER = RGB>
class WS2812Controller800Khz : public ClocklessController<DATA_PIN, 2 * FMUL, 5 * FMUL, 3 * FMUL, RGB_ORDER>
{
};

template<EOrder RGB_ORDER> class ISimulatorController
{
public:
    virtual void init() = 0;
    virtual void showPixels(PixelController<RGB_ORDER>& pixels) = 0;
};

template<uint8_t DATA_PIN, EOrder RGB_ORDER> class WS2812 : public WS2812Controller800Khz<DATA_PIN, RGB_ORDER>
///< @copydoc WS2812Controller800Khz
{
private:
    static ISimulatorController<RGB_ORDER> * m_pSimulator;

public:
    /// <summary>
    /// Set the simulator.
    /// </summary>
    /// <param name="_pSimulator"></param>
    static void SetSimulator(ISimulatorController<RGB_ORDER> * _pSimulator)
    {
        m_pSimulator = _pSimulator;
    }

protected:
    /// Initialize the LED controller
    virtual void init() override
    {
        if (m_pSimulator != nullptr)
        {
            m_pSimulator->init();
        }
    }

    virtual void showPixels(PixelController<RGB_ORDER>&pixels) override
    {
        if (m_pSimulator != nullptr)
        {
            m_pSimulator->showPixels(pixels);
        }
    }
}; ///< WS2852 controller class. @copyd

template<uint8_t DATA_PIN, EOrder RGB_ORDER>
    ISimulatorController<RGB_ORDER>* WS2812<DATA_PIN, RGB_ORDER>::m_pSimulator = nullptr;

//=============================================================================================
//=============================================================================================
//
//                                        RANDOM
//
//=============================================================================================
//=============================================================================================
#define random  rand

/// Multiplier value for pseudo-random number generation
#define FASTLED_RAND16_2053  ((uint16_t)(2053))
/// Increment value for pseudo-random number generation
#define FASTLED_RAND16_13849 ((uint16_t)(13849))

/// Multiplies a value by the pseudo-random multiplier
#define APPLY_FASTLED_RAND16_2053(x) (x * FASTLED_RAND16_2053)

/// Seed for the random number generator functions
extern uint16_t rand16seed; // = RAND16_SEED;

/// Generate an 8-bit random number
/// @returns random 8-bit number, in the range 0-255
LIB8STATIC uint8_t random8()
{
    rand16seed = APPLY_FASTLED_RAND16_2053(rand16seed) + FASTLED_RAND16_13849;
    // return the sum of the high and low bytes, for better
    //  mixing and non-sequential correlation
    return (uint8_t)(((uint8_t)(rand16seed & 0xFF)) +
        ((uint8_t)(rand16seed >> 8)));
}

/// Generate a 16-bit random number
/// @returns random 16-bit number, in the range 0-65535
LIB8STATIC uint16_t random16()
{
    rand16seed = APPLY_FASTLED_RAND16_2053(rand16seed) + FASTLED_RAND16_13849;
    return rand16seed;
}

/// Generate an 8-bit random number between 0 and lim
/// @param lim the upper bound for the result, exclusive
LIB8STATIC uint8_t random8(uint8_t lim)
{
    uint8_t r = random8();
    r = (r * lim) >> 8;
    return r;
}

/// Generate an 8-bit random number in the given range
/// @param min the lower bound for the random number, inclusive
/// @param lim the upper bound for the random number, exclusive
LIB8STATIC uint8_t random8(uint8_t min, uint8_t lim)
{
    uint8_t delta = lim - min;
    uint8_t r = random8(delta) + min;
    return r;
}

/// Generate an 16-bit random number between 0 and lim
/// @param lim the upper bound for the result, exclusive
LIB8STATIC uint16_t random16(uint16_t lim)
{
    uint16_t r = random16();
    uint32_t p = (uint32_t)lim * (uint32_t)r;
    r = p >> 16;
    return r;
}

/// Generate an 16-bit random number in the given range
/// @param min the lower bound for the random number, inclusive
/// @param lim the upper bound for the random number, exclusive
LIB8STATIC uint16_t random16(uint16_t min, uint16_t lim)
{
    uint16_t delta = lim - min;
    uint16_t r = random16(delta) + min;
    return r;
}

/// Set the 16-bit seed used for the random number generator
LIB8STATIC void random16_set_seed(uint16_t seed)
{
    rand16seed = seed;
}

/// Get the current seed value for the random number generator
LIB8STATIC uint16_t random16_get_seed()
{
    return rand16seed;
}

/// Add entropy into the random number generator
LIB8STATIC void random16_add_entropy(uint16_t entropy)
{
    rand16seed += entropy;
}


#pragma warning( pop ) 

#endif
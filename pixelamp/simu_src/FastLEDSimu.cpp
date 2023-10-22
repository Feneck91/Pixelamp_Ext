#pragma once
#include "..\simu_include\FastLedSimu.h"
#include <chrono>
#include <thread>

#define  _AFXDLL
#include <afxcoll.h>

#ifdef SIMULATION
    #ifdef _DEBUG
        #include "../simu_include/TrackAllocation.h"
    #endif
#endif

CSerial Serial;
CFastLED FastLED;
EEPROMClass EEPROM;

/// Platform-independent alias of the fast sin implementation
#define sin16 sin16_avr

/// Fast 16-bit approximation of sin(x). This approximation never varies more than
/// 0.69% from the floating point value you'd get by doing
///    @code{.cpp}
///    float s = sin(x) * 32767.0;
///    @endcode
///
/// @param theta input angle from 0-65535
/// @returns sin of theta, value between -32767 to 32767.
int16_t sin16_avr(uint16_t theta)
{
    static const uint8_t data[] =
    { 0,         0,         49, 0, 6393 % 256,   6393 / 256, 48, 0,
      12539 % 256, 12539 / 256, 44, 0, 18204 % 256, 18204 / 256, 38, 0,
      23170 % 256, 23170 / 256, 31, 0, 27245 % 256, 27245 / 256, 23, 0,
      30273 % 256, 30273 / 256, 14, 0, 32137 % 256, 32137 / 256,  4 /*,0*/ };

    uint16_t offset = (theta & 0x3FFF);

    // AVR doesn't have a multi-bit shift instruction,
    // so if we say "offset >>= 3", gcc makes a tiny loop.
    // Inserting empty volatile statements between each
    // bit shift forces gcc to unroll the loop.
    offset >>= 3; // 0..8191

    if (theta & 0x4000) offset = 2047 - offset;

    uint8_t sectionX4;
    sectionX4 = offset / 256;
    sectionX4 *= 4;

    uint8_t m;

    union {
        uint16_t b;
        struct {
            uint8_t blo;
            uint8_t bhi;
        };
    } u;

    //in effect u.b = blo + (256 * bhi);
    u.blo = data[sectionX4];
    u.bhi = data[sectionX4 + 1];
    m = data[sectionX4 + 2];

    uint8_t secoffset8 = (uint8_t)(offset) / 2;

    uint16_t mx = m * secoffset8;

    int16_t  y = mx + u.b;
    if (theta & 0x8000) y = -y;

    return y;
}

/// Fast 16-bit approximation of cos(x). This approximation never varies more than
/// 0.69% from the floating point value you'd get by doing
///    @code{.cpp}
///    float s = cos(x) * 32767.0;
///    @endcode
///
/// @param theta input angle from 0-65535
/// @returns cos of theta, value between -32767 to 32767.
int16_t cos16(uint16_t theta)
{
    return sin16(theta + 16384);
}

static char szPathApp[_MAX_PATH];

const char* GetIniPath()
{
    ::GetModuleFileNameA(NULL, szPathApp, _MAX_PATH);
    szPathApp[strlen(szPathApp) - 1] = 'i';
    szPathApp[strlen(szPathApp) - 2] = 'n';
    szPathApp[strlen(szPathApp) - 3] = 'i';
    return szPathApp;
}

uint8_t EEPROMClass::read(int idx)
{
    char szBuffer[20];
    sprintf_s(szBuffer, "ADDR_%d", idx);
    auto uiValue = ::GetPrivateProfileIntA("EEPROM", szBuffer, 0, GetIniPath());

    return (uint8_t) uiValue;
}

void EEPROMClass::write(int idx, uint8_t val)
{
    char szBuffer[20], szIntBuffer[4];
    sprintf_s(szBuffer, "ADDR_%d", idx);
    sprintf_s(szIntBuffer, "%d", val);
    WritePrivateProfileStringA("EEPROM", szBuffer, szIntBuffer, GetIniPath());
}


// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
size_t Print::write(const uint8_t* buffer, size_t size)
{
    size_t n = 0;
    while (size--) {
        if (write(*buffer++)) n++;
        else break;
    }
    return n;
}

size_t Print::print(const String& s)
{
    return write(s.c_str(), s.length());
}

size_t Print::print(const char str[])
{
    return write(str);
}

size_t Print::print(char c)
{
    return write(c);
}

size_t Print::print(unsigned char b, int base)
{
    return print((unsigned long)b, base);
}

size_t Print::print(int n, int base)
{
    return print((long)n, base);
}

size_t Print::print(unsigned int n, int base)
{
    return print((unsigned long)n, base);
}

size_t Print::print(long n, int base)
{
    if (base == 0) {
        return write((uint8_t) n);
    }
    else if (base == 10) {
        if (n < 0) {
            size_t t = print('-');
            n = -n;
            return printNumber(n, 10) + t;
        }
        return printNumber(n, 10);
    }
    else {
        return printNumber(n, base);
    }
}

size_t Print::print(unsigned long n, int base)
{
    if (base == 0) return printNumber(n, 10);
    else return printNumber(n, base);
}

size_t Print::print(double n, int digits)
{
    return printFloat(n, digits);
}

size_t Print::println(void)
{
    return write("\r\n");
}

size_t Print::println(const String& s)
{
    size_t n = print(s);
    n += println();
    return n;
}

size_t Print::println(const char c[])
{
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(char c)
{
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(unsigned char b, int base)
{
    size_t n = print(b, base);
    n += println();
    return n;
}

size_t Print::println(int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(double num, int digits)
{
    size_t n = print(num, digits);
    n += println();
    return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t Print::printNumber(unsigned long n, uint8_t base)
{
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char* str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1
    if (base < 2) base = 10;

    do {
        char c = n % base;
        n /= base;

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

size_t Print::printFloat(double number, uint8_t digits)
{
    size_t n = 0;

    if (isnan(number)) return print("nan");
    if (isinf(number)) return print("inf");
    if (number > 4294967040.0) return print("ovf");  // constant determined empirically
    if (number < -4294967040.0) return print("ovf");  // constant determined empirically

    // Handle negative numbers
    if (number < 0.0)
    {
        n += print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    n += print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
        n += print('.');
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
        remainder *= 10.0;
        unsigned int toPrint = (unsigned int)(remainder);
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}

uint8_t qadd8(uint8_t i, uint8_t j)
{
    unsigned int t = i + j;
    if (t > 255) t = 255;
    return t;
}

uint8_t qmul8(uint8_t i, uint8_t j)
{
    unsigned p = (unsigned)i * (unsigned)j;
    if (p > 255) p = 255;
    return p;
}

uint8_t qsub8(uint8_t i, uint8_t j)
{
    int t = i - j;
    if (t < 0) t = 0;
    return t;
}

/// Calculate an integer average of two signed 15-bit
/// integers (int16_t). 
/// If the first argument is even, result is rounded down.
/// If the first argument is odd, result is rounded up.
/// @param i first value to average
/// @param j second value to average
/// @returns mean average of i and j, rounded
int16_t avg15(int16_t i, int16_t j)
{
    return (i >> 1) + (j >> 1) + (i & 0x1);
}
/*
#define FL_PGM_READ_BYTE_NEAR(x)  (*((const  uint8_t*)(x)))
#define FL_PGM_READ_WORD_NEAR(x)  (*((const uint16_t*)(x)))
#define FL_PGM_READ_DWORD_NEAR(x) (*((const uint32_t*)(x)))
*/
/// Reads a single byte from the p array
#define P(x) FL_PGM_READ_BYTE_NEAR(p + x)
#define AVG15(U,V) (avg15((U),(V)))

static uint8_t const p[] =
{
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
    151
};

static int16_t inline __attribute__((always_inline))  grad16(uint8_t hash, int16_t x, int16_t y, int16_t z) {
#if 0
    switch (hash & 0xF) {
    case  0: return ((x)+(y)) >> 1;
    case  1: return ((-x) + (y)) >> 1;
    case  2: return ((x)+(-y)) >> 1;
    case  3: return ((-x) + (-y)) >> 1;
    case  4: return ((x)+(z)) >> 1;
    case  5: return ((-x) + (z)) >> 1;
    case  6: return ((x)+(-z)) >> 1;
    case  7: return ((-x) + (-z)) >> 1;
    case  8: return ((y)+(z)) >> 1;
    case  9: return ((-y) + (z)) >> 1;
    case 10: return ((y)+(-z)) >> 1;
    case 11: return ((-y) + (-z)) >> 1;
    case 12: return ((y)+(x)) >> 1;
    case 13: return ((-y) + (z)) >> 1;
    case 14: return ((y)+(-x)) >> 1;
    case 15: return ((-y) + (-z)) >> 1;
    }
#else
    hash = hash & 15;
    int16_t u = hash < 8 ? x : y;
    int16_t v = hash < 4 ? y : hash == 12 || hash == 14 ? x : z;
    if (hash & 1) { u = -u; }
    if (hash & 2) { v = -v; }

    return AVG15(u, v);
#endif
}

static int16_t inline __attribute__((always_inline)) grad16(uint8_t hash, int16_t x, int16_t y) {
    hash = hash & 7;
    int16_t u, v;
    if (hash < 4) { u = x; v = y; }
    else { u = y; v = x; }
    if (hash & 1) { u = -u; }
    if (hash & 2) { v = -v; }

    return AVG15(u, v);
}

static int16_t inline __attribute__((always_inline)) grad16(uint8_t hash, int16_t x) {
    hash = hash & 15;
    int16_t u, v;
    if (hash > 8) { u = x; v = x; }
    else if (hash < 4) { u = x; v = 1; }
    else { u = 1; v = x; }
    if (hash & 1) { u = -u; }
    if (hash & 2) { v = -v; }

    return AVG15(u, v);
}


#define EASE8(x)  (ease8InOutQuad(x) )
#define EASE16(x) (ease16InOutQuad(x))
#define FADE(x) scale16(x,x)
#define LERP(a,b,u) lerp15by16(a,b,u)

int16_t inoise16_raw(uint32_t x, uint32_t y, uint32_t z)
{
    // Find the unit cube containing the point
    uint8_t X = (x >> 16) & 0xFF;
    uint8_t Y = (y >> 16) & 0xFF;
    uint8_t Z = (z >> 16) & 0xFF;

    // Hash cube corner coordinates
    uint8_t A = P(X) + Y;
    uint8_t AA = P(A) + Z;
    uint8_t AB = P(A + 1) + Z;
    uint8_t B = P(X + 1) + Y;
    uint8_t BA = P(B) + Z;
    uint8_t BB = P(B + 1) + Z;

    // Get the relative position of the point in the cube
    uint16_t u = x & 0xFFFF;
    uint16_t v = y & 0xFFFF;
    uint16_t w = z & 0xFFFF;

    // Get a signed version of the above for the grad function
    int16_t xx = (u >> 1) & 0x7FFF;
    int16_t yy = (v >> 1) & 0x7FFF;
    int16_t zz = (w >> 1) & 0x7FFF;
    uint16_t N = 0x8000L;

    u = EASE16(u); v = EASE16(v); w = EASE16(w);

    // skip the log fade adjustment for the moment, otherwise here we would
    // adjust fade values for u,v,w
    int16_t X1 = LERP(grad16(P(AA), xx, yy, zz), grad16(P(BA), xx - N, yy, zz), u);
    int16_t X2 = LERP(grad16(P(AB), xx, yy - N, zz), grad16(P(BB), xx - N, yy - N, zz), u);
    int16_t X3 = LERP(grad16(P(AA + 1), xx, yy, zz - N), grad16(P(BA + 1), xx - N, yy, zz - N), u);
    int16_t X4 = LERP(grad16(P(AB + 1), xx, yy - N, zz - N), grad16(P(BB + 1), xx - N, yy - N, zz - N), u);

    int16_t Y1 = LERP(X1, X2, v);
    int16_t Y2 = LERP(X3, X4, v);

    int16_t ans = LERP(Y1, Y2, w);

    return ans;
}

uint16_t inoise16(uint32_t x, uint32_t y, uint32_t z)
{
    int32_t ans = inoise16_raw(x, y, z);
    ans = ans + 19052L;
    uint32_t pan = ans;
    // pan = (ans * 220L) >> 7.  That's the same as:
    // pan = (ans * 440L) >> 8.  And this way avoids a 7X four-byte shift-loop on AVR.
    // Identical math, except for the highest bit, which we don't care about anyway,
    // since we're returning the 'middle' 16 out of a 32-bit value anyway.
    pan *= 440L;
    return (pan >> 8);
}


size_t CSerial::write(uint8_t _uiValue)
{
    char szText[2];
    szText[0] = _uiValue;
    szText[1] = 0;

    OutputDebugStringA(szText);
    return 1;
}

size_t CSerial::write(const uint8_t* buffer, size_t size)
{
    OutputDebugStringA((char *)buffer);

    return size;
}

void CSerial::flush()
{
}

void CSerial::begin(uint32_t)
{
}

static funcGetAnalog g_pGetAnalogFunc        = nullptr;
static void *        g_pGetAnalogFuncContext = nullptr;

void setAnalogReadFunction(void *_pContext, funcGetAnalog _function)
{
    g_pGetAnalogFuncContext = _pContext;
    g_pGetAnalogFunc        = _function;
}

int analogRead(uint8_t pin)
{
    int iValue = 0;

    if (g_pGetAnalogFunc != nullptr)
    {
        iValue = g_pGetAnalogFunc(g_pGetAnalogFuncContext, pin);
    }

    return iValue;
}


CFastLED::CFastLED() {
	// clear out the array of led controllers
	// m_nControllers = 0;
	m_Scale = 255;
	m_nFPS = 0;
	m_pPowerFunc = NULL;
	m_nPowerData = 0xFFFFFFFF;
	srand((unsigned int) time(0)); // Initialize random generator
}

CLEDController& CFastLED::addLeds(CLEDController* pLed,
	struct CRGB* data,
	int nLedsOrOffset, int nLedsIfOffset) {
	int nOffset = (nLedsIfOffset > 0) ? nLedsOrOffset : 0;
	int nLeds = (nLedsIfOffset > 0) ? nLedsIfOffset : nLedsOrOffset;

	pLed->init();
	pLed->setLeds(data + nOffset, nLeds);
	FastLED.setMaxRefreshRate(pLed->getMaxRefreshRate(), true);
	return *pLed;
}

void CFastLED::show(uint8_t scale)
{
	CLEDController* pCur = CLEDController::head();
	while (pCur) {
		uint8_t d = pCur->getDither();
		if (m_nFPS < 100) { pCur->setDither(0); }
		pCur->showLeds(scale);
		pCur->setDither(d);
		pCur = pCur->next();
	}
	countFPS();
}

int CFastLED::count() {
	int x = 0;
	CLEDController* pCur = CLEDController::head();
	while (pCur) {
		++x;
		pCur = pCur->next();
	}
	return x;
}

CLEDController& CFastLED::operator[](int x) {
	CLEDController* pCur = CLEDController::head();
	while (x-- && pCur) {
		pCur = pCur->next();
	}
	if (pCur == NULL) {
		return *(CLEDController::head());
	}
	else {
		return *pCur;
	}
}

void CFastLED::showColor(const struct CRGB& color, uint8_t scale)
{
	// If we have a function for computing power, use it!
	if (m_pPowerFunc) {
		scale = (*m_pPowerFunc)(scale, m_nPowerData);
	}

	CLEDController* pCur = CLEDController::head();
	while (pCur) {
		uint8_t d = pCur->getDither();
		if (m_nFPS < 100) { pCur->setDither(0); }
		pCur->showColor(color, scale);
		pCur->setDither(d);
		pCur = pCur->next();
	}
	countFPS();
}

void CFastLED::clear(bool writeData)
{
	if (writeData) {
		showColor(CRGB(0, 0, 0), 0);
	}
	clearData();
}

void CFastLED::clearData() {
	CLEDController* pCur = CLEDController::head();
	while (pCur) {
		pCur->clearLedData();
		pCur = pCur->next();
	}
}

void CFastLED::delay(unsigned long ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void CFastLED::setTemperature(const struct CRGB& temp) {
	CLEDController* pCur = CLEDController::head();
	while (pCur) {
		pCur->setTemperature(temp);
		pCur = pCur->next();
	}
}

void CFastLED::setCorrection(const struct CRGB& correction) {
	CLEDController* pCur = CLEDController::head();
	while (pCur) {
		pCur->setCorrection(correction);
		pCur = pCur->next();
	}
}

void CFastLED::setDither(uint8_t ditherMode) {
	CLEDController* pCur = CLEDController::head();
	while (pCur) {
		pCur->setDither(ditherMode);
		pCur = pCur->next();
	}
}

/// Get the number of leds in the first controller
/// @returns the number of LEDs in the first controller
int CFastLED::size()
{
	return (*this)[0].size();
}

/// Get a pointer to led data for the first controller
/// @returns pointer to the CRGB buffer for the first controller
CRGB* CFastLED::leds()
{
	return (*this)[0].leds();
}

void CFastLED::countFPS(int nFrames) {
	static int br = 0;
	static uint32_t lastframe = millis();

	if (br++ >= nFrames) {
		uint32_t now = millis();
		now -= lastframe;
		if (now == 0) {
			now = 1; // prevent division by zero below
		}
		m_nFPS = (br * 1000) / now;
		br = 0;
		lastframe = millis();
	}
}

void CFastLED::setMaxRefreshRate(uint16_t refresh, bool constrain) {
	if (constrain) {
		// if we're constraining, the new value of m_nMinMicros _must_ be higher than previously (because we're only
		// allowed to slow things down if constraining)
		if (refresh > 0) {
			m_nMinMicros = (((uint32_t) (1000000 / refresh)) > m_nMinMicros) ? (1000000 / refresh) : m_nMinMicros;
		}
	}
	else if (refresh > 0) {
		m_nMinMicros = 1000000 / refresh;
	}
	else {
		m_nMinMicros = 0;
	}
}

long long milliseconds_now()
{
	auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return millisec_since_epoch;
}

static long long timems_start = milliseconds_now();

unsigned long millis()
{
	return (unsigned long)(milliseconds_now() - timems_start);
}

uint16_t rand16seed; // = RAND16_SEED;

CLEDController* CLEDController::m_pHead = NULL;
CLEDController* CLEDController::m_pTail = NULL;

/// Add an SPI based CLEDController instance to the world.
template<class CHIPSET, uint8_t DATA_PIN, uint8_t CLOCK_PIN, EOrder RGB_ORDER >
static CLEDController& addLeds(struct CRGB* data, int nLedsOrOffset, int nLedsIfOffset)
{
    static WS2812<DATA_PIN, RGB_ORDER> c;
    return addLeds(&c, data, nLedsOrOffset, nLedsIfOffset);
}

inline void SetColorPalette(int)
{
}

/// Force a variable reference to avoid compiler over-optimization. 
/// Sometimes the compiler will do clever things to reduce
/// code size that result in a net slowdown, if it thinks that
/// a variable is not used in a certain location.
/// This macro does its best to convince the compiler that
/// the variable is used in this location, to help control
/// code motion and de-duplication that would result in a slowdown.
#define FORCE_REFERENCE(var)  var

/// This version of scale8_video() does not clean up the R1 register on AVR. 
/// If you are doing several "scale8_video()'s" in a row, use this, and
/// then explicitly call cleanup_R1().
/// @warning You **MUST** call cleanup_R1() after using this function!
/// @param i input value to scale
/// @param scale scale factor, in n/256 units
/// @returns scaled value
/// @see scale8_video()

/// @cond
#define K255 255
#define K171 171
#define K170 170
#define K85  85


void hsv2rgb_rainbow(const CHSV& hsv, CRGB& rgb)
{
    // Yellow has a higher inherent brightness than
    // any other color; 'pure' yellow is perceived to
    // be 93% as bright as white.  In order to make
    // yellow appear the correct relative brightness,
    // it has to be rendered brighter than all other
    // colors.
    // Level Y1 is a moderate boost, the default.
    // Level Y2 is a strong boost.
    const uint8_t Y1 = 1;
    const uint8_t Y2 = 0;

    // G2: Whether to divide all greens by two.
    // Depends GREATLY on your particular LEDs
    const uint8_t G2 = 0;

    // Gscale: what to scale green down by.
    // Depends GREATLY on your particular LEDs
    const uint8_t Gscale = 0;


    uint8_t hue = hsv.hue;
    uint8_t sat = hsv.sat;
    uint8_t val = hsv.val;

    uint8_t offset = hue & 0x1F; // 0..31

    // offset8 = offset * 8
    uint8_t offset8 = offset;
    {
#if defined(__AVR__)
        // Left to its own devices, gcc turns "x <<= 3" into a loop
        // It's much faster and smaller to just do three single-bit shifts
        // So this business is to force that.
        offset8 <<= 1;
        asm volatile("");
        offset8 <<= 1;
        asm volatile("");
        offset8 <<= 1;
#else
        // On ARM and other non-AVR platforms, we just shift 3.
        offset8 <<= 3;
#endif
    }

    uint8_t third = scale8(offset8, (256 / 3)); // max = 85

    uint8_t r, g, b;

    if (!(hue & 0x80)) {
        // 0XX
        if (!(hue & 0x40)) {
            // 00X
            //section 0-1
            if (!(hue & 0x20)) {
                // 000
                //case 0: // R -> O
                r = K255 - third;
                g = third;
                b = 0;
                FORCE_REFERENCE(b);
            }
            else {
                // 001
                //case 1: // O -> Y
                if (Y1) {
                    r = K171;
                    g = K85 + third;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
                if (Y2) {
                    r = K170 + third;
                    //uint8_t twothirds = (third << 1);
                    uint8_t twothirds = scale8(offset8, ((256 * 2) / 3)); // max=170
                    g = K85 + twothirds;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
            }
        }
        else {
            //01X
            // section 2-3
            if (!(hue & 0x20)) {
                // 010
                //case 2: // Y -> G
                if (Y1) {
                    //uint8_t twothirds = (third << 1);
                    uint8_t twothirds = scale8(offset8, ((256 * 2) / 3)); // max=170
                    r = K171 - twothirds;
                    g = K170 + third;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
                if (Y2) {
                    r = K255 - offset8;
                    g = K255;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
            }
            else {
                // 011
                // case 3: // G -> A
                r = 0;
                FORCE_REFERENCE(r);
                g = K255 - third;
                b = third;
            }
        }
    }
    else {
        // section 4-7
        // 1XX
        if (!(hue & 0x40)) {
            // 10X
            if (!(hue & 0x20)) {
                // 100
                //case 4: // A -> B
                r = 0;
                FORCE_REFERENCE(r);
                //uint8_t twothirds = (third << 1);
                uint8_t twothirds = scale8(offset8, ((256 * 2) / 3)); // max=170
                g = K171 - twothirds; //K170?
                b = K85 + twothirds;

            }
            else {
                // 101
                //case 5: // B -> P
                r = third;
                g = 0;
                FORCE_REFERENCE(g);
                b = K255 - third;

            }
        }
        else {
            if (!(hue & 0x20)) {
                // 110
                //case 6: // P -- K
                r = K85 + third;
                g = 0;
                FORCE_REFERENCE(g);
                b = K171 - third;

            }
            else {
                // 111
                //case 7: // K -> R
                r = K170 + third;
                g = 0;
                FORCE_REFERENCE(g);
                b = K85 - third;

            }
        }
    }

    // This is one of the good places to scale the green down,
    // although the client can scale green down as well.
    if (G2) g = g >> 1;
    if (Gscale) g = scale8_video_LEAVING_R1_DIRTY(g, Gscale);

    // Scale down colors if we're desaturated at all
    // and add the brightness_floor to r, g, and b.
    if (sat != 255) {
        if (sat == 0) {
            r = 255; b = 255; g = 255;
        }
        else {
            uint8_t desat = 255 - sat;
            desat = scale8_video(desat, desat);

            uint8_t satscale = 255 - desat;
            //satscale = sat; // uncomment to revert to pre-2021 saturation behavior

            //nscale8x3_video( r, g, b, sat);
#if (FASTLED_SCALE8_FIXED==1)
            r = scale8_LEAVING_R1_DIRTY(r, satscale);
            g = scale8_LEAVING_R1_DIRTY(g, satscale);
            b = scale8_LEAVING_R1_DIRTY(b, satscale);
            cleanup_R1();
#else
            if (r) r = scale8(r, satscale) + 1;
            if (g) g = scale8(g, satscale) + 1;
            if (b) b = scale8(b, satscale) + 1;
#endif
            uint8_t brightness_floor = desat;
            r += brightness_floor;
            g += brightness_floor;
            b += brightness_floor;
        }
    }

    // Now scale everything down if we're at value < 255.
    if (val != 255) {

        val = scale8_video_LEAVING_R1_DIRTY(val, val);
        if (val == 0) {
            r = 0; g = 0; b = 0;
        }
        else {
            // nscale8x3_video( r, g, b, val);
#if (FASTLED_SCALE8_FIXED==1)
            r = scale8_LEAVING_R1_DIRTY(r, val);
            g = scale8_LEAVING_R1_DIRTY(g, val);
            b = scale8_LEAVING_R1_DIRTY(b, val);
            cleanup_R1();
#else
            if (r) r = scale8(r, val) + 1;
            if (g) g = scale8(g, val) + 1;
            if (b) b = scale8(b, val) + 1;
#endif
        }
    }

    // Here we have the old AVR "missing std X+n" problem again
    // It turns out that fixing it winds up costing more than
    // not fixing it.
    // To paraphrase Dr Bronner, profile! profile! profile!
    //asm volatile(  ""  :  :  : "r26", "r27" );
    //asm volatile (" movw r30, r26 \n" : : : "r30", "r31");
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
}

//===================================================
// SIMU ADD CODE
//
// match8.h

/// Blend a variable proportion (0-255) of one byte to another. 
/// @param a the starting byte value
/// @param b the byte value to blend toward
/// @param amountOfB the proportion (0-255) of b to blend
/// @returns a byte value between a and b, inclusive
#if (FASTLED_BLEND_FIXED == 1)
uint8_t blend8(uint8_t a, uint8_t b, uint8_t amountOfB)
{

    // The BLEND_FIXED formula is
    //
    //   result = (  A*(amountOfA) + B*(amountOfB)              )/ 256
    //
    // …where amountOfA = 255-amountOfB.
    //
    // This formula will never return 255, which is why the BLEND_FIXED + SCALE8_FIXED version is
    //
    //   result = (  A*(amountOfA) + A + B*(amountOfB) + B      ) / 256
    //
    // We can rearrange this formula for some great optimisations.
    //
    //   result = (  A*(amountOfA) + A + B*(amountOfB) + B      ) / 256
    //          = (  A*(255-amountOfB) + A + B*(amountOfB) + B  ) / 256
    //          = (  A*(256-amountOfB) + B*(amountOfB) + B      ) / 256
    //          = (  A*256 + B + B*(amountOfB) - A*(amountOfB)  ) / 256  // this is the version used in SCALE8_FIXED AVR below
    //          = (  A*256 + B + (B-A)*(amountOfB)              ) / 256  // this is the version used in SCALE8_FIXED C below

    uint16_t partial;
    uint8_t result;

#if BLEND8_C == 1

#   if (FASTLED_SCALE8_FIXED == 1)
    partial = (a << 8) | b; // A*256 + B

    // on many platforms this compiles to a single multiply of (B-A) * amountOfB
    partial += (b * amountOfB);
    partial -= (a * amountOfB);

#   else
    uint8_t amountOfA = 255 - amountOfB;

    // on the other hand, this compiles to two multiplies, and gives the "wrong" answer :]
    partial = (a * amountOfA);
    partial += (b * amountOfB);
#   endif

    result = partial >> 8;

    return result;

#elif BLEND8_AVRASM == 1

#   if (FASTLED_SCALE8_FIXED == 1)

    // 1 or 2 cycles depending on how the compiler optimises
    partial = (a << 8) | b;

    // 7 cycles
    asm volatile (
        "  mul %[a], %[amountOfB]        \n\t"
        "  sub %A[partial], r0           \n\t"
        "  sbc %B[partial], r1           \n\t"
        "  mul %[b], %[amountOfB]        \n\t"
        "  add %A[partial], r0           \n\t"
        "  adc %B[partial], r1           \n\t"
        "  clr __zero_reg__              \n\t"
        : [partial] "+r" (partial)
        : [amountOfB] "r" (amountOfB),
        [a] "r" (a),
        [b] "r" (b)
        : "r0", "r1"
        );

#   else

    // non-SCALE8-fixed version

    // 7 cycles
    asm volatile (
        /* partial = b * amountOfB */
        "  mul %[b], %[amountOfB]        \n\t"
        "  movw %A[partial], r0          \n\t"

        /* amountOfB (aka amountOfA) = 255 - amountOfB */
        "  com %[amountOfB]              \n\t"

        /* partial += a * amountOfB (aka amountOfA) */
        "  mul %[a], %[amountOfB]        \n\t"

        "  add %A[partial], r0           \n\t"
        "  adc %B[partial], r1           \n\t"

        "  clr __zero_reg__              \n\t"

        : [partial] "=r" (partial),
        [amountOfB] "+a" (amountOfB)
        : [a] "a" (a),
        [b] "a" (b)
        : "r0", "r1"
        );

#   endif

    result = partial >> 8;

    return result;

#else
#   error "No implementation for blend8 available."
#endif
}
#endif

// SIMU ADD CODE
//===================================================

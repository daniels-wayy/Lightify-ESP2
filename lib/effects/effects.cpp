#include "effects.h"

constexpr auto WIDTH = DEVICE_CONFIG.ledCount;
constexpr auto HEIGHT = 1;

static CRGBPalette16 currentPalette(PartyColors_p);
static uint8_t speed = 4;
static uint8_t scale = 8;
static uint16_t x;
static uint16_t y;
static uint16_t z;
static uint8_t noise[WIDTH];

LedEffects::LedEffects(
    LedService &strip, 
    Config &cfg, 
    bool &fxPropertyChanged
) : 
    _strip(&strip), 
    _cfg(&cfg),
    _fxPropertyChanged(fxPropertyChanged)
{
}

void LedEffects::loadingEffect(CRGB color)
{
    static int8_t dir = 1;
    static uint8_t val = MIN_LOAD_ANIM_BRI;
    static Timer tmr(30);
    if (tmr.period())
    {
        val += dir;
        if (val >= MAX_LOAD_ANIM_BRI || val <= MIN_LOAD_ANIM_BRI)
            dir = -dir;
        _strip->fill(color);
        _strip->setBrightness(val);
        _strip->update();
    }
    yield();
}

void LedEffects::lavaNoise(uint8_t fxScale, uint8_t fxSpeed)
{
    if (_fxPropertyChanged)
    {
        _fxPropertyChanged = false;
        currentPalette = LavaColors_p;
        scale = fxScale;
        speed = fxSpeed; 
    }
    _fillNoiseLED();
}

void LedEffects::cloudsNoise(uint8_t fxScale, uint8_t fxSpeed)
{
    if (_fxPropertyChanged)
    {
        _fxPropertyChanged = false;
        currentPalette = CloudColors_p;
        scale = fxScale;
        speed = fxSpeed; 
    }
    _fillNoiseLED();
}

void LedEffects::forestNoise(uint8_t fxScale, uint8_t fxSpeed)
{
    if (_fxPropertyChanged)
    {
        _fxPropertyChanged = false;
        currentPalette = ForestColors_p;
        scale = fxScale;
        speed = fxSpeed; 
    }
    _fillNoiseLED();
}

void LedEffects::oceanNoise(uint8_t fxScale, uint8_t fxSpeed)
{
    if (_fxPropertyChanged)
    {
        _fxPropertyChanged = false;
        currentPalette = OceanColors_p;
        scale = fxScale;
        speed = fxSpeed; 
    }
    _fillNoiseLED();
}

void LedEffects::sparkles(uint8_t fxScale)
{
    if (random(0, 255) < (fxScale / 4))
    {
        uint16_t pos = random(_cfg->ledCount);
        _strip->getLeds()[pos] = blend(_cfg->currentColor(), SPARKLES_COLOR, 48);
    }
    for (uint16_t i = 0; i < _cfg->ledCount; i++)
    {
        if (_strip->getLeds()[i] != SPARKLES_COLOR)
        {
            _strip->getLeds()[i] = blend(_strip->getLeds()[i], _cfg->currentColor(), 48);
        }
    }
}

static uint8_t hue = 0;
void LedEffects::rainbow(uint8_t fxScale)
{
    hue += 2;
    for (byte j = 0; j < WIDTH; j++)
    {
        auto thisColor = CHSV((byte)(hue + j * fxScale), 255, 255);
        _strip->fillPixel(j, thisColor);
    }
}

static int counter = 0;
void LedEffects::fire(uint8_t scale) {
    for (uint16_t i = 0; i < _cfg->ledCount; i++) {
        _strip->fillPixel(i, _getFireColor((inoise8(i * /*FIRE_STEP*/ (uint8_t)(scale / 2), counter))));
    }
    counter += 20;
}

void LedEffects::setSmoothBrightness(uint8_t oldBri, uint8_t newBri) {
    if(oldBri == newBri) {
        return;
    }

    if (oldBri < newBri) {
        for (int i = oldBri; i < newBri; i++) {
            if (i >= 255) {
                _strip->setBrightness(255);
            } else {
                _strip->setBrightness(i);
            }
            _strip->update();
        }
    } else if (oldBri > newBri) {
        for (int i = oldBri; i > newBri; i--) {
            if (i <= newBri) {
                _strip->setBrightness(newBri);
            } else {
                _strip->setBrightness(i);
            }
            _strip->update();
        }
    }
}

void LedEffects::_fillNoiseLED()
{
    uint8_t dataSmoothing = 0;
    if (speed < 50)
    {
        dataSmoothing = 200 - (speed * 4);
    }
    for (int i = 0; i < WIDTH; i++)
    {
        int ioffset = scale * i;
        for (int j = 0; j < WIDTH; j++)
        {
            int joffset = scale * j;

            uint8_t data = inoise8(x + ioffset, y + joffset, z);

            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            if (dataSmoothing)
            {
                uint8_t olddata = noise[i];
                uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
                data = newdata;
            }

            noise[j] = data;
        }
    }

    z += speed;
    x += speed / 8;
    y -= speed / 16;

    for (int i = 0; i < WIDTH; i++)
    {
            uint8_t index = noise[i];
            uint8_t bri = noise[i];
            if (bri > 127)
            {
                bri = 255;
            }
            else
            {
                bri = dim8_raw(bri * 2);
            }
            _strip->fillPixel(i, ColorFromPalette(currentPalette, index, bri));
    }
}

CHSV LedEffects::_getFireColor(int val)
{
    return CHSV(
        HUE_START + map(val, 0, 255, 0, /*HUE_GAP*/ _cfg->colorHue),    // H
        constrain(map(val, 0, 255, MAX_SAT, MIN_SAT), 0, 255),          // S
        constrain(map(val, 0, 255, MIN_BRIGHT, MAX_BRIGHT), 0, 255)     // V
    );
}
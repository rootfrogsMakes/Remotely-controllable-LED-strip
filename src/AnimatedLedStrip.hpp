/*
* Class to do some animations on an Adafruit Neopixel compatible LED strip.
* Tested with an WS2812 LED strip with 140 LEDs
* All methods are self explained named.
*
* Author: rootfrogsMakes
* URL:    https://makes.rootfrogs.com
*/

#ifndef ANIMATED_LED_STRIPE_HPP
#define ANIMATED_LED_STRIPE_HPP

#include <Adafruit_NeoPixel.h>

class Colors
{
    //Helper class for some constant color definitions
    public:
        static const uint32_t RED       = 0x00FF0000;
        static const uint32_t GREEN     = 0x0000FF00;
        static const uint32_t BLUE      = 0x000000FF;
        static const uint32_t YELLOW    = 0x00FFFF00;
        static const uint32_t WHITE     = 0x00FFFFFF;
};

typedef enum 
{
    NONE = 0,
    FADING = 1,
    LAOLA_WAVE = 2,
    STROBO = 3,
    FADING_RAINBOW = 4,
    RUNNING_RAINBOW = 5
}Animation_t;

class AnimatedLedStrip{
    public:
        AnimatedLedStrip(Adafruit_NeoPixel& strip);
        
        //call frequently in your main-loop:
        void run();

        void setBrightness(uint8_t percent);
        void moreBrightness(uint8_t add);
        void lessBrightness(uint8_t sub);

        void setColor(uint32_t color);
        void setAnimation(Animation_t animation);
        void setSineWave(uint32_t color, uint8_t startPos, uint8_t stepWidth);

        void turnOff();
        void turnOn();

        bool isOn();

    private:
        Adafruit_NeoPixel& ledStrip;
        bool update = false;
        Animation_t animation = NONE;
        uint32_t lastTimestamp = 0;
        uint32_t currentColor = 0;
        uint32_t aimColor = 0;
        uint32_t backupColor = 0;
        uint8_t animationFlag = false;
        uint8_t rainbowPos = 0;
        bool on = true;

        bool incrementColor(uint32_t* color, uint8_t amount);
        uint32_t Wheel(byte WheelPos);
        void refreshColor();

};

#endif
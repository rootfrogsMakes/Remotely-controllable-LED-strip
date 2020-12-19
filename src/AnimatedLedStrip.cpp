#include "AnimatedLedStrip.hpp"

AnimatedLedStrip::AnimatedLedStrip(Adafruit_NeoPixel& strip) :
    ledStrip(strip)
{

}

void AnimatedLedStrip::run()
{
    if(on == true)
    {
        if(animation == NONE)
        {
            if((millis() - lastTimestamp) > 1)
            {
                refreshColor();
                lastTimestamp = millis();
            }
        }
        else if(animation == FADING)
        {
            if((millis() - lastTimestamp) > 1)
            {
                if(animationFlag == 0)
                {
                    aimColor = 0;
                    refreshColor();
                    if(currentColor == aimColor)
                    {
                        animationFlag = 1;
                    }
                }
                else
                {
                    aimColor = backupColor;
                    refreshColor();
                    if(currentColor == aimColor)
                    {
                        animationFlag = 0;
                    }
                }

                lastTimestamp = millis();
                update = true;
            }
        }
        else if(animation == LAOLA_WAVE)
        {
            if((millis() - lastTimestamp) > 5)
            {
                animationFlag ++;
                setSineWave(aimColor, animationFlag, 8);

                lastTimestamp = millis();
                update = true;
            }
        }
        else if(animation == STROBO)
        {
            if((millis() - lastTimestamp) > 70)
            {
                if(animationFlag == 0)
                {
                    currentColor = 0;
                    animationFlag = 1;
                }
                else
                {
                    currentColor = aimColor;
                    animationFlag = 0;
                }

                ledStrip.fill(currentColor);

                lastTimestamp = millis();
                update = true;
            }
        }
        else if(animation == FADING_RAINBOW)
        {
            if((millis() - lastTimestamp) > 10)
            {
                rainbowPos++;
                ledStrip.fill(Wheel(rainbowPos));
                lastTimestamp = millis();
                update = true;
            }
            
        }
        else if(animation == RUNNING_RAINBOW)
        {
            if((millis() - lastTimestamp) > 10)
            {
                uint8_t tmpPos;
                rainbowPos++;
                for(uint16_t i = 0; i < ledStrip.numPixels(); i++)
                {
                    tmpPos = rainbowPos + (i*4);
                    ledStrip.setPixelColor(i, Wheel(tmpPos));
                }

                lastTimestamp = millis();
                update = true;
            }
            
        }    
    }
    else
    {
        //on == false
        if((millis() - lastTimestamp) > 1)
        {
            //for fading out all LEDs
            refreshColor();
            lastTimestamp = millis();
        }
    }

    if(update == true)
    {
        ledStrip.show();
        update = false;
    }
}

void AnimatedLedStrip::setBrightness(uint8_t percent)
{
    ledStrip.setBrightness(percent);
    update = true;
}

void AnimatedLedStrip::moreBrightness(uint8_t add)
{
    uint8_t newBrightness;
    uint8_t currentBrightness = ledStrip.getBrightness();

    if(currentBrightness < 100)
    {
        newBrightness = currentBrightness + add;
        if(newBrightness > 100)
        {
            newBrightness = 100;
        }

        ledStrip.setBrightness(newBrightness);
        update = true;
    }
}

void AnimatedLedStrip::lessBrightness(uint8_t sub)
{
    uint8_t newBrightness;
    uint8_t currentBrightness = ledStrip.getBrightness();

    if(currentBrightness > 1)
    {
        if(sub >= currentBrightness)
        {
            newBrightness = 1;
        }
        else
        {
            newBrightness = currentBrightness - sub;
        }

        ledStrip.setBrightness(newBrightness);
        update = true;
    }
}

void AnimatedLedStrip::setColor(uint32_t color)
{
    aimColor = color;
    backupColor = color;

    if(animation == FADING_RAINBOW || animation == RUNNING_RAINBOW)
    {
        //setting a fix color will leave animations which dont use userset color
        animation = NONE;
    }
}

void AnimatedLedStrip::setAnimation(Animation_t animation)
{
    this->animation = animation;
    aimColor = backupColor;

    if(animation == LAOLA_WAVE || animation == FADING_RAINBOW || animation == RUNNING_RAINBOW)
    {
        //this animations doesnt use the currentColor, so set it to 0 --> force to update color on leaving animation
        currentColor = 0;
    }
}

void AnimatedLedStrip::turnOff()
{
    this->aimColor = 0;
    on = false;
}

void AnimatedLedStrip::turnOn()
{
    this->aimColor = this->backupColor;
    on = true;
}

bool AnimatedLedStrip::isOn()
{
    return on;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t AnimatedLedStrip::Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return ledStrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return ledStrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return ledStrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void AnimatedLedStrip::setSineWave(uint32_t color, uint8_t startPos, uint8_t stepWidth)
{
    uint32_t tmpColor;
    uint8_t wavePos = startPos;
    uint8_t sinValue;

    for(uint16_t i = 0; i < ledStrip.numPixels(); i++)
    {
        sinValue = ledStrip.sine8(wavePos);
        tmpColor = 0;

        if((color & 0xFF) > 0)
        {
            tmpColor |= sinValue;
        }

        if((color & 0xFF00) > 0)
        {
            tmpColor |= (((uint32_t)sinValue) << 8);
        }     

        if((color & 0xFF0000) > 0)
        {
            tmpColor |= (((uint32_t)sinValue) << 16);
        }

        ledStrip.setPixelColor(i, tmpColor);

        wavePos += stepWidth;
    }

}

void AnimatedLedStrip::refreshColor()
{
    bool colorChanged = false;

    if((currentColor & 0xFF) > (aimColor & 0xFF))
    {
        currentColor--;
        colorChanged = true;
    }
    else if((currentColor & 0xFF) < (aimColor & 0xFF))
    {
        currentColor++;
        colorChanged = true;
    }

    if((currentColor & 0xFF00) > (aimColor & 0xFF00))
    {
        currentColor-= 0x0100;
        colorChanged = true;
    }
    else if((currentColor & 0xFF00) < (aimColor & 0xFF00))
    {
        currentColor+=0x0100;
        colorChanged = true;
    }

    if((currentColor & 0xFF0000) > (aimColor & 0xFF0000))
    {
        currentColor-= 0x010000;
        colorChanged = true;
    }
    else if((currentColor & 0xFF0000) < (aimColor & 0xFF0000))
    {
        currentColor+=0x010000;
        colorChanged = true;
    }

    if(colorChanged == true)
    {
        ledStrip.fill(currentColor);
        update = true;
    }
}
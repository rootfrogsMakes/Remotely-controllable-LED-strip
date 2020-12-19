/*
* Arduino project to control an animated RGB LED strip with any IR remote control.
* Tested on Arduino Nano
*
* For more details, see Hackaday.io project description:
* https://hackaday.io/project/176189-rgb-led-strip-controlled-by-ir-remote-control
*
* Author: rootfrogsMakes
* URL:    https://makes.rootfrogs.com
*/

#include <Adafruit_NeoPixel.h>
#include "src/AnimatedLedStrip.hpp"
#include "src/IR2IO.hpp"

#define LED_PIN     2
#define LED_COUNT   141

#define IR2IO_CH1_PIN   12
#define IR2IO_CH2_PIN   11
#define IR2IO_CH3_PIN   10
#define IR2IO_CH4_PIN   9

void outputChanged(uint8_t activeOutputs);

Adafruit_NeoPixel ledStrip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
AnimatedLedStrip animatedStrip(ledStrip);
IR2IO ir2io(12, 11, 10, 9, outputChanged);

void setup() {
  ledStrip.begin();
  animatedStrip.setColor(Colors::GREEN);
  animatedStrip.setAnimation(LAOLA_WAVE);
}

void loop() {
  ir2io.run();
  animatedStrip.run();
}

void outputChanged(uint8_t activeOutputs)
{

  switch(activeOutputs)
  {
    case 0:
      //all outputs off (button on remote released)
      break;

    case 1:
      animatedStrip.setColor(Colors::RED);
      break;

    case 2:
      animatedStrip.setColor(Colors::GREEN);
      break;

    case 3:
      animatedStrip.setColor(Colors::YELLOW);
      break;

    case 4:
      animatedStrip.setColor(Colors::BLUE);
      break;

    case 5:
      animatedStrip.setColor(Colors::WHITE);
      break;

    case 6:
      animatedStrip.moreBrightness(5);
      break;

    case 7:
      animatedStrip.lessBrightness(5);
      break;

    case 8:
      if(animatedStrip.isOn())
      {
        animatedStrip.turnOff();
      }
      else
      {
        animatedStrip.turnOn();
      }
      break;

    case 9:
      animatedStrip.setAnimation(Animation_t::NONE);
      break;

    case 10:
      animatedStrip.setAnimation(Animation_t::FADING);
      break;

    case 11:
      animatedStrip.setAnimation(Animation_t::LAOLA_WAVE);
      break;

    case 12:
      animatedStrip.setAnimation(Animation_t::STROBO);
      break;

    case 13:
      animatedStrip.setAnimation(Animation_t::FADING_RAINBOW);
      break;

    case 14:
      animatedStrip.setAnimation(Animation_t::RUNNING_RAINBOW);
      break;

    case 15:
      //unused at the moment
      break;      

    default:
      break;
  } 
}

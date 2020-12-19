#include "IR2IO.hpp"
#include <Arduino.h>

IR2IO::IR2IO(uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4, void(*callBack)(uint8_t function)):
    ch1(ch1),
    ch2(ch2),
    ch3(ch3),
    ch4(ch4),
    callBack(callBack)
{
    
}

void IR2IO::run()
{
    uint8_t function = 0;

    if((millis() - lastTimestamp) > 1)
    {

        if(digitalRead(ch1) == LOW)
        {
            function |= 0x01;
        }

        if(digitalRead(ch2) == LOW)
        {
            function |= 0x02;
        }

        if(digitalRead(ch3) == LOW)
        {
            function |= 0x04;
        }  

        if(digitalRead(ch4) == LOW)
        {
            function |= 0x08;
        }


        if(function != lastSentFunctionID)
        {
            if(function == newFunctionID)
            {
                newFunctionIDCnt++;
            }
            else
            {
                newFunctionIDCnt = 0;
                newFunctionID = function;
            }

            if(newFunctionIDCnt > 2)
            {
                lastSentFunctionID = newFunctionID;
                callBack(newFunctionID);
            }
        }
        else
        {
            newFunctionIDCnt = 0;
        }
    }
    
}
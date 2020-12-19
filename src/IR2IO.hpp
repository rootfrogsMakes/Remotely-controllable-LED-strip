/*
* Helper class for IR2IO Modul
* This class will call a callback function if the output state of IR2IO Modul has changed.
* The 4 outputs are interpreted as a decimal number which is putted as parameter to the callback.
*
* IR2IO Modul available on Tindie: https://www.tindie.com/products/rootfrogs/ir2io
* 
* Author: rootfrogsMakes
* URL:    https://makes.rootfrogs.com
*/


#ifndef IR2IO_HPP
#define IR2IO_HPP

#include <stdint.h>

class IR2IO{
    public:
        IR2IO(uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4, void(*callBack)(uint8_t activeOutputs));
        
        void run();

    private:
        uint8_t ch1, ch2, ch3, ch4;
        void(*callBack)(uint8_t activeOutputs);
        uint8_t lastSentFunctionID = 0;
        uint8_t newFunctionID = 0;
        uint8_t newFunctionIDCnt = 0;
        uint32_t lastTimestamp = 0;
};

#endif
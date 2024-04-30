/*
 * PWM - Basic.cpp
 *
 * Created: 4/29/2024 11:20:12 AM
 * Author : rotes
 */ 

#include "PWM/PWM.h"
#include <util/delay.h>

uint8_t value = 0;

int main(void)
{
    PWM.begin();
    PWM1.begin();
    while (1) 
    {
        PWM.write(0, value);
        PWM.write(1, 255 - value);
        PWM1.write(1, value);
        PWM1.write(0, 255 - value);
        value++;
        _delay_ms(10);
    }
}


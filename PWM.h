#ifndef __PWM_H__
#define __PWM_H__

/* Dependencies */
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h> /* Interrupts */

class __PWM__
{
    public:
        __PWM__(volatile uint8_t* tccra, volatile uint8_t* tccrb, volatile uint8_t* ocra, volatile uint8_t* ocrb);
        ~__PWM__();
        void begin(void);
        void write(const uint8_t channel, const uint8_t value);
        void end  (void);
    private:
        uint8_t hasBegun;
        volatile uint8_t* tccra;
        volatile uint8_t* tccrb;
        volatile uint8_t* ocra;
        volatile uint8_t* ocrb;
};

#if defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__)
    #define HAS_PWM
    extern __PWM__ PWM;
    #define HAS_PWM1
    extern __PWM__ PWM1;
#else
    #error "No PWM implementation available"
#endif

#endif

/* Dependencies */
#include "PWM.h"

/*!
 * @brief  __PWM__ constructor
 * @param  TCCRA, TCCRB, OCRA, OCRB
 *         The PWM registers
 */
__PWM__::__PWM__(volatile uint8_t* tccra, volatile uint8_t* tccrb, volatile uint8_t* ocra, volatile uint8_t* ocrb)
{
    this->tccra = tccra;
    this->tccrb = tccrb;
    this->ocra = ocra;
    this->ocrb = ocrb;
}

/*!
 * @brief  __PWM__ destructor
 */
__PWM__::~__PWM__()
{
    this->tccra = NULL;
    this->tccrb = NULL;
    this->ocra = NULL;
    this->ocrb = NULL;
}

/*!
 * @brief  Begins the PWM implementation
 */
void __PWM__::begin(void)
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    if (this->tccra == &TCCR0A)
    {
        DDRD = DDRD | ((1 << PD6) | (1 << PD5));                                     // Set OCA & OCB as OUTPUT
        *this->tccra = (*this->tccra & ~(1 << COM0A0)) | (1 << COM0A1);              // Non-inverted PWM on OCA
        *this->tccra = (*this->tccra & ~(1 << COM0B0)) | (1 << COM0B1);              // Non-inverted PWM on OCB
        *this->tccrb = *this->tccrb & ~(1 << WGM02);                                 // Put working mode in Fast PWM mode
        *this->tccra = *this->tccra | ((1 << WGM01) | (1 << WGM00));
        *this->tccrb = ((*this->tccrb & ~(1 << CS02)) | (1 << CS01)) & ~(1 << CS00); // Set prescaler to 8
    }
    else if (this->tccra == &TCCR1A)
    {
        DDRB = DDRB | ((1 << PB1) | (1 << PB2));                                     // Set OCA & OCB as OUTPUT
        *this->tccra = (*this->tccra & ~(1 << COM1A0)) | (1 << COM1A1);              // Non-inverted PWM on OCA
        *this->tccra = (*this->tccra & ~(1 << COM1B0)) | (1 << COM1B1);              // Non-inverted PWM on OCB
        *this->tccra = (*this->tccra & ~(1 << WGM11)) | (1 << WGM10);
        *this->tccrb = (*this->tccrb & ~(1 << WGM13)) | (1 << WGM12);                // Put working mode in <Fast PWM> 8 bit mode
        *this->tccrb = ((*this->tccrb & ~(1 << CS12)) | (1 << CS11)) & ~(1 << CS10); // Set prescaler to 8
    }
    else if (this->tccra == &TCCR2A)
    {
        DDRB = DDRB | (1 << PB3);                                                    // Set OCA as OUTPUT
        DDRD = DDRD | (1 << PD3);                                                    // Set OCB as OUTPUT
        *this->tccra = (*this->tccra & ~(1 << COM2A0)) | (1 << COM2A1);              // Non-inverted PWM on OCA
        *this->tccra = (*this->tccra & ~(1 << COM2B0)) | (1 << COM2B1);              // Non-inverted PWM on OCB
        *this->tccrb = *this->tccrb & ~(1 << WGM22);                                 // Put working mode in Fast PWM mode
        *this->tccra = *this->tccra | ((1 << WGM21) | (1 << WGM20));
        *this->tccrb = ((*this->tccrb & ~(1 << CS22)) | (1 << CS21)) & ~(1 << CS20); // Set prescaler to 8
    }
    sei(); // Enable the interrupts
    #endif
}

/*!
 * @brief  Writing a PWM channel with a value
 * @param  channel
 *         The PWM channel
 * @param  value
 *         The PWM channel value
 */
void __PWM__::write(const uint8_t channel, const uint8_t value)
{
    switch (channel)
    {
        #if defined(__AVR_ATmega328P__) || \
            defined(__AVR_ATmega328PB__)
        case 0:
            *this->ocra = value;
            break;
        case 1:
            *this->ocrb = value;
            break;
        #endif
        default:
            break;
    }
}

/*!
 * @brief  Ends the PWM implementation
 */
void __PWM__::end(void)
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    if (this->tccra == &TCCR0A)
        DDRD = DDRD & ~((1 << PD6) | (1 << PD5)); // Set OCA & OCB as INPUT
    else if (this->tccra == &TCCR1A)
        DDRB = DDRB & ~((1 << PB1) | (1 << PB2)); // Set OCA & OCB as INPUT
    else if (this->tccra == &TCCR2A)
    {
        DDRB = DDRB & ~(1 << PB3); // Set OCA as INPUT
        DDRD = DDRD & ~(1 << PD3); // Set OCB as INPUT
    }
    *this->tccra = 0;
    *this->tccrb = 0;
    #endif
}

/*
void _PWM_::begin(const uint8_t channel)
{
    switch (channel)
    {
        case 4:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRB |= (1 << PB3);                                              //Set OC2A as an <OUTPUT>
                TCCR2A = TCCR2A | ((1 << WGM21) | (1 << WGM20));                 // Put working mode in <Fast PWM> mode
                TCCR2B = TCCR2B & ~(1 << WGM22);                 
                TCCR2A = (TCCR2A | (1 << COM2A1)) & ~(1 << COM2A0);              // Non-inverted PWM on OC2A
                TCCR2B = ((TCCR2B & ~(1 << CS22)) | (1 << CS21)) & ~(1 << CS20); // Set prescaler to 8
            #endif
            }
            break;
        case 5:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRD |= (1 << PD3);                              //Set OC2B as an <OUTPUT>
                TCCR2B = TCCR2B & ~(1 << WGM22);                 // Put working mode in <Fast PWM> mode
                TCCR2A = TCCR2A | ((1 << WGM21) | (1 << WGM20));
                TCCR2A = TCCR2A & ~(1 << COM2B0);                // Non-inverted PWM on OC2A
                TCCR2A = TCCR2A | (1 << COM2B1);
                TCCR2B &= ~((1 << CS22) | (1 << CS20));          // Set prescaler to 8
                TCCR2B |= (1 << CS21);
            #endif
            }
            break;
        default:
            break;
    }
}

void _PWM_::update(const uint8_t channel, const uint8_t value)
{
    switch (channel)
    {
        case 0:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                OCR0A = value;
            break;
        case 1:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                OCR0B = value;
            break;
        case 2:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                OCR1A = value;
            break;
        case 3:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                OCR1B = value;
            break;
        case 4:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                OCR2A = value;
            break;
        case 5:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                OCR2B = value;
            break;
        default:
            break;
    }
}

void _PWM_::end(const uint8_t channel)
{
    switch (channel)
    {
        case 0:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRD &= ~(1 << PD6);
                TCCR0A = TCCR0A & ~((1 << COM0A0) | (1 << COM0A1));
                #endif
            }
            break;
        case 1:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRD &= ~(1 << PD5);
                TCCR0A = TCCR0A & ~((1 << COM0B0) | (1 << COM0B1));
                #endif
            }
            break;
        case 2:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRB &= ~(1 << PB1);                                // Set OC1A as an input
                TCCR1A = TCCR1A & ~((1 << COM1A1) | (1 << COM1A0)); // Disable non-inverted PWM on OC1A
                #endif
            }
            break;
        case 3:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRB &= ~(1 << PB2);                                // Set OC1B as an input
                TCCR1A = TCCR1A & ~((1 << COM1B1) | (1 << COM1B0)); // Disable non-inverted PWM on OC1B
                #endif
            }
            break;
        case 4:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRB &= ~(1 << PB3);                                // Set OC0A as an input
                TCCR2A = TCCR2A & ~((1 << COM2A0) | (1 << COM2A1)); // Disbale non-inverted PWM on OC2A
                #endif
            }
            break;
        case 5:
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
            #if defined(__AVR_ATmega328P__) || \
                defined(__AVR_ATmega328PB__)
                DDRD &= ~(1 << PD3);                                // Set OC2A as an input
                TCCR2A = TCCR2A & ~((1 << COM2B0) | (1 << COM2B1)); // Disbale non-inverted PWM on OC2B
                #endif
            }
            break;
        default:
            break;
    }
}*/

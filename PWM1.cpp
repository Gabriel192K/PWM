/* Dependencies */
#include "PWM.h"

#if defined(HAS_PWM1)
__PWM__ PWM1 = __PWM__(&TCCR1A, &TCCR1B, &OCR1AL, &OCR1BL);
#endif
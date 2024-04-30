/* Dependencies */
#include "PWM.h"

#if defined(HAS_PWM)
__PWM__ PWM = __PWM__(&TCCR0A, &TCCR0B, &OCR0A, &OCR0B);
#endif
#ifndef __DIGITAL__
#define __DIGITAL__

#include <inttypes.h>

#define PIN0_00  0
#define PIN0_01  1
#define PIN0_02  2
#define PIN0_03  3
#define PIN0_04  4
#define PIN0_05  5
#define PIN0_31  31

#define PIN1_00  32
#define PIN1_29  61
#define PIN1_23  55
#define PIN1_31  63

#define PIN2_00  64
#define PIN2_01  65
#define PIN2_02  66
#define PIN2_03  67
#define PIN2_04  68
#define PIN2_05  69
#define PIN2_06  70
#define PIN2_07  71
#define PIN2_08  72
#define PIN2_09  73
#define PIN2_10  74
#define PIN2_11  75

#define PIN2_31  95

#define PIN3_00  96
#define PIN3_31  127

#define PIN4_00  128
#define PIN4_29  157
#define PIN4_31  159

#define INPUT  0
#define OUTPUT 1

#define HIGH 1
#define LOW 0

void    pinMode ( uint8_t pino, uint8_t tipo );
uint8_t digitalRead ( uint8_t pino );
void    digitalWrite ( uint8_t pino, uint8_t valor);





#endif

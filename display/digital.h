#ifndef __DIGITAL__
#define __DIGITAL__

#include <inttypes.h>

#define PIN0_00  0
#define PIN0_01  1
#define PIN0_02  2
#define PIN0_03  3
#define PIN0_04  4
#define PIN0_05  5
#define PIN0_06  6
#define PIN0_07  7
#define PIN0_08  8
#define PIN0_09  9
#define PIN0_10  10


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


#define PIN3_25  121
#define PIN3_26  122
#define PIN3_27  123
#define PIN3_28  124
#define PIN3_29  125
#define PIN3_30  126
#define PIN3_31  127

#define PIN4_00  128
#define PIN4_01  129
#define PIN4_02  130
#define PIN4_03  131
#define PIN4_04  132
#define PIN4_05  133
#define PIN4_06  134
#define PIN4_07  135
#define PIN4_08  136
#define PIN4_09  137
#define PIN4_10  138
#define PIN4_11  139
#define PIN4_12  140
#define PIN4_13  141
#define PIN4_14  142
#define PIN4_15  143
#define PIN4_16  144
#define PIN4_17  145
#define PIN4_18  146
#define PIN4_19  147
#define PIN4_20  148
#define PIN4_21  149
#define PIN4_22  150
#define PIN4_23  151
#define PIN4_24  152

#define PIN4_25  153
#define PIN4_26  154
#define PIN4_27  155
#define PIN4_28  156
#define PIN4_29  157
#define PIN4_30  158
#define PIN4_31  159

#define PINO(PORTA,BIT)  (PORTA*32+BIT)

#define INPUT  0
#define OUTPUT 1

#define HIGH 1
#define LOW 0


class DIGITAL {
	
	public:

		void    pinMode ( uint8_t pino, uint8_t tipo );
		uint8_t digitalRead ( uint8_t pino );
		void    digitalWrite ( uint8_t pino, uint8_t valor);

};

extern DIGITAL Digital;






#endif

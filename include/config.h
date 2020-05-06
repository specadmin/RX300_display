#ifndef CONFIG_H
#define CONFIG_H
//----------------------------------------------------------------------------
#include <avr/io.h>
//----------------------------------------------------------------------------

#define LCD_MODEL       TM060RDH01

#define RTD_CS_PORT     PORTD
#define RTD_CS_DDR      DDRD
#define RTD_CS_BIT      6

#define RTD_CLK_PORT    PORTD
#define RTD_CLK_DDR     DDRD
#define RTD_CLK_BIT     7

#define RTD_TX_PORT     PORTD
#define RTD_TX_DDR      DDRD
#define RTD_TX_BIT      5

#define RTD_RX_PIN      PIND
#define RTD_RX_BIT      4

#define DEBUG_PORT      PORTB
#define DEBUG_DDR       DDRB
#define DEBUG_BIT       6
#define DEBUG_BAUD_RATE 1152000
#define DEBUG_BUF_SIZE  200
#define DEBUG_EXTENDED_DUMP

#define RAM_GUARD_ENABLED

#define X_OFFSET        34
#define Y_OFFSET        4

#define MPX_RX_PORT         PORTD
#define MPX_RX_DIR          DDRD
#define MPX_RX_PIN          PIND
#define MPX_RX_BIT          2

#define MPX_RX_INT          INT0
#define MPX_RX_INT_VECT     INT0_vect
#define MPX_RX_ISC0         ISC00
#define MPX_RX_ISC1         ISC01

#define MPX_TX_PORT         PORTB
#define MPX_TX_DIR          DDRB
#define MPX_TX_BIT          1

#define MPX_RX_LED_PORT     PORTB
#define MPX_RX_LED_DIR      DDRB
#define MPX_RX_LED_BIT      0

//----------------------------------------------------------------------------
#endif


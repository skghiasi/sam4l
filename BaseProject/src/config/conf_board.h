/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H


// yellow led is on port PC07 
#define LED_PORT 2
#define LED_MASK (1<<7)

#define SARA_PWR_ON_PORT 2
#define SARA_PWR_ON_MASK (1 << 3)

#define PIOC	2
#define PC26	(1 << 26)
#define PC27	(1 << 27)

#define GEN_USART_RX_PIN PIN_PB00
#define GEN_USART_TX_PIN PIN_PB01

#define COM_PORT_RX_PIN	PIN_PC26
#define COM_PORT_TX_PIN PIN_PC27



#endif // CONF_BOARD_H

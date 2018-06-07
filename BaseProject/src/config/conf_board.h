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

#define CONSOLE_UART	USART0 
// yellow led is on port PC07 
#define LED_PORT 2
#define LED_MASK (1<<7)

#define PIOC	2
#define PC26	(1 << 26)
#define PC27	(1 << 27)


#endif // CONF_BOARD_H

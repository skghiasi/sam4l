/*
 * LTE_interface.c
 *
 * Created: 06/06/2018 14:04:41
 *  Author: eth
 */ 
#include <asf.h>
#include "LTE_interface.h"


void configure_LTE(void)
{
	usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		//#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
		//#endif
		.paritytype = CONF_UART_PARITY,
		//#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
		//#endif
	};
	
	usart_serial_init(CONF_UART, &uart_serial_options);
}

void sendString(void){
	usart_write_line(CONF_UART , "hello"); 
}
/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "conf_uart_serial.h"
#include "LTE_interface.h"

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)

#define COM_PORT_RX_PIN	90
#define COM_PORT_TX_PIN 91

int main (void)
{
	//const char str1[] = "Type 'a' to continue...\r\n";
	//uint8_t rx_char = 0;
	
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init(); 
	board_init();
	ioport_init(); 

	//ioport_set_port_dir(LED_PORT , LED_MASK ,	IOPORT_DIR_OUTPUT);
	 
	ioport_set_pin_peripheral_mode(COM_PORT_RX_PIN, IOPORT_MODE_MUX_A);
	ioport_set_pin_peripheral_mode(COM_PORT_TX_PIN, IOPORT_MODE_MUX_A);
		
	//configure_LTE(); 
	usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits = CONF_UART_STOP_BITS,
	};

	/* Configure console UART. */
	
	usart_serial_init(CONF_UART , &uart_serial_options) ;
	
	/* Insert application code here, after the board has been initialized. */
	/* a small demo of the yellow LED on the Xplain board, including the ioport driver from asf */ 
	
	/*enable uart0 */ 
	/*enable uart1 */ 
	//sendString(); 
	
	//usart_serial_write_packet(CONF_UART, (const uint8_t*)str1, sizeof(str1) - 1);
	//do {
		// get a single character
	//	usart_serial_getchar(CONF_UART, &rx_char);
	//} while (rx_char != 'a');
	// send a single character
	//usart_serial_putchar(CONF_UART, 'A');
	 
	while (1)
	{
		int a = 16; 
		char b [50]; 
		sprintf(b , "%d" , a);  
		delay_ms(1);   	
		usart_write_line(CONF_UART , b); 
		/* sample project of io init*/
		//ioport_toggle_port_level(LED_PORT, LED_MASK);
					
	}

}



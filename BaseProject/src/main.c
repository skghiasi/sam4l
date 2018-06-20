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

#define USART_FUNCTION 0 
#define LED_FUNCTION 1

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)


/* usart receive interrupt handler */
void CONF_UART_ISR_HANDLER(void)
{
	uint32_t dw_status = usart_get_status(CONF_UART);
	if (dw_status & US_CSR_RXRDY) {
		uint32_t received_byte;
		usart_read(CONF_UART, &received_byte);
		usart_write_line(CONF_UART, "HELLO");
	}
}



int main (void)
{
	
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init(); 
	board_init();
	ioport_init(); 

	//ioport_set_port_dir(LED_PORT , LED_MASK ,	IOPORT_DIR_OUTPUT);
	 
	ioport_set_pin_peripheral_mode(COM_PORT_RX_PIN, IOPORT_MODE_MUX_A);
	ioport_set_pin_peripheral_mode(COM_PORT_TX_PIN, IOPORT_MODE_MUX_A);
	
	ioport_set_pin_peripheral_mode(GEN_USART_RX_PIN, IOPORT_MODE_MUX_B);
	ioport_set_pin_peripheral_mode(GEN_USART_TX_PIN, IOPORT_MODE_MUX_B);
		
	//configure_LTE(); 
	usart_serial_options_t uart_serial_options = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT,
	};
	
	usart_serial_options_t conf_uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits = CONF_UART_STOP_BITS,
	};
	
	
	/* configuration of UART0 and UART1 */
	
	usart_serial_init(CONF_UART , &conf_uart_serial_options) ;
	usart_serial_init(USART_SERIAL , &uart_serial_options) ; 
	
	
	/* setting the type of usart interrupt: in this case receive ready*/
	 usart_enable_interrupt(CONF_UART,US_IER_RXRDY);
	 /* turning usart interrupt on (nested vector interrupt controller */
	 NVIC_EnableIRQ(CONF_UART_IRQn);
	while (1)
	{
	    #if USART_FUNCTION
		uint8_t received_byte = 0 ; 
		int a = sysclk_get_cpu_hz();
		char b [50]; 
		sprintf(b , "%d\n" , a);  
		delay_ms(200);
		   	
		//usart_write_line(CONF_UART , b); 
		
		/* sample project of io init*/
		//ioport_toggle_port_level(LED_PORT, LED_MASK);
		//a = sysclk_get_cpu_hz(); 
		
		//usart_write_line(USART_SERIAL, "hello");
		
		/* put this part of USART receive into an interrupt service routine */		
		//usart_write_line(CONF_UART , "hello"); 		
		usart_write_line(USART_SERIAL , "to arduino"); 			
		#endif
		
		
		
		
	}

}



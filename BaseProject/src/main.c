/**77
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

typedef enum {before_run , main_data_send , sara_error_handle} sara_functional_state_ ;
sara_functional_state_  sara_functional_state = before_run;


#define USART_FUNCTION 0
#define LED_FUNCTION 0
#define SPI_FUNCTION 0
#define TURN_ON_SARA_TEST 0
#define SARA_MAIN 1

#define TCP_SCENARIO 1
#define UDP_SCENARIO 0
#define FTP_SCENARIO 0

#define SARA_GPIO6 PIN_PC30

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)

#define UART_BUFFER_SIZE	100 

/******************************************************** variabls ******************************************/ 


char uartBuffer [2][UART_BUFFER_SIZE]; 
int bufferCounter [2]; 

int uartUser = 0 ; 
int uartKernel = 1; 


/* config usart receive interrupt handler */
void CONF_UART_ISR_HANDLER(void)
{

	uint32_t dw_status = usart_get_status(CONF_UART);
	if (dw_status & US_CSR_RXRDY) {
		uint32_t received_byte;		
		usart_read(CONF_UART, &received_byte);
		usart_write_line(USART_SERIAL, received_byte);
	}
}

/* USART to arduino handler */ 
#define test 1 
void USART_SERIAL_ISR_HANDLER(void)
{
	
		char b [50]; 
		uint32_t dw_status = usart_get_status(USART_SERIAL);
		if (dw_status & US_CSR_RXRDY) {
			//uint32_t received_byte;
			char received_byte;
			usart_read(USART_SERIAL, &received_byte);
			//usart_write_line(CONF_UART, "rec from ard");
			sprintf(b , "%c" , received_byte); 
			usart_write_line(CONF_UART, b );
		}

}

void printDebug (const char * stringInput){
	usart_write_line(CONF_UART , stringInput); 
}

void saraWriteCommand(const char * stringInput){	
	
		uint8_t byte[1];
		byte[0] = 0x0D;
		usart_write_line(USART_SERIAL, stringInput );
		usart_serial_write_packet(USART_SERIAL , byte , 1);
		delay_ms(3000);

}

void switchBuffers(){
	uartUser = (!uartUser) & 0x01; 
	uartKernel = (!uartKernel) & 0x01; 
	
	bufferCounter[uartKernel] = 0; 
}

/* function for using software to turn on 
   SARA module 
   
   return type: 
   0: successful and the module is on 
   1: module already on
   2: error after 5 attempts   
   
   */ 
int saraTurnOn(){
	int attempt_number = 1; 
	int max_num_attempts = 6;
	int hold_down_delay_ms = 250; 
	 
	int sara_gpio6_status = ioport_get_pin_level(SARA_GPIO6); 
	if(sara_gpio6_status == 1){
		return 1; 
	}
	
	while(sara_gpio6_status == 0 && attempt_number < max_num_attempts){
		//ioport_enable_port(SARA_PWR_ON_PORT , SARA_PWR_ON_MASK); 
		ioport_set_port_level(SARA_PWR_ON_PORT , SARA_PWR_ON_MASK , 1); 
		delay_ms(10); 
		ioport_set_port_level(SARA_PWR_ON_PORT , SARA_PWR_ON_MASK , 0); 
		delay_ms(hold_down_delay_ms); 		
		//ioport_disable_port(SARA_PWR_ON_PORT , SARA_PWR_ON_MASK); 
		//ioport_set_port_level(SARA_PWR_ON_PORT , SARA_PWR_ON_MASK , 1); 
		//delay_ms(1000); 
		sara_gpio6_status = ioport_get_pin_level(SARA_GPIO6); 
		attempt_number++; 	
	}
	
	if(sara_gpio6_status == 1)
		return 0; 
	else 
		return 2; 
}

/*
	returns: 
	0: sara is on 
	1: sara is off
*/ 
int sara_is_off(){
	if(ioport_get_pin_level(SARA_GPIO6)){
		return 0; 
	}
	return 1; 
}




int main (void)
{
	int one_time_run = 0; 
	int tog  = 1; 
	int turn_on_log = -1; 
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init(); 
	board_init();
	ioport_init(); 

	ioport_set_port_dir(LED_PORT , LED_MASK ,	IOPORT_DIR_OUTPUT);
	ioport_set_port_dir(LED_PORT , SARA_PWR_ON_MASK , IOPORT_DIR_OUTPUT); 
	
	ioport_set_pin_dir(SARA_GPIO6, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(SARA_GPIO6, IOPORT_MODE_PULLUP);
	
	 
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
	 /* setting the type of usart interrupt: in this case receive ready*/
	 usart_enable_interrupt(USART_SERIAL,US_IER_RXRDY);
	 /* turning usart interrupt on (nested vector interrupt controller */
	 NVIC_EnableIRQ(USART_SERIAL_IRQn);
	 
	while (1)
	{
		#if USART_FUNCTION
		uint8_t received_byte = 0 ; 
		int a = sysclk_get_cpu_hz();
		char b [50]; 		
		sprintf(b , "%d\n" , a);  
		usart_write_line(USART_SERIAL, "AT+CGMR" );
		usart_serial_write_packet(USART_SERIAL , byte , 1); 
		delay_ms(1000);
		#endif 
		
		
		
		#if SARA_MAIN 
		
	    while(sara_functional_state == before_run){
			printDebug("inside before run \n"); 
			delay_ms(100); 
			turn_on_log = saraTurnOn();  
			if(turn_on_log == 0 || turn_on_log == 1){
				sara_functional_state = main_data_send; 
				one_time_run = 0; 
			}
			else 
				sara_functional_state = sara_error_handle; 
		}
		
		while(sara_functional_state == main_data_send){
			printDebug("inside main data send\n\r"); 
			delay_ms(2000); 
			
			//saraWriteCommand("AT+CGMR");  //firmware version 
			
			if(one_time_run==0){
				one_time_run = 1; 
				saraWriteCommand("AT+CMEE=2"); // setting error reporting method to verbose 
				
				#if FTP_SCENARIO
					saraWriteCommand("AT+UFTP?"); 
					saraWriteCommand("AT+UFTP=1,\"ftp.u-blox.com\"");
					saraWriteCommand("AT+UFTP=2,\"anonymous\"");
					saraWriteCommand("AT+UFTP=3,\"user@somedomain.com\"");
					saraWriteCommand("AT+UFTP=6,0");
					saraWriteCommand("AT+UDNSRN=0,\"ftp.u-blox.com\"");
					saraWriteCommand("AT+UFTPC=1");
					saraWriteCommand("AT+UFTPC=13");
				#endif
				
				#if TCP_SCENARIO					
					saraWriteCommand("AT+CGACT?"); 
					saraWriteCommand("AT+CGATT?"); 
					saraWriteCommand("AT+CEREG?");
					saraWriteCommand("AT+COPS?");
					delay_ms(2000); 
					saraWriteCommand("AT+USOCR=6");
					delay_ms(2000);  
					saraWriteCommand("AT+USOCO=0,\"195.34.89.241\",7"); 	
					delay_ms(2000); 
					saraWriteCommand("AT+USORD=0,32"); 	
					saraWriteCommand("AT+USOCL=0"); 			
				#endif
				
				#if UDP_SCENARIO
					saraWriteCommand("AT+CGACT?");
					saraWriteCommand("AT+CGATT?");
					saraWriteCommand("AT+COPS?");
					saraWriteCommand("AT+USOCR=17");
					saraWriteCommand("AT+USOST=0,\"195.34.89.241\",7,13,\"TestNumberOne\"");
					saraWriteCommand("AT+USORF=0,13"); 				
				#endif 
			}
			
				//saraWriteCommand("AT+CPIN?");   
				//saraWriteCommand("AT+URAT?");  // getting Radio Access Technology, for SARA is 7, 8 = LTE CAT M1/B
				//saraWriteCommand("AT+CFUN?");  // getting module functionality
				//saraWriteCommand("AT+COPS?");  // getting network registration mode: 0 for automatic 
				saraWriteCommand("AT+CEREG?"); 
				saraWriteCommand("AT+CGACT?"); 
				saraWriteCommand("AT+CGDCONT?"); 
				saraWriteCommand("AT+CGATT?"); 
				
			#if FTP_SCENARIO
				saraWriteCommand("AT+UFTP?");
				saraWriteCommand("AT+CGDCONT?");
				saraWriteCommand("AT+UFTPC?");
			#endif 
			
			#if TCP_SCENARIO
				
			#endif 
			
			
			if(sara_is_off()){		//handles unexpected turn off conditions 
				sara_functional_state = before_run; 
			}
		
		}
		
		while(sara_functional_state == sara_error_handle){
			printDebug("inside error"); 
			delay_ms(500); 			
		}
		//usart_write_line(CONF_UART , "trans"); 
		
		/* sample project of io init*/
		//ioport_toggle_port_level(LED_PORT, LED_MASK);
		//a = sysclk_get_cpu_hz(); 
		
		//usart_write_line(USART_SERIAL, "hello");
		
		/* put this part of USART receive into an interrupt service routine */		
		//usart_write_line(CONF_UART , "hello"); 		
		//usart_write_line(USART_SERIAL , "to arduino"); 			
		#endif
	
		#if LED_FUNCTION
		ioport_set_port_level(SARA_PWR_ON_PORT , SARA_PWR_ON_MASK , 0);
		int indic = ioport_get_pin_level(SARA_GPIO6); 
		char b [50] ; 
		sprintf(b , "%d" , indic); 
		printDebug(b);
		delay_ms(500);   
		/*if(one_time_run == 0){
			turn_on_log = saraTurnOn(); 
			one_time_run = 1; 	
		}
		
		char b [50]; 
		if(turn_on_log == 0) usart_write_line(CONF_UART , "success"); 
		else if (turn_on_log == 1) usart_write_line(CONF_UART , "already on");
		else usart_write_line(CONF_UART, "not on " );  
		*/
		#endif
		
		
		#if SPI_FUNCTION
		
		
		
		
		#endif 
		
		#if	TURN_ON_SARA_TEST
			int tog = 1; 
			ioport_set_pin_dir(SARA_GPIO6, IOPORT_DIR_INPUT);
			ioport_set_pin_mode(SARA_GPIO6, IOPORT_MODE_PULLUP);
			
			
			
			
			
			if (ioport_get_pin_level(SARA_GPIO6) == 0) {
				/* Yes, so turn LED on. */
				ioport_set_pin_level(PIN_PC07, 0);
				} else {
				/* No, so turn LED off. */
				ioport_set_pin_level(PIN_PC07, 1);
			}
		#endif 
		
		
	}

}



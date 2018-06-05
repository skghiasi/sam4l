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

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init(); 
	board_init();
	ioport_init(); 

	/* Insert application code here, after the board has been initialized. */
	/* a small demo of the yellow LED on the Xplain board, including the ioport driver from asf */ 
	ioport_set_port_dir(LED_PORT , LED_MASK ,	IOPORT_DIR_OUTPUT); 
	/*enable uart0 */ 
	/*enable uart1 */ 
	
	while (1)
	{
		/* sample project of io init*/
		ioport_toggle_port_level(LED_PORT, LED_MASK);
		delay_ms(500); 		
				
	}

}



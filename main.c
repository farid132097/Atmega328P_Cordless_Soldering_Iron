

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main(void){
  
  UART_Init(38400);
  
  while(1){
    
	UART_Transmit_Text((char*)"Hello\r\n");
	_delay_ms(1000);
	
	}
}

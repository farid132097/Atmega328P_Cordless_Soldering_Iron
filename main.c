

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "uart.h"

int main(void){
  
  Timebase_Init(100);
  UART_Init(38400);
  
  while(1){
    
	UART_Transmit_Number(Timebase_Timer_Get_SubSeconds());
	UART_Transmit_Text((char*)"\r\n");
	_delay_ms(10);
	
	}
}

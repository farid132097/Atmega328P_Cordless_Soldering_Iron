

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "uart.h"
#include "adc.h"
#include "pwm.h"

int main(void){
  
  Timebase_Init(100);
  UART_Init(38400);
  ADC_Init();
  PWM_Init();
  
  uint32_t last_val=0;
  
  while(1){
    
	while(Timebase_Timer_Get_SubSeconds() == last_val);
	last_val=Timebase_Timer_Get_SubSeconds();
	
	UART_Transmit_Number(ADC_Read_Temperature(80));
	UART_Transmit_Text((char*)"\r\n");
	_delay_ms(10);
	
	}
}

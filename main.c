

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
  uint16_t temp=0;
  uint16_t volt=0;
  
  while(1){
    
	//while(Timebase_Timer_Get_SubSeconds() == last_val);
	//last_val=Timebase_Timer_Get_SubSeconds();
	
	if( (UART_Last_Received_Byte()=='o') && (temp<260) ){
	  Iron_Heater_On(500);
	}
	_delay_ms(20);
	uint16_t curr = ADC_Read_Current();
	Iron_Heater_Off();
	_delay_ms(5);
	
	temp = ADC_Read_Temperature(80);
	volt = ADC_Read_Input_Voltage();
	
	
	UART_Transmit_Number(volt);
	UART_Transmit_Text((char*)",");
	UART_Transmit_Number(curr);
	UART_Transmit_Text((char*)",");
	UART_Transmit_Number(temp);
	UART_Transmit_Text((char*)"\r\n");
	//_delay_ms(50);
	
	}
}

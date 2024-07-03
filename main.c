

#include <avr/io.h>
#include <util/delay.h>
#include "timebase.h"
#include "debug.h"
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "lpf.h"
#include "pid.h"

int main(void){
  
  Timebase_Init(50);
  PWM_Init();
  ADC_Init();
  PID_Init();
  Debug_Init();
  Timebase_Timer_Await_SubSeconds(2);
  Iron_Set_Max_PWM_Div_Fact(4);
  
  while(1){
    
	Timebase_Timer_Await_SubSeconds(5);
    Iron_Heater_Off();
	_delay_ms(5);
	ADC_Read_Temperature();
	Iron_Heater_On(250);
	
	/*ADC_Set_Temperature_Factor(80);       //update tempfact
	PID_Set_Constants(800, 20, 20);       //update const
	PID_Set_Max_IError(250);              //update ierror
	ADC_Set_Max_Power(36000);             //update power mW
	
	PID_Set_SetPoint(200);
	PID_Set_ProcessValue(ADC_Get_Temperature());
	PID_Execute_Routine();
	Iron_Heater_Control();*/
	
	_delay_ms(3);
	
	//Debug_Print_UpTime();
    Debug_Print_PID_SetPoint();
    Debug_Print_Current_Temperature();
	//Debug_Print_PWM();
	//Debug_Print_PID_PError();
	//Debug_Print_PID_IError();
	//Debug_Print_PID_DError();
	//Debug_Print_PID_PError_Product();
	//Debug_Print_PID_IError_Product();
	//Debug_Print_PID_DError_Product();
	
	//Debug_Print_Max_PWM();
	//Debug_Print_Vin();
	//Debug_Print_Current_Draw();
	Debug_Print_Power_Draw();
	//Debug_Print_Power_Bar_Value();
	
	//Debug_Print_Display_Temperature();
	UART_Transmit_New_Line();
	
  }
}

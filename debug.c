
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "timebase.h"
#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "pid.h"

#define  DEBUG_BAUD_RATE 57600
//#define  DEBUG_VERBOSE_OUTPUT
#define  DEBUG_COMMA_SEPERATED
//#define  DEBUG_SPACE_SEPERATED

void Debug_Init(void){
  UART_Init(57600);
}

void Debug_Print_Seperation(void){
  #ifdef DEBUG_COMMA_SEPERATED
    UART_Transmit_Text(",");
  #elif DEBUG_SPACE_SEPERATED
    UART_Transmit_Text(" ");
  #endif
}

void Debug_Print_UpTime(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("UpTime ");
  #endif
  UART_Transmit_Number(Timebase_Timer_Get_Seconds());
  Debug_Print_Seperation();
}

void Debug_Print_SubSeconds(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("SubSeconds ");
  #endif
  UART_Transmit_Number(Timebase_Timer_Get_SubSeconds());
  Debug_Print_Seperation();
}

void Debug_Print_PID_Kp(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("Kp ");
  #endif
  UART_Transmit_Number(PID_Get_Kp());
  Debug_Print_Seperation();
}

void Debug_Print_PID_Ki(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("Ki ");
  #endif
  UART_Transmit_Number(PID_Get_Ki());
  Debug_Print_Seperation();
}

void Debug_Print_PID_Kd(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("Kd ");
  #endif
  UART_Transmit_Number(PID_Get_Kd());
  Debug_Print_Seperation();
}

void Debug_Print_PID_SetPoint(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID SP ");
  #endif
  UART_Transmit_Number(PID_Get_Set_Point());
  Debug_Print_Seperation();
}

void Debug_Print_PID_Process_Value(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID PV ");
  #endif
  UART_Transmit_Number(PID_Get_Process_Value());
  Debug_Print_Seperation();
}

void Debug_Print_PID_PError(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID PError ");
  #endif
  UART_Transmit_Number(PID_Get_PError());
  Debug_Print_Seperation();
}

void Debug_Print_PID_IError(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID IError ");
  #endif
  UART_Transmit_Number(PID_Get_IError());
  Debug_Print_Seperation();
}

void Debug_Print_PID_DError(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID DError ");
  #endif
  UART_Transmit_Number(PID_Get_DError());
  Debug_Print_Seperation();
}

void Debug_Print_PID_Max_IError(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID Max IError ");
  #endif
  UART_Transmit_Number(PID_Get_Max_IError());
  Debug_Print_Seperation();
}


void Debug_Print_PID_PError_Product(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID PError Product ");
  #endif
  UART_Transmit_Number(PID_Get_PError_Product());
  Debug_Print_Seperation();
}

void Debug_Print_PID_IError_Product(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID IError Product ");
  #endif
  UART_Transmit_Number(PID_Get_IError_Product());
  Debug_Print_Seperation();
}

void Debug_Print_PID_DError_Product(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID DError Product ");
  #endif
  UART_Transmit_Number(PID_Get_DError_Product());
  Debug_Print_Seperation();
}









void Debug_Print_PID_Output(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID Output ");
  #endif
  UART_Transmit_Number(PID_Get_Output());
  Debug_Print_Seperation();
}

void Debug_Print_PID_Output_NonNegative(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PID OutputNN ");
  #endif
  UART_Transmit_Number(PID_Get_Output_NonNegative());
  Debug_Print_Seperation();
}


void Debug_Print_PWM(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PWM ");
  #endif
  UART_Transmit_Number(Iron_Get_PWM());
  Debug_Print_Seperation();
}

void Debug_Print_Max_PWM(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PWM ");
  #endif
  UART_Transmit_Number(Iron_Get_Max_PWM());
  Debug_Print_Seperation();
}

void Debug_Print_Vin(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("Vin ");
  #endif
  UART_Transmit_Number(ADC_Get_Input_Voltage());
  Debug_Print_Seperation();
}


void Debug_Print_Current_Draw(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("Current ");
  #endif
  UART_Transmit_Number(ADC_Get_Current());
  Debug_Print_Seperation();
}

void Debug_Print_Power_Draw(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("Power ");
  #endif
  UART_Transmit_Number(ADC_Get_Power_Draw());
  Debug_Print_Seperation();
}

void Debug_Print_Power_Bar_Value(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("PowerBar ");
  #endif
  UART_Transmit_Number(ADC_Get_Power_Bar());
  Debug_Print_Seperation();
}

void Debug_Print_Current_Temperature(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("Temp ");
  #endif
  UART_Transmit_Number(ADC_Get_Temperature());
  Debug_Print_Seperation();
}

void Debug_Print_Display_Temperature(void){
  #ifdef DEBUG_VERBOSE_OUTPUT
    UART_Transmit_Text("DispTemp ");
  #endif
  UART_Transmit_Number(ADC_Get_Display_Temperature());
  Debug_Print_Seperation();
}


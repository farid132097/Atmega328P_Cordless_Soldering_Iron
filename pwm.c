

#include <avr/io.h>
#include "pwm.h"
#include "parameters.h"
#include "adc.h"
#include "pid.h"



typedef struct {
  uint16_t TopValue;
  uint16_t MaxValue;
  uint16_t CurrentValue;
}PWM_t;


typedef struct heater_t{
  uint8_t  Status;
  PWM_t    PWM;
}heater_t;

heater_t HEATER_type;
heater_t *HEATER;

void PWM_Struct_Init(void){
  HEATER = &HEATER_type;
  HEATER->PWM.MaxValue = PWM_MAX_VAL;
  HEATER->PWM.TopValue = ICR1_TOP_VAL;
  HEATER->PWM.CurrentValue = OCR1_INIT_VAL;
  HEATER->Status = 0;
}
  
void PWM_Reg_Init(void){
  DDRB   |= (1<<1);
  PORTB  &=~(1<<1);
  TCCR1A  = (1<<COM1A1)|(1<<WGM11);
  TCCR1B  = (1<<CS10)|(1<<WGM12)|(1<<WGM13);
  ICR1    = ICR1_TOP_VAL;
  OCR1A   = OCR1_INIT_VAL;
  TCCR1A &=~(1<<COM1A1);
}


void Iron_Heater_On(uint16_t val){
  if( (HEATER->Status == 0) && (val > 0) ){
    if(val > HEATER->PWM.MaxValue){
	  val = HEATER->PWM.MaxValue;
	}
    OCR1A   = val;
    TCCR1A |= (1<<COM1A1);
	HEATER->PWM.CurrentValue = val;
	HEATER->Status = 1;
  }
}


void Iron_Heater_Off(void){
  if(HEATER->Status == 1){
    TCCR1A &=~(1<<COM1A1);
	OCR1A   = 0;
	HEATER->PWM.CurrentValue = 0;
	HEATER->Status = 0;
  }
}


void Iron_Heater_Control(void){
  if(ADC_Get_Temperature()>ABSOLUTE_MAX_TEMP){
    Iron_Heater_Off();
  }else{
    Iron_Heater_On(PID_Get_Output_NonNegative());
  }
}



void Iron_Set_Max_PWM(uint16_t val){
  HEATER->PWM.MaxValue = val;
}

void Iron_Set_Max_PWM_Div_Fact(uint16_t val){
  HEATER->PWM.MaxValue = PWM_MAX_VAL/val;
}

void Iron_Set_Max_PWM_Full(void){
  HEATER->PWM.MaxValue = PWM_MAX_VAL;
}

void Iron_Set_Max_PWM_Half(void){
  HEATER->PWM.MaxValue = PWM_MAX_VAL/2;
}

void Iron_Set_Max_PWM_Quarter(void){
  HEATER->PWM.MaxValue = PWM_MAX_VAL/4;
}

void Iron_Set_Max_PWM_One_Eighth(void){
  HEATER->PWM.MaxValue = PWM_MAX_VAL/8;
}

uint16_t Iron_Get_PWM(void){
  return HEATER->PWM.CurrentValue;
}


uint16_t Iron_Get_Max_PWM(void){
  return HEATER->PWM.MaxValue;
}



void PWM_Init(void){
  PWM_Struct_Init();
  PWM_Reg_Init();
}
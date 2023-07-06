

#include <avr/io.h>

/*PWM Parameters*/
#define ICR1_TOP_VAL         600
#define PWM_MAX_VAL          ICR1_TOP_VAL-1
#define OCR1_INIT_VAL        1


typedef struct heater_t{
  uint8_t  Status;
  uint16_t PwmTopValue;
  uint16_t PwmCurrentValue;
  uint16_t RawPWMInput;
}heater_t;

heater_t heater;


void Iron_Heater_On(uint16_t val){
  if((heater.Status==0) && (val>0)){
    heater.RawPWMInput=val;
    if(val>PWM_MAX_VAL){val=PWM_MAX_VAL;}
    OCR1A=val;
    TCCR1A|=(1<<COM1A1);
	heater.PwmCurrentValue=val;
	heater.Status=1;
  }
}



void Iron_Heater_Off(void){
  if(heater.Status==1){
    OCR1A=1;
	heater.PwmCurrentValue=0;
	heater.Status=0;
    TCCR1A&=~(1<<COM1A1);
  }
}

uint16_t Get_Raw_PWM_Input(void){
  return heater.RawPWMInput;
}

uint16_t Get_Current_PWM(void){
  return heater.PwmCurrentValue;
}


void PWM_Init(void){
  DDRB|=(1<<1);
  PORTB&=~(1<<1);
  TCCR1A=(1<<COM1A1)|(1<<WGM11);
  TCCR1B=(1<<CS10)|(1<<WGM12)|(1<<WGM13);
  ICR1=ICR1_TOP_VAL;
  OCR1A=OCR1_INIT_VAL;
  TCCR1A&=~(1<<COM1A1);
  heater.PwmTopValue=ICR1_TOP_VAL;
  heater.RawPWMInput=0;
  Iron_Heater_Off();
}

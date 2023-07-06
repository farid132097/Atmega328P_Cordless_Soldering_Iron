

#include <avr/io.h>


#define IIR_BUTTERWORTH_TEMP_ALPHA      60
#define IIR_BUTTERWORTH_CURR_ALPHA      30
#define IIR_BUTTERWORTH_VOLT_ALPHA      70


#define ADC_IVREF      1110
#define ADC_VSNS_VDIV  130
#define ADC_CHANNEL_T  0
#define ADC_CHANNEL_I  1
#define ADC_CHANNEL_V  2
#define ADC_SMPL_DUMMY 4
#define ADC_SMPL_T     64
#define ADC_SMPL_I     16
#define ADC_SMPL_V     16
#define SENSE_RES_I    50

typedef struct iir_t{
  uint8_t   Alpha;
  int32_t   Input;
  int32_t   Output;
}iir_t;


iir_t   IIR[3];

void IIR_Butterworth_Init(void){
  for(uint8_t i=0;i<3;i++){
    IIR[i].Alpha=0;
    IIR[i].Input=0;
    IIR[i].Output=0;
  }
  #ifdef IIR_BUTTERWORTH_TEMP_ALPHA
  IIR[0].Alpha=IIR_BUTTERWORTH_TEMP_ALPHA;
  #endif
  
  #ifdef IIR_BUTTERWORTH_CURR_ALPHA
  IIR[1].Alpha=IIR_BUTTERWORTH_CURR_ALPHA;
  #endif
  
  #ifdef IIR_BUTTERWORTH_VOLT_ALPHA
  IIR[2].Alpha=IIR_BUTTERWORTH_VOLT_ALPHA;
  #endif
}

int32_t IIR_Butterworth(uint8_t channel, int32_t input){
  IIR[channel].Input=input;
  IIR[channel].Output =(IIR[channel].Input*IIR[channel].Alpha)+((100-IIR[channel].Alpha)*IIR[channel].Output);
  IIR[channel].Output/= 100;
  return IIR[channel].Output;
}

void ADC_Init(void){
  ADMUX  =(1<<REFS0)|(1<<REFS1);
  ADCSRA =(1<<ADPS1)|(1<<ADPS2);
  ADCSRA|=(1<<ADEN);
  IIR_Butterworth_Init();
}

uint16_t ADC_Read(uint8_t channel){
  uint8_t admux=(0xC0|channel);
  if(ADMUX!=admux){ADMUX=admux;}
  ADCSRA|=(1<<ADSC);
  while(!(ADCSRA &(1<<ADIF)));
  ADCSRA|=(1<<ADIF);
  return ADCW;
}


uint16_t ADC_Read_Temperature(uint8_t tempearure_factor){
  uint32_t sum=0;
  for(uint8_t i=0;i<ADC_SMPL_DUMMY;i++){
    uint16_t dummy=ADC_Read(ADC_CHANNEL_T);
	dummy=0xFF;
  }
  
  for(uint16_t i=0;i<ADC_SMPL_T;i++){
    sum+=ADC_Read(ADC_CHANNEL_T);
  }
  
  sum/=ADC_SMPL_T;
  sum*=tempearure_factor;
  sum/=100;
  if(sum<0){sum=0;}
  #ifdef IIR_BUTTERWORTH_TEMP_ALPHA
  sum=IIR_Butterworth(0,sum);
  #endif
  return sum;
}

uint16_t ADC_Read_Current(void){
  uint32_t sum=0;
  for(uint8_t i=0;i<ADC_SMPL_DUMMY;i++){
    uint16_t dummy=ADC_Read(ADC_CHANNEL_I);
	dummy=0xFF;
  }
  for(uint8_t i=0;i<ADC_SMPL_I;i++){
    sum+=ADC_Read(ADC_CHANNEL_I);
  }
  sum/=ADC_SMPL_I;
  sum*=1000;
  sum/=SENSE_RES_I;
  #ifdef IIR_BUTTERWORTH_CURR_ALPHA
  sum=IIR_Butterworth(1,sum);
  #endif
  return sum;
}

uint8_t ADC_Read_Input_Voltage(void){
  uint32_t sum=0;
  for(uint8_t i=0;i<ADC_SMPL_DUMMY;i++){
    uint16_t dummy=ADC_Read(ADC_CHANNEL_V);
	dummy=0xFF;
  }
  for(uint8_t i=0;i<ADC_SMPL_V;i++){
    sum+=ADC_Read(ADC_CHANNEL_V);
  }
  sum/=ADC_SMPL_V;
  sum*=ADC_IVREF;
  sum/=1024;
  sum*=ADC_VSNS_VDIV;
  sum/=1000;
  #ifdef IIR_BUTTERWORTH_VOLT_ALPHA
  sum=IIR_Butterworth(2,sum);
  #endif
  return (uint8_t)sum;
}


uint16_t ADC_Read_Power_Draw(uint8_t input_voltage){ 
  uint32_t curr=ADC_Read_Current();
  uint32_t pwr= (input_voltage*curr)/1000;
  return (uint16_t)pwr;
}

uint8_t Calculate_Power_Bar(uint16_t max_power, uint16_t power_draw){
  uint32_t bar_val=power_draw*100;
  bar_val=bar_val/max_power;
  return (uint8_t)bar_val;
}
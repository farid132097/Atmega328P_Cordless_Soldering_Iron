

#include <avr/io.h>
#include "lpf.h"

/*ADC Parameters Select*/
#define MCU_SUPPLY_VOLTAGE_VDD            3300   //mV
#define ADC_INTERNAL_VOLTAGE_REFERENCE    1100   //mV
#define VOLTAGE_ADC_CHNL_VOLTAGE_DIVIDER  310    //Factor x10
#define TEMPERATURE_SENSE_ADC_CHANNEL     0      //ADC0 
#define CURRENT_SENSE_ADC_CHANNEL         1      //ADC1
#define VOLTAGE_SENSE_ADC_CHANNEL         2      //ADC2
#define IRON_ID_SENSE_ADC_CHANNEL         5      //ADC5
#define DISCARD_ADC_SAMPLE                4      //4 samples
#define TEMPERATURE_SENSE_ADC_SAMPLE      128    //128 samples
#define CURRENT_SESNSE_ADC_SAMPLE         16     //16 samples
#define VOLTAGE_SENSE_ADC_SAMPLE          16     //16 samples
#define IRON_ID_SENSE_ADC_SAMPLE          16     //16 samples
#define IRON_ID_SENSE_PULL_UP_RESISTOR    10000  //Ohm
#define IRON_ID_SENSE_PULL_DOWN_RESISTOR  4700   //Ohm
#define CURRENT_SESNSE_ADC_SHUNT_RESISTOR 100    //mOhm


#define TEMPERATURE_SENSE_LPF_ALPHA       90
#define CURRENT_SENSE_LPF_ALPHA           70
#define VOLTAGE_SENSE_LPF_ALPHA           90
//#define IRON_ID_SENSE_LPF_ALPHA           90
#define DISP_TEMPERATURE_LPF_ALPHA        50

typedef struct calibration_t{
  uint16_t Vdd;
  uint16_t VrefInt;
  uint16_t VdivFact;
  uint16_t TempFact;
  uint16_t IDSensePUR;
  uint16_t IDSensePDR;
  uint16_t IDSenseShunt;
  uint32_t MaxPower;
}calibration_t;

typedef struct channel_t{
  uint8_t TSense;
  uint8_t VSense;
  uint8_t ISense;
  uint8_t IronID;
}channel_t;

typedef struct sample_t{
  uint8_t Dummy;
  uint8_t TSense;
  uint8_t VSense;
  uint8_t ISense;
  uint8_t IronID;
}sample_t;

typedef struct adc_result_t{
  uint32_t Temporary;
  uint16_t TSense;
  uint16_t VSense;
  uint16_t ISense;
  uint32_t PSense;
  uint16_t IronID;
  uint16_t PowBar;
  uint16_t DispTemp;
}adc_result_t;


typedef struct ADC_t{
  calibration_t Calibration;
  channel_t     Channels;
  sample_t      NSamples;
  adc_result_t  Result;
}ADC_t;


ADC_t ADC_type;
ADC_t *ADCC;

void ADC_Struct_Init(void){
  ADCC=&ADC_type;
  ADCC->Calibration.Vdd=MCU_SUPPLY_VOLTAGE_VDD;
  ADCC->Calibration.VdivFact=VOLTAGE_ADC_CHNL_VOLTAGE_DIVIDER;
  ADCC->Calibration.TempFact=80;
  ADCC->Calibration.VrefInt=ADC_INTERNAL_VOLTAGE_REFERENCE;
  ADCC->Calibration.IDSensePUR=IRON_ID_SENSE_PULL_UP_RESISTOR;
  ADCC->Calibration.IDSensePDR=IRON_ID_SENSE_PULL_DOWN_RESISTOR;
  ADCC->Calibration.IDSenseShunt=CURRENT_SESNSE_ADC_SHUNT_RESISTOR;
  ADCC->Calibration.MaxPower=360;
  ADCC->Channels.TSense=TEMPERATURE_SENSE_ADC_CHANNEL;
  ADCC->Channels.VSense=VOLTAGE_SENSE_ADC_CHANNEL;
  ADCC->Channels.ISense=CURRENT_SENSE_ADC_CHANNEL;
  ADCC->Channels.IronID=IRON_ID_SENSE_ADC_CHANNEL;
  ADCC->NSamples.Dummy=DISCARD_ADC_SAMPLE;
  ADCC->NSamples.TSense=TEMPERATURE_SENSE_ADC_SAMPLE;
  ADCC->NSamples.VSense=VOLTAGE_SENSE_ADC_SAMPLE;
  ADCC->NSamples.ISense=CURRENT_SESNSE_ADC_SAMPLE;
  ADCC->NSamples.IronID=IRON_ID_SENSE_ADC_SAMPLE;
  ADCC->Result.Temporary=0;
  ADCC->Result.TSense=0;
  ADCC->Result.VSense=0;
  ADCC->Result.ISense=0;
  ADCC->Result.IronID=0;
  ADCC->Result.PSense=0;
  ADCC->Result.PowBar=0;
  ADCC->Result.DispTemp=0;
}

void ADC_Init(void){
  ADC_Struct_Init();
  ADMUX  =(1<<REFS0)|(1<<REFS1);
  ADCSRA =(1<<ADPS1)|(1<<ADPS2);
  ADCSRA|=(1<<ADEN);
  LPF_Init();
  
  #ifdef TEMPERATURE_SENSE_LPF_ALPHA
    LPF_Set_Alpha(0, TEMPERATURE_SENSE_LPF_ALPHA);
  #endif
  
  #ifdef VOLTAGE_SENSE_LPF_ALPHA
    LPF_Set_Alpha(1, VOLTAGE_SENSE_LPF_ALPHA);
  #endif
  
  #ifdef CURRENT_SENSE_LPF_ALPHA
    LPF_Set_Alpha(2, CURRENT_SENSE_LPF_ALPHA);
  #endif
  
  #ifdef DISP_TEMPERATURE_LPF_ALPHA
    LPF_Set_Alpha(3, DISP_TEMPERATURE_LPF_ALPHA);
  #endif
}

uint16_t ADC_Read(uint8_t channel){
  uint8_t admux=(0xC0|channel);
  if(ADMUX!=admux){ADMUX=admux;}
  ADCSRA|=(1<<ADSC);
  while(!(ADCSRA &(1<<ADIF)));
  ADCSRA|=(1<<ADIF);
  return ADCW;
}

void ADC_Read_Chennel_Average(uint8_t channel, uint8_t nsamples){
  uint32_t sum=0;
  for(uint8_t i=0;i<ADCC->NSamples.Dummy;i++){
    uint16_t dummy=ADC_Read(channel);
    dummy=0xFF;
  }
  for(uint8_t i=0;i<nsamples;i++){
    sum+=ADC_Read(channel);
  }
  sum/=nsamples;
  ADCC->Result.Temporary=sum;
}

void ADC_Read_Temperature(void){
  ADC_Read_Chennel_Average(ADCC->Channels.TSense, ADCC->NSamples.TSense);
  ADCC->Result.Temporary*=ADCC->Calibration.TempFact;
  ADCC->Result.Temporary/=100;
  if(ADCC->Result.Temporary<0){ADCC->Result.Temporary=0;}
  ADCC->Result.TSense=(uint16_t)ADCC->Result.Temporary;
  #ifdef TEMPERATURE_SENSE_LPF_ALPHA
    ADCC->Result.TSense=(uint16_t)LPF_Get_Filtered_Value(0, ADCC->Result.TSense);
  #endif
  
  #ifdef DISP_TEMPERATURE_LPF_ALPHA
    ADCC->Result.DispTemp=(uint16_t)LPF_Get_Filtered_Value(3, ADCC->Result.TSense);
  #endif
}

void ADC_Read_Input_Voltage(void){ // Result in mV
  ADC_Read_Chennel_Average(ADCC->Channels.VSense, ADCC->NSamples.VSense);
  ADCC->Result.Temporary*=ADCC->Calibration.VrefInt;
  ADCC->Result.Temporary/=1024;    // mV
  ADCC->Result.Temporary*=ADCC->Calibration.VdivFact;
  ADCC->Result.Temporary/=10;      // mV*voltage_divider_factorx10/10=Input_mV
  ADCC->Result.VSense=(uint16_t)ADCC->Result.Temporary;
  #ifdef VOLTAGE_SENSE_LPF_ALPHA
    ADCC->Result.VSense=(uint16_t)LPF_Get_Filtered_Value(1, ADCC->Result.VSense);
  #endif
}

void ADC_Read_Current(void){       // Result in mA
  ADC_Read_Chennel_Average(ADCC->Channels.ISense, ADCC->NSamples.ISense);
  ADCC->Result.Temporary*=ADCC->Calibration.VrefInt;
  ADCC->Result.Temporary/=1024;    // mV
  ADCC->Result.Temporary*=1000;    // uV
  ADCC->Result.Temporary/=ADCC->Calibration.IDSenseShunt; // uV/mOhm=mA
  ADCC->Result.ISense=(uint16_t)ADCC->Result.Temporary;
  #ifdef CURRENT_SENSE_LPF_ALPHA
    ADCC->Result.ISense=(uint16_t)LPF_Get_Filtered_Value(2, ADCC->Result.ISense);
  #endif
}

void ADC_Read_Iron_ID_Resistor(void){ // Result in Ohms
  uint32_t iron_id_channel_voltage_div_output_mv=0,R_equivalent=0;
  ADC_Read_Chennel_Average(ADCC->Channels.IronID, ADCC->NSamples.IronID);
  ADCC->Result.Temporary*=ADCC->Calibration.VrefInt;
  ADCC->Result.Temporary/=1024;    // mV
  iron_id_channel_voltage_div_output_mv=ADCC->Result.Temporary;
  ADCC->Result.Temporary*=ADCC->Calibration.IDSensePUR; // Vout*Pull_up_resistor
  ADCC->Result.Temporary/=(ADCC->Calibration.Vdd-iron_id_channel_voltage_div_output_mv); 
  // Req=Vout*Pull_up_resistor/(Vin-Vout)
  R_equivalent=ADCC->Result.Temporary;
  ADCC->Result.Temporary*=ADCC->Calibration.IDSensePDR; // Req*Pull_down_resistor
  ADCC->Result.Temporary/=(ADCC->Calibration.IDSensePDR-R_equivalent);
  // Rsense=Req*Pull_down_resistor/ (Pull_down_resistor-Req)
  ADCC->Result.IronID=(uint16_t)ADCC->Result.Temporary;
}

void ADC_Read_Power_Draw(void){         // Result in mW 
  uint32_t vin=ADCC->Result.VSense;
  vin/=100;                             // mV/100
  uint32_t curr=ADCC->Result.ISense/10; // mA/10
  curr*=vin;
  ADCC->Result.PSense=curr;
}

void ADC_Calculate_Power_Bar(void){
  uint32_t bar_val=ADCC->Result.PSense*100; //max 100%
  bar_val/=ADCC->Calibration.MaxPower;
  ADCC->Result.PowBar=(uint16_t)bar_val;
  if(ADCC->Result.PowBar>100){
    ADCC->Result.PowBar=100;
  }
}

void ADC_Set_Max_Power(uint32_t power){
  ADCC->Calibration.MaxPower=power;
}

void ADC_Set_Temperature_Factor(uint16_t factor){
  ADCC->Calibration.TempFact=factor;
}

uint16_t ADC_Get_Temperature(void){
  return ADCC->Result.TSense;
}

uint16_t ADC_Get_Input_Voltage(void){
  return ADCC->Result.VSense;
}

uint16_t ADC_Get_Current(void){
  return ADCC->Result.ISense;
}

uint16_t ADC_Get_Iron_ID_Resistor(void){
  return ADCC->Result.IronID;
}

uint16_t ADC_Get_Power_Draw(void){
  return ADCC->Result.PSense;
}

uint16_t ADC_Get_Power_Bar(void){
  return ADCC->Result.PowBar;
}

uint16_t ADC_Get_Display_Temperature(void){
  return ADCC->Result.DispTemp;
}
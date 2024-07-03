



void     ADC_Struct_Init(void);
void     ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
void     ADC_Read_Chennel_Average(uint8_t channel, uint8_t nsamples);
void     ADC_Read_Temperature(void);
void     ADC_Read_Input_Voltage(void);
void     ADC_Read_Current(void);
void     ADC_Read_Iron_ID_Resistor(void);
void     ADC_Read_Power_Draw(void);
void     ADC_Calculate_Power_Bar(void);
void     ADC_Set_Max_Power(uint32_t power);
void     ADC_Set_Temperature_Factor(uint16_t factor);
uint16_t ADC_Get_Temperature(void);
uint16_t ADC_Get_Input_Voltage(void);
uint16_t ADC_Get_Current(void);
uint16_t ADC_Get_Iron_ID_Resistor(void);
uint16_t ADC_Get_Power_Draw(void);
uint16_t ADC_Get_Power_Bar(void);
uint16_t ADC_Get_Display_Temperature(void);
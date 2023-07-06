


void     IIR_Butterworth_Init(void);
int32_t  IIR_Butterworth(uint8_t channel, int32_t input);
void     ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
uint16_t ADC_Read_Temperature(uint8_t tempearure_factor);
uint16_t ADC_Read_Current(void);
uint8_t  ADC_Read_Input_Voltage(void);
uint16_t ADC_Read_Power_Draw(uint8_t input_voltage);
uint8_t  Calculate_Power_Bar(uint16_t max_power, uint16_t power_draw);


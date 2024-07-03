

/*PWM Parameters*/
#define  ICR1_TOP_VAL         600
#define  PWM_MAX_VAL          (ICR1_TOP_VAL-1)
#define  OCR1_INIT_VAL        0

void     PWM_Struct_Init(void);
void     PWM_Reg_Init(void);
void     Iron_Heater_On(uint16_t val);
void     Iron_Heater_Off(void);
void     Iron_Heater_Control(void);

void     Iron_Set_Max_PWM(uint16_t val);
void     Iron_Set_Max_PWM_Div_Fact(uint16_t val);

void     Iron_Set_Max_PWM_Full(void);
void     Iron_Set_Max_PWM_Half(void);
void     Iron_Set_Max_PWM_Quarter(void);
void     Iron_Set_Max_PWM_One_Eighth(void);

uint16_t Iron_Get_PWM(void);
uint16_t Iron_Get_Max_PWM(void);
void     PWM_Init(void);
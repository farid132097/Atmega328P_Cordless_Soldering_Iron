
void        PID_Struct_Init(void);
void        PID_Set_Kp(signed long kp);
void        PID_Set_Ki(signed long ki);
void        PID_Set_Kd(signed long kd);
void        PID_Set_Constants(signed long kp, signed long ki, signed long kd);
void        PID_Set_SetPoint(signed long set_point);
void        PID_Set_ProcessValue(signed long process_value);
void        PID_Set_Max_IError(signed long max_ierror);

void        PID_Reset_IError(void);
void        PID_Calculate_Error(void);
void        PID_Calculate_Error_Products(void);
void        PID_Execute_Routine(void);

signed long PID_Get_Kp(void);
signed long PID_Get_Ki(void);
signed long PID_Get_Kd(void);

signed long PID_Get_Set_Point(void);
signed long PID_Get_Process_Value(void);

signed long PID_Get_PError(void);
signed long PID_Get_IError(void);
signed long PID_Get_DError(void);
signed long PID_Get_Max_IError(void);

signed long PID_Get_PError_Product(void);
signed long PID_Get_IError_Product(void);
signed long PID_Get_DError_Product(void);

signed long PID_Get_Output(void);
signed long PID_Get_Output_NonNegative(void);

void        PID_Init(void);
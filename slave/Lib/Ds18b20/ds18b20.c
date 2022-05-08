#include <stdio.h>
#include <ds18b20.h>

void TIM6_delay (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim6))<time);
}

/* to initialize the sensor */
uint8_t DS18B20_Start (void)
{
	OS_ERR os_err;
	uint8_t Response = 0;
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);   // set the pin as output
	HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin low
	TIM6_delay(480);   // delay according to datasheet

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);    // set the pin as input
	TIM6_delay(80);    // delay according to datasheet

	if (!(HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = -1;

	TIM6_delay(400); // 480 us delay totally.

	return Response;
}

void DS18B20_Write (uint8_t data)
{
	OS_ERR os_err;
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			TIM6_delay (1);  // wait for 1 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // set as input
			TIM6_delay (60);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			TIM6_delay (60);  // wait for 60 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
		}
	}
}
// 12bits coreresponding to change 0.0625 celsius degree 
uint8_t DS18B20_Read (void)
{
	uint8_t value=0;
	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);

	for (int i=0;i<8;i++)
	{
		Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);   // set as output

		HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the data pin LOW
		TIM6_delay (2);  // wait for > 1us

		Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // set as input
		if (HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		TIM6_delay (60);  // wait for 60 us
	}
	return value;
}

uint16_t DS18B20_Operation(void){
	OS_ERR os_err;
    uint8_t Temp_byte1, Temp_byte2;
    uint16_t TEMP;
    uint8_t Presence = 0;
        Presence = DS18B20_Start ();
            OSTimeDlyHMSM(0, 0, 0, 1, OS_OPT_TIME_HMSM_STRICT, &os_err);
        DS18B20_Write (0xCC);  // skip ROM
        DS18B20_Write (0x44);  // convert t
            OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);

        Presence = DS18B20_Start ();
            OSTimeDlyHMSM(0, 0, 0, 1, OS_OPT_TIME_HMSM_STRICT, &os_err);
        DS18B20_Write (0xCC);  // skip ROM
        DS18B20_Write (0xBE);  // Read Scratch-pad

        Temp_byte1 = DS18B20_Read();
        Temp_byte2 = DS18B20_Read();
        TEMP = (Temp_byte2<<8)|Temp_byte1;
        return TEMP;

}
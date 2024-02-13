#include "ultrasonic.h"
#include "i2c_lcd.h"

extern volatile int TIM10_10ms_ultrasonic;
extern uint8_t lcd_display_mode_flag;

extern void lcd_string(uint8_t *str);
extern void move_cursor(uint8_t row, uint8_t column);

volatile int ultra_on;

void ultrasonic_processing(void);

//1.Drivers/STM32F4XX_hal_Driver/ Src/stm32f4xx_hal_timec
//2.초음파 센서의 ECHO판의 상승 에지와 하강에지 발생시 이곳으로 들어온다.

volatile uint8_t is_first_capture = 0; //0:상승에지  1.하강에지
volatile int distance; // 거리를 측정한 철스 갯수를 저장하는 변수
volatile int ic_cpt_finish_flag = 0; // 총음파 거리 측정 완료 indicater
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance== TIM3)
	{
		if(is_first_capture ==0)  // 상승에지
		{
			__HAL_TIM_SET_COUNTER(htim,0); // clear H/W counter
			is_first_capture = 1; // 상승에지를 만났다는 flag변수 indicator를 set
		}
		else if(is_first_capture == 1) // 하강에지
		{
			is_first_capture = 0; // 다음 echo 펄수를 vount 파기 위해 변수
			distance = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			ic_cpt_finish_flag = 1;
		}
	}

}
int dis; // 거리를 측정한 펄스 갯수를 저장하는 변수
void ultrasonic_processing(void)
{

	char lcd_buff[20];

	if(TIM10_10ms_ultrasonic >= 100) // 1초
	{
		TIM10_10ms_ultrasonic = 0;
		make_trigger();
		if(ic_cpt_finish_flag) // 초음파 측정이 완료 되었으면 if(ic_cpt_finish_flag >= 1)
		{
			ic_cpt_finish_flag = 0;
			dis = distance;
			dis = dis * 0.034 / 2;  // 1us가 0.034cm를 이동
			//왕복거리를 리턴해 주기 때문에 /2를 해주는 것이다.
			if (lcd_display_mode_flag == 2)
			{
				sprintf(lcd_buff, "dis: %dcm", dis);
				move_cursor(0,0);
				lcd_string(lcd_buff);
			}
			if(ultra_on)
			{
				printf("dis: %dcm\n",dis);
			}
		}
	}
}

void make_trigger()
{
	HAL_GPIO_WritePin(ULTRASONIC_TRIGGER_GPIO_Port, ULTRASONIC_TRIGGER_Pin, 0);
	delay_us(2);
	HAL_GPIO_WritePin(ULTRASONIC_TRIGGER_GPIO_Port, ULTRASONIC_TRIGGER_Pin, 1);
	delay_us(10);
	HAL_GPIO_WritePin(ULTRASONIC_TRIGGER_GPIO_Port, ULTRASONIC_TRIGGER_Pin, 0);
}

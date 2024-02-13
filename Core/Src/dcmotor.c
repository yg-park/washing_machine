#include "button.h"
#include "dcmotor.h"
#include "led.h"

extern TIM_HandleTypeDef htim4;
extern volatile int t1ms_counter;

void dcmotor_pwm_control(void);

int i;

// BUTTON0 : start/stop - LED1(toggle)
// BUTTON1 : SPEED-UP - LED2(toggle)
// BUTTON2 : SPEED-DOWN - LED3(toggle)
uint8_t pwm_start_flag = 0;  // dcmotor start/stop indicaator flag
uint16_t CCR_UP_Value = 0;  // PWM UP control
uint16_t CCR_DOWN_Value = 0;  // PWM DOWN control
uint16_t CCR_UP_CCR_Value = 0;  // PWM UP control
uint16_t CCR_DOWN_CCR_Value = 0;  // PWM DOWN control
uint8_t forward_backword_dcmotor = 0;  // PWM DOWN control

void dcmotor_pwm_control(void)
{
	// start/stop : button0
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);  // LD1

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);

		if (!pwm_start_flag)
		{
			pwm_start_flag = 1;
			HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, 1);
		}
		else
		{
			pwm_start_flag = 0;
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
			led_all_off();
		}
	}

	// speed-up : button1
	if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);  // LD2
		CCR_UP_Value = __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);

		CCR_UP_Value += 10;
		CCR_UP_CCR_Value = CCR_UP_Value;
		if (CCR_UP_Value > 100)
		{
			CCR_UP_Value = 100;
		}
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, CCR_UP_Value);
		for (i=1; i<=(CCR_UP_CCR_Value/10)-2; i++)
		{
			HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		}
	}

	// speed-down : button2
	if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);  // LD3
		CCR_DOWN_Value = __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);
		CCR_DOWN_Value -= 10;
		CCR_DOWN_CCR_Value = CCR_DOWN_Value;
		if (CCR_DOWN_Value < 30)
		{
			CCR_DOWN_Value = 30;
		}
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, CCR_DOWN_Value);
		HAL_GPIO_WritePin(GPIOD, 0x01 << (CCR_DOWN_CCR_Value/10)-1, 0);
	}

	// 정,역회전 : button3
	if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS)
	{
		if (forward_backword_dcmotor == 0)
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
			if (t1ms_counter >= 200)
			{
				t1ms_counter = 0;
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			}
			forward_backword_dcmotor = 1;

		}
		else
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
			if (t1ms_counter >= 50)
			{
				t1ms_counter = 0;
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			}
			forward_backword_dcmotor = 0;

		}
	}

	if (pwm_start_flag)  // LED
		{
			if (forward_backword_dcmotor == 0)
			{
				if (t1ms_counter >= 50)
				{
					t1ms_counter = 0;
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
				}
			}
			else
			{
				if (t1ms_counter >= 100)
				{
					t1ms_counter = 0;
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
				}
			}
		}
}

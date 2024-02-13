#include "main.h"
#include "button.h"
#include "dcmotor.h"
#include "i2c_lcd.h"
#include "uart.h"
#include "ultrasonic.h"
#include "led.h"

extern void buzzer_end(void);
extern void buzzer_button(void);
extern TIM_HandleTypeDef htim4;
extern void FND_update(unsigned int value);
extern void fnd4digit_sec_clock(void);
extern void fnd4digit_spin(void);

extern volatile int t1ms_counter;
extern int dis;

uint8_t lcd_display_mode_num = 0;
uint8_t wash_dry_sel = 0;
uint8_t power_sel = 0;
uint8_t time_sel = 0;
uint8_t operator_sel = 0;
uint8_t spin_num = 0;
uint8_t time_stop = 0;
uint8_t time_coun = 1;
uint8_t time_num = 0;
uint8_t k = 0;

int wash_dry_num = 0;
int power_level = 0;
int oper_num1 = 60;
int oper_num2 = 60;

int btn4_press=0;


void mode_select(void)
{

	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		lcd_command(CLEAR_DISPLAY);

		lcd_display_mode_num++;
		lcd_display_mode_num %= 2;

		if(lcd_display_mode_num == 1)
		{
			lcd_command(CLEAR_DISPLAY);

			move_cursor(0, 0);
			lcd_string(" MODE SELECT");
			move_cursor(1, 0);
			lcd_string("1wash2power3time");
		}

		if(lcd_display_mode_num == 0)
		{
			lcd_command(CLEAR_DISPLAY);

			move_cursor(0, 0);
			lcd_string(" RESTING");
			move_cursor(1, 0);
			lcd_string("PRESS BUTTON0");
		}
	}

	if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
	{
		lcd_command(CLEAR_DISPLAY);
		move_cursor(0, 0);
		lcd_string("  WASH or DRY");

		wash_dry_sel++;
		wash_dry_sel %= 2;

		if (wash_dry_sel == 1)
		{
			move_cursor(1, 0);
			lcd_string("1.WASH");  // 정3역2정3역2
			wash_dry_num = 1;
		}

		if (wash_dry_sel == 0)
		{
			move_cursor(1, 0);
			lcd_string("2.DRY");  // 정2역3정2역3
			wash_dry_num = 0;
		}
	}

	if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
	{
		lcd_command(CLEAR_DISPLAY);
		move_cursor(0, 0);
		lcd_string("  POWER SELECT");

		power_sel++;
		power_sel %= 3;

		if (power_sel == 1)
		{
			move_cursor(1, 0);
			lcd_string("level : 1");

			power_level = 40;

		}

		if (power_sel == 2)
		{
			move_cursor(1, 0);
			lcd_string("level : 2");

			power_level = 70;
		}

		if (power_sel == 0)
		{
			move_cursor(1, 0);
			lcd_string("level : 3");

			power_level = 100;
		}
	}

	if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS)
	{
		lcd_command(CLEAR_DISPLAY);

		move_cursor(0, 0);
		lcd_string("  TIME SELECT");

		time_sel++;
		time_sel %= 3;

		if (time_sel == 1)
		{
			move_cursor(1, 0);
			lcd_string("TIME : 5sec");
			time_num=5;
		}

		if (time_sel == 2)
		{
			move_cursor(1, 0);
			lcd_string("TIME : 10sec");
			time_num = 10;
		}

		if (time_sel == 0)
		{
			move_cursor(1, 0);
			lcd_string("TIME : 20sec");
			time_num = 20;
		}
	}

	if (get_button(GPIOC, GPIO_PIN_13, 4) == BUTTON_PRESS)
	{
		btn4_press = 1;
		btn4_press %= 2;
	}

	if (btn4_press==1)
	{
		operator_start();
	}
}

char time__num[4];


void operator_start(void)
{
	if (wash_dry_sel == 1)
	{
		if (power_sel == 1)
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, power_level);
			if (time_sel == 1)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");
						buzzer_main();
					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 2)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");

					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 0)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}
		}

		if (power_sel == 2)
		{
			if (!time_num)
				return;
			if (t1ms_counter % 50 == 0)
			{
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
			}
			if (t1ms_counter >= 1000)
			{
				t1ms_counter = 0;
				time_num--;
				k++;
				k%=2;

				if(time_num <= 0)
				{
					lcd_command(CLEAR_DISPLAY);
					time_num=0;

					move_cursor(1,0);
					sprintf(time__num, "%02d", time_num);
					lcd_string(time__num);

					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
					move_cursor(0,0);
					lcd_string("clear!!");
					move_cursor(1,0);
					lcd_string("PRESS BUTTON0");


				}

				else
				{
					lcd_command(CLEAR_DISPLAY);
					spin_motor();
					move_cursor(0,0);
					lcd_string("  washing!!");
					move_cursor(1,0);
					sprintf(time__num, "%02d", time_num);
					lcd_string(time__num);
				}
			}
		}

			if (time_sel == 2)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 0)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}
		}

		if (power_sel == 0)
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, power_level);

			if (time_sel == 1)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 2)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 0)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}
		}


	if (wash_dry_sel == 0)
	{
		if (power_sel == 1)
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, power_level);

			if (time_sel == 1)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 2)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 0)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}
		}

		if (power_sel == 2)
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, power_level);

			if (time_sel == 1)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 2)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");
						buzzer_main();


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 0)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}
		}

		if (power_sel == 0)
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, power_level);

			if (time_sel == 1)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 2)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}

			if (time_sel == 0)
			{
				if (!time_num)
					return;
				if (t1ms_counter % 50 == 0)
				{
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
				}
				if (t1ms_counter >= 1000)
				{
					t1ms_counter = 0;
					time_num--;
					k++;
					k%=2;

					if(time_num <= 0)
					{
						lcd_command(CLEAR_DISPLAY);
						time_num=0;

						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);

						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
						move_cursor(0,0);
						lcd_string("clear!!");
						move_cursor(1,0);
						lcd_string("PRESS BUTTON0");


					}

					else
					{
						lcd_command(CLEAR_DISPLAY);
						spin_motor();
						move_cursor(0,0);
						lcd_string("  washing!!");
						move_cursor(1,0);
						sprintf(time__num, "%02d", time_num);
						lcd_string(time__num);
					}
				}
			}
		}
	}
}

void operator_stop(void)
{
	lcd_command(CLEAR_DISPLAY);

	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);

	move_cursor(0, 0);
	lcd_string("  RESTING");
	move_cursor(1, 0);
	lcd_string("PRESS BUTTON0");
}

void spin_motor(void)
{
	if (k == 0)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
	}
	else if (k == 1)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
	}
}


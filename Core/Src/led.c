#include "led.h"  // < >

void led_main(void);
void led_all_on(void);
void led_all_off(void);
void led_on_up(void);
void led_on_down(void);
void led_keepon_up(void);
void led_keepon_down(void);
void flower_on(void);
void flower_off(void);

void led_main(void)
{
	while (1)
	{
		led_all_off();
		flower_on();
		led_all_on();
		flower_off();


		// phase 4 [STM32] led_keepon_up/down
		// 1. source 2. 동작 동영상
		// 0->1->2->3->4->5->6->7(앞전에 ON 했던 LED는 그대로 유지)
//		led_keepon_up();
//		led_all_off();
//		// 7->6->5->4->3->2->1->0(앞전에 OFF 했던 LED는 그대로 유지)
//		led_keepon_down();
//		led_all_off();

		// phase 3
		// 해당되는 bit의 LED만 ON
//		// 0->1->2->3->4->5->6->7
//		led_on_up();
//		// 7->6->5->4->3->2->1->0
//		led_on_down();

		// phase 2
//		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_All);
//		HAL_Delay(500);

		// phase 1
//		led_all_on();
//		HAL_Delay(300);
//		led_all_off();
//		HAL_Delay(300);
	}
}

void led_all_on(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOD, 0xff, 1);
}

void led_all_off(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOD, 0xff, 0);
}

void led_on_up(void)
{
	led_all_off();
	for (int i=0; i<8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		HAL_Delay(200);
	}
}

void led_on_down(void)
{
	led_all_off();
	for (int i=0; i<8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		HAL_Delay(200);
	}
}

void led_keepon_up(void)
{
	led_all_off();
	for (int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		HAL_Delay(200);
	}
}

void led_keepon_down(void)
{
	led_all_off();
	for (int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		HAL_Delay(200);
	}
}

void flower_on(void)
{
	led_all_off();
	for (int i=0; i<4; i++)
	{
		int delay=100;

		HAL_GPIO_WritePin(GPIOD, 0x10 << i, 1);
		HAL_GPIO_WritePin(GPIOD, 0x08 >> i, 1);
		HAL_Delay(delay);

		delay+=100;
	}
}

void flower_off(void)
{
	led_all_on();
	int delay=400;

	for (int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x10 << (3-i), 0);
		HAL_GPIO_WritePin(GPIOD, 0x08 >> (3-i), 0);
		HAL_Delay(delay);

		delay-=100;
	}
}

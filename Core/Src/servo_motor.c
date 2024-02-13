#include "main.h"

void servo_motor_test_main(void);

extern TIM_HandleTypeDef htim2;
// 84MHz / 1680 = 50KHz
// T = 1/f = 1/50000 = 0.00002s(20us)
// 2ms(180도) : 0.00002 x 100
// 1.5ms(90도) : 0.00002 x 75
// 1ms(0도) : 0.00002 x 50
void servo_motor_test_main(void)
{
	while (1)
	{
		//  0도 회전
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 25);  // 0도
		HAL_Delay(1000);

		// 90도 회전
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 65);  // 180도
		HAL_Delay(1000);

		// 180도 회전
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 115);  // 180도
		HAL_Delay(1000);
	}
}

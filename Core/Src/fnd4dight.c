#include "main.h"
#include "fnd4digit.h"
#include "button.h"

extern volatile int fnd1ms_counter;  // fnd1ms
extern uint8_t time_num;
extern uint8_t k;
extern int btn4_press;

int spin_num1 = 0;

void fnd4digit_sec_clock(void);
uint16_t FND_digit[4] =
{
	FND_d1, FND_d10, FND_d100, FND_d1000
};

uint32_t FND_font[10] =
{
  FND_a|FND_b|FND_c|FND_d|FND_e|FND_f,   // 0
  FND_b|FND_c,                           // 1
  FND_a|FND_b|FND_d|FND_e|FND_g,         // 2
  FND_a|FND_b|FND_c|FND_d|FND_g,         // 3
  FND_b|FND_c|FND_f|FND_g,   // 4
  FND_a|FND_c|FND_d|FND_f|FND_g,  // 5
  FND_a|FND_c|FND_d|FND_e|FND_f|FND_g,  // 6
  FND_a|FND_b|FND_c,      // 7
  FND_a|FND_b|FND_c|FND_d|FND_e|FND_f|FND_g,   // 8
  FND_a|FND_b|FND_c|FND_d|FND_f|FND_g   // 9
};

uint16_t FND[4];    // FND에 쓰기 위한 값을 준비하는 변수

uint32_t FND_spin1[4] =
{
	FND_a, NULL, FND_d, FND_e|FND_f
};

uint32_t FND_spin2[4] =
{
	FND_a, FND_b|FND_c, FND_d, NULL
};

void fnd4digit_main(void)
{
	unsigned int value=0;   // 1초가 되었을때 up count
	unsigned int msec=0;    // ms counter
	static int i=0;     // FND position indicator

	FND4digit_off();

	while(1)
	{
#if 1   // SYSTICK interrupt 방식
		if (fnd1ms_counter >= 2)   // 2ms reached
		{

			fnd1ms_counter=0;
			msec += 2;   // 2ms
			if (msec > 1000)   // 1000ms reached
			{
				msec = 0;
				value--;       // sec count를 증가
				FND_update(value);
			}

			FND4digit_off();
#if 1 // common 애노우드  WCN4-
			HAL_GPIO_WritePin(FND_COM_PORT,FND_digit[i], GPIO_PIN_SET);
			HAL_GPIO_WritePin(FND_DATA_PORT, FND[i], GPIO_PIN_RESET);
#else // common 캐소우드 CL5642AH30
			HAL_GPIO_WritePin(FND_COM_PORT,FND_digit[i], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(FND_DATA_PORT, FND[i], GPIO_PIN_SET);
#endif
			i++;   // 다음 display할 FND를 가리킨다.
			i %= 4;
		}
#else   // orginal delay 방식
		msec += 8;   // 8ms
		if (msec > 1000)   // 1000ms reached
		{
			msec = 0;
			value++;       // sec count를 증가
			FND_update(value);
		}
		for (int i=0; i < 4; i++)
		{
			FND4digit_off();
			HAL_GPIO_WritePin(FND_COM_PORT,FND_digit[i], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(FND_DATA_PORT, FND[i], GPIO_PIN_SET);
			HAL_Delay(2);
		}
#endif
	}
}

void FND_update(unsigned int value)
{
	FND[0] = FND_font[value % 10];
	FND[1] = FND_font[value / 10 % 10];
//	FND[2] = FND_font[value / 100 % 10];
//	FND[3] = FND_font[value / 1000 % 10];

	return;
}

void FND_downdate(unsigned int value)
{
//	FND[0] = FND_font[value % 10];
//	FND[1] = FND_font[value / 10 % 10];
	FND[2] = FND_spin1[spin_num1];
	FND[3] = FND_spin2[spin_num1];

	return;
}


void FND4digit_off(void)
{
#if 1 // common 에노우드 WCN4-
	HAL_GPIO_WritePin(FND_COM_PORT, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FND_DATA_PORT,FND_font[8]|FND_p, GPIO_PIN_SET);
#else // common 캐소우드 CL5642AH30
	HAL_GPIO_WritePin(FND_COM_PORT, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(FND_DATA_PORT,FND_font[8]|FND_p, GPIO_PIN_RESET);
#endif
	return;
}
void FND4digit_on(void)
{
#if 1  // common 에노우드
	HAL_GPIO_WritePin(FND_COM_PORT, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(FND_DATA_PORT,FND_font[8]|FND_p, GPIO_PIN_RESET);
#else  // CL5642AH30
	HAL_GPIO_WritePin(FND_COM_PORT, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FND_DATA_PORT,FND_font[8]|FND_p, GPIO_PIN_SET);
#endif
	return;
}

void fnd4digit_sec_clock(void)
{
	static unsigned int value=0;   // 1초가 되었을때 up count
	static unsigned int msec=0;    // ms counter
	static int i=0;     // FND position indicator



	FND_update(time_num);

#if 1   // SYSTICK interrupt 방식
		if (fnd1ms_counter >= 2)   // 2ms reached
		{
			fnd1ms_counter=0;

			FND4digit_off();
#if 1 // common 애노우드  WCN4-
			HAL_GPIO_WritePin(FND_COM_PORT,FND_digit[i], GPIO_PIN_SET);
			HAL_GPIO_WritePin(FND_DATA_PORT, FND[i], GPIO_PIN_RESET);
#else // common 캐소우드 CL5642AH30
			HAL_GPIO_WritePin(FND_COM_PORT,FND_digit[i], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(FND_DATA_PORT, FND[i], GPIO_PIN_SET);
#endif
			i++;   // 다음 display할 FND를 가리킨다.
			i %= 4;
		}
#else   // orginal delay 방식
		msec += 8;   // 8ms
		if (msec > 1000)   // 1000ms reached
		{
			msec = 0;
			value++;       // sec count를 증가
			FND_update(value);
		}
		for (int i=0; i < 4; i++)
		{
			FND4digit_off();
			HAL_GPIO_WritePin(FND_COM_PORT,FND_digit[i], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(FND_DATA_PORT, FND[i], GPIO_PIN_SET);
			HAL_Delay(2);
		}

#endif

}


void fnd4digit_spin(void)
{
	static unsigned int value=0;   // 1초가 되었을때 up count
	static unsigned int msec=0;    // ms counter
	static unsigned int back = 3;
	static int i=0;     // FND position indicator

	if (fnd1ms_counter >= 2)   // 2ms reached
	{
		fnd1ms_counter=0;
		msec += 2;   // 2ms
		if (msec > 300)   // 300ms reached
		{
			msec = 0;
			if (btn4_press == 1)
			{
				if (k == 1)
				{
					FND_downdate(spin_num1);
					spin_num1++;
					spin_num1%=4;
				}
				else if(k == 0)
				{
					FND_downdate(back);
					if (back <= 0)
					{
						back = 4;
					}
					back--;
				}
			}
		}
	}
	FND4digit_off();

	HAL_GPIO_WritePin(FND_COM_PORT,FND_digit[i], GPIO_PIN_SET);
	HAL_GPIO_WritePin(FND_DATA_PORT, FND[i], GPIO_PIN_RESET);
	i++;
	i %= 4;
}











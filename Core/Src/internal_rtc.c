#include "main.h"
#include "button.h"
#include "i2c_lcd.h"
#include "led.h"

#include <stdio.h>
#include <string.h>

void get_rtc(void);
void set_rtc(char *date_time);
void set_time_button_ui(void);
void lcd_set_time(void);

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef mTime;  // time information
extern uint8_t lcd_display_mode_flag;

extern char lcd_buff1[40];
extern char lcd_buff2[40];
extern char lcd_buff3[40];

RTC_TimeTypeDef sTime = {0};  // time information
RTC_DateTypeDef sDate = {0};  // date information

// 예) 23년이 save된 binary format
// 7654 3210
// 0010 0011
// STM32의 RTC로부터 날짜 & 시각 정보를 읽어오는 함수
// BCD --> decimal
unsigned char bin2dec(unsigned char byte)
{
	unsigned char high, low;
	low = byte & 0x0f;  // 하위 4bit(low nibble)
	high = (byte >> 4) * 10;  // 상위 4bit(high nibble)

	return high + low;
}
// decimal --> BCD  ex) 23 : 0010111
unsigned char dec2bin(unsigned char byte)
{
	unsigned char high, low;

	high = (byte / 10) << 4;
	low = byte % 10;

	return high + low;
}

void get_rtc(void)
{
	static RTC_TimeTypeDef oldTime;

	char lcd_buff[40];

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

	if (oldTime.Seconds != sTime.Seconds)
	{
		oldTime.Seconds = sTime.Seconds;
		// YYYY-MM-DD hh:MM
		printf("%04d-%02d-%02d %02d:%02d:%02d\n", bin2dec(sDate.Year)+2000, bin2dec(sDate.Month),
				bin2dec(sDate.Date), bin2dec(sTime.Hours), bin2dec(sTime.Minutes), bin2dec(sTime.Seconds));
		//lcd_set_time();
	}

}
// setrtc231016103800
//       678901234567
void set_rtc(char *date_time)
{
	char yy[4], mm[4], dd[4];  // date
	char hh[4], min[4], ss[4];  // time

	strncpy(yy, date_time+6, 2);  // yy[0] = '2', yy[1] = '3', yy[2] = 0
	strncpy(mm, date_time+8, 2);
	strncpy(dd, date_time+10, 2);

	strncpy(hh, date_time+12, 2);
	strncpy(min, date_time+14, 2);
	strncpy(ss, date_time+16, 2);

	// ascii --> int --> bcd
	sDate.Year = dec2bin(atoi(yy));
	sDate.Month = dec2bin(atoi(mm));
	sDate.Date = dec2bin(atoi(dd));

	sTime.Hours = dec2bin(atoi(hh));
	sTime.Minutes = dec2bin(atoi(min));
	sTime.Seconds = dec2bin(atoi(ss));

	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
}
// button0 : 시간 정보룰 변경하는 버튼 00~23 (up counter)
// button1 : 분 정보룰 변경하는 버튼 00~59 (up counter)
// button2 : 초 정보룰 변경하는 버튼 00~59 (up counter)
// button3 : 변경 완료, 현재까지 변경된 내용을 저장
//           HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
void set_time_button_ui(void)
{
	if (lcd_display_mode_flag == 3)
	{
		if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
		{
			// 시간 정보
			mTime.Hours++;
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			if (mTime.Hours == 24)
			{
				mTime.Hours = 0;
			}
		}
		if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
		{
			// 분 정보
			mTime.Minutes++;
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			if (mTime.Minutes == 60)
			{
				mTime.Minutes = 0;
				mTime.Hours++;
			}
		}
		if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
		{
			// 초 정보
			mTime.Seconds++;
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			if (mTime.Seconds == 60)
			{
				mTime.Seconds = 0;
				mTime.Minutes++;
			}
		}
		if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS)
		{
			HAL_RTC_SetTime(&hrtc, &mTime, RTC_FORMAT_BCD);
			/*for (int i=0; i<5; i++)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
				HAL_Delay(500);
			}*/
		}
	}
}

void lcd_set_time(void)
{
	if (lcd_display_mode_flag == 0)
	{
		sprintf(lcd_buff1, "DATE:%04d-%02d-%02d", bin2dec(sDate.Year)+2000, bin2dec(sDate.Month),bin2dec(sDate.Date));
		move_cursor(0, 0);
		lcd_string(lcd_buff1);
		sprintf(lcd_buff2, "TIME:%02d:%02d:%02d", bin2dec(sTime.Hours), bin2dec(sTime.Minutes), bin2dec(sTime.Seconds));
		move_cursor(1, 0);
		lcd_string(lcd_buff2);
	}
	if (lcd_display_mode_flag == 3)
	{
		lcd_command(CLEAR_DISPLAY);
		sprintf(lcd_buff2, "TIME:%02d:%02d:%02d", bin2dec(mTime.Hours), bin2dec(mTime.Minutes), bin2dec(mTime.Seconds));
		move_cursor(0, 0);  // 시간 정보 field로 커서 이동
		lcd_string(lcd_buff2);
	}
}

#include "uart.h"
#include <string.h>
#include <stdlib.h>
void pc_command_processing(void);
void bt_command_processing(void);
extern uint8_t rx_data;  // uart3 rx data
extern uint8_t bt_rx_data;  // uart6 rx data

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

extern void led_main();
extern void led_all_on();
extern void led_all_off();
extern void led_on_up();
extern void led_on_down();
extern void led_keepon_up();
extern void led_keepon_down();
extern void flower_on();
extern void flower_off();
extern void DHT11_processing();
extern void set_rtc(char *date_time);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
extern int dht11time;
extern volatile int dht11on;
extern volatile int ultra_on;

#define COMMAND_SU 20
#define COMMAND_LENGTH 40


volatile int rx_index = 0; // rx_buff의 save위치
volatile unsigned char rx_buff[COMMAND_LENGTH]; // UART3으로부터 수신된 char 를 저장하는 공간(\n을 만날 때 까지)
volatile int rx_index2 = 0;
volatile int newline_detect_flag = 0;  //  new line을 만났을때의 indicator 예) ledallon\n


volatile unsigned char bt_rx_buff[COMMAND_LENGTH]; // UART6으로부터 수신된 char 를 저장하는 공간(\n을 만날 때 까지)
volatile int bt_rx_index = 0; // rx_buff의 save위치
volatile int bt_newline_detect_flag = 0;  //  bt_new line을 만났을때의 indicator 예) ledallon\n


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


	if(huart == &huart3)  //  comport master와 연결된 uart
	{
		if(rx_index < COMMAND_LENGTH)  //  현재까지 돌아온 byte가 40byte를 넘지 않으면
			{
				if(rx_data == '\n' || rx_data == '\r')
				{
					rx_buff[rx_index] = 0; // '\0'
					newline_detect_flag = 1;
					rx_index = 0;  // 다음 message 저장을 위해서 rx_index값을 0으로 한다.
				}
				else
				{
					rx_buff[rx_index++] = rx_data;
				}

			}
		else
		{
			rx_index = 0;
			printf("Message Overflow !!!!\n");
		}
		//주의 : 반드시 HAL_UART Receive_IT를 call 해줘야 다음 INT가 발생이 된다.
		HAL_UART_Receive_IT(&huart3, &rx_data, 1);
	}

	if(huart == &huart6)  //  comport master와 연결된 uart
		{
			if(bt_rx_index < COMMAND_LENGTH)  //  현재까지 돌아온 byte가 40byte를 넘지 않으면
				{
					if(bt_rx_data == '\n' || bt_rx_data == '\r')
					{
						bt_rx_buff[bt_rx_index] = 0; // '\0'
						bt_newline_detect_flag = 1;
						bt_rx_index = 0;  // 다음 message 저장을 위해서 bt_rx_index값을 0으로 한다.
					}
					else
					{
						bt_rx_buff[bt_rx_index++] = bt_rx_data;
					}

				}
			else
			{
				bt_rx_index = 0;
				printf("BT Message Overflow !!!!\n");
			}
			//주의 : 반드시 HAL_UART Receive_IT를 call 해줘야 다음 INT가 발생이 된다.
			HAL_UART_Receive_IT(&huart6, &bt_rx_data, 1);
		}

}


void pc_command_processing(void)
{
	if(newline_detect_flag)  // comport master로 부터 완전한 문장이 들어오면 (\n을 만나면)
	{
		newline_detect_flag = 0;
		printf("%s\n", rx_buff);
		if(!strncmp(rx_buff, "dht11on", strlen("dht11on")))  //
		{
			dht11on = 1;
			return;
		}
		if(!strncmp(rx_buff, "dht11off", strlen("dht11off")))  //
		{
			dht11on = 0;
			return;
		}
		if(!strncmp(rx_buff, "ultra_on", strlen("ultra_on")))  //
		{
			ultra_on = 1;
			return;
		}
		if(!strncmp(rx_buff, "ultra_off", strlen("ultra_off")))  //
		{
			ultra_on = 0;
			return;
		}
		if(!strncmp(rx_buff, "setrtc", strlen("setrtc")))  //
		{
			set_rtc(rx_buff);
			return;
		}
		//		if(!strncmp(rx_buff, "dht11time150", strlen("dht11time150")))  //
//		{
//			dht11time = 150;
//			return;
//		}
//		if(!strncmp(rx_buff, "dht11time200", strlen("dht11time200")))  //
//		{
//			dht11time = 200;
//			return;
//		}
//		if(!strncmp(rx_buff, "dht11time300", strlen("dht11time300")))  //
//		{
//			dht11time = 300;
//			return;
//		}
//		if(!strncmp(rx_buff, "dht11time500", strlen("dht11time500")))  //
//		{
//			dht11time = 500;
//			return;
//		}
/*
		if(!strncmp(rx_buff, "dht11time", strlen("dht11time")))  //
		{
			dht11time = atoi(rx_buff+9);
			return;
		}

		if(!strncmp(rx_buff, "led_all_on", strlen("led_all_on")))  //
		{
			led_all_on();
			return;
		}
		if(!strncmp(rx_buff, "led_all_off", strlen("led_all_off")))  //
		{
			led_all_off();
			return;
		}
		if(!strncmp(rx_buff, "led_on_up", strlen("led_on_up")))  //
		{
			led_on_up();
			return;
		}
		if(!strncmp(rx_buff, "led_on_down", strlen("led_on_down")))  //
		{
			led_on_down();
			return;
		}
		if(!strncmp(rx_buff, "led_keepon_up", strlen("led_keepon_up")))  //
		{
			led_keepon_up();
			return;
		}
		if(!strncmp(rx_buff, "led_keepon_down", strlen("led_keepon_down")))  //
		{
			led_keepon_down();
			return;
		}
		if(!strncmp(rx_buff, "flower_on", strlen("flower_on")))  //
		{
			flower_on();
			return;
		}
		if(!strncmp(rx_buff, "flower_off", strlen("flower_off")))  //
		{
			flower_off();
			return;
		}
*/
	}
}

void bt_command_processing(void)
{
	if(bt_newline_detect_flag)  // comport master로 부터 완전한 문장이 들어오면 (\n을 만나면)
	{
		bt_newline_detect_flag = 0;
		printf("%s\n", bt_rx_buff);
		if(!strncmp(bt_rx_buff, "led_all_on", strlen("led_all_on")))  //
		{
			led_all_on();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_all_off", strlen("led_all_off")))  //
		{
			led_all_off();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_on_up", strlen("led_on_up")))  //
		{
			led_on_up();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_on_down", strlen("led_on_down")))  //
		{
			led_on_down();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_keepon_up", strlen("led_keepon_up")))  //
				{
			led_keepon_up();
					return;
				}
		if(!strncmp(bt_rx_buff, "led_keepon_down", strlen("led_keepon_down")))  //
						{
			led_keepon_down();
							return;
						}
		if(!strncmp(bt_rx_buff, "flower_on", strlen("flower_on")))  //
						{
			flower_on();
							return;
						}
		if(!strncmp(bt_rx_buff, "flower_off", strlen("flower_off")))  //
						{
			flower_off();
							return;
						}

	}
}

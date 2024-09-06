#ifndef __BSP_H_
#define __BSP_H_
#include "main.h"
#include <stdio.h>
#include <string.h>


#include "adc.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"



#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "croutine.h"
//#include "semphr.h"
//#include "event_groups.h"

//bsp
#include "bsp_delay.h"
#include "bsp_smg.h"
#include "bsp_uart_fifo.h"
#include "bsp_dht11.h"
#include "bsp_fan.h"
#include "bsp_buzzer.h"
#include "bsp_ultrasonic.h"
#include "bsp_cmd_link.h"
#include "interrupt_manager.h"
#include "bsp_cmd_link.h"
#include "bsp_run.h"
#include "bsp_freertos.h"





#define  USE_FreeRTOS      1

#if USE_FreeRTOS == 1
	//#include "FreeRTOS.h"
	///#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef struct _action_state{


  uint8_t gmouse;
  uint8_t disp_rx_cmd_done_flag ;
  uint8_t gai_modle_flag ;
   




}action_state;


extern action_state gpro_t;

void bsp_Init(void); 



void receive_data_fromm_display(uint8_t *pdata);


uint8_t bcc_check(const unsigned char *data, int len) ;



#endif 

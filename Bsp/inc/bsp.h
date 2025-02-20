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
  uint8_t receive_copy_cmd;
  uint8_t send_ack_cmd;

  uint8_t gTimer_again_send_power_on_off;



}action_state;


extern action_state gpro_t;

typedef enum{

	none_cmd,
	power_cmd=0x01,
	dry_cmd=0x02,
	plasma_cmd,
	mouse_cmd,
	wifi_cmd,
	buzzer_cmd =0x06,
	ai_cmd = 0x07,
	temp_warning,
	fan_warning,
	temp_hum_data = 0x1A,
	beijing_times_data =0x1C,
	dry_notice_cmd= 0x22, //don't sound
	copy_cmd = 0xff


}message_info;






void bsp_Init(void); 



uint8_t bcc_check(const unsigned char *data, int len) ;






#endif 

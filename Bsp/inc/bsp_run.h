#ifndef __BSP_RUN_H__
#define __BSP_RUN_H__
#include "main.h"

#define WIFI_TUYA      1  //
#define AI_ENABLE      1

#define DEBUG          0

typedef enum {                   /* enumeration for CProcess signals */
    kill =1,notkill,dry,notdry,ai,notai
}Signal;

typedef enum
{
  AIENABLE = 0,
  AIDISABLE = !AIENABLE
} AiStatus;

typedef enum
{
  WIFIENABLE =1,
  WIFIDISABLE = !WIFIENABLE

}WifiStatus;

typedef enum{

   POWER_OFF,
   POWER_ON =0x01,
   POWER_CONNECTOR_WIFI,
   UPDATE_TO_PANEL_DATA,
   ADC_UPDATE_DATA,
   WIFI_RESTART_INIT,
   WIFI_NORMAL_POWER_ON,
   FAN_CONTINUCE_RUN_ONE_MINUTE, //7
   POWER_OFF_NULL

   

}run_state_t;

typedef enum{

	 DRY_ON = 0x40,
	 DRY_OFF = 0x41,

	 DRY_ON_NO_BUZZER = 0x90,
	 DRY_OFF_NO_BUZZER = 0x91,

	 PLASMA_ON = 0x20,
	 PLASMA_OFF = 0x21,
	 PLASM_ON_NO_BUZZER = 0xA0,
     PLASM_OFF_NO_BUZZER = 0xA1,

     FAN_LEVEL_MIN = 0x10,       //
     MOUSE_RUN = 0x11,
     FAN_LEVEL_MAX_NO_SOUND=0x12,
     MOUSE_STOP = 0x13,
     

     AI_MODE_ON = 0x30,   //EDIT 2023.09.13 new Add
     AI_MODE_OFF =0x31, 

	 PTC_WARNING= 0xE1,
	 FAN_WARNING = 0xE2,

	 FAN_REMOVE_WARNING = 0xF2
     

}works_t;




typedef enum{

   COMMAND_ID=0x50,
   DATA_ID,

   ANSWER_POWER_OFF =0x00,

   ANSWER_POWER_ON = 0x1


}answering_state;

typedef enum{

   mouse_stop,
   mouse_run
  

}fan_level_state;




typedef struct _RUN_T{
    
    //power on
	uint8_t gPower_On;


	uint8_t gDht11_flag;
	

	uint8_t gDht11_humidity ;
	uint8_t gDht11_temperature;

    

	uint8_t RunCommand_Label;
    uint8_t run_masin_process_step;
	//buzzer
	uint8_t buzzer_sound_flag ;
  
    uint8_t decodeFlag;
    uint8_t power_on_send_data_flag;
    uint8_t gTimer_usart_error_2;
	uint8_t interval_time_stop_run;

 
 
	
    //ptc
    uint8_t  gDry;
    uint8_t gTimer_ptc_adc_times ;
    uint8_t ptc_too_heat_value;
	uint8_t open_ptc_detected_flag;
	
	//fan 
	uint8_t gFan_continueRun;
	uint8_t  gTimer_fan_run_one_minutes;
    uint8_t gTimer_fan_adc_times;
    
  
	//adc 
	uint8_t ADC_channel_No;
      

      uint8_t  gPlasma;
     
	 
	  uint8_t  gUltrasonic;
	  uint8_t  gTimer;
      uint8_t  gTemperature ;

	  uint8_t  gHumidity;
	 
	//fault 
	 uint8_t fan_warning ;
	 uint8_t ptc_warning;
	  

	  //usart1
	
     uint8_t process_run_guarantee_flag ;
    uint8_t gTimer_check_iwdg_flag;
    uint8_t gTimer_check_iwdg_fla;
    uint8_t gTimer_usart_error;
    uint8_t iwdg_feed_success_flag;

	
	
	  
	  uint8_t  set_temperature_value;
	  uint8_t  set_timer_timing_value;
	  uint8_t  set_temperature_decade;
	  uint8_t  set_temperature_unit;
	 
  

	  uint8_t first_power_on_flag;
	  uint8_t app_appointment_time_power_on;
	  uint8_t theFirst_input_power_flag;
	 
	  
	  
	uint8_t gTimer_1s;
	uint8_t gTimer_read_dht11_temp_value;
	uint8_t gTimer_send_cloud_state;
	uint8_t gTimer_senddata_panel;

	uint8_t  time_remaining_minutes_one;
	uint8_t  time_remaining_minutes_two;
	uint8_t  work_time_minutes_one;
	uint8_t  work_time_minutes_two;
    uint8_t  gTime_run_board_action ;

	uint16_t gTimer_continuce_works_time;
	uint16_t fan_detect_voltage; 
	
	uint16_t ptc_temp_voltage;

	

	
}RUN_T;

//#define CProcessInit(me_) ((me_)->cmdCtr__ =0,(me_)->state__ = IDLE)
extern uint8_t ReceiveBuffer[1];
extern RUN_T run_t; 




void  RunCommand_MainBoard_Fun(void);


void mainboard_run_handler(void);




void Single_RunCmd(uint8_t sig);

void Single_Mode(void);

void Single_ReceiveCmd(uint8_t cmd);
void SystemReset(void);

void SetPowerOff_ForDoing(void);

void SetPowerOn_ForDoing(void);

void ActionEvent_Handler(void);




void Single_Power_ReceiveCmd(uint8_t cmd);

void Read_TempSensor_Data(void);

void PowerOn_Run_Pro(void);


void PowerOff_Run_Pro(void);

void Decode_RunCmd(uint8_t cmdType_1, uint8_t cmdType_2);


#endif 



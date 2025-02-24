/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

#include "bsp.h"

#define USE_FAN_OLDER     0

// 温度阈值定义
#if USE_FAN_OLDER
#define PTC_TEMP_THRESHOLD 373 // 90 degree
#else
#define PTC_TEMP_THRESHOLD 197 // 119 degree
#endif

// 传输给显示板 命令定义
#define DISP_CMD_PTC_WARNING 		0x08
#define DISP_DATA_PTC_WARNING 	0x01

// 状态标志定义
#define  PTC_OVERHEAT_FLAG 	1
#define  PTC_NORMAL_FLAG 	0

//FAN DEFINE

// 风扇电压阈值定义
#if USE_FAN_OLDER
#define FAN_VOLTAGE_LOWER_THRESHOLD 400  // 下限阈值（旧版本）
#define FAN_VOLTAGE_UPPER_THRESHOLD 3000 // 上限阈值
#else
#define FAN_VOLTAGE_LOWER_THRESHOLD 350  // 下限阈值（新版本）
#define FAN_VOLTAGE_UPPER_THRESHOLD 3000 // 上限阈值
#endif

// DISPLAY BOARD 命令定义
#define DISP_CMD_FAN_WARNING 			0x09
#define DISP_DATA_FAN_WARNING 			0x01

// 状态标志定义
#define FAN_WARNING_FLAG 1
#define FAN_NORMAL_FLAG 0

// 蜂鸣器报警次数
#define BUZZER_ALERT_TIMES 4





static  uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);
static uint16_t Get_Adc_Channel(uint32_t ch)  ;
static  void HandlePtcOverheat(void) ;
static void HandleFanWarning(void);
static uint8_t IsFanVoltageNormal(uint16_t voltage);

uint8_t detect_error_times=0;



/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    GPIO_InitStruct.Pin = ADC_FAN_Pin|ADC_NTC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    HAL_GPIO_DeInit(GPIOA, ADC_FAN_Pin|ADC_NTC_Pin);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Channel(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;

	ADC1_ChanConf.Channel=ch;                                   //Í¨µÀ
    ADC1_ChanConf.Rank= ADC_REGULAR_RANK_1;                                    //第一个序�?
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_239CYCLES_5;      //²ÉÑùÊ±¼ä               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //Í¨µÀÅäÖÃ
	
    HAL_ADC_Start(&hadc1);                               //start ADC transmit
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
}
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
//static uint16_t Get_Adc_Average(uint8_t times)
//{
//	
//	HAL_ADC_Start(&hadc1);								
//    
//    temp_value= ADC1->DR;
//    return temp_value;
//}

static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc_Channel(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 



void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times)
{
 
	uint16_t adcx;
	adcx = Get_Adc_Average(channel,times);

    run_t.ptc_temp_voltage  =(uint16_t)((adcx * 3300)/4096); //amplification 100 ,3.11V -> 311

	if(run_t.open_ptc_detected_flag == 0){ //power on the voltage is small 
         run_t.open_ptc_detected_flag++ ;
         run_t.ptc_temp_voltage = 500;
    }
	#if DEBUG
      printf("ptc= %d",run_t.ptc_temp_voltage);
	#endif 

   if(run_t.ptc_temp_voltage < PTC_TEMP_THRESHOLD){
	  run_t.ptc_too_heat_value =1;
	  run_t.ptc_warning =1;
   }
}


/*****************************************************************
	*
	*Function Name: void Judge_PTC_Temperature_Value(void)
	*Function: PTC adc read voltage
	*Input Ref: NO
	*Return Ref: No
	*
	*
*****************************************************************/
void Judge_PTC_Temperature_Value(void)
{

	HandlePtcOverheat() ;
#if 0
	#if FAN_OLDER
	  if(run_t.ptc_temp_voltage < 373 || run_t.ptc_temp_voltage ==373){ //90 degree
	#else
		  if(run_t.ptc_temp_voltage < 197){ //119 degree
	#endif
	    run_t.gDry =0 ;
	    PTC_SetLow(); //turn off
        run_t.ptc_too_heat_value =1;
		run_t.ptc_warning =1;
        SendWifiData_To_Cmd(0x08,0x01) ;       //SendWifiCmd_To_Order(PTC_WARNING);

         buzzer_sound();//Buzzer_KeySound();

		HAL_Delay(200);
       buzzer_sound();//Buzzer_KeySound();
       HAL_Delay(100);
	   buzzer_sound();//Buzzer_KeySound();
       HAL_Delay(100);
	   buzzer_sound();//Buzzer_KeySound();
       HAL_Delay(100);
	   buzzer_sound();//Buzzer_KeySound();
       HAL_Delay(100);
   	      
   }
#endif
   
}

  /**
   * @brief 处理 PTC 温度过高逻辑
   * @param None
   * @return None
   */
static void HandlePtcOverheat(void)
{


  if( run_t.ptc_warning == PTC_OVERHEAT_FLAG){
	  // 关闭 PTC 加热器
	  run_t.gDry =0 ;
	  PTC_SetLow();

	  // 设置温度过高标志


	  // 发送 WiFi 警告命令
	  SendWifiData_To_Cmd(DISP_CMD_PTC_WARNING , DISP_DATA_PTC_WARNING);

	  // 触发蜂鸣器报警
	   buzzer_sound();//Buzzer_KeySound();

	   HAL_Delay(200);
	   buzzer_sound();//Buzzer_KeySound();
	   HAL_Delay(100);
	   buzzer_sound();//Buzzer_KeySound();
	   HAL_Delay(100);
	   buzzer_sound();//Buzzer_KeySound();
	   HAL_Delay(100);
	   buzzer_sound();//Buzzer_KeySound();
	   HAL_Delay(100);

  }
}

/*****************************************************************
	*
	*Function Name: void Get_Fan_Adc_Fun(uint8_t channel,uint8_t times)
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?, hexadecimal of average
	*Return Ref: No
	*
	*
*****************************************************************/
void Get_Fan_Adc_Fun(uint32_t channel,uint8_t times)
{
	uint16_t adc_fan_hex;
	
	
   Fan_CCW_Run_Max();
	adc_fan_hex = Get_Adc_Average(channel,times);

    run_t.fan_detect_voltage  =(uint16_t)((adc_fan_hex * 3300)/4096); //amplification 1000 ,3.111V -> 3111
	//HAL_Delay(5);
    if(IsFanVoltageNormal(run_t.fan_detect_voltage)){  //1 --正常  0 --检测风扇故障
    	detect_error_times =0; // 重置错误计数
    	run_t.fan_warning = FAN_NORMAL_FLAG; // 清除风扇警告标志

    }
    else {
        // 风扇电压异常
        if (detect_error_times > 0) {
            detect_error_times = 0; // 重置错误计数
            HandleFanWarning(); // 处理风扇警告
        }
        else
        	detect_error_times++; // 错误计数加 1
    }

}

/**
 * @brief 检测风扇电压是否正常
 * @param voltage 风扇电压值
 * @return uint8_t 1: 正常, 0: 异常
 */
static uint8_t IsFanVoltageNormal(uint16_t voltage)
{
    return (voltage > FAN_VOLTAGE_LOWER_THRESHOLD && voltage < FAN_VOLTAGE_UPPER_THRESHOLD);
}

/**
 * @brief 处理风扇警告逻辑
 * @param None
 * @return None
 */
static void HandleFanWarning(void)
{
    // 设置风扇警告标志
    run_t.fan_warning = FAN_WARNING_FLAG;

    // 关闭干燥功能
    run_t.gDry = 0;

    // 关闭 PTC 加热器
    PTC_SetLow();

    // 触发蜂鸣器报警
    for (uint8_t i = 0; i < BUZZER_ALERT_TIMES; i++) {
        buzzer_sound();
        HAL_Delay(i == BUZZER_ALERT_TIMES - 1 ? 100 : 200); // 最后一次延迟 100ms，其他延迟 200ms
    }

    // 发送 WiFi 警告命令
    SendWifiData_To_Cmd(DISP_CMD_FAN_WARNING, DISP_DATA_FAN_WARNING);
}



#if 0

#if FAN_OLDER
	if(run_t.fan_detect_voltage >400 &&  run_t.fan_detect_voltage < 3000){ //600
           detect_error_times=0;
		   #if DEBUG
             printf("adc= %d",run_t.fan_detect_voltage);
		   #endif 
           run_t.fan_warning = 0;
    }
#else
	if(run_t.fan_detect_voltage >350 &&  run_t.fan_detect_voltage < 3000){ //600
	           detect_error_times=0;
			   #if DEBUG
	             printf("adc= %d",run_t.fan_detect_voltage);
			   #endif
	           run_t.fan_warning = 0;
	  }
#endif
	else{

	          
			   if(detect_error_times >0){
			   		detect_error_times=0;
		           run_t.fan_warning = 1;
                   run_t.gDry =0 ;
             
	               PTC_SetLow(); //turn off
	             
        
	
				
			       HAL_Delay(200);
			       buzzer_sound();//Buzzer_KeySound();
			       HAL_Delay(100);
				   buzzer_sound();//Buzzer_KeySound();
			       HAL_Delay(100);
				   buzzer_sound();//Buzzer_KeySound();
			       HAL_Delay(100);
				   buzzer_sound();//Buzzer_KeySound();
			       HAL_Delay(100);
				   SendWifiData_To_Cmd(0x09,0x01);//SendWifiCmd_To_Order(FAN_WARNING);
				  

			   	}
	           detect_error_times++;

     }
}
#endif
/* USER CODE END 1 */

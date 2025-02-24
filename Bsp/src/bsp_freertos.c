#include "bsp.h"

#define DECODER_BIT_0        (1<< 0)
uint8_t inputBuf[1];



/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
//static void vTaskRunPro(void *pvParameters);
//static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/* 创建任务通信机制 */
//static void AppObjCreate(void);


/***********************************************************************************************************
											变量声明
***********************************************************************************************************/
//static TaskHandle_t xHandleTaskRunPro = NULL;
//static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;


static void ProcessUARTData(const uint8_t *data, uint8_t length);
static void HandlePowerOnLogic(void);
static void UART_ReceiveStateMachine(uint8_t data);




typedef struct Msg
{
	uint8_t ucMessageID;
	uint8_t usData[12];
	uint8_t link_wifi_net_flag;
    uint8_t rx_data_counter;
    uint8_t rx_end_flag;
    uint8_t  rx_end_counter;
    uint8_t  uid;

    uint8_t check_code;

    uint8_t bcc_check_code;
    uint8_t disp_rx_cmd_done_flag;
}UART_Message_T;

//MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */

typedef struct{
	UART_Message_T  uart_msg;
	uint8_t uart_state;
}AppData_T;

static AppData_T appData;

uint8_t decoder_flag;

uint8_t ulid,uldata,usdata;

//uint8_t test_counter;

/**********************************************************************************************************
*
*	Function Name:void freeRTOS_Handler(void)
*   Function :
*	
*  
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
     /* 创建任务 */
	  AppTaskCreate();
	  
	  /* 创建任务通信机制 */
	 //  AppObjCreate();
	  
	  /* 启动调度，开始执行任劄1�7 */
	   vTaskStartScheduler();


}

/**********************************************************************************************************
*	
*	Function Name:static void vTaskMsgPro(void *pvParameters)
*   Function : priority is 2
*   Input  Ref:
*	Return Ref:
*   Priority Ref: 2 (数值越小优先级越低，数字越大优先级越高)
*   
**********************************************************************************************************/
#if 0
static void vTaskRunPro(void *pvParameters)
{
	//vTaskDelay(pdMS_TO_TICKS(5000)); // 假设经过 5 秒后，条件变化
   // static uint8_t power_on_sound_flag ;
   static uint8_t power_on_sound_flag ;
    while(1)
    {
      
    

           if(power_on_sound_flag == 0){
                power_on_sound_flag ++;

                //Buzzer_KeySound(void);
                run_t.power_off_flag = 0xff;
                buzzer_sound();

            }



            if(run_t.RunCommand_Label== POWER_ON){

                mainboard_run_handler();

                Read_TempSensor_Data();

                works_two_hours_detected_handler();

            // test_counter ++;

            }
            else{

                PowerOff_Run_Pro();

            }

    
		
    
   
     vTaskDelay(20); 
     
    }

}
#endif 
/*
*********************************************************************************************************
*	Function Name: vTaskStart
*	Function: priority is 1
*	Input Ref:: pvParameters 是在创建该任务时传��的形参
*	Return Ref: 
*   Priority Ref: 1  (数��越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
  //  MSG_T *ptMsg;
	BaseType_t xResult;
    uint32_t ulValue;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(1000); /* 1.测试设定的-设置最大等待时间为50ms */
  // static uint8_t power_on_sound_flag ;
	
    while(1)
    {
		
      
         xResult = xTaskNotifyWait(0x00000000,      
						           0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间,等待时间-block portMAX_DELAY */
       if(xResult == pdPASS){

        if((ulValue & DECODER_BIT_0 ) != 0){

              ProcessUARTData(appData.uart_msg.usData, appData.uart_msg.rx_data_counter); // 处理UART数据

       
        }
        else{

            HandlePowerOnLogic() ;
        }

     }
    }
}
/**********************************************************************************************************
*	Function Name: ProcessUARTData
*	Function: 处理UART接收到的数据
*	Input Ref: 数据指针和数据长度
**********************************************************************************************************/
static void ProcessUARTData(const uint8_t *data, uint8_t length) {
    appData.uart_msg.disp_rx_cmd_done_flag = 0;
    appData.uart_msg.check_code = bcc_check(data, appData.uart_msg.uid);

    if (appData.uart_msg.check_code == appData.uart_msg.bcc_check_code) {
        receive_data_from_display(data); // 处理有效数据  //receive_data_fromm_display
    }
}
/**********************************************************************************************************
*	Function Name: HandlePowerOnLogic
*	Function: 处理电源逻辑
**********************************************************************************************************/
static void HandlePowerOnLogic(void)
{
    static uint8_t power_on_sound_flag = 0;

    if (power_on_sound_flag == 0) {
        power_on_sound_flag++;
        run_t.power_off_flag = 0xFF;
        buzzer_sound();
    }

    if (run_t.RunCommand_Label == POWER_ON) {
        mainboard_run_handler();
        Read_TempSensor_Data();
        works_two_hours_detected_handler();
    } else {
        PowerOff_Run_Pro();
    }
}

/**********************************************************************************************************
*	Function Name: UART_ReceiveStateMachine
*	Function: UART接收状态机
*	Input Ref: 接收到的数据
**********************************************************************************************************/
static void UART_ReceiveStateMachine(uint8_t data)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    switch (appData.uart_state) {
        case 0: // 初始状态，等待起始字节
            if (data == 0xA5) { // 0xA5 -- 显示命令头
                appData.uart_msg.rx_data_counter = 0;
                appData.uart_msg.usData[appData.uart_msg.rx_data_counter] = data;
                appData.uart_state = 1;
            }
            break;

        case 1: // 接收数据状态
            if (appData.uart_msg.disp_rx_cmd_done_flag == 0) {
                appData.uart_msg.rx_data_counter++;
                appData.uart_msg.usData[appData.uart_msg.rx_data_counter] = data;

                if (appData.uart_msg.rx_end_flag == 1) {
                    appData.uart_state = 0;
                    appData.uart_msg.uid = appData.uart_msg.rx_data_counter;
                    appData.uart_msg.rx_end_flag = 0;
                    appData.uart_msg.rx_data_counter = 0;
                    appData.uart_msg.disp_rx_cmd_done_flag = 1;
                    appData.uart_msg.bcc_check_code = data;

                    xTaskNotifyFromISR(xHandleTaskStart, /* 目标任务 */
                    		          DECODER_BIT_0, 	/* 设置目标任务事件标志位bit0  */
									  eSetBits, 		/* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
									  &xHigherPriorityTaskWoken);
                    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                }

                if (appData.uart_msg.usData[appData.uart_msg.rx_data_counter] == 0xFE &&
                    appData.uart_msg.rx_end_flag == 0 &&
                    appData.uart_msg.rx_data_counter > 4) {
                    appData.uart_msg.rx_end_flag = 1;
                }
            }
            break;
    }
}
/**********************************************************************************************************
*	凄1�7 敄1�7 各1�7: AppTaskCreate
*	功能说明: 创建应用任务
*	彄1�7    参：旄1�7
*	迄1�7 囄1�7 倄1�7: 旄1�7
**********************************************************************************************************/
void AppTaskCreate (void)
{

//	xTaskCreate( vTaskRunPro,    		/* 任务函数  */
//                 "vTaskRunPro",  		/* 任务各1�7    */
//                 128,         		/* stack大小，单位word，也就是4字节 */
//                 NULL,        		/* 任务参数  */
//                 1,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
//                 &xHandleTaskRunPro); /* 任务句柄  */
	
//	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
//                 "vTaskMsgPro",   		/* 任务各1�7    */
//                 128,             		/* 任务栈大小，单位word，也就是4字节 */
//                 NULL,           		/* 任务参数  */
//                 2,               		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
//                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�7    */
                 256,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,              		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */
}
#if 0
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     static uint8_t state;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // MSG_T *ptMsg;

	if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
	{
      // DISABLE_INT();
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0xA5){  // // 0xA5 --didplay command head
			    gl_tMsg.rx_data_counter=0;
				
				gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];
                state=1; //=1
             }   
            else
                state=0;
		break;
		case 1: //#1
            if(gl_tMsg.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;
		     
	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];
              

              if(gl_tMsg.rx_end_flag == 1){

                state = 0;
            
                gl_tMsg.uid = gl_tMsg.rx_data_counter;
                gl_tMsg.rx_end_flag=0;

                gl_tMsg.rx_data_counter =0;

                 gl_tMsg.disp_rx_cmd_done_flag = 1 ;

                state=0;

                gl_tMsg.bcc_check_code=inputBuf[0];

            

                #if 1

                xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                DECODER_BIT_0,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                #endif 
                  
              }

              }

              if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE && gl_tMsg.rx_end_flag == 0 &&gl_tMsg.  rx_data_counter > 4){
                     
                          gl_tMsg.rx_end_flag = 1 ;
                          
                        
               }
      
        
       
        break;
	
			
		}

    //    ENABLE_INT();
	    __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
		
	 }
    
  
 }
#endif
/**********************************************************************************************************
*	Function Name: HAL_UART_RxCpltCallback
*	Function: UART接收中断回调函数
*	Input Ref: UART_HandleTypeDef结构体指针
**********************************************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        UART_ReceiveStateMachine(inputBuf[0]); // 调用状态机处理数据
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        HAL_UART_Receive_IT(&huart1, inputBuf, 1); // UART接收数据中断，1字节
    }
}







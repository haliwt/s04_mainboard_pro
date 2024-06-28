/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
#if 0
//static void vTaskTaskUserIF(void *pvParameters);
//static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/* 创建任务通信机制 */
static void AppObjCreate(void);





/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;
//static QueueHandle_t xQueue3 = NULL;

//static QueueSetHandle_t 		xQueueSet;          /* 定义队列�?1�?7 */
//QueueHandle_t           		xQueue1;            /* 定义队列1 */
//QueueHandle_t           		xQueue2;            /* 定义队列2 */
//SemaphoreHandle_t       		xSemaphore;         /* 定义二��信号量 */

#define QUEUE_LENGTH            	1                   /* 队列支持的消息个�?1�?7 */
#define QUEUE_ITEM_SIZE         	sizeof(uint8_t)    /* 队列中每条消息的大小 */
#define SEMAPHORE_BINARY_LENGTH 	1                   /* 二��信号量的有效长�?1�?7 */
#define QUEUESET_LENGTH         	((2 * QUEUE_LENGTH) + SEMAPHORE_BINARY_LENGTH)  /* 队列集支持的消息个数 */


typedef struct MMsg
{
	uint8_t  ucMessageID;
	uint16_t usData[2];
	uint32_t ulData[2];
}MMSG_T;

MMSG_T   gg_tMsg; /* 定义丢�个结构体用于消息队列 */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
void StartTask04(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityBelowNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityBelowNormal, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, StartTask04, osPriorityAboveNormal, 0, 128);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
     /* 创建任务通信机制 */
	  AppObjCreate();
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
   
	MMSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* ÉèÖÃ×î´óµÈ´ýÊ±¼äÎª200ms */
    while(1)
    {


        xResult = xQueueReceive(xQueue2,                   /* ÏûÏ¢¶ÓÁÐ¾ä±ú */
		                        (void *)&ptMsg,  		   /* ÕâÀï»ñÈ¡µÄÊÇ½á¹¹ÌåµÄµØÖ· */
		                        portMAX_DELAY);/* ÉèÖÃ×èÈûÊ±¼ä */
		
		
		if(xResult == pdPASS)
		{
			/* ³É¹¦½ÓÊÕ£¬²¢Í¨¹ý´®¿Ú½«Êý¾Ý´òÓ¡³öÀ´ */
			//printf("½ÓÊÕµ½ÏûÏ¢¶ÓÁÐÊý¾ÝptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			//printf("½ÓÊÕµ½ÏûÏ¢¶ÓÁÐÊý¾ÝptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
			//printf("½ÓÊÕµ½ÏûÏ¢¶ÓÁÐÊý¾ÝptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
			//Decode_Function(ptMsg->ulData[0],ptMsg->usData[0]);

              /* ÏòÏûÏ¢¶ÓÁÐ·¢Êý¾Ý£¬Èç¹ûÏûÏ¢¶ÓÁÐÂúÁË£¬µÈ´ý10¸öÊ±ÖÓ½ÚÅÄ */
           // ptMsg->ucMessageID ++;
           run_t.buzzer_sound_flag =1;
           Update_DHT11_Value();
           xQueueSend(xQueue1,(void *) &ptMsg->ucMessageID,portMAX_DELAY);
					
					
		}
	
       // vTaskDelay(10);
    }
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void const * argument)
{
  /* USER CODE BEGIN StartTask04 */
  /* Infinite loop */
   MMSG_T   *ptMsg;
  for(;;)
  {


    if(gg_tMsg.ucMessageID==1){
        gg_tMsg.ucMessageID++;

        ptMsg->ucMessageID++;

        Decode_Function(gg_tMsg.ulData[0],gg_tMsg.usData[0]);
        xQueueSend(xQueue2,                  /* ÏûÏ¢¶ÓÁÐ¾ä±ú */
					(void *) &ptMsg,           /* ·¢ËÍ½á¹¹ÌåÖ¸Õë±äÁ¿ptMsgµÄµØÖ· */
					(TickType_t)10);
				

        }
         
       
	
        vTaskDelay(10);
    //osDelay(10);
  }
  /* USER CODE END StartTask04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/*
*********************************************************************************************************
*	�? �? �?: COMProcessHelp
*	功能说明:串口操作提示
*	�?    参：�?
*	�? �? �?: �?
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*	�?1�?7 �?1�?7 �?1�?7: vTaskTaskUserIF
*	功能说明: 接口消息处理�?1�?7
*	�?1�?7    �?1�?7: pvParameters 是在创建该任务时传��的形参
*	�?1�?7 �?1�?7 �?1�?7: �?1�?7
*   �?1�?7 �?1�?7 �?1�?7: 1  (数��越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
//void freeRTOS_Handler(void)
//{
//     /* 创建任务 */
//	  AppTaskCreate();
//	  
//	  /* 创建任务通信机制 */
//	  AppObjCreate();
//	  
//	  /* 启动调度，开始执行任�?1�?7 */
//	  vTaskStartScheduler();
//
//
//}


/*
*********************************************************************************************************
*	º¯ Êý Ãû: vTaskLED
*	¹¦ÄÜËµÃ÷: Ê¹ÓÃº¯ÊýxQueueReceive½ÓÊÕÈÎÎñvTaskTaskUserIF·¢ËÍµÄÏûÏ¢¶ÓÁÐÊý¾Ý(xQueue2)	
*	ÐÎ    ²Î: pvParameters ÊÇÔÚ´´½¨¸ÃÈÎÎñÊ±´«µÝµÄÐÎ²Î
*	·µ »Ø Öµ: ÎÞ
*   ÓÅ ÏÈ ¼¶: 2  
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
    MMSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* ÉèÖÃ×î´óµÈ´ýÊ±¼äÎª200ms */
    while(1)
    {


        xResult = xQueueReceive(xQueue2,                   /* ÏûÏ¢¶ÓÁÐ¾ä±ú */
		                        (void *)&ptMsg,  		   /* ÕâÀï»ñÈ¡µÄÊÇ½á¹¹ÌåµÄµØÖ· */
		                        portMAX_DELAY);/* ÉèÖÃ×èÈûÊ±¼ä */
		
		
		if(xResult == pdPASS)
		{
			/* ³É¹¦½ÓÊÕ£¬²¢Í¨¹ý´®¿Ú½«Êý¾Ý´òÓ¡³öÀ´ */
			//printf("½ÓÊÕµ½ÏûÏ¢¶ÓÁÐÊý¾ÝptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			//printf("½ÓÊÕµ½ÏûÏ¢¶ÓÁÐÊý¾ÝptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
			//printf("½ÓÊÕµ½ÏûÏ¢¶ÓÁÐÊý¾ÝptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
			//Decode_Function(ptMsg->ulData[0],ptMsg->usData[0]);

              /* ÏòÏûÏ¢¶ÓÁÐ·¢Êý¾Ý£¬Èç¹ûÏûÏ¢¶ÓÁÐÂúÁË£¬µÈ´ý10¸öÊ±ÖÓ½ÚÅÄ */
           // ptMsg->ucMessageID ++;
           run_t.buzzer_sound_flag =1;
           Update_DHT11_Value();
           xQueueSend(xQueue1,(void *) &ptMsg->ucMessageID,portMAX_DELAY);
					
					
		}
	
       // vTaskDelay(10);
    }

}
/*
*********************************************************************************************************
*	�?1�?7 �?1�?7 �?1�?7: vTaskMsgPro
*	功能说明: 消息处理，使用函数comGetChar获取串口命令，使用函数comSendBuf发��串口消�?1�?7
*	�?1�?7    �?1�?7: pvParameters 是在创建该任务时传��的形参
*	�?1�?7 �?1�?7 �?1�?7: �?1�?7
*   �?1�?7 �?1�?7 �?1�?7: 3  (数��越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
	

	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* ÉèÖÃ×î´óµÈ´ýÊ±¼äÎª300ms */
	uint8_t ucQueueMsgValue;
	
    while(1)
    {
       xResult = xQueueReceive(xQueue1,                   /* ÏûÏ¢¶ÓÁÐ¾ä±ú */
		                        (void *)&ucQueueMsgValue,  /* ´æ´¢½ÓÊÕµ½µÄÊý¾Ýµ½±äÁ¿ucQueueMsgValueÖÐ */
		                        xMaxBlockTime);/* ÉèÖÃ×èÈûÊ±¼ä */
		
		if(xResult == pdPASS)
		{
			/* ³É¹¦½ÓÊÕ£¬²¢Í¨¹ý´®¿Ú½«Êý¾Ý´òÓ¡³öÀ´ */
			//printf("½ÓÊÕµ½ÏûÏ¢¶ÓÁÐÊý¾ÝucQueueMsgValue = %d\r\n", ucQueueMsgValue);
		}
		else
		{
			/* ³¬Ê± */
			//bsp_LedToggle(1);
			//bsp_LedToggle(4);
			RunCommand_MainBoard_Fun();
		}
    

	 
     
    }

}
/*
*********************************************************************************************************
*	�?1�?7 �?1�?7 �?1�?7: vTaskStart
*	功能说明: 启动任务，也就是朢�高优先级任务，这里用作按键扫描��?1�?7
*	�?1�?7    �?1�?7: pvParameters 是在创建该任务时传��的形参
*	�?1�?7 �?1�?7 �?1�?7: �?1�?7
*   �?1�?7 �?1�?7 �?1�?7: 4  (数��越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{

   MMSG_T   *ptMsg;
    while(1)
    {


     /* °´¼üÉ¨Ãè */
		//bsp_KeyScan();
		if(gg_tMsg.ucMessageID==1){
        gg_tMsg.ucMessageID++;

        ptMsg->ucMessageID++;

        Decode_Function(gg_tMsg.ulData[0],gg_tMsg.usData[0]);
        xQueueSend(xQueue2,                  /* ÏûÏ¢¶ÓÁÐ¾ä±ú */
					(void *) &ptMsg,           /* ·¢ËÍ½á¹¹ÌåÖ¸Õë±äÁ¿ptMsgµÄµØÖ· */
					(TickType_t)10);
				

        }
         
       
	
        vTaskDelay(10);
    
}
}				
/*
*********************************************************************************************************
*	�?1�?7 �?1�?7 �?1�?7: AppTaskCreate
*	功能说明: 创建应用任务
*	�?1�?7    参：�?1�?7
*	�?1�?7 �?1�?7 �?1�?7: �?1�?7
*********************************************************************************************************
*/
void AppTaskCreate (void)
{
//    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
//                 "vTaskUserIF",     	/* 任务�?1�?7    */
//                 128,               	/* 任务栈大小，单位word，也就是4字节 */
//                 NULL,              	/* 任务参数  */
//                 1,                 	/* 任务优先�?1�?7 数��越小优先级越低，这个跟uCOS相反 */
//                 &xHandleTaskUserIF );  /* 任务句柄  */
//	
//	
	xTaskCreate( vTaskLED,    		/* 任务函数  */
                 "vTaskLED",  		/* 任务�?1�?7    */
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 1,           		/* 任务优先�?1�?7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskLED ); /* 任务句柄  */
	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务�?1�?7    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先�?1�?7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务�?1�?7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先�?1�?7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */
}



/*
*********************************************************************************************************
*	�?1�?7 �?1�?7 �?1�?7: AppObjCreate
*	功能说明: 创建任务通信机制
*	�?1�?7    �?1�?7: �?1�?7
*	�?1�?7 �?1�?7 �?1�?7: �?1�?7
*********************************************************************************************************
*/
void AppObjCreate (void)
{
    #if 1

   /* 创建10个uint8_t型消息队�?1�?7 */
	xQueue1 = xQueueCreate(5, sizeof(uint8_t));
    if( xQueue1 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机�?1�?7 */
    }
	
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核�?1�?732位机，一个指针变量占�?1�?74个字�?1�?7 */
	xQueue2 = xQueueCreate(5, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机�?1�?7 */
    }

	

	#endif 

    #if 0

	 /* 创建队列�?1�?7 */
    xQueueSet = xQueueCreateSet(QUEUESET_LENGTH);
    /* 创建队列*/
    xQueue1 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    xQueue2 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
	
    /* 创建二��信号量 */
    xSemaphore = xSemaphoreCreateBinary();
	
    /* 将队列和二��信号量添加到队列集�?1�?7 */
    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);
    xQueueAddToSet(xSemaphore, xQueueSet);
    #endif 
}


/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    static uint8_t state;
//     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//     MMSG_T *ptMsg;

//	if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
//	{
//      
//		switch(state)
//		{
//		case 0:  //#0
//			if(inputBuf[0] == 'T')  //hex :54 - "T" -fixed
//				state=1; //=1
//            else
//                state=0;
//		break;
//		case 1: //#1
//             if(inputBuf[0] == 'K'){//hex :4B - "K" -fixed
//               
//				state=2; //=1

//             }
//			else{
//			   state =0;
//			}
//			break;
//            
//        case 2:
//			 if(inputBuf[0] == 'P' || inputBuf[0] == 'C' ||inputBuf[0] == 'Z'){
//			    inputCmd[0]= inputBuf[0];
//				 state =3;

//			 }
//			 else
//                 state =0;
//        
//        break;
//        
//        case 3:
//		    
//	           inputCmd[1]= inputBuf[0];

//          //     {
////               /* ÏòÏûÏ¢¶ÓÁÐ·¢Êý¾Ý */
////	            xQueueSendFromISR(xQueue3,
////				       &g_tMsg.voice_data[2],
////				       &xHigherPriorityTaskWoken);
////
////	                /* Èç¹ûxHigherPriorityTaskWoken = pdTRUE£¬ÄÇÃ´ÍË³öÖÐ¶ÏºóÇÐµ½µ±Ç°×î¸ßÓÅÏÈ¼¶ÈÎÎñÖ´ÐÐ */
////	            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

//                    gg_tMsg.ucMessageID =1;

//                    
//					gg_tMsg.ulData[0]=inputCmd[0];
//					gg_tMsg.usData[0]=inputCmd[1];
//					
////					/* Ê¹ÓÃÏûÏ¢¶ÓÁÐÊµÏÖÖ¸Õë±äÁ¿µÄ´«µÝ */
////					xQueueSendFromISR(xQueue2, 
////					                    (void *) &g_tMsg, 
////					                    &xHigherPriorityTaskWoken);
////                    
////					portYIELD_FROM_ISR(xHigherPriorityTaskWoken) ;    /* ·¢ËÍ½á¹¹ÌåÖ¸Õë±äÁ¿ptMsgµÄµØÖ· */
////			  //}
//			
//	       
//		 
//			 
//	         state = 0;
//        
//        break;

//	
//			
//		}
//	    __HAL_UART_CLEAR_OREFLAG(&huart1);
//		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
//		
//	 }
//    
//  
// }

#endif 
/* USER CODE END Application */


#include "bsp.h"



//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//   uint32_t temp;
//
//    if(huart->Instance==USART1){
//	
//		if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_ORE)!=RESET){
//
//		__HAL_UART_CLEAR_OREFLAG(&huart1);
//		UART_Start_Receive_IT(&huart1,inputBuf,1);
//
//		}
//		__HAL_UNLOCK(&huart1);
//        __HAL_UART_CLEAR_OREFLAG(&huart1);
//		  
//		temp = USART1->RDR;
//		UART_Start_Receive_IT(&huart1,inputBuf,1);
//	
//		}
//
//
//
//
//}


/**
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim3 interrupt call back function
  * Tim3 timer :timing time 10ms
  * 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint16_t tm0;
    if(htim->Instance==TIM17){
		
	   tm0 ++ ;

       if(tm0>999){

         tm0 =0;
         run_t.gTimer_senddata_panel++;
         run_t.gTimer_read_dht11_temp_value++;
         run_t.gTimer_ptc_adc_times ++;
    
         run_t.gTimer_continuce_works_time++;
         run_t.gTimer_fan_adc_times++;
     
         run_t.gTimer_fan_run_one_minutes++;
         run_t.gTime_run_board_action++;
         



       }
     
	 
		
	 

	  
	  
	   
	 
	}
 }
#if 0
/**
  * 函数功能：重新定向c库函数printf到DEBUG_USARTx
  * ÊäÈë²ÎÊý: ÎÞ
  * ·µ »Ø Öµ: ÎÞ
  * Ëµ    Ã÷£ºÎÞ
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * 函数功能： 重新定向c库函数getchar,scanf到DEBUG_USARTx
  * ÊäÈë²ÎÊý: ÎÞ
  * ·µ »Ø Öµ: ÎÞ
  * Ëµ    Ã÷£ºÎÞ
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart2,&ch, 1, 0xffff);
  return ch;
}

#endif 
 

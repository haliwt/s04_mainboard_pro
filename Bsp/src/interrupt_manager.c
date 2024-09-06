#include "bsp.h"



void usart1_error_clear_fun(UART_HandleTypeDef *huart)
{
   uint32_t temp;


   if(run_t.gTimer_usart1_rx_clear_data > 5){
     run_t.gTimer_usart1_rx_clear_data=0;
    if(huart->Instance==USART1){
	
		
		
        __HAL_UART_CLEAR_OREFLAG(&huart1);
		  
		temp = USART1->RDR;
		UART_Start_Receive_IT(&huart1,inputBuf,1);
	
		}


    }

}

/**
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim3 interrupt call back function
  * Tim3 timer :timing time 10ms
  * 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint16_t tm0,tm1;
    if(htim->Instance==TIM17){
		
	   tm0 ++ ;

       if(tm0>999){

         tm0 =0;
         tm1++;
         run_t.gTimer_senddata_panel++;
         run_t.gTimer_read_dht11_temp_value++;
         run_t.gTimer_ptc_adc_times ++;
    
         
         run_t.gTimer_fan_adc_times++;
     
         run_t.gTimer_fan_run_one_minutes++;
         run_t.gTime_run_board_action++;
         run_t.gTimer_usart1_rx_clear_data++;
         
        if(tm1> 59){ // 1 minute

          tm1 =0; 

          run_t.gTimer_continuce_works_time ++ ;


        }


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
 

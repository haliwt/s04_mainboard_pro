#include "bsp.h"



#define MAX_BUFFER_SIZE  30




uint8_t  inputBuf[5];
uint8_t  inputCmd[5];
uint8_t  wifiInputBuf[1];

uint32_t temp;


static uint8_t transferSize;
static uint8_t outputBuf[MAX_BUFFER_SIZE];

volatile uint8_t transOngoingFlag;
volatile uint8_t usart2_transOngoingFlag;





/*****************************************************************************
	*
	*Function Name:void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart)
    *Function: receive dsipay panel of order
    *Input Ref:NO
    *Return Ref:NO

******************************************************************************/ 
void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart)
{
   
  if(huart->Instance==USART1){
    

	  if(run_t.gTimer_usart_error >5){
	  	run_t.gTimer_usart_error=0;

        __HAL_UART_CLEAR_OREFLAG(&huart1);
		
        
        //  temp = USART1->ISR;
          temp = USART1->RDR;
  
		    
          
	       UART_Start_Receive_IT(&huart1,inputBuf,1);
          
         
          
      }
  }
         
}
//void USART2_Cmd_Error_Handler(UART_HandleTypeDef *huart)
//{
//   uint32_t temp;
//  if(huart->Instance==USART2){
//    
//
//	  if(run_t.gTimer_usart_error_2 >37){
//	  	run_t.gTimer_usart_error_2=0;
//
//          __HAL_UART_CLEAR_OREFLAG(&huart2);
//          temp= USART2->RDR;
//		    
//          
//	   }
//	  }
//         
//}

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
{

	outputBuf[0]=0x5A; //main board head = 0xA5
	outputBuf[1]=0x10; //display device Number:is 0x01
	outputBuf[2]=0x1A; // command type = 0x1A -> temperature of value 
	outputBuf[3]=0x0f; // command order -> 0x0f -> is data , don't order.
	outputBuf[4]=0x02; // data is length: 00 ->don't data 
	outputBuf[5]=hum; // frame of end code -> 0xFE.
	outputBuf[6]=temp; // frame of end code -> 0xFE.
	outputBuf[7]=0xFE; // frame of end code -> 0xFE.
    outputBuf[8] = bcc_check(outputBuf,8);
		
		transferSize=9;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}



}
void SendWifiData_To_OnlyTemp(uint8_t temp)
{
   
	
	outputBuf[0]=0x5A; //main board head = 0xA5
	outputBuf[1]=0x10; //display device Number:is 0x01
	outputBuf[2]=0x1A; // command type = 0x1A -> temperature of value 
	outputBuf[3]=0x0f; // command order -> 0x0f -> is data , don't order.
	outputBuf[4]=0x01; // data is length: 00 ->don't data 
	outputBuf[5]=temp; // frame of end code -> 0xFE.
	
	outputBuf[6]=0xFE; // frame of end code -> 0xFE.
    outputBuf[7] = bcc_check(outputBuf,7);
		
		transferSize=8;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}



}
/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void SendWifiData_To_OnlyHumidity(uint8_t hum)
{
   
        outputBuf[0]=0x5A; //main board head = 0xA5
        outputBuf[1]=0x10; //display device Number:is 0x01
        outputBuf[2]=0x1B; // command type = 0x1A -> temperature of value 
        outputBuf[3]=0x0f; // command order -> 0x0f -> is data , don't order.
        outputBuf[4]=0x02; // data is length: 00 ->don't data 
        outputBuf[5]=hum; // frame of end code -> 0xFE.
        outputBuf[6]=0xFE; // frame of end code -> 0xFE.
        outputBuf[7] = bcc_check(outputBuf,7);
            
            transferSize=8;
            if(transferSize)
            {
                while(transOngoingFlag);
                transOngoingFlag=1;
                HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
            }

}

/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void SendWifiCmd_To_Order(uint8_t odata)
{
  

			outputBuf[0]='M'; //mainboard
			outputBuf[1]='A'; //A ->
			outputBuf[2]='E'; //'C' ->control 
			outputBuf[3]=odata; //	
		
			transferSize=4;
			if(transferSize)
			{
				while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
				transOngoingFlag=1;
				HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
			}



}

void SendData_Real_GMT(uint8_t hdata,uint8_t mdata,uint8_t sdata)
{
   

	outputBuf[0]='M'; //4D
	outputBuf[1]='A'; //41
	outputBuf[2]='B'; //44	// 'C' ->control 
	outputBuf[3]=hdata; //	
	outputBuf[4]=mdata; //
	outputBuf[5] =sdata;
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=6;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}

}
 
/********************************************************************************
	**
	*Function Name:void Answering_Signal_USART1_Handler(void)
	*Function :UART1 acknowleg 
	*Input Ref: cmddata =0,command ,cmddata =1,data
	*Return Ref:NO
	*
*******************************************************************************/
void Answering_Signal_USART1_Handler(uint8_t cmd,uint8_t data)
{
    outputBuf[0]=0x55; //head :0x55
	outputBuf[1]=0x42;   //'B'      
	outputBuf[2]=0X4B;   //'K'; 
	outputBuf[3]=cmd;
	outputBuf[4]=data; //

	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=5;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}

}


/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}

//	if(huart== &huart2){
//
//       usart2_transOngoingFlag =0;
//
//	}

}

#if 0

/**
  * º¯Êý¹¦ÄÜ: ÖØ¶¨Ïòc¿âº¯Êýprintfµ½DEBUG_USARTx
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
  * º¯Êý¹¦ÄÜ: ÖØ¶¨Ïòc¿âº¯Êýgetchar,scanfµ½DEBUG_USARTx
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




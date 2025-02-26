#include "bsp.h"



#define MAX_BUFFER_SIZE  30


static uint8_t PrepareFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength, uint8_t frameType) ;
static uint8_t PrepareAnswerFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength, uint8_t frameType);



// 缓冲区定义缓冲区
static uint8_t outputBuf[MAX_BUFFER_SIZE]; // 输出缓冲区

// 传输状态标志
static volatile uint8_t transOngoingFlag = 0; // USART1 传输标志
static volatile uint8_t usart2_transOngoingFlag = 0; // USART2 传输标志

// 传输数据长度
static uint8_t transferSize = 0;


/******************************************************************************
    * Function Name: USART1_Cmd_Error_Handler
    * Function: 处理 USART1 错误
    * Input Ref: UART_HandleTypeDef 指针
******************************************************************************/
void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1 && run_t.gTimer_usart_error > 5) {
        run_t.gTimer_usart_error = 0; // 重置错误计时器

        __HAL_UART_CLEAR_OREFLAG(&huart1); // 清除溢出标志
        uint32_t temp = USART1->RDR; // 读取接收数据寄存器

        UART_Start_Receive_IT(&huart1, inputBuf, 1); // 重新启动接收
    }
}


/******************************************************************************
    * Function Name: SendData
    * Function: 发送数据到 USART1
    * Input Ref: 数据缓冲区指针和数据长度
******************************************************************************/
static void SendData(uint8_t *data, uint8_t length)
{
    if (length > 0) {
        while (transOngoingFlag); // 等待上一次传输完成
        transOngoingFlag = 1; // 设置传输标志
        HAL_UART_Transmit_IT(&huart1, data, length); // 启动传输
    }
}

/******************************************************************************
    * Function Name: PrepareFrame
    * Function: 准备数据帧
    * Input Ref: 命令、数据、数据长度和帧类型
    * Output Ref: 数据帧长度
******************************************************************************/
static uint8_t PrepareAnswerFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength, uint8_t frameType)
{
    outputBuf[0] = 0x5A; // 帧头
    outputBuf[1] = 0x10; // 设备号
    outputBuf[2] = 0xFF; // 设备号
    outputBuf[3] = cmd;  // 命令类型
    outputBuf[4] = frameType ; //帧类型：00-open/clouse ,0x01- open/link/turn on 0x02-not-ai-mode , 0x10--多指令，0x0f:数据不是指令。 0xff：主板和显示板，通讯测试模式
    outputBuf[5] = dataLength; // 数据长度

    if (dataLength > 0) {
        memcpy(&outputBuf[6], data, dataLength); // 拷贝数据
         outputBuf[7+ dataLength] = 0xFE; // 帧尾
         outputBuf[8 + dataLength] = bcc_check(outputBuf, 8 + dataLength); // BCC 校验
          
        return 9 + dataLength; // 返回帧长度
    }
    else{

        outputBuf[6] = 0xFE;
        outputBuf[7] = bcc_check(outputBuf, 7); // BCC 校验

        return 8 ; // 返回帧长度
    }

}
/******************************************************************************
    * Function Name: PrepareFrame
    * Function: 准备数据帧
    * Input Ref: 命令、数据、数据长度和帧类型
    * Output Ref: 数据帧长度
******************************************************************************/
static uint8_t PrepareFrame(uint8_t cmd, uint8_t *data, uint8_t dataLength, uint8_t frameType)
{
    outputBuf[0] = 0x5A; // 帧头
    outputBuf[1] = 0x10; // 设备号
    outputBuf[2] = cmd;  // 命令类型
    outputBuf[3] = frameType ; //帧类型：00-open/clouse ,0x01- open/link/turn on 0x02-not-ai-mode , 0x10--多指令，0x0f:数据不是指令。 0xff：主板和显示板，通讯测试模式
    outputBuf[4] = dataLength; // 数据长度

    if (dataLength > 0) {
        memcpy(&outputBuf[5], data, dataLength); // 
        outputBuf[5 + dataLength] = 0xFE; // 帧尾
        outputBuf[6 + dataLength] = bcc_check(outputBuf, 6 + dataLength); // BCC 校验

        return 7 + dataLength; // 返回帧长度
    }
    else{
       outputBuf[5] = 0xFE; // 帧尾
       outputBuf[6] = bcc_check(outputBuf, 6); // BCC 校验
    
       return 7 ; //  返回帧长度


    }

  
}
/******************************************************************************
    * Function Name: sendData_Real_TimeHum
    * Function: 发送实时湿度和温度数据
    * Input Ref: 湿度值 (hum) 和温度值 (temp)
******************************************************************************/
void sendData_Real_TimeHum(uint8_t hum, uint8_t temp)
{
    uint8_t data[2] = {hum, temp}; // 湿度和温度数据
    transferSize = PrepareFrame(0x1A, data, 2, 0x0F); // 准备数据帧 0x0F --表示传输的是数据
    SendData(outputBuf, transferSize); // 发送数据
}

/******************************************************************************
    * Function Name: SendData_Set_Command
    * Function: 发送设置命令
    * Input Ref: 命令 (cmd) 和数据 (data)
******************************************************************************/
void SendData_Set_Command(uint8_t cmd,uint8_t frameType )
{
   // uint8_t dataArray[1] = {data}; // 命令数据
    transferSize = PrepareFrame(cmd, 0, 0, frameType); // 指令：数据是0，长度是0：
    SendData(outputBuf, transferSize); // 发送数据
}


/******************************************************************************
    * Function Name: SendWifiData_Answer_Cmd
    * Function: 发送 WiFi 应答命令
    * Input Ref: 命令 (cmd) 和数据 (data)
******************************************************************************/
void SendWifiData_Answer_Cmd(uint8_t cmd,uint8_t frameType)
{
   // uint8_t dataArray[1] = {data}; // 命令数据
    transferSize = PrepareAnswerFrame(cmd,0,0,frameType); // 指令：数据是0，长度是0：
    SendData(outputBuf, transferSize); // 发送数据
}

/******************************************************************************
    * Function Name: SendWifiData_To_Cmd
    * Function: 发送 WiFi 命令
    * Input Ref: 命令 (cmd) 和数据 (data)
******************************************************************************/
void SendWifiData_To_Cmd(uint8_t cmd,uint8_t frameType)
{
    //uint8_t dataArray[1] = {data}; // 命令数据
    transferSize = PrepareFrame(cmd,0,0, frameType); // 指令：数据是0，长度是0：
    SendData(outputBuf, transferSize); // 发送数据
}

/******************************************************************************
    * Function Name: SendWifiData_To_Data
    * Function: 发送 WiFi 数据
    * Input Ref: 命令 (cmd) 和数据 (data)
******************************************************************************/
void SendWifiData_To_Data(uint8_t cmd, uint8_t data,uint8_t datalen)
{
    uint8_t dataArray[1] = {data}; // 数据
    transferSize = PrepareFrame(cmd, dataArray, datalen, 0x0F); // 传输的是数据：帧类型：0x0F -->is data don't command备数据帧
    SendData(outputBuf, transferSize); // 发送数据
}

/******************************************************************************
    * Function Name: HAL_UART_TxCpltCallback
    * Function: UART 发送完成回调函数
    * Input Ref: UART_HandleTypeDef 指针
******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        transOngoingFlag = 0; // 清除传输标志
    }
}

#if 0
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

	//crc=0x55;
	outputBuf[0]=0x5A; //head : mainboard Board = 0x5A
	outputBuf[1]=0x10; //main board device No: 0x10
	outputBuf[2]=0x1A; //command : temperature of value 
	outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
	outputBuf[4]= 0x02; //data of length: 0x01 - 2 byte.
	outputBuf[5] =hum;
    outputBuf[6] =temp;

    outputBuf[7] = 0xFE;
    outputBuf[8] = bcc_check(outputBuf,8);
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=9;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}



}



/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Set_Command(uint8_t cmd,uint8_t data)
{
    outputBuf[0]=0x5A; //display board head = 0xA5
	outputBuf[1]= 0x10; //display device Number:is 0x01
	outputBuf[2]=cmd; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]= data; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
    outputBuf[6] = bcc_check(outputBuf,6);


		transferSize=7;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}


/********************************************************************************
    **
    *Function Name
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data)
{
        outputBuf[0]=0x5A; //head : main board 0x5A
        outputBuf[1]=0x10; //main board device No: 0x10
        outputBuf[2]=0xff; // answer cmd :
        outputBuf[3]= cmd_name;
        outputBuf[4]= cmd_function_code; // cmd_name what's cmd name ?
        outputBuf[5]= datalength; //if datalength = 0;
        if(datalength ==0){
            
           outputBuf[6] = 0xFE; //frame is end of byte.
           outputBuf[7] = bcc_check(outputBuf,6);

        }
        else{
            outputBuf[6]= data; //

            outputBuf[7] = 0xFE; //frame is end of byte.
            outputBuf[8] = bcc_check(outputBuf,6);
         }
        
        transferSize=9;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
	
}

/********************************************************************************
    **
    *Function Name:void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
{
        outputBuf[0]=0x5A; //head : main board 0x5A
        outputBuf[1]=0x10; //main board device No: 0x10
        outputBuf[2]=cmd; //command type: fan speed of value 
        outputBuf[3]=data; // 0x0F : is data ,don't command order.
        outputBuf[4]= 0x0; // don't data ,onlay is command order,recieve data is 1byte .
       
        outputBuf[5] = 0xFE; //frame is end of byte.
        outputBuf[6] = bcc_check(outputBuf,6);
        
        transferSize=7;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
	
}

/********************************************************************************
    **
    *Function Name:void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_Data(uint8_t cmd,uint8_t data)
{
        outputBuf[0]=0x5A; //head : main board 0x5A
        outputBuf[1]=0x10; //main board device No: 0x10
        outputBuf[2]=cmd; //command type: fan speed of value 
        outputBuf[3]=0x0F; // 0x0F : is data ,don't command order.
        outputBuf[4]= 0x01; // don't data ,onlay is command order,recieve data is 1byte .
        outputBuf[5]= data; // don't data 
        
        outputBuf[6] = 0xFE;
        outputBuf[7] = bcc_check(outputBuf,7);
        
        transferSize=8;
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
#endif
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




#ifndef __BSP_CMD_LINK_H_
#define __BSP_CMD_LINK_H_
#include "main.h"

#define USART_CMD_NUMBERS   25
#define USART_WIFI_NUMBERS  25


typedef enum {
    
     WIFI_SUCCESS ,
     WIFI_FAIL 

}wifi_enum_state_type;

typedef enum {
    WIFI_CLOUD_FAIL,
    WIFI_CLOUD_SUCCESS,
    WIFI_DEFAULT_POWER_OFF

}wifi_enum_link_cloud_state;
typedef struct _usart_cmd_t {
    uint8_t usart_cmd[USART_CMD_NUMBERS];
    uint8_t usart_cmd_len;
    uint8_t cmd_data[1];

}USART_CMD_T;





extern uint8_t inputBuf[5];

extern uint8_t  inputCmd[5];
extern uint8_t wifiInputBuf[1];



//void sendData_Reference_Data(uint8_t dry,uint8_t ultra,uint8_t mouse);


void sendData_Real_TimeHum(uint8_t hum,uint8_t temp);


void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data);

void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data);

void sendData_Real_TimeHum(uint8_t hum,uint8_t temp);

void SendWifiData_To_Data(uint8_t cmd,uint8_t data);




void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart);
void USART2_Cmd_Error_Handler(UART_HandleTypeDef *huart);

void Answering_Signal_USART1_Handler(uint8_t cmdordata,uint8_t data);

void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data);



#endif 

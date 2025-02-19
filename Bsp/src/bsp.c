#include "bsp.h"

void bsp_Init(void)
{

buzzer_init();

}

/**********************************************************************
*
*Function Name:void send_cmd_ack_hanlder(void);
*Function: 
*Input Ref:NO
*Return Ref:NO
*
**********************************************************************/
#if 0
void send_cmd_ack_hanlder(void)
{

    
    switch(gpro_t.send_ack_cmd){
    
        case ack_null:
    
    
        break;
    
        case ack_app_power_on:
            
          if(gpro_t.receive_copy_cmd == ack_app_power_on){
             gpro_t.receive_copy_cmd =0;
             gpro_t.send_ack_cmd = 0;
            
          }
          else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
             gpro_t.gTimer_again_send_power_on_off =0;
              SendWifiData_To_Cmd(0x31,0x01); //smart phone is power on
          }
                    
         
        break;
    
        case ack_app_power_off :
    
         if(gpro_t.receive_copy_cmd == ack_app_power_off){
            gpro_t.receive_copy_cmd =0;
             gpro_t.send_ack_cmd = 0;
             
          }
          else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
              gpro_t.gTimer_again_send_power_on_off =0;
               SendWifiData_To_Cmd(0x31,0x0); //smart phone is power off
          }
    
    
        break;

        case ack_app_timer_power_on:

           if(gpro_t.receive_copy_cmd == ack_app_timer_power_on){
             gpro_t.receive_copy_cmd =0;
             gpro_t.send_ack_cmd = 0;
            
          }
          else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
             gpro_t.gTimer_again_send_power_on_off =0;
              SendWifiData_To_Cmd(0x21,0x01); //smart phone is power on
          }

        break;
    
        case ack_wifi_on:
    
          if(gpro_t.receive_copy_cmd == ack_wifi_on){
             gpro_t.receive_copy_cmd =0;
             gpro_t.send_ack_cmd = 0;
            
          }
          else if(gpro_t.receive_copy_cmd != 0 && gpro_t.gTimer_again_send_power_on_off >1){
             gpro_t.gTimer_again_send_power_on_off =0;
             SendWifiData_To_Data(0x1F,0x0);
          }
    
    
        break;
    
        case ack_ptc_on:
    
    
        break;
    
        case ack_ptc_off:
    
        break;
    
    
        }


}

#endif 
/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC校验函数
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}



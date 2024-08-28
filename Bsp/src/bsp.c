#include "bsp.h"

void bsp_Init(void)
{

buzzer_init();

}

/**********************************************************************
    *
    *Function Name:void receive_data_fromm_display(uint8_t *pdata,uint8_t len)
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void receive_data_fromm_display(uint8_t *pdata)
{

  // static uint8_t first_power_on ;

   if(pdata[1] == 0x01){

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示开机指令

        if(pdata[3] == 0x01){ //open
           buzzer_sound();
           
           run_t.RunCommand_Label= POWER_ON;


        }
        else if(pdata[3] == 0x0){ //close 
           buzzer_sound();
           run_t.RunCommand_Label=POWER_OFF;


        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){
          buzzer_sound();

       if(run_t.interval_time_stop_run==0){
          run_t.gDry = 1;
          PTC_SetHigh();
        }
       }
       else if(pdata[3] == 0x0){
         buzzer_sound();
         run_t.gDry =0;
          PTC_SetLow();

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
           
           buzzer_sound();
             if(run_t.interval_time_stop_run==0){
               run_t.gPlasma = 1;
               PLASMA_SetHigh(); //
            }
        }
        else if(pdata[3] == 0x0){
          buzzer_sound();

          run_t.gPlasma = 0;
           PLASMA_SetLow(); //

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01 ){  //open 
            buzzer_sound();
           if(run_t.interval_time_stop_run==0){
           gpro_t.gmouse =1;
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 

            }

        }
        else if(pdata[3] == 0x0){ //close 
           buzzer_sound();
            gpro_t.gmouse= 0;
           HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
        }


     break;

      case 0x05: // link wifi command

        if(pdata[3] == 0x01){  // link wifi 

             

        }
        else if(pdata[3] == 0x0){ //don't link wifi 



        }


     break;

     case 0x06: //buzzer sound done

        if(pdata[3] == 0x01){  //buzzer sound 
             buzzer_sound();

        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .



        }


     break;

     case 0x07 : //AI mode 


         if(pdata[3] == 0x01){  //buzzer sound 
                 buzzer_sound();
                 if(run_t.interval_time_stop_run==0){
                     gpro_t.gai_modle_flag = 1; //???
                     run_t.gDry = 1;
                     gpro_t.gmouse = 1;
                     run_t.gPlasma = 1;
                   
                     PTC_SetHigh();
                     PLASMA_SetHigh(); //
                     HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
                 }
     
            }
            else if(pdata[3] == 0x0){ // don't buzzer sound .
     
              buzzer_sound();
              gpro_t.gai_modle_flag = 0;
     
            }


     break;


      case 0x1A: //温度数据

        if(pdata[3] == 0x0F){ //数据

        

        }
      break;

      case 0x1B: //湿度数据

        if(pdata[3] == 0x0F){ //数据
            

        }
      break;

      case 0x1C: //表示时间：小时，分，秒

        if(pdata[3] == 0x0F){ //数据

           


        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[3] == 0x0F){ //数据

             
            

        }
      break;
     
     }

   }

}


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






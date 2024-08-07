#include "bsp.h"


RUN_T run_t; 
action_state gpro_t;


//static void Single_Power_ReceiveCmd(uint8_t cmd);
static void Single_Command_ReceiveCmd(uint8_t cmd); 
static void Fan_ContinueRun_OneMinute_Fun(void);

static void Works_Rest_Cycle_TenMinutes(void);



volatile uint8_t run_state =0;



uint8_t power_off_flag;
uint8_t power_on_flag;



/**********************************************************************
*
*Function Name:void Decode_RunCmd(void)
*Function: receive usart touchkey of command 
*Input Ref:NO
*Return Ref:NO
*
**********************************************************************/
void Decode_RunCmd(uint8_t cmdType_1, uint8_t cmdType_2)
{

   switch(cmdType_1){
  
      case 'P' : //'P': //power on and off
        
      	Single_Power_ReceiveCmd(cmdType_2);  
           
      break;
      

	
        
      case 'C':// 0x43 = 67
           if(run_t.gPower_On==POWER_ON){
              Single_Command_ReceiveCmd(cmdType_2); //Single_ReceiveCmd(cmdType_2); 
              #if DEBUG
	        	printf("rx_dp=%d\n",cmdType_2);
	         #endif 
              
           }
     
         
      break;

	 
       case 'Z' ://buzzer sound 
	    if(run_t.gPower_On==POWER_ON){

		    if(cmdType_2== 'Z'){//turn off AI
		       run_t.buzzer_sound_flag = 1;
			    
			}
			 
		
		}

		case 'T': //works time with works two hours have a rest ten minutes
		   if(run_t.gPower_On==POWER_ON){
			

		   }
		break;
 	
}   
}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
void Single_Power_ReceiveCmd(uint8_t cmd)
{
  
 
    switch(cmd){

    case 0x00: //power off
      
	
	   run_t.RunCommand_Label= POWER_OFF;
       run_t.run_masin_process_step =0;
       Answering_Signal_USART1_Handler(COMMAND_ID,ANSWER_POWER_OFF);

 

    cmd = 0xff;
    break;

    case 0x01: // power on
      
        
		 // printf("pon\n");
         run_t.RunCommand_Label= POWER_ON;
         run_t.run_masin_process_step =0;
        Answering_Signal_USART1_Handler(COMMAND_ID,ANSWER_POWER_ON);
      
     cmd=0xff;  
     break;

    

     default:

     break;

    }

}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Single_Command_ReceiveCmd(uint8_t cmd)
{

    
	switch(cmd){

	   case DRY_ON_NO_BUZZER:

	  case DRY_ON:
         run_t.gDry = 1;
         PTC_SetHigh();

       break;

	   case DRY_OFF_NO_BUZZER :

	   case DRY_OFF:
 			run_t.gDry = 0;
            PTC_SetLow();
	  break;

       case PLASMA_ON:
       		run_t.gPlasma=1;
          
            PLASMA_SetHigh();
		 
	  
	    
       break;

       case PLASM_ON_NO_BUZZER:
           run_t.gPlasma=1;
           PLASMA_SetHigh();


       break;

       case PLASM_OFF_NO_BUZZER:
          
           run_t.gPlasma=0;
           PLASMA_SetLow();

       break;

       

       case PLASMA_OFF:
           run_t.gPlasma=0;
		   PLASMA_SetLow();
	
	   
       break;


	   case MOUSE_RUN : //this is fan_speed_max
	   	  gpro_t.gmouse=mouse_run;
          Ultrasonic_Pwm_Output();
       
	  break;

       case MOUSE_STOP:
        
         gpro_t.gmouse=mouse_stop;
         Ultrasonic_Pwm_Stop();
        
        break;

       

       case AI_MODE_ON:
        run_t.gDry = 1;
        run_t.gPlasma=1;
        gpro_t.gmouse=1;

      

       break;

       case AI_MODE_OFF:
          
  

       break;

       


      default :
        cmd =0;

      break; 


    }



}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
void SystemReset(void)
{
    if(run_t.gPower_On ==POWER_ON){
	
		
		__set_PRIMASK(1) ;
		HAL_NVIC_SystemReset();
    }

}

/**********************************************************************
	*
	*Functin Name: void  RunCommand_MainBoard_Fun(void)
	*Function : be check key of value 
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void PowerOn_Run_Pro(void)
{

  
   if(power_on_flag ==0){
       power_on_flag ++;
       power_off_flag=0;
   
	   run_t.power_on_send_data_flag=0;
	 
        run_t.gPower_On = POWER_ON;
		run_t.gTimer_read_dht11_temp_value=20;
		run_t.gTimer_continuce_works_time=0;
		run_t.interval_time_stop_run=0;
		 run_t.fan_warning =0;
		 run_t.ptc_warning =0;
		 run_t.gTimer_ptc_adc_times=0;
		 run_t.open_ptc_detected_flag=0;
        run_t.power_on_send_data_flag=0;

        run_t.gPower_On=POWER_ON;
    
     	SetPowerOn_ForDoing();
     	
	
        }
    
}


void PowerOff_Run_Pro(void)
{
    static uint8_t power_off_fan;
       switch(power_off_flag){

       case 0:

          power_on_flag=0;
	     run_t.gPower_On = POWER_OFF;
		 run_t.power_on_send_data_flag=0;
	     run_t.gTimer_continuce_works_time=0;
		 run_t.interval_time_stop_run=0;
		 run_t.fan_warning =0;
		 run_t.ptc_warning =0;

     
		if(power_off_fan==0){
             
		    power_off_fan++;
			run_t.gTimer_fan_run_one_minutes =0;
		    run_t.gFan_continueRun =0;


		}
		else{
			run_t.gTimer_fan_run_one_minutes =0;
			run_t.gFan_continueRun =1;
		}
	
	
	   power_off_flag= FAN_CONTINUCE_RUN_ONE_MINUTE;
	 break;


     case  FAN_CONTINUCE_RUN_ONE_MINUTE: //7

	   if(power_off_flag==0){
		 	power_off_flag++;
			SetPowerOff_ForDoing();
          //  Answering_Signal_USART1_Handler(COMMAND_ID,ANSWER_POWER_OFF);
     
		 }
         
	     Fan_ContinueRun_OneMinute_Fun();

	break;

 }

}	

/**********************************************************************************
*
*Function Name: static void mainboard_run_handler(void)
*
*
*
**********************************************************************************/
void mainboard_run_handler(void)
{

	  switch(run_t.run_masin_process_step){

		   case 0:

    
            run_t.power_on_send_data_flag=0;

            run_t.gPower_On = POWER_ON;
            run_t.gTimer_read_dht11_temp_value=20;
            run_t.gTimer_continuce_works_time=0;
            run_t.interval_time_stop_run=0;
            run_t.fan_warning =0;
            run_t.ptc_warning =0;
            run_t.gTimer_ptc_adc_times=0;
            run_t.open_ptc_detected_flag=0;
            run_t.power_on_send_data_flag=0;

            run_t.gPower_On=POWER_ON;

            SetPowerOn_ForDoing();
     	
	
            run_t.run_masin_process_step =1;

           break;

			case 1:

				if(run_t.gTimer_ptc_adc_times > 3 && run_t.interval_time_stop_run ==0){ //3 minutes 120s
					run_t.gTimer_ptc_adc_times=0;
					Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,10);
					//run_t.ptc_temp_voltage=200;
					Judge_PTC_Temperature_Value();


				}
				 run_t.run_masin_process_step =3;
            break;

		    case 3:
			
				if(run_t.gTimer_fan_adc_times > 8 && run_t.interval_time_stop_run ==0){ //2 minute 180s
					run_t.gTimer_fan_adc_times =0;

				 
					Get_Fan_Adc_Fun(ADC_CHANNEL_0,10);
				
	                

				}
	              run_t.run_masin_process_step=4;
			break;
			
		   case  4:
           if(run_t.fan_warning ==1){

               if(run_t.gTimer_fan_adc_times > 1 ){
			   run_t.gTimer_fan_adc_times=0;

			   	#if DEBUG
				 printf("fan_warning\n");

				#endif
		       HAL_Delay(200);
		       buzzer_sound();//Buzzer_KeySound();
		       HAL_Delay(100);
			   buzzer_sound();//Buzzer_KeySound();
		       HAL_Delay(100);
			   buzzer_sound();//Buzzer_KeySound();
		       HAL_Delay(100);
			   buzzer_sound();//Buzzer_KeySound();
		       HAL_Delay(100);
			   SendWifiCmd_To_Order(FAN_WARNING);
			  
               	}
		   	}
		   


		   if(run_t.ptc_warning==1){

		     if(run_t.gTimer_ptc_adc_times > 2){
			 	run_t.gTimer_ptc_adc_times=0;
		   	    
			   #if DEBUG
				 printf("ptc_warning\n");

				#endif 
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
			   SendWifiCmd_To_Order(PTC_WARNING);
		     }

		   }
		   run_t.run_masin_process_step=1;
		   break;

		}
}
/********************************************************************
*
*Function Name:static void Works_Rest_Cycle_TenMinutes(void)
*
*
*
*
********************************************************************/
static void Works_Rest_Cycle_TenMinutes(void)
{
	

	if(run_t.gTimer_continuce_works_time < 10){
		PLASMA_SetLow(); //
		Ultrasonic_Pwm_Stop();//HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
		PTC_SetLow(); 
    }
    else if(run_t.gTimer_continuce_works_time > 10){
		run_t.gTimer_continuce_works_time=0;
		run_t.interval_time_stop_run =0;
  


	}

	if(run_t.gFan_continueRun ==1){

		if(run_t.gTimer_fan_run_one_minutes < 60){

		Fan_Run_Fun();
		}       

		if(run_t.gTimer_fan_run_one_minutes > 59){

		run_t.gTimer_fan_run_one_minutes=0;

		run_t.gFan_continueRun++;
		FAN_Stop();
		}

	}
}
/**********************************************************
************************************************************/
static void Fan_ContinueRun_OneMinute_Fun(void)
{

	if(run_t.gFan_continueRun ==1 && run_t.gPower_On ==POWER_OFF){

	if(run_t.gTimer_fan_run_one_minutes < 60){

		Fan_CCW_Run_Max();
	}       
	else if(run_t.gTimer_fan_run_one_minutes > 59){

		run_t.gTimer_fan_run_one_minutes=0;

		run_t.gFan_continueRun++;
		FAN_Stop();
	}
	}



}

void SetPowerOn_ForDoing(void)
{

    run_t.gDry = 1;
	run_t.gPlasma =1;       //"??"
	gpro_t.gmouse = 1; // "??"
	PLASMA_SetHigh(); //
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetHigh();
	Fan_Run_Fun();
	
    
	
    
 }

void SetPowerOff_ForDoing(void)
{

   
	
	run_t.gPower_On = POWER_OFF;

 
   
    run_t.gDry = 0;
	run_t.gPlasma =0;       //"??"
    gpro_t.gmouse = 0;
	
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	 //input ac power be detect fan default 
	FAN_Stop();

}



void ActionEvent_Handler(void)
{
   if(run_t.gTime_run_board_action > 1  && run_t.interval_time_stop_run ==0){

      run_t.gTime_run_board_action =0;

   if(run_t.ptc_warning ==0){
	if(run_t.gDry == 1){
	
        PTC_SetHigh();
	
	}
	else{
	   PTC_SetLow();
	  
	}
   }
    
	//kill
	if(run_t.gPlasma == 1){
	    PLASMA_SetHigh();
		
		 
	}
	else{

		PLASMA_SetLow();
		
		
	}

    if(gpro_t.gmouse == 1){
     HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 

    }
    else{
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off


    }

   //fan_run_handler();
   Fan_CCW_Run_Max();

    }
			
}


void Read_TempSensor_Data(void)
{
  if((run_t.gTimer_read_dht11_temp_value>6 && run_t.gPower_On == POWER_ON)|| run_t.power_on_send_data_flag < 2){
        run_t.power_on_send_data_flag = 3 ;
        run_t.gTimer_read_dht11_temp_value=0;
        Update_DHT11_Value();

    }


}


void works_two_hours_detected_handler(void)
{


    switch(run_t.interval_time_stop_run ){


    case 0:
    
    if(run_t.gTimer_continuce_works_time > 120){//120
        run_t.gTimer_continuce_works_time =0;
        run_t.interval_time_stop_run =1;
        run_t.gFan_continueRun =1;
        run_t.gTimer_fan_run_one_minutes=0;
      }

       ActionEvent_Handler();

      break;

    case 1:

        Works_Rest_Cycle_TenMinutes();

    break;

    }

}


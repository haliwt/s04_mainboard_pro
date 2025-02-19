#include "bsp.h"





void Fan_CCW_Run_Max(void)
{
   FAN_CW_SetLow();
   //MX_TIM3_Init(23,9,10); //100KHz
   //HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
   FAN_CCW_SetHigh();  
  
}

void Fan_CCW_Run_Min(void)
{
    FAN_CW_SetLow();
   // MX_TIM3_Init(23,99,50); //10KHz
   /// HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
   FAN_CCW_SetHigh();  


}

 
void FAN_Stop(void)
{
    FAN_CCW_SetLow(); //brake
   // FAN_CCW_SetLow(); //PA6
   //HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
   FAN_CCW_SetLow();  
    
}
void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	//HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON
	Ultrasonic_Pwm_Stop();
	PTC_SetLow();
	FAN_Stop();



}
void Fan_Run_Fun(void)
{

    FAN_CW_SetLow();
    FAN_CCW_SetHigh();  


}




//"杀毒" 
void SterIlization(uint8_t sel)
{
    if(sel==0){//open 
		 
		 PLASMA_SetHigh();
		

	}
	else{ //close

		PLASMA_SetLow();
	

	}



}
//PTC
void Dry_Function(uint8_t sel)
{
   if(sel ==0){

  

		PTC_SetHigh();

   }
   else{

       PTC_SetLow();

   }

}


//FAN 

#if 0
void fan_run_handler(void)
{
    if(gpro_t.gmouse==mouse_run){
         Fan_Run_Fun();
    }
    else if(gpro_t.gmouse == mouse_stop ){

        if(run_t.gTimer_fan_run_one_minutes < 60){

    		Fan_CCW_Run_Max();
    	}       
    	else if(run_t.gTimer_fan_run_one_minutes > 59){

    		
    		FAN_Stop();
    	}

    }
    else{

       FAN_Stop();


    }
   
    

}
#endif 


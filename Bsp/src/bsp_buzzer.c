#include "bsp.h"


void(*buzzer_sound)(void);

static void Buzzer_KeySound(void);



static void Buzzer_KeySound_Off(void);


void buzzer_init(void)
{

   BuzzerSound_Handler(Buzzer_KeySound);

}






static void Buzzer_KeySound(void)
{

//HAL_GPIO_TogglePin(BUZZER_GPIO,BUZZER_PIN);//BEEP=!BEEP;
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    HAL_Delay(20);
	Buzzer_KeySound_Off();
   
    
 }


static void Buzzer_KeySound_Off(void)
{
  HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
	
}

void BuzzerSound_Handler(void(*buzzer_sound_handler)(void))
{

    buzzer_sound = buzzer_sound_handler;

}




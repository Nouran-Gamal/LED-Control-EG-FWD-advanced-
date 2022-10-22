
/**********************************************************************************************************************
 *                                                              INCLUDES
 *********************************************************************************************************************/

#include "Port.h"
#include "Dio.h"
#include "NVIC.h"
#include "LED.h"
#include "SysTick_Timer.h"

#define Enable_Exceptions()    __asm("CPSIE I")


volatile static uint8 ticks = 0;


void SysTick_Handler()
{
	ticks++;
}

int main ()
{

	Port_Init(&Port_PinConfig);

	Dio_Init(&Dio_Configuration);

	SysTick_Disable();

	SysTickPeriodSet(1000);  //Time in ms //

	SysTick_Enable();

	Enable_Exceptions();

	NVIC_Init();

	while(1)
	{

		if(ticks <= 5)
		{
			LED_ON(RED_LED);
		}
		else if(ticks > 5 && ticks <= 7)
		{
			LED_OFF(RED_LED);
		}
		else if(ticks > 7 && ticks <= 11)
		{
			LED_ON(BLUE_LED);
		}
		else if(ticks > 11 && ticks <= 14)
		{
			LED_OFF(BLUE_LED);
		}
		else if(ticks > 14 && ticks <= 17)
		{
			LED_ON(GREEN_LED);
		}
		else if(ticks > 17 && ticks <= 20)
		{
			LED_OFF(GREEN_LED);
			ticks = 0;
		}
	}
}

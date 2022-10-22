 /******************************************************************************
 *
 * File Name: SysTick_Timer.c
 *
 * Author: Nouran gamal
 ******************************************************************************/


#include "SysTick_Timer.h"
#include "NVIC.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define SYSTICK_PRIORITY_MASK            0x1FFFFFFF
#define SYSTICK_INTERRUPT_PRIORITY       3
#define SYSTICK_PRIORITY_BITS_POS        29

/*******************************************************************************
 *                              Functions Definations                          *
 *******************************************************************************/

void SysTick_Disable(void)
{
	NVIC_ST_CTRL_R   = 0;              /* Disable the SysTick Timer by Clear the ENABLE Bit */
}


void SysTick_Enable(void)
{

	NVIC_ST_CURRENT_R = 0;              /* Clear the Current Register value */
	/* Configure the SysTick Control Register
	 * Enable the SysTick Timer (ENABLE = 1)
	 * Enable SysTick Interrupt (INTEN = 1)
	 * Choose the clock source to be System Clock (CLK_SRC = 1) */
	NVIC_ST_CTRL_R   |= 0x07;

	NVIC_PRI3_R =  (NVIC_PRI3_R & SYSTICK_PRIORITY_MASK) | (SYSTICK_INTERRUPT_PRIORITY << SYSTICK_PRIORITY_BITS_POS);
}
void SysTickPeriodSet(uint32 Period)
{
	volatile uint32 tick = 0;
	tick = ((Period * 16000)-1);
	NVIC_ST_RELOAD_R = tick;
}
uint32 SysTickPeriodGet (void)
{ 
	uint32 Value = 0;
	Value = NVIC_ST_RELOAD_R;
	return Value;
}
boolean SysTick_Is_Time_out(void)
{

	if((NVIC_ST_CTRL_R & (1<<16)) == 0)
	{
		return 0;
	}

	else
	{
		NVIC_ST_CURRENT_R = 0;
		return 1;
	}
}  

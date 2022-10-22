/******************************************************************************
 *
 * Module: GPT
 *
 * File Name: GPT.c
 *
 * Description: Source file for GPT Driver
 *
 *Author:Nouran Gamal
 ******************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "GPT.h"
#include "GPT_REGS.h"

/**********************************************************************************************************************
 *  LOCAL MACROS CONSTANT\FUNCTION
 *********************************************************************************************************************/
#define TIMERA 0

#define TIMERB 1

#define TIMER16 0

#define TIMER32 1

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static uint8 Counter = 0;
static uint32 BaseAddress;
static uint8 Timer_A_B;
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
static GPT_ChannelConfigSet Gpt_ConfigDynamic[GPT_CHANNLES_CONFIGURED]; 
STATIC const GPT_CONFIGURATION * GPT_Channels = NULL_PTR;
STATIC Gpt_ModeType Gpt_Mode = GPT_MODE_NORMAL;
volatile static uint32 * GPT_Ptr = NULL_PTR;     /* point to the required timer Registers base address */

static const uint32 GPTM_TimersBaseAddresses[GPTM_MAXIMUM_CHANNLES_CONFIGURED] = 
{
		GPT_TIMER0_BASE_ADDRESS,
		GPT_TIMER1_BASE_ADDRESS,
		GPT_TIMER2_BASE_ADDRESS,
		GPT_TIMER3_BASE_ADDRESS,
		GPT_TIMER4_BASE_ADDRESS,
		GPT_TIMER5_BASE_ADDRESS,
		GPT_WIDE_TIMER0_BASE_ADDRESS,
		GPT_WIDE_TIMER1_BASE_ADDRESS,
		GPT_WIDE_TIMER2_BASE_ADDRESS,
		GPT_WIDE_TIMER3_BASE_ADDRESS,
		GPT_WIDE_TIMER4_BASE_ADDRESS,
		GPT_WIDE_TIMER5_BASE_ADDRESS,
};


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
 * \Syntax          : void Gpt_Init(const Gpt_ConfigType* ConfigPtr)
 * \Description     : Describe this service
 *
 * \Sync\Async      : Synchronous
 * \Re-entrancy      : Non Reentrant
 * \Parameters (in) : parameterName   Parameter Describtion
 * \Parameters (out): None
 * \Return value:   : Std_ReturnType  E_OK
 *                                    E_NOT_OK
 *******************************************************************************/
void Gpt_Init(const GPT_ConfigType* ConfigPtr)
{
	GPT_Channels = ConfigPtr -> Channels;
	Gpt_Mode = GPT_MODE_NORMAL;
	for(uint8 i = 0; i < GPT_CHANNLES_CONFIGURED; i++)
	{
		// store the ID for this channel
		Gpt_ConfigDynamic[i].ChannelID = (GPT_Channels + i) -> ChannelID;

		// convert timers number from 0->24 to 0->12 to point on the base address 12 registers
		uint8 temp = ((GPT_Channels) -> ChannelID) / 2 ;
		if(temp >= 0 && temp <= 7)
		{
			GPT_Ptr = (volatile uint32 *)GPT_TIMER0_BASE_ADDRESS + (1000 * temp);
		}
		else
		{
			GPT_Ptr = (volatile uint32 *)GPT_WIDE_TIMER2_BASE_ADDRESS + (1000 * (temp - 8));
		}

		if( ((GPT_Channels) -> ChannelID) %2 == 0) //  TIMER A
		{
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMCTL_REG_OFFSET),BIT0); /* Clear the TAEN to disable the timer in the begining of configs */

			if((GPT_Channels) -> TimerMode == GPT_ONE_SHOT) // step 3 in initialization of one shot / periodic mode
			{
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTAMR_REG_OFFSET) &= 0xFC;
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTAMR_REG_OFFSET) |= 0x01; // select one shot mode
			}
			else
			{
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTAMR_REG_OFFSET) &= 0xFC;
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTAMR_REG_OFFSET) |= 0x02; // select periodic mode
			}
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTAMR_REG_OFFSET),BIT4); /* make the timer count down*/
		}
		else // TIMER B
		{
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMCTL_REG_OFFSET),BIT8);/* Clear the TBEN to disable the timer in the begining of configs */
			if((GPT_Channels) -> TimerMode == GPT_ONE_SHOT) // step 3 in initialization of one shot / periodic mode
			{
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTBMR_REG_OFFSET) &= 0xFC;
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTBMR_REG_OFFSET) |= 0x01; // select one shot mode
			}
			else
			{
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTBMR_REG_OFFSET) &= 0xFC;
				*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTBMR_REG_OFFSET) |= 0x02; // select periodic mode
			}
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTBMR_REG_OFFSET),BIT4); /* make the timer count down*/
		}
		*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMCFG_REG_OFFSET) = 0x04; // operate the timer not concatenated mode
	}

}
/******************************************************************************
 * \Syntax          : void Gpt_EnableNotification(Gpt_ChannelType Channel)
 * \Description     : Describe this service
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : parameterName   Parameter Describtion
 * \Parameters (out): None
 * \Return value:   : Std_ReturnType  E_OK
 *                                    E_NOT_OK
 *******************************************************************************/
void Gpt_EnableNotification(Gpt_ChannelType Channel)
{
	if(Channel/TIMER_COUNT)
	{
		Timer_A_B = TIMERB;
		Channel -= TIMER_COUNT;
	}
	BaseAddress = GPTM_TimersBaseAddresses[Channel];

	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT0);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT1);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT2);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT3);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT4);

	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT8);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT9);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT10);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT11);
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT16);
}
/******************************************************************************
 * \Syntax          : void Gpt_DisbaleNotification(Gpt_ChannelType Channel)
 * \Description     : Describe this service
 *
 * \Sync\Async      : Synchronous
 * \Re-entrancy      : Non Reentrant
 * \Parameters (in) : parameterName   Parameter Describtion
 * \Parameters (out): None
 * \Return value:   : Std_ReturnType  E_OK
 *                                    E_NOT_OK
 *******************************************************************************/
void Gpt_DisbaleNotification(Gpt_ChannelType Channel) 
{
	if(Channel/TIMER_COUNT)
	{
		Timer_A_B = TIMERB;
		Channel -= TIMER_COUNT;
	}
	BaseAddress = GPTM_TimersBaseAddresses[Channel];

	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT0);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT1);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT2);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT3);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT4);

	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT8);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT9);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT10);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT11);
	CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMIMR_REG_OFFSET),BIT16);
}

/******************************************************************************
 * \Syntax          : void Gpt_StartTimer(Gpt_ChannelType Channel,Gpt_ValueType Value)
 * \Description     : Describe this service
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : parameterName   Parameter Describtion
 * \Parameters (out): None
 * \Return value:   : Std_ReturnType  E_OK
 *                                    E_NOT_OK
 *******************************************************************************/
void Gpt_StartTimer(Gpt_ChannelType Channel,Gpt_ValueType Value)
{
	uint8 temp = Channel / 2; // convert timers number from 0->24 to 0->12 to point on the base address 12 registers
	if(temp >= 0 && temp <= 7)
	{
		GPT_Ptr = (volatile uint32 *)GPT_TIMER0_BASE_ADDRESS +(1000 * temp);
	}
	else
	{
		GPT_Ptr = (volatile uint32 *)GPT_WIDE_TIMER0_BASE_ADDRESS +(1000 * (temp - 8));
	}

	if( Channel %2 == 0) // Timer A
	{
		*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTAILR_REG_OFFSET) = (uint16)Value; // put the value
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMCTL_REG_OFFSET),BIT0);/* set the TAEN to enable the timer  */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTAMR_REG_OFFSET),BIT5);/* enable generating interrupt*/
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMIMR_REG_OFFSET),BIT0);/* enable generating interrupt WHEN TIME OUT OCCUR */
	}
	else //Timer B
	{
		*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTBILR_REG_OFFSET) = (uint16)Value; // put the value
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMCTL_REG_OFFSET),BIT8);/* set the TAEN to enable the timer */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMTBMR_REG_OFFSET),BIT5);/* enable generating interrupt*/
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)GPT_Ptr + GPT_GPTMIMR_REG_OFFSET),BIT8);/* enable generating interrupt WHEN TIME OUT OCCUR */
	}
}	
/******************************************************************************
 * \Syntax          : void Gpt_StopTimer(Gpt_ChannelType Channel)
 * \Description     : Describe this service
 *
 * \Sync\Async      : Synchronous
 * \Re-entrancy      : Non Reentrant
 * \Parameters (in) : parameterName   Parameter Describtion
 * \Parameters (out): None
 * \Return value:   : Std_ReturnType  E_OK
 *                                    E_NOT_OK
 *******************************************************************************/
void Gpt_StopTimer(Gpt_ChannelType Channel)
{
	if(Channel/TIMER_COUNT)
	{
		Timer_A_B = TIMERB;
		Channel -= TIMER_COUNT;
	}
	BaseAddress = GPTM_TimersBaseAddresses[Channel];
	if(Timer_A_B == TIMERA)

	{

		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMCTL_REG_OFFSET),BIT0);

	}

	else

	{

		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMCTL_REG_OFFSET),BIT8);

	}

}

/******************************************************************************
 * \Syntax          : void Gpt_ClearFlag(Gpt_ChannelType Channel)
 * \Description     : Describe this service
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Return value:   : Std_ReturnType  E_OK
 *                                    E_NOT_OK
 *******************************************************************************/
void Gpt_ClearFlag(Gpt_ChannelType Channel)
{
	BaseAddress = GPTM_TimersBaseAddresses[Channel];

	SET_BIT(*(volatile uint32 *)((volatile uint8 *)BaseAddress + GPT_GPTMICR_REG_OFFSET),BIT0);
}

/**********************************************************************************************************************
 *  END OF FILE: GPT.c
 *********************************************************************************************************************/

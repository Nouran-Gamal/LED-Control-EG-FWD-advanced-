/******************************************************************************
 *
 * Module: NVIC
 *
 * File Name: NVIC.c
 *
 * Description: source file for NVIC Driver.
 *
 * Author: Nouran Gamal
 ******************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "NVIC.h"
#include "Platform_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
 * Syntax          : Void NVIC_Init(void)
 * Description     : Initialize the interrupts
 * Sync            : Synchronous
 * Re-entrancy     : Non Reentrant
 * Parameters (in) : No
 * Parameters (out): NO
 * Return value:   : Std_ReturnType  E_OK
 *                                   E_NOT_OK
 *******************************************************************************/
void NVIC_Init(void)
{
	uint8 i;
	uint32*RegisterAdd;
	uint32 BitNum;
	uint32 TotalPriority;
	*((volatile uint32*) (0xE000E000 + 0xD0C)) = ((0x05FA<<16) + (NVIC_SUB_GP_Cfg << 8));
	for(i = 0; i < NVIC_NUM_OF_ENABLED_INTERRUPT; i++)
	{
		/* enable the corresponding interrupt */
		RegisterAdd = (uint32*)(NVIC_EN0_R + ((NVIC_PCfg[i].InterrNum / 32) * 4));
		BitNum = NVIC_PCfg[i].InterrNum % 32;
		*RegisterAdd |= 1<< BitNum;

		/* set the priority */
		RegisterAdd = (uint32*)(NVIC_PRI0_R + ( (NVIC_PCfg[i].InterrNum / 4) * 4 ) );
		BitNum = (NVIC_PCfg[i].InterrNum % 4);
		BitNum = (BitNum * 8) + 5;
		TotalPriority = (NVIC_PCfg[i].PriorityGP << (NVIC_SUB_GP_Cfg - 0x4)) | (NVIC_PCfg[i].PrioritySubGP);
		*RegisterAdd |= TotalPriority << BitNum;
	}

}

/**********************************************************************************************************************
 *  END OF FILE: NVIC.c
 *********************************************************************************************************************/

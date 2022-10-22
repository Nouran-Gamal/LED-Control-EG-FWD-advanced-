/******************************************************************************
 *
 * File Name: SysTick_Timer.c
 *
 * Author: Nouran gamal
 ******************************************************************************/


#include "Std_Types.h"


#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))


/**********************************************************************************************************************
 *                                             FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void SysTick_Disable(void);
void SysTick_Enable(void);
void SysTickPeriodSet(uint32 Period);
boolean SysTick_Is_Time_out(void);

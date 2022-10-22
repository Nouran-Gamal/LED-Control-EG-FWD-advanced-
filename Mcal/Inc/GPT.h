 /******************************************************************************
 *
 * Module: GPT
 *
 * File Name: GPT.h
 *
 * Description: Header file for GPT Driver Registers
 *
 *Author: Nouran Gamal
 ******************************************************************************/
#ifndef GPT_H
#define GPT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
/* Standard AUTOSAR types */
#include "Std_Types.h"
#include "GPT_CFG.h"
#include "Common_Macros.h"
#include "Platform_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define GPTM_MAXIMUM_CHANNLES_CONFIGURED     (24U)



/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#define BIT0  0
#define BIT1  1
#define BIT2  2
#define BIT3  3
#define BIT4  4
#define BIT5  5
#define BIT8  8
#define BIT9  9
#define BIT10 10
#define BIT11 11
#define BIT16 16

#define TIMER_COUNT 12
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* Type definition for GPT_ChannelType used by the GPT APIs */
typedef uint8 Gpt_ChannelType;

/* Type definition for Gpt_ValueType used by the GPT APIs */
typedef uint32 Gpt_ValueType;

/* Type definition for Gpt_ModeType used by the GPT APIs */
typedef enum
{
  GPT_MODE_NORMAL, GPT_MODE_SLEEP
} Gpt_ModeType;

typedef enum
{
    // GPT_PREDEF_TIMER_100US_32BIT=1,
    GPT_PREDEF_TIMER_1US_16BIT = 4,
    GPT_PREDEF_TIMER_1US_24BIT = 4,
    GPT_PREDEF_TIMER_1US_32BIT = 0
} Gpt_PredefTimerType;

typedef enum
{
  GPT_ONE_SHOT = 1, GPT_CONTINUOUS // Periodic//
} Gpt_RunningMode;

typedef enum
{
    TIMER0_A, TIMER0_B,
    TIMER1_A, TIMER1_B, 
    TIMER2_A, TIMER2_B,  
    TIMER3_A, TIMER3_B,  
    TIMER4_A, TIMER4_B,  
    TIMER5_A, TIMER5_B,  
    TIMER0_A_WIDE, TIMER0_B_WIDE,   
    TIMER1_A_WIDE, TIMER1_B_WIDE,  
    TIMER2_A_WIDE, TIMER2_B_WIDE,
    TIMER3_A_WIDE, TIMER3_B_WIDE,   
    TIMER4_A_WIDE, TIMER4_B_WIDE, 
    TIMER5_A_WIDE, TIMER5_B_WIDE   
} GPT_ChannelNum;

typedef struct 
{
   GPT_ChannelNum ChannelID;
  Gpt_RunningMode TimerMode;
}GPT_CONFIGURATION;

typedef struct 
{
  GPT_ChannelNum ChannelID;
  Gpt_RunningMode TimerMode;
  Gpt_ValueType Max_Ticks;
  boolean Gpt_DirtyBitWakeup;
  boolean Gpt_DirtyBitNotification;
}GPT_ChannelConfigSet;

/* Structure required for initializing the GPT Driver */
typedef struct 
{
	GPT_ChannelConfigSet Channels[GPT_CHANNLES_CONFIGURED];
} GPT_ConfigType;


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern const GPT_ConfigType  Gpt_Configuration;
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void Gpt_Init(const Gpt_ConfigType* ConfigPtr)      
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Gpt_Init(const GPT_ConfigType* ConfigPtr);

/******************************************************************************
* \Syntax          : void Gpt_EnableNotification(Gpt_ChannelType Channel)      
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Gpt_EnableNotification(Gpt_ChannelType Channel);

/******************************************************************************
* \Syntax          : void Gpt_DisbaleNotification(Gpt_ChannelType Channel)      
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Gpt_DisbaleNotification(Gpt_ChannelType Channel);

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
void Gpt_StartTimer(Gpt_ChannelType Channel,Gpt_ValueType Value);

/******************************************************************************
* \Syntax          : void Gpt_StopTimer(Gpt_ChannelType Channel)      
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Gpt_StopTimer(Gpt_ChannelType Channel);

/******************************************************************************
* \Syntax          : void Gpt_ClearFlag(Gpt_ChannelType Channel)      
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Gpt_ClearFlag(Gpt_ChannelType Channel);
 
#endif  /* GPT_H */

/**********************************************************************************************************************
 *  END OF FILE: GPT.h
 *********************************************************************************************************************/
 

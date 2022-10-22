/******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio.c
 *
 * Description: Header file for Dio Driver
 *
 *Author:Nouran Gamal
 ******************************************************************************/

#include "Dio.h"
#include "Dio_Regs.h"

STATIC const Dio_ConfigChannel * Dio_PortChannels = NULL_PTR;

/************************************************************************************
 * Service Name: Dio_Init
 * Sync: Synchronous
 * Re-entrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to post-build configuration data
 * Parameters (out): None
 * Description: Function to Initialize the Dio module.
 ************************************************************************************/
void Dio_Init(const Dio_ConfigType * ConfigPtr)
{
	Dio_PortChannels = ConfigPtr -> Channels;  /* pointer to address of the first Channels */
}


/************************************************************************************
 * Service Name: Dio_WriteChannel
 * Parameters (in): ChannelId - ID of DIO channel.
 * Level - Value to be written.
 * Parameters (out): None
 * Return value: None
 * Description: Function to set a level of a channel.
 ************************************************************************************/
void Dio_WriteChannel(Dio_PortType PortID,Dio_ChannelType ChannelId, Dio_LevelType Level)
{
	if(Level == STD_LOW)
	{
		switch(PortID)
		{
		case PORTA: CLEAR_BIT(GPIO_PORTA_DATA_REG, ChannelId); break;
		case PORTB: CLEAR_BIT(GPIO_PORTB_DATA_REG, ChannelId); break;
		case PORTC: CLEAR_BIT(GPIO_PORTC_DATA_REG, ChannelId); break;
		case PORTD: CLEAR_BIT(GPIO_PORTD_DATA_REG, ChannelId); break;
		case PORTE: CLEAR_BIT(GPIO_PORTE_DATA_REG, ChannelId); break;
		case PORTF: CLEAR_BIT(GPIO_PORTF_DATA_REG, ChannelId); break;
		}
	}
	else if(Level == STD_HIGH)
	{
		switch(PortID)
		{
		case PORTA: SET_BIT(GPIO_PORTA_DATA_REG, ChannelId); break;
		case PORTB: SET_BIT(GPIO_PORTB_DATA_REG, ChannelId); break;
		case PORTC: SET_BIT(GPIO_PORTC_DATA_REG, ChannelId); break;
		case PORTD: SET_BIT(GPIO_PORTD_DATA_REG, ChannelId); break;
		case PORTE: SET_BIT(GPIO_PORTE_DATA_REG, ChannelId); break;
		case PORTF: SET_BIT(GPIO_PORTF_DATA_REG, ChannelId); break;
		}
	}
}

/************************************************************************************
 * Service Name: Dio_ReadChannel
 * Parameters (in): ChannelId - ID of DIO channel.
 * Parameters (out): None
 * Return value: Dio_LevelType
 * Description: Function to return the value of the specified DIO channel.
 ************************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
	volatile uint32 * Port_Ptr = NULL_PTR;
	Dio_LevelType output = STD_LOW;

	/* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
	switch(Dio_PortChannels[ChannelId].Port_Num)
	{
	case 0:    Port_Ptr = &GPIO_PORTA_DATA_REG;
	break;
	case 1:    Port_Ptr = &GPIO_PORTB_DATA_REG;
	break;
	case 2:    Port_Ptr = &GPIO_PORTC_DATA_REG;
	break;
	case 3:    Port_Ptr = &GPIO_PORTD_DATA_REG;
	break;
	case 4:    Port_Ptr = &GPIO_PORTE_DATA_REG;
	break;
	case 5:    Port_Ptr = &GPIO_PORTF_DATA_REG;
	break;
	}
	/* Read the required channel */
	if(BIT_IS_SET(*Port_Ptr,Dio_PortChannels[ChannelId].Ch_Num))
	{
		output = STD_HIGH;
	}
	else
	{
		output = STD_LOW;
	}

	return output;
}

/************************************************************************************
 * Service Name: Dio_FlipChannel
 * Parameters (in): ChannelId - ID of DIO channel.
 * Parameters (out): None
 * Return value: Dio_LevelType
 * Description: Function to flip the level of a channel and return the level of the channel after flip.
 ************************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
	volatile uint32 * Port_Ptr = NULL_PTR;
	Dio_LevelType output = STD_LOW;

	/* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
	switch(Dio_PortChannels[ChannelId].Port_Num)
	{
	case 0:    Port_Ptr = &GPIO_PORTA_DATA_REG;
	break;
	case 1:    Port_Ptr = &GPIO_PORTB_DATA_REG;
	break;
	case 2:    Port_Ptr = &GPIO_PORTC_DATA_REG;
	break;
	case 3:    Port_Ptr = &GPIO_PORTD_DATA_REG;
	break;
	case 4:    Port_Ptr = &GPIO_PORTE_DATA_REG;
	break;
	case 5:    Port_Ptr = &GPIO_PORTF_DATA_REG;
	break;
	}
	/* Read the required channel and write the required level */
	if(BIT_IS_SET(*Port_Ptr,Dio_PortChannels[ChannelId].Ch_Num))
	{
		CLEAR_BIT(*Port_Ptr,Dio_PortChannels[ChannelId].Ch_Num);
		output = STD_LOW;
	}
	else
	{
		SET_BIT(*Port_Ptr,Dio_PortChannels[ChannelId].Ch_Num);
		output = STD_HIGH;
	}

	return output;
}

/************************************************************************************
 * Service Name: Dio_ReadPort
 * Parameters (in): PortID - ID of DIO Port.
 * Parameters (out): None
 * Return value: Dio_PortLevelType
 * Description: Function to return the value of the specified DIO Port.
 ************************************************************************************/
Dio_PortLevelType DIO_ReadPort(Dio_PortType PortID)
{
	uint32 ret_value = 0;
	switch(PortID)
	{
	case PORTA: ret_value = GPIO_PORTA_DATA_REG; break;
	case PORTB: ret_value = GPIO_PORTB_DATA_REG; break;
	case PORTC: ret_value = GPIO_PORTC_DATA_REG; break;
	case PORTD: ret_value = GPIO_PORTD_DATA_REG; break;
	case PORTE: ret_value = GPIO_PORTE_DATA_REG; break;
	case PORTF: ret_value = GPIO_PORTF_DATA_REG; break;
	}
	return ret_value;
}

/************************************************************************************
 * Service Name: Dio_WritePort
 * Parameters (in): PortID - ID of DIO Port.
                   Level - Value to be written.
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to set a level of a Port.
 ************************************************************************************/
void DIO_WritePort(Dio_PortType PortID,Dio_PortLevelType Level)
{
	switch(PortID)
	{
	case PORTA: GPIO_PORTA_DATA_REG = Level; break;
	case PORTB: GPIO_PORTB_DATA_REG = Level; break;
	case PORTC: GPIO_PORTC_DATA_REG = Level; break;
	case PORTD: GPIO_PORTD_DATA_REG = Level; break;
	case PORTE: GPIO_PORTE_DATA_REG = Level; break;
	case PORTF: GPIO_PORTF_DATA_REG = Level; break;
	}
}

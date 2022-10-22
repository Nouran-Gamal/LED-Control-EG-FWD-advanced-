/******************************************************************************
 *
 * Module: DIO
 *
 * File Name: DIO_Cfg.h
 *
 * Description: Header file for DIO Driver
 *
 * Author: Nouran Gamal
 ******************************************************************************/
#ifndef DIO_CFG_H
#define DIO_CFG_H

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define DIO_CONFIGURED_CHANNLES              3u

/* Channel Index in the array of structures */
#define DIO_LED1_CHANNEL_ID_INDEX         0
#define DIO_LED2_CHANNEL_ID_INDEX         1
#define DIO_LED3_CHANNEL_ID_INDEX         2

/* DIO Configured Port ID's  */
#define DIO_LED1_PORT_NUM                 (Dio_PortType)5 /* PORTF */
#define DIO_LED2_PORT_NUM                 (Dio_PortType)5 /* PORTF */
#define DIO_LED3_PORT_NUM                 (Dio_PortType)5 /* PORTF */

/* DIO Configured Channel ID's */
#define DIO_LED1_CHANNEL_NUM              (Dio_ChannelType)1 /* Pin 1 in PORTF */
#define DIO_LED2_CHANNEL_NUM              (Dio_ChannelType)2 /* Pin 2 in PORTF */
#define DIO_LED3_CHANNEL_NUM              (Dio_ChannelType)3 /* Pin 3 in PORTF */


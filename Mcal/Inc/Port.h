/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for Port Driver.
 *
 * Author: Nouran Gamal
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

#include "Std_Types.h"
#include "Port_Cfg.h"
#include "Common_Macros.h"

#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


/*******************************************************************************
 *                              Global Data Types and Structures                             *
 *******************************************************************************/


typedef uint8 Port_PinType;    /* Type Definition for Port_PinType used by the PORT APIs  */

typedef uint8 Port_PortType;

/* Description: enum to hold PIN direction */
typedef enum
{
	PORT_PIN_IN,PORT_PIN_OUT
}Port_PinDirectionType;

/*Type definition for the Port_PinModeType*/
typedef enum
{
	PORT_PIN_MODE_ADC,
	PORT_PIN_MODE_ALT1,
	PORT_PIN_MODE_ALT2,
	PORT_PIN_MODE_ALT3,
	PORT_PIN_MODE_ALT4,
	PORT_PIN_MODE_ALT5,
	PORT_PIN_MODE_ALT6,
	PORT_PIN_MODE_ALT7,
	PORT_PIN_MODE_ALT8,
	PORT_PIN_MODE_ALT9,
	PORT_PIN_MODE_DIO
}Port_PinModeType;

/* Description: Port Pin Level value from Port pin list */
typedef enum
{
	PORT_PIN_LEVEL_LOW,
	PORT_PIN_LEVEL_HIGH
}Port_PinLevelValue;


/*Description: Enum to hold internal resistor type for PIN */
typedef enum
{
	OFF,PULL_UP,PULL_DOWN,OPEN_DRAIN
}Port_PinInternalAttachType;

/*Description: enum to choose internal current value for PIN */
typedef enum
{
	GPIODR2R,GPIODR4R,GPIODR8R
}Port_PinOutputCurrent;


/* Description: Structure to configure each individual PIN:
 *  1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *  2. the number of the pin in the PORT.
 *  3. the direction of pin --> INPUT or OUTPUT
 *  4. the internal resistor --> Disable, Pull up or Pull down
 *  5. initial port pin value (HIGH, LOW)
 *  6. initial port pin mode  (ADC, PORT, ..)
 */
typedef struct 
{
	Port_PortType port_num;
	Port_PinType pin_num;
	Port_PinDirectionType direction;
	Port_PinInternalAttachType resistor;
	Port_PinLevelValue initial_value;	        /* PORT_PIN_LEVEL_LOW,PORT_PIN_LEVEL_HIGH */
	Port_PinModeType initial_mode;	        /* e.g. PORT_PIN_MODE_PORT */
	Port_PinOutputCurrent electric_current_value;    /* e.g. 2mA,4mA,8mA */
}Port_ConfigPin;

/* Array of Port_ConfigPin */
typedef struct
{
	Port_ConfigPin Pin[PORT_CONFIGURED_PINS];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
 * Service Name: Port_Init
 * Re-entrancy: Reentrant
 * Parameters (in): ConfigPtr - Pointer to configuration set
 * Parameters (out): None
 * Return value: None
 * Description:Initialize the Port driver
 ************************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr );

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern const Port_ConfigType Port_PinConfig;

#endif /* PORT_H */

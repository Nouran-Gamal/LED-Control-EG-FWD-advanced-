 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for Port Driver.
 *
 * Author: Nouran gamal
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

/* Holds the status of the Port
 * options: 	PORT_NOT_INITIALIZED
 * 		        PORT_INITIALIZED		(set by Port_Init())
*/

STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/* Holds the pointer of the Port_PinConfig */
STATIC const Port_ConfigType* Port_ConfigPtr = NULL_PTR;

/************************************************************************************
* Service Name: Port_Init
* Re-entrancy: Reentrant
* Parameters (in): ConfigPtr - Pointer to configuration set
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Port Driver module.
************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
	Port_Status = PORT_INITIALIZED;
	Port_ConfigPtr = ConfigPtr;
 for (Port_PinType index = ZERO; index < PORT_CONFIGURED_PINS; index++)
  {
           volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
           volatile uint32 delay = 0;
    
           switch(ConfigPtr->Pin[index].port_num)
           {
      case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		     break;
	    case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		     break;
	    case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		     break;
	    case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		     break;
      case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		     break;
      case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		     break;
           }
    
    /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<Port_ConfigPtr->Pin[index].port_num);
    delay = SYSCTL_REGCGC2_REG;
					 
		if(((Port_ConfigPtr->Pin[index].port_num == 3) && (Port_ConfigPtr->Pin[index].pin_num == 7)) || ((Port_ConfigPtr->Pin[index].port_num == 5) && (Port_ConfigPtr->Pin[index].pin_num == 0))) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , ConfigPtr->Pin[index].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
   else if((Port_ConfigPtr->Pin[index].port_num == 2) && (Port_ConfigPtr->Pin[index].pin_num <= 3)) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
    }
    else
    {
        /* Do Nothing ... No need to unlock the commit register for this pin */
    }
     if (Port_ConfigPtr->Pin[index].initial_mode == PORT_PIN_MODE_DIO)
		 {
			/* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

			/* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

			/* Clear the PMCx bits for this pin */
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ConfigPtr->Pin[index].pin_num * 4));

			/* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
		 }
		else if (Port_ConfigPtr->Pin[index].initial_mode == PORT_PIN_MODE_ADC)
		 {
			/* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

			/* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

			/* Clear the PMCx bits for this pin */
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ConfigPtr->Pin[index].pin_num * 4));

			/* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
		  }
		else /* Another mode */
		  {
			/* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

			/* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

			/* Set the PMCx bits for this pin */
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (Port_ConfigPtr->Pin[index].initial_mode & 0x0000000F << (Port_ConfigPtr->Pin[index].pin_num * 4));

			/* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
		  }
               if(Port_ConfigPtr->Pin[index].direction == PORT_PIN_OUT)
		  {
				/* Set the corresponding bit in the GPIODIR register to configure it as output pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

				if(Port_ConfigPtr->Pin[index].initial_value == PORT_PIN_LEVEL_HIGH)
				{
						/* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
					SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
				}
				else
				{
						/* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
					CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
				}
		 }
		else if(Port_ConfigPtr->Pin[index].direction == PORT_PIN_IN)
		 {
				/* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
				CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

				if(Port_ConfigPtr->Pin[index].resistor == PULL_UP)
				{
						/* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
				        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
				}
				else if(Port_ConfigPtr->Pin[index].resistor == PULL_DOWN)
				{
						/* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
				        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
				}
				else
				{
						/* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
				        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);

						/* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
				        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
				}
		  }
		 else if(Port_ConfigPtr->Pin[index].electric_current_value == GPIODR2R)
		 {
			 /* Set the corresponding bit in the GPIODR2R register to enable 2mA Drive */
				        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_GPIODR2R_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
		 }
		 else if(Port_ConfigPtr->Pin[index].electric_current_value == GPIODR4R)
		 {
			 /* Set the corresponding bit in the GPIODR2R register to enable 4mA Drive */
				        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_GPIODR4R_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
		 }
		 else if(Port_ConfigPtr->Pin[index].electric_current_value == GPIODR8R)
		 {
			 /* Set the corresponding bit in the GPIODR2R register to enable 4mA Drive */
				        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_GPIODR8R_REG_OFFSET) , Port_ConfigPtr->Pin[index].pin_num);
		 }
		else
		 {
				/* Do Nothing */
		 }
  }
}

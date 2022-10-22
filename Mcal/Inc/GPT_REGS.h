 /******************************************************************************
 *
 * Module: GPT
 *
 * File Name: GPT_Regs.h
 *
 * Description: Header file for GPT Driver Registers
 *
 *Author: Nouran Gamal
 ******************************************************************************/
#ifndef GPT_REGS_H
#define GPT_REGS_H

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
 /* GPT Registers base addresses */
/*For 16/32 bit wide*/
#define GPT_TIMER0_BASE_ADDRESS           0x40030000
#define GPT_TIMER1_BASE_ADDRESS           0x40031000
#define GPT_TIMER2_BASE_ADDRESS           0x40032000
#define GPT_TIMER3_BASE_ADDRESS           0x40033000
#define GPT_TIMER4_BASE_ADDRESS           0x40034000
#define GPT_TIMER5_BASE_ADDRESS           0x40035000

/*For 32/64 bit wide*/
#define GPT_WIDE_TIMER0_BASE_ADDRESS      0x40036000
#define GPT_WIDE_TIMER1_BASE_ADDRESS      0x40037000
#define GPT_WIDE_TIMER2_BASE_ADDRESS      0x4004C000
#define GPT_WIDE_TIMER3_BASE_ADDRESS      0x4004D000
#define GPT_WIDE_TIMER4_BASE_ADDRESS      0x4004E000
#define GPT_WIDE_TIMER5_BASE_ADDRESS      0x4004F000

/* Timers Registers offset addresses */
#define GPT_GPTMCFG_REG_OFFSET              0x000
#define GPT_GPTMTAMR_REG_OFFSET             0x004
#define GPT_GPTMTBMR_REG_OFFSET             0x008
#define GPT_GPTMCTL_REG_OFFSET              0x00C
#define GPT_GPTMSYNC_REG_OFFSET             0x010
#define GPT_GPTMIMR_REG_OFFSET              0x018
#define GPT_GPTMRIS_REG_OFFSET              0x01C
#define GPT_GPTMMIS_REG_OFFSET              0x020
#define GPT_GPTMICR_REG_OFFSET              0x024
#define GPT_GPTMTAILR_REG_OFFSET            0x028
#define GPT_GPTMTBILR_REG_OFFSET            0x02C
#define GPT_GPTMTAMATCHR_REG_OFFSET         0x030
#define GPT_GPTMTBMATCHR_REG_OFFSET         0x034
#define GPT_GPTMTAPR_REG_OFFSET             0x038
#define GPT_GPTMTBPR_REG_OFFSET             0x03C
#define GPT_GPTMTAPMR_REG_OFFSET            0x040
#define GPT_GPTMTBPMR_REG_OFFSET            0x044
#define GPT_GPTMTAR_REG_OFFSET              0x048
#define GPT_GPTMTBR_REG_OFFSET              0x04C
#define GPT_GPTMTAV_REG_OFFSET              0x050
#define GPT_GPTMTBV_REG_OFFSET              0x054
#define GPT_GPTMRTCPD_REG_OFFSET            0x058
#define GPT_GPTMTAPS_REG_OFFSET             0x05C
#define GPT_GPTMTBPS_REG_OFFSET             0x060
#define GPT_GPTMTAPV_REG_OFFSET             0x064
#define GPT_GPTMTBPV_REG_OFFSET             0x068


/* Port Clock register */
#define SYSCTL_REGCGC2_REG        (*((volatile uint32 *)0x400FE108))

 
#endif  /*GPT_REGS_H */

/**********************************************************************************************************************
 *                          END OF FILE: GPT_REGS.h
 *********************************************************************************************************************/
 

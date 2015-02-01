/**
 * @file  hal_board.c
 *
 * Copyright 2010 Texas Instruments, Inc.
******************************************************************************/
#include "msp430f5529.h"
#include "hal_MSP-EXP430F5438.h"

/**********************************************************************//**
 * @brief  Initializes ACLK, MCLK, SMCLK outputs on P11.0, P11.1,
 *         and P11.2, respectively.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halBoardOutputSystemClock(void) //outputs clock to testpoints
{
  CLK_PORT_DIR |= BIT0;
  CLK_PORT_SEL |= BIT0;
}

/**********************************************************************//**
 * @brief  Stops the output of ACLK, MCLK, SMCLK on P11.0, P11.1, and P11.2.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halBoardStopOutputSystemClock(void)
{
  CLK_PORT_OUT &= ~BIT0;
  CLK_PORT_DIR |= BIT0;
  CLK_PORT_SEL &= ~BIT0;
}

/**********************************************************************//**
 * @brief  Initializes all GPIO configurations.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halBoardInit(void)
{
  //Tie unused ports

  PAOUT  = 0;
  PADIR  = 0xFFFF;
  PASEL  = 0;
  PBOUT  = 0;
  PBDIR  = 0xFFFF;
  PBSEL  = 0;
  PCOUT  = 0;
  PCDIR  = 0xFFFF;
  PCSEL  = 0; //Selects the crystal pins XT1 (pin 5.4, 5.5)
  P5SEL = BIT4+BIT5;
  PDOUT  = 0;
  PDDIR  = 0xFFFF;
  PDSEL  = 0;


}

/**********************************************************************//**
 * @brief  Set function for MCLK frequency.
 *
 *
 * @return none
 *************************************************************************/
void hal430SetSystemClock(unsigned long req_clock_rate, unsigned long ref_clock_rate)
{
  /* Convert a Hz value to a KHz value, as required
   *  by the Init_FLL_Settle() function. */
  unsigned long ulCPU_Clock_KHz = req_clock_rate / 1000UL;

  //Make sure we aren't overclocking
  if(ulCPU_Clock_KHz > 25000L)
  {
    ulCPU_Clock_KHz = 25000L;
  }

  //Set VCore to a level sufficient for the requested clock speed.
  if(ulCPU_Clock_KHz <= 8000L)
  {
    SetVCore(PMMCOREV_0);
  }
  else if(ulCPU_Clock_KHz <= 12000L)
  {
    SetVCore(PMMCOREV_1);
  }
  else if(ulCPU_Clock_KHz <= 20000L)
  {
    SetVCore(PMMCOREV_2);
  }
  else
  {
    SetVCore(PMMCOREV_3);
  }

  //Set the DCO
  Init_FLL_Settle( ( unsigned short )ulCPU_Clock_KHz, req_clock_rate / ref_clock_rate );
}

/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd. 
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


/* BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER.   
 * 
 * This file only supports UART 1
 */

/* Standard includes. */
#include <stdlib.h>
#include <legacymsp430.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

/* Demo application includes. */
#include "serial.h"

/* Constants required to setup the hardware. */
//OLD(not used any more) #define serTX_AND_RX			( ( unsigned char ) 0x03 )

/* Misc. constants. */
#define serNO_BLOCK				( ( TickType_t ) 0 )

/* Enable the UART Tx interrupt. */
#define vInterruptOn() IFG2 |= UTXIFG1

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars; 

/* The queue used to hold characters waiting transmission. */
static QueueHandle_t xCharsForTx; 

static volatile short sTHREEmpty;

/* Interrupt service routines. */
interrupt (USCI_A1_VECTOR) wakeup vRxTxISR( void );
//REMOVED: interrupt is in a single vector!
//interrupt (USCI_A1_VECTOR) wakeup vTxISR( void );

/*-----------------------------------------------------------*/

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
unsigned long ulBaudRateCount;

	/* Initialise the hardware. */

	/* Generate the baud rate constants for the wanted baud rate. */
	ulBaudRateCount = configCPU_CLOCK_HZ / ulWantedBaud;

	portENTER_CRITICAL();
	{
		/* Create the queues used by the com test task. */
		xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
		xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );

		/* Reset UART. */
		//OLD: UCTL1 |= SWRST;
		UCA1CTL1 |= UCSWRST;

		/* Set pin function. */
		// OLD: P4SEL |= serTX_AND_RX;
		P4SEL |= BIT4 + BIT5; // pin 4.4,5=UCA1RXD,TXD


		/* All other bits remain at zero for n, 8, 1 interrupt driven operation. 
		LOOPBACK MODE!*/
		//REMOVED: U1CTL |= CHAR + LISTEN;
		//OLD: U1TCTL |= SSEL1;
		UCA1CTL1 |= UCSSEL1;

		/* Setup baud rate low byte. */
		//OLD: U1BR0 = ( unsigned char ) ( ulBaudRateCount & ( unsigned long ) 0xff );
		UCA1BR0 = ( unsigned char ) ( ulBaudRateCount & ( unsigned long ) 0xff );

		/* Setup baud rate high byte. */
		ulBaudRateCount >>= 8UL;
		//OLD: U1BR1 = ( unsigned char ) ( ulBaudRateCount & ( unsigned long ) 0xff );
		UCA1BR1 = ( unsigned char ) ( ulBaudRateCount & ( unsigned long ) 0xff );

		/* Enable ports. */
		//REMOVED: ME2 |= UTXE1 + URXE1;

		//NEW (from uscia0_uart_01 example):
		UCA1MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0

		/* Set. */
		//OLD: UCTL1 &= ~SWRST;
		UCA1CTL1 &= ~UCSWRST;

		/* Nothing in the buffer yet. */
		sTHREEmpty = pdTRUE;

		/* Enable interrupts. */
		//OLD: IE2 |= URXIE1 + UTXIE1;
		UCA1IE |= UCRXIE + UCTXIE;
	}
	portEXIT_CRITICAL();
	
	/* Unlike other ports, this serial code does not allow for more than one
	com port.  We therefore don't return a pointer to a port structure and can
	instead just return NULL. */
	return NULL;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
signed portBASE_TYPE xReturn;

	/* Transmit a character. */

	portENTER_CRITICAL();
	{
		if( sTHREEmpty == pdTRUE )
		{
			/* If sTHREEmpty is true then the UART Tx ISR has indicated that 
			there are no characters queued to be transmitted - so we can
			write the character directly to the shift Tx register. */
			sTHREEmpty = pdFALSE;
			//OLD: U1TXBUF = cOutChar;
			UCA1TXBUF = cOutChar;
			xReturn = pdPASS;
		}
		else
		{
			/* sTHREEmpty is false, so there are still characters waiting to be
			transmitted.  We have to queue this character so it gets 
			transmitted	in turn. */

			/* Return false if after the block time there is no room on the Tx 
			queue.  It is ok to block inside a critical section as each task
			maintains it's own critical section status. */
			xReturn = xQueueSend( xCharsForTx, &cOutChar, xBlockTime );

			/* Depending on queue sizing and task prioritisation:  While we 
			were blocked waiting to post on the queue interrupts were not 
			disabled.  It is possible that the serial ISR has emptied the 
			Tx queue, in which case we need to start the Tx off again
			writing directly to the Tx register. */
			if( ( sTHREEmpty == pdTRUE ) && ( xReturn == pdPASS ) )
			{
				/* Get back the character we just posted. */
				xQueueReceive( xCharsForTx, &cOutChar, serNO_BLOCK );
				sTHREEmpty = pdFALSE;
				//OLD: U1TXBUF = cOutChar;
				UCA1TXBUF = cOutChar;
			}
		}
	}
	portEXIT_CRITICAL();

	return pdPASS;
}
/*-----------------------------------------------------------*/

/*
 * UART RX interrupt service routine.
 */
//OLD: interrupt (UART1RX_VECTOR) wakeup vRxISR( void )
interrupt (USCI_A1_VECTOR) wakeup vRxTxISR( void )
{
signed char cChar;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
//TODO make a select case if it is RX or TX (looking at the interrupt flag)

	/* Get the character from the UART and post it on the queue of Rxed 
	characters. */
	//OLD: cChar = U1RXBUF;
	cChar = UCA1RXBUF;

	xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );

	if( xHigherPriorityTaskWoken )
	{
		/*If the post causes a task to wake force a context switch 
		as the woken task may have a higher priority than the task we have 
		interrupted. */
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/
//REMOVED!!!
/*
 * UART Tx interrupt service routine.

//OLD: interrupt (UART1TX_VECTOR) wakeup vTxISR( void )
interrupt (USCI_A1_VECTOR) wakeup vTxISR( void ) //TODO: this conflicts with the interrupt in the port.c file
{
signed char cChar;
portBASE_TYPE xTaskWoken = pdFALSE;

	/* The previous character has been transmitted.  See if there are any
	further characters waiting transmission.

	if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xTaskWoken ) == pdTRUE )
	{
		/* There was another character queued - transmit it now.
		//OLD: U1TXBUF = cChar;
		UCA1TXBUF = cChar;
	}
	else
	{
		/* There were no other characters to transmit.
		sTHREEmpty = pdTRUE;
	}
}
*/
/*---------------------------------------------------------------------------
 *      RL-ARM - RTX  --  example code for "RTOS" lab, CPE-312
 *---------------------------------------------------------------------------*/

#include "stm32l1xx.h"
#include "stdio.h"
#include "discover_board.h"
#include "stm32l_discovery_lcd.h"
#include "additional_config.h"

int data = 3;
char strDisp[6] ;

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {
	
	SystemCoreClockSetHSI();		/* set HSI as SystemCoreClock */
	LCD_Configuration();				/* configure LCD */
	PB6_7_OUTPUT();							/* configure PB6 and PB7 */
	PA12_OUTPUT();							/* configure PA12 as OUTPUT */
	PC12_OUTPUT();							/* configure PC12 as OUTPUT */
	PD2_OUTPUT();								/* configure PD2 as OUTPUT */
	PA0_4_5_INPUT();						/* configure PA0, PA4 and PA5 as INPUT */
	PA11_INPUT();								/* configure PA11 as INPUT */
//	PA11_INPUT_INT();						/* configure PA11 for external interrupt */

	data = 3;
	sprintf(strDisp, "RTX-L%d", data);
	LCD_GLASS_DisplayString((unsigned char *) strDisp);
}

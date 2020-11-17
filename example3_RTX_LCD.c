/*---------------------------------------------------------------------------
 *      RL-ARM - RTX  --  example code for "RTOS" lab, CPE-312
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include "stm32l1xx.h"
#include "stdio.h"
#include "discover_board.h"
#include "stm32l_discovery_lcd.h"
#include "additional_config.h"

#define r 0
#define p 1

int sw=1, state=r;
int data = 0;
char strDisp[6] ;

/* Task IDs will contain task identifications at run-time. */
OS_TID id0, PA11, show;

/* Forward declaration of tasks. */
__task void initTask (void);
__task void swPA11 (void);
__task void showLCD (void);

/*---------------------------------------------------------------------------
  Tasks : Implement tasks
 *---------------------------------------------------------------------------*/
__task void initTask (void){
  id0 = os_tsk_self();
	os_tsk_prio_self(5);

  PA11 = os_tsk_create (swPA11, 2);
  show = os_tsk_create (showLCD, 2);

	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	os_tsk_delete_self();
}
	
__task void swPA11 (void){
  for (;;) {
		sw = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
		if((sw == 0) && (state == r)) {
			state = p;
			data ++;
			sprintf(strDisp, "RTOS %d", data );
			os_evt_set(0x4, show);
		}
		else if ((sw == 1) && (state == p)) {
			state = r;
		}
  }
}

__task void showLCD (void){
  for (;;) {
    os_evt_wait_or(0x4, 0xFFFF);
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString((unsigned char *) strDisp);
		os_tsk_pass();
  }
}

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

	/* Start the RTX kernel, and then create and execute "initTask". */
  os_sys_init(initTask);
}

void EXTI15_10_IRQHandler (void) {
	if(EXTI_GetITStatus(EXTI_Line11) == SET)
  {
    sprintf(strDisp, "exPA11");
    isr_evt_set(0x0004, show);
    EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

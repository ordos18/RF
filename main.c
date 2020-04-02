#include "avr_compiler.h"
#include "led.h"
#include "rf.h"
#include "port.h"
#include <avr/sleep.h>

#define RF_IRQ_bm (1<<2)

static unsigned char ucMsgCtr=0;

ISR(PORTC_INT0_vect){ // interrupt service routine
	rf_send_byte(ucMsgCtr++);
	led_toggle();
	//PORT_ClearInterruptFlags(&PORTC, RF_IRQ_bm);
	rf_clear_int_flags();
};

int main (void){
	led_init();
	PORTC.DIRCLR = RF_IRQ_bm;
	PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTC.INT0MASK = RF_IRQ_bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei(); // global interrupts enabling
	rf_init_as_tx();
	rf_send_byte(ucMsgCtr++);
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	while(1) {
		sleep_cpu();
	}
}
/*
current consumption (V_PROG): 
	4.88 mA (98% compared to 4.98 mA)
		
Result: 0.1 mA less
*/

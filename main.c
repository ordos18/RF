#include "avr_compiler.h"
#include "led.h"
#include <avr/sleep.h>

#define BOOT_BTN_bm (1<<3)

ISR(PORTC_INT0_vect){ // interrupt service routine
	led_toggle();
};

int main (void){
	led_init();
	PORTC.DIRCLR = BOOT_BTN_bm;
	PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTC.INT0MASK = BOOT_BTN_bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei(); // global interrupts enabling
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	while(1) {
		sleep_cpu();
	};
}
/*
current consumption (V_PROG): 
	LED OFF:
		0.71 mA (52% compared to 1.37 mA)
	LED ON:
		0.94 mA (59% compared to 1.60 mA)
		
Result: 0.66 mA less
*/

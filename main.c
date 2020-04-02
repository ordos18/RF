#include "avr_compiler.h"
#include "led.h"
#include <avr/sleep.h>

ISR(RTC_OVF_vect) { led_set(); }
ISR(RTC_COMP_vect) {led_clear();}

int main (void) {
	led_init();
	RTC.PER = 1000;
	RTC.CNT = 0;
	RTC.COMP = 50;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_COMPINTLVL_LO_gc | RTC_OVFINTLVL_LO_gc;
	CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	sei();
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	while(1) {
		sleep_cpu();
	}
}
/*
current consumption (V_PROG): 
!! POWER_SAVE used instead of POWER_DOWN

uC:		~1 uA (0.074% compared to 1.35 mA)
LED:	~230 uA
AVG:	~12.5 uA
		
Result: 1.35 mA less
Typical capacity of CR2032 battery: 220 mAh
Battery should last 17670 h (~2 years)
*/

#include "avr_compiler.h"
#include "acc.h"
#include "rf.h"
#include <avr/sleep.h>

#define SEND_RATE_HZ 100

ISR(RTC_OVF_vect) { 
	rf_send_byte(acc_read());
}

int main (void) {
	acc_init();
	RTC.PER = 1000/SEND_RATE_HZ - 1;
	RTC.CNT = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;
	CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
	
	rf_init_as_tx();
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	while(1) {
		sleep_cpu();
	}
}
/*
current consumption (V_PROG): 

2 Hz	~100 uA (2200 h = 92 days on CR2032 battery)
100 Hz	430 uA (511 h 21 days on CR2032 battery)

Typical capacity of CR2032 battery: 220 mAh
*/

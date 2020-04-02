#include "avr_compiler.h"
#include "led.h"
#include "rf.h"
#include <avr/sleep.h>

static unsigned char ucMsgCtr=0;

#define SEND_RATE_HZ 100

ISR(RTC_OVF_vect) { 
	rf_send_byte(ucMsgCtr++);
	//led_toggle();
}

int main (void) {
	led_init();
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

2 Hz	~30 uA (7333 h = 306 days on CR2032 battery)
100 Hz	180 uA (1222 h = 51 days on CR2032 battery)

Typical capacity of CR2032 battery: 220 mAh
*/

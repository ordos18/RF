#include "led.h"
#include "rf.h"
#include "port.h"

int main (void){
	
	led_init();
	rf_init_as_tx();
	while(1) {
		static unsigned char ucMsgCtr=0;
		delay_ms(10);
		rf_send_byte(ucMsgCtr++);
		led_toggle();
	}
}

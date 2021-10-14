#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "adc.h"
#include "servo.h"
#include "lcd.h"
#include "timer.h"

#define _SETTING_PWM_
// #define _SETTTING_LCD_
#define DUR 500
#define POT_OFFSET 0
#define POT_GAIN 1.25

/*==================================================
[Arduino UNO]
TIFR0: R | R | R | R | R | OCFB | OCFA | TOV
==================================================*/
int main() {

#ifdef _SETTING_PWM_
  PWM_init();
  SERVO_set_duty(DUTY_0DEG);
  _delay_ms(1000);
  SERVO_set_duty(DUTY_180DEG);
  _delay_ms(1000);
#endif

#ifdef _SETTING_LCD_
  LCD_init();
  LCD_clear();
  LCD_str("READY>");
  _delay_ms(300);  
  LCD_clear();
#endif

  ADC_init();
  ADC_set_channel(0);


  while (1) {
#ifdef _SETTING_LCD_    
    LCD_clear();
    LCD_str(utoa(ADC_read(), string, 10));
#endif
    
#ifdef _SETTING_PWM_
    SERVO_set_duty((ADC_read() / POT_GAIN ) - POT_OFFSET);
    // _delay_us(1);
#endif  

  }
  return 0;
}
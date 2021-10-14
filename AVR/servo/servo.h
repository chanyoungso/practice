#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"
/*==================================================
[Tower Pro SG90]
Orange: PWM
Red:    VCC(5V)
Brown:  GND
==================================================*/

#define DUTY_0DEG 170
#define DUTY_45DEG
#define DUTY_90DEG 410
#define DUTY_135DEG
#define DUTY_180DEG 710  // 730

void SERVO_set_deg(char deg) {
    int duty = (int) deg;
    PWM_set(duty);
}

void SERVO_set_duty(int duty) {
    PWM_set(duty);
}
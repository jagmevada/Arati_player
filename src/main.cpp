#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "header.h"

uint8_t state = 0;
void setup_timer_rtc() {
  RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;  // 1khz internal
  RTC.CTRLA = CLKCTRL_RUNSTDBY_bm | RTC_RTCEN_bm |
              RTC_PRESCALER0_bm;  // run in standby sleep mode and enable rtc no
                                  // prescaler
  RTC.PITCTRLA = RTC_PERIOD_CYC1024_gc | RTC_PITEN_bm;  // every 2 sec interrupt
  RTC.PITINTCTRL = RTC_PI_bm;                           // enable interrupt
}
void button5() {
  if (digitalRead(10) == HIGH) {
    state = !state;
    digitalWrite(3, state);
  }
}

void setup() {
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  // pinMode(7, INPUT_PULLUP);
  // pinMode(8, INPUT_PULLUP);
  // pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(7, 0);
  digitalWrite(8, 0);
  digitalWrite(9, 0);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  ADC0.CTRLA &= ~ADC_ENABLE_bm;
  //   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // set_sleep_mode(SLEEP_MODE_IDLE); //380UA
  set_sleep_mode(SLEEP_MODE_STANDBY);
  sleep_enable();
  attachInterrupt(10, button5, CHANGE);
  // setup_timer_a();
  // setup_timer_b0();  // use either a0 or b0
  setup_timer_rtc();
  sei();  // Enable global interrupts
}

void loop() {
  // put your main code here, to run repeatedly:
  // ADC0.CTRLA &= ~ADC_ENABLE_bm;
  sleep_cpu();
  // delay(1000);
  // digitalWrite(3, 1);
  // delay(1000);
  // digitalWrite(3, 0);

  Serial.begin(9600);
  pinMode(IRPIN, INPUT);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_MIDDLE, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(LEDPIN, OUTPUT);
  ledstate = 1;
  digitalWrite(LEDPIN, ledstate);
  attachInterrupt(IRPIN, remotex, FALLING);
  attachInterrupt(BUTTON_LEFT, button_left, FALLING);
  attachInterrupt(BUTTON_MIDDLE, button_middle, FALLING);
  attachInterrupt(BUTTON_RIGHT, button_right, FALLING);

  tblink = millis();
  tdbl = tblink;
  tdbm = tblink;
  tdbr = tblink;

  player.begin();
  player.setVolume(volume);
}

ISR(RTC_PIT_vect) {  /// 1sec timer  /// 5 sleep and 5 sec awake
  static unsigned int sec = 5, timercount = sec, xx = 0;
  timercount--;
  if (timercount == 0) {
    timercount = sec;
    xx = !xx;
    if (xx) {
      pinMode(7, OUTPUT);
      pinMode(8, OUTPUT);
      pinMode(9, OUTPUT);

      sleep_disable();
    } else {
      // PORTB_OUT &= ~(PIN0_bm | PIN1_bm | PIN2_bm);
      // digitalWrite(7, 1);
      // digitalWrite(8, 1);
      // digitalWrite(9, 1);
      pinMode(7, INPUT_PULLUP);  // Only pullup saves power in sleep mode ,
                                 // output low doesnt
      pinMode(8, INPUT_PULLUP);
      pinMode(9, INPUT_PULLUP);
      sleep_enable();
    }
    // digitalWrite(3, xx);
  }
  RTC.PITINTFLAGS = RTC_PI_bm; /* Clear the interrupt flag */
}
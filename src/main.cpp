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
// void button5() {
//   if (digitalRead(10) == HIGH) {
//     state = !state;
//     digitalWrite(3, state);
//   }
// }

void setup() {
  pinMode(0, INPUT_PULLUP);              // 0
  pinMode(BUTTON_LEFT, INPUT_PULLUP);    // 1 // in PU
  pinMode(IRPIN, INPUT);                 // 2 // ex PU
  pinMode(BUTTON_MIDDLE, INPUT_PULLUP);  // 3 // in PU
  pinMode(MGATE, OUTPUT);                // 4 // ex PD OUT
  pinMode(CHRGIN, INPUT);                // 5 // ex PD IN
  pinMode(LEDPIN, OUTPUT);               // 6 // DFLT 0
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);   // 7 // in PU
  pinMode(9, INPUT_PULLUP);              // 9
  pinMode(10, INPUT_PULLUP);             // 10
  ADC0.CTRLA &= ~ADC_ENABLE_bm;

  ledstate = 1;
  digitalWrite(LEDPIN, ledstate);
  gatestate = 1;
  digitalWrite(MGATE, gatestate);
  // digitalWrite(CHARGEIN, gatestate);
  attachInterrupt(IRPIN, remotex, FALLING);
  attachInterrupt(CHRGIN, chargein, CHANGE);
  attachInterrupt(BUTTON_LEFT, button_left, FALLING);
  attachInterrupt(BUTTON_MIDDLE, button_middle, FALLING);
  attachInterrupt(BUTTON_RIGHT, button_right, FALLING);

  //   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // set_sleep_mode(SLEEP_MODE_IDLE); //380UA
  set_sleep_mode(SLEEP_MODE_STANDBY);
  sleep_enable();
  // setup_timer_a();
  // setup_timer_b0();  // use either a0 or b0
  setup_timer_rtc();

  // Serial.begin(9600);

  tblink = millis();
  tdbl = tblink;
  tdbm = tblink;
  tdbr = tblink;
  tdbc = tblink;
  tplayertime = tblink;
  player.begin();
  player.setVolume(volume);

  sei();  // Enable global interrupts
}

void loop() {
  delay(10);
  if (chargeint) {
    chargeint = 0;
    delay(10);
    chargestate();
  }
  check_remote();
  if (blinkon) {
    static unsigned char blinkcount = 0;
    if (millis() - tblink > 20) {
      blinkcount++;
      if (ledstate)
        ledstate = 0;
      else
        ledstate = 1;
      digitalWrite(LEDPIN, ledstate);
      tblink = millis();
    }
    if (blinkcount > 8) {
      blinkon = 0;
      ledstate = 1;
      blinkcount = 0;
      digitalWrite(LEDPIN, ledstate);
    }
  }
  // sleep_cpu();
}

ISR(RTC_PIT_vect) {  /// 1sec timer  /// 5 sleep and 5 sec awake
  static unsigned int sec = 5, timercount = sec, xx = 0;

  timercount--;
  if (timercount == 0) {
    timercount = sec;
    chargeint = 1;
  }
  RTC.PITINTFLAGS = RTC_PI_bm; /* Clear the interrupt flag */
}
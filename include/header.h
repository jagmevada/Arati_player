#include <Arduino.h>

#include "DYPlayerArduino.h"
#define BUTTON_LEFT 1    // in PU
#define IRPIN 2          // ex PU
#define BUTTON_MIDDLE 3  // in PU
#define MGATE 4          // ex PD OUT
#define CHRGIN 5         // ex PD IN
#define LEDPIN 6         // DFLT 0
#define BUTTON_RIGHT 7   // in PU
#define DEBOUNCE_DELAY 200
#define MAXPLAYTIME 3600000

#define UP 2000
#define CHPLUS 0xA25D
#define CHONLY 0x629D
#define CHMINUS 0xE21D
#define PREV 0x22DD
#define NEXT 0x02FD
#define PAUSE 0xC23D
#define MINUS 0xE01F
#define PLUS 0xA857
#define EQUA 0x906F
#define ONEHUNDERD 0x9867
#define TWOHUNDRED 0xB04F
#define DZERO 0x6897
#define DONE 0x30CF
#define DTWO 0x18E7
#define DTHREE 0x7A85
#define DFOUR 0x10EF
#define DFIVE 0x38C7
#define DSIX 0x5AA5
#define DSEVEN 0x42BD
#define DEIGHT 0x4AB5
#define DNINE 0x52AD
#define MORNINGTIME 920000
#define EVENINGTIME 3730000
#define OTHERTIME 600000

DY::Player player(&Serial1);
unsigned long int t = 0, t1 = 0, tblink = 0, tdbl, tdbm, tdbr, tdbc,
                  tplayertime, maxplaytime = 1200000;
unsigned int xx[40];
unsigned int rx = 0, blinkon = 0, ledstate = 0;
unsigned char newx = 0, i = 0, k = 0;
unsigned char volume = 25, pauseplay = 0;
uint8_t gatestate = 0, chargeint = 0;

void remotex();
void check_remote();
void button_left();
void button_middle();
void button_right();
void chargein();
void chargestate();

//////////////////////////////////
void chargein() {
  if (millis() - tdbc > DEBOUNCE_DELAY) {
    chargeint = 1;
    tdbc = millis();
  }
}

void chargestate() {
  if (!digitalRead(CHRGIN)) {
    if (millis() - tplayertime > maxplaytime) {
      gatestate = 1;
    }
  } else {
    tplayertime = millis();
    gatestate = 0;
  }
  digitalWrite(MGATE, gatestate);
}

//////////////////////////////////
void button_left() {
  if (millis() - tdbl > DEBOUNCE_DELAY) {
    // // Serial.println("ONEHUNDERD");
    blinkon = 1;
    player.playSpecified(1);
    tdbl = millis();
    maxplaytime = MORNINGTIME;
  }
}

//////////////////////////////////
void button_middle() {
  if (millis() - tdbm > DEBOUNCE_DELAY) {
    // // Serial.println("TWOHUNDERD");
    blinkon = 1;
    player.playSpecified(2);
    tdbm = millis();
    maxplaytime = EVENINGTIME;
  }
}
//////////////////////////////////
void button_right() {
  if (millis() - tdbr > DEBOUNCE_DELAY) {
    // // Serial.println("PAUSE");
    blinkon = 1;
    if (pauseplay) {
      pauseplay = 0;
      player.play();
    } else {
      pauseplay = 1;
      player.pause();
    }
    tdbr = millis();
  }
}
////////////////////////////////
void remotex() {
  if (i > 17 && i < 34) {
    if ((micros() - t) > UP) {
      xx[i - 18] = 2;
      rx |= 1 << k;
    } else
      xx[i - 18] = 1;
    k--;
  } else if (i <= 17) {
    k = 15;
    rx = 0;
  } else
    ;
  i++;
  t = micros();
  newx = 1;
}
//////////////////////////////////////////////////////
void check_remote() {
  if (micros() - t > 80000) {
    if (!newx)
      t = micros();
    else {
      // Serial.println(rx, HEX);
      t = micros();
      newx = 0;
      switch (rx) {
        case CHPLUS:
          // Serial.println("CHPLUS");
          break;
        case CHONLY:
          // Serial.println("CHONLY");
          break;
        case CHMINUS:
          // Serial.println("CHMINUS");
          break;
        case PREV:
          // Serial.println("PREV");
          break;
        case NEXT:
          // Serial.println("NEXT");
          break;
        case PAUSE:
          // Serial.println("PAUSE");
          blinkon = 1;
          if (pauseplay) {
            pauseplay = 0;
            player.play();
          } else {
            pauseplay = 1;
            player.pause();
          }
          break;
        case MINUS:
          // Serial.println("MINUS");
          blinkon = 1;
          if (volume >= 5) volume -= 5;
          player.setVolume(volume);
          break;
        case PLUS:
          // Serial.println("PLUS");
          blinkon = 1;
          volume += 5;
          if (volume > 30) volume = 30;
          player.setVolume(volume);
          break;
        case EQUA:
          // Serial.println("EQUA");
          break;
        case ONEHUNDERD:
          // Serial.println("ONEHUNDERD");
          blinkon = 1;
          player.playSpecified(1);
          maxplaytime = MORNINGTIME;
          break;
        case TWOHUNDRED:
          // Serial.println("TWOHUNDRED");
          blinkon = 1;
          player.playSpecified(2);
          maxplaytime = EVENINGTIME;
          break;
        case DZERO:
          // Serial.println("DZERO");
          blinkon = 1;
          player.playSpecified(12);
          maxplaytime = OTHERTIME;
          break;
        case DONE:
          // Serial.println("DONE");
          blinkon = 1;
          player.playSpecified(3);
          maxplaytime = OTHERTIME;
          break;
        case DTWO:
          // Serial.println("DTWO");
          blinkon = 1;
          player.playSpecified(4);
          maxplaytime = OTHERTIME;
          break;
        case DTHREE:
          // Serial.println("DTHREE");
          blinkon = 1;
          player.playSpecified(5);
          maxplaytime = OTHERTIME;
          break;
        case DFOUR:
          // Serial.println("DFOUR");
          blinkon = 1;
          player.playSpecified(6);
          maxplaytime = OTHERTIME;
          break;
        case DFIVE:
          // Serial.println("DFIVE");
          blinkon = 1;
          player.playSpecified(7);
          maxplaytime = OTHERTIME;
          break;
        case DSIX:
          // Serial.println("DSIX");
          blinkon = 1;
          player.playSpecified(8);
          maxplaytime = OTHERTIME;
          break;
        case DSEVEN:
          // Serial.println("DSEVEN");
          blinkon = 1;
          player.playSpecified(9);
          maxplaytime = OTHERTIME;
          break;
        case DEIGHT:
          // Serial.println("DEIGHT");
          blinkon = 1;
          player.playSpecified(10);
          maxplaytime = OTHERTIME;
          break;
        case DNINE:
          // Serial.println("DNINE");
          blinkon = 1;
          player.playSpecified(11);
          maxplaytime = OTHERTIME;
          break;

        default:
          // Serial.println("MISS");
          break;
      }
    }
    i = 0;
  }
}
//////##################################################################

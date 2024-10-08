#include <string.h>
#define IBUS_BUFFSIZE 32    
#define IBUS_MAXCHANNELS 10 // I am using only 10 channels because my TX (FlySky i6) supports max 10 channels
#include <Servo.h>
static uint8_t ibusIndex = 0;
static uint8_t ibus[IBUS_BUFFSIZE] = {0};
static uint16_t rcValue[IBUS_MAXCHANNELS];

static boolean rxFrameDone;

int ch_width_1;
int ch_width_2;
int ch_width_3;
int ch_width_4;
int ch_width_5;
int ch_width_6;
int ch_width_7;
int ch_width_8;
int ch_width_9;
int ch_width_10;

Servo ch1; // create servo object to control a servo
Servo ch2;
Servo ch3;
Servo ch4;
Servo ch5;
Servo ch6;
Servo ch7;
Servo ch8;
Servo ch9;
Servo ch10;

void setup() {
  Serial.begin(115200);
  ch1.attach(2); // Digital pin of Arduino UNO
  ch2.attach(3); // attaches the servo on pin 3 to the servo object
  ch3.attach(4);
  ch4.attach(5);
  ch5.attach(6);
  ch6.attach(7);
  ch7.attach(8);
  ch8.attach(9);
  ch9.attach(10);
  ch10.attach(11);

  pinMode(12, OUTPUT); // 12번 핀을 출력으로 설정
  pinMode(11, OUTPUT); // 11번 핀을 출력으로 설정
  pinMode(13, OUTPUT); // 13번 핀을 출력으로 설정
}

void loop() {
  readRx();
}

void readRx() {
  rxFrameDone = false;

  if (Serial.available()) {
    uint8_t val = Serial.read();
    // Look for 0x2040 as start of packet
    if (ibusIndex == 0 && val != 0x20){
      ibusIndex = 0;
      return;
    }
    if (ibusIndex == 1 && val != 0x40) {
      ibusIndex = 0;
      return;
    }
    if (ibusIndex == IBUS_BUFFSIZE){
      ibusIndex = 0;
      int high=3;
      int low=2;
      for(int i=0;i<IBUS_MAXCHANNELS; i++) {
        rcValue[i] = (ibus[high] << 8) + ibus[low];
        high += 2;
        low += 2;
      }
      ch_width_1 = map(rcValue[0], 1000, 2000, 1000, 2000);
      ch1.writeMicroseconds(ch_width_1);

      Serial.print(ch_width_1);
      Serial.print("     ");

      ch_width_2 = map(rcValue[1], 1000, 2000, 1000, 2000);
      ch2.writeMicroseconds(ch_width_2);

      Serial.print(ch_width_2);
      Serial.print("     ");

      ch_width_3 = map(rcValue[2], 1000, 2000, 1000, 2000);
      ch3.writeMicroseconds(ch_width_3);

      Serial.print(ch_width_3);
      Serial.print("     ");

      ch_width_4 = map(rcValue[3], 1000, 2000, 1000, 2000);
      ch4.writeMicroseconds(ch_width_4);

      Serial.print(ch_width_4);
      Serial.print("     ");

      ch_width_5 = map(rcValue[4], 1000, 2000, 1000, 2000);
      ch5.writeMicroseconds(ch_width_5);

      Serial.print(ch_width_5);
      Serial.print("      ");

      ch_width_6 = map(rcValue[5], 1000, 2000, 1000, 2000);
      ch6.writeMicroseconds(ch_width_6);

      Serial.print(ch_width_6);
      Serial.print("      ");

      ch_width_7 = map(rcValue[6], 1000, 2000, 1000, 2000);
      ch7.writeMicroseconds(ch_width_7);

      Serial.print(ch_width_7);
      Serial.print("      ");

      ch_width_8 = map(rcValue[7], 1000, 2000, 1000, 2000);
      ch8.writeMicroseconds(ch_width_8);
      Serial.print(ch_width_8);
      Serial.print("     ");

      ch_width_9 = map(rcValue[8], 1000, 2000, 1000, 2000);
      ch9.writeMicroseconds(ch_width_9);

      Serial.print(ch_width_9);
      Serial.print("     ");

      ch_width_10 = map(rcValue[9], 1000, 2000, 1000, 2000);
      ch10.writeMicroseconds(ch_width_10);

      Serial.print(ch_width_10);
      Serial.println("     ");

      // Check if channel 6 (switch B) is in the shutdown position
      if (rcValue[5] >= 1900) {
        Serial.println("SHUTDOWN"); // 젯슨 나노에 셧다운 명령 전송
      }

      // 추가된 코드: ch5의 pwm 값에 따라 다이오드 제어
      if (rcValue[4] >= 1900) {
        digitalWrite(12, HIGH); // 12번 핀 ON
        digitalWrite(10, LOW);  // 11번 핀 OFF
        digitalWrite(13, LOW);  // 13번 핀 OFF
      } else if (rcValue[4] >= 1300 && rcValue[4] <= 1700) {
        digitalWrite(12, LOW);  // 12번 핀 OFF
        digitalWrite(10, HIGH); // 11번 핀 ON
        digitalWrite(13, LOW);  // 13번 핀 OFF
      } else if (rcValue[4] <= 1100) {
        digitalWrite(12, LOW);  // 12번 핀 OFF
        digitalWrite(10, LOW);  // 11번 핀 OFF
        digitalWrite(13, HIGH); // 13번 핀 ON
      }

      rxFrameDone = true;
      return;
    }
    else {
      ibus[ibusIndex] = val;
      ibusIndex++;
    }
  }
}

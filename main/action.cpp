#include <Arduino.h>
#include <ESP32Servo.h>
#include <Preferences.h>

#define TIMES_WALK 1
#define SERVO_LOOP_DELAY 1
#define STEP_DELAY 1000

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

Preferences preferences;

int minUs = 500;
int maxUs = 2500;
int servo1Pin = 35;  // leftfront
int servo2Pin = 36;  // leftback
int servo3Pin = 37;  // rightfront
int servo4Pin = 38;  // rightback
int servo5Pin = 39;  // head
float ang[] = { 0, 0, 0, 0, 0, 0 };
float osang[] = { 0, 0, 0, 0, 0, 0 };  //offset angle
float AngS[] = { 0, 0, 0, 0, 0, 0 };   // the current angle pos
int timewalk1 = 100;
int timewalk2 = 50;
int timeLT = 300;  // time for left turn
int timeST = 100;  // time for right turn



float input_ang(int x, float y) {
  float Aang, Bang;  //0-180
  switch (x) {
    case 1:
      Aang = 220 - y - osang[1];
      break;
    case 2:
      Aang = 40 - y - osang[2];
      break;
    case 3:
      Aang = 40 + y + osang[3];
      break;
    case 4:
      Aang = 220 + y + osang[4];
      break;
    case 5:
      Aang = 180 - y;
  }
  Bang = 180 * (Aang) / 260;
  return Bang;
}

void servo(float AngE1, float AngE2, float AngE3, float AngE4, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  float AngTemp[] = { 0, AngE1 - AngS[1], AngE2 - AngS[2], AngE3 - AngS[3], AngE4 - AngS[4] };
  for (int i = 1; i <= timewalk; i++) {
    servo1.write(input_ang(1, AngS[1] + AngTemp[1] * i / timewalk));
    servo2.write(input_ang(2, AngS[2] + AngTemp[2] * i / timewalk));
    servo3.write(input_ang(3, AngS[3] + AngTemp[3] * i / timewalk));
    servo4.write(input_ang(4, AngS[4] + AngTemp[4] * i / timewalk));

    delay(servo_delay);
  }
  AngS[1] = AngE1;
  AngS[2] = AngE2;
  AngS[3] = AngE3;
  AngS[4] = AngE4;
}

// void Servo_Offset();
void MoveInit() {
  Serial.println("MoveInit start");
  preferences.begin("memory", false);

  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);
  servo3.attach(servo3Pin, minUs, maxUs);
  servo4.attach(servo4Pin, minUs, maxUs);
  // servo5.attach(servo5Pin, minUs, maxUs);

  // get osang value from preferences
  osang[1] = preferences.getInt("LFAngle", osang[1]);
  osang[2] = preferences.getInt("LBAngle", osang[2]);
  osang[3] = preferences.getInt("RFAngle", osang[3]);
  osang[4] = preferences.getInt("RBAngle", osang[4]);
  for (int i = 0; i < 4; i++) {
    AngS[i] = osang[i];
    Serial.print("osang");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(osang[i]);
  }

  Serial.println("MoveInit end");
}

void MoveReset() {
  Serial.println("MoveReset start");
  Serial.println("MoveReset servo write");
  // servo1.write(180 * (220 - osang[1]) / 260);
  // servo2.write(180 * (40 - osang[2]) / 260);
  // servo3.write(180 * (40 + osang[3]) / 260);
  // servo4.write(180 * (220 + osang[4]) / 260);
  servo(0, 0, 0, 0, TIMES_WALK, SERVO_LOOP_DELAY);

  Serial.println("MoveReset servo5");
  // servo5.write(input_ang(5, 0));
  Serial.println("MoveReset end");
}

void servoLeftFront(float ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoLeftFront start: ang = ");
  Serial.println(ange);
  float angtmp = ange - AngS[1];
  for (int i = 1; i <= timewalk; i++) {
    // Serial.print("walk loop: ");
    // Serial.println(i);
    servo1.write(input_ang(1, AngS[1] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[1] = ange;
  Serial.println("servoLeftFront end!");
}

void servoLeftBack(float ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoLeftBack start: ang = ");
  Serial.println(ange);
  float angtmp = ange - AngS[2];
  for (int i = 1; i <= timewalk; i++) {
    // Serial.print("walk loop: ");
    // Serial.println(i);
    servo2.write(input_ang(2, AngS[2] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[2] = ange;
  Serial.println("servoLeftBack end!");
}

void servoRightFront(float ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoRightFront start: ang = ");
  Serial.println(ange);
  float angtmp = ange - AngS[3];
  for (int i = 1; i <= timewalk; i++) {
    // Serial.print("walk loop: ");
    // Serial.println(i);
    servo3.write(input_ang(3, AngS[3] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[3] = ange;
  Serial.println("servoRightFront end!");
}

void servoRightBack(float ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoRightBack start: ang = ");
  Serial.println(ange);
  float angtmp = ange - AngS[4];
  for (int i = 1; i <= timewalk; i++) {
    // Serial.print("walk loop: ");
    // Serial.println(i);
    servo4.write(input_ang(4, AngS[4] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[4] = ange;
  Serial.println("servoRightBack end!");
}

void MoveForward(int step_delay) {
  for (int i = 0; i < 3; i++) {
    Serial.print("MoveForward loop count:");
    Serial.println(i);
    servoLeftFront(15, TIMES_WALK, SERVO_LOOP_DELAY);
    delay(step_delay);
    servoRightBack(15, TIMES_WALK, TIMES_WALK);
    delay(step_delay);
    servoRightFront(-15, TIMES_WALK, TIMES_WALK);
    delay(step_delay);
    servoLeftBack(15, TIMES_WALK, TIMES_WALK);
    delay(step_delay);

    servoLeftFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
    delay(step_delay);
    servoRightBack(0, TIMES_WALK, TIMES_WALK);
    delay(step_delay);
    servoRightBack(0, TIMES_WALK, TIMES_WALK);
    delay(step_delay);
    servoLeftBack(0, TIMES_WALK, TIMES_WALK);
    delay(step_delay);
    // servo(15, 0, 0, 0, TIMES_WALK, SERVO_LOOP_DELAY);
    // servo(15, 0, 0, 15, TIMES_WALK, SERVO_LOOP_DELAY);
    // servo(15, 0, -15, 15, TIMES_WALK, SERVO_LOOP_DELAY);
    // servo(15, -15, -15, 15, TIMES_WALK, SERVO_LOOP_DELAY);
    // servo(0, -15, -15, 15, TIMES_WALK, SERVO_LOOP_DELAY);
    // servo(0, -15, -15, 0, TIMES_WALK, SERVO_LOOP_DELAY);
    // servo(0, 0, -15, 0, TIMES_WALK, SERVO_LOOP_DELAY);
    // servo(0, 0, 0, 0, TIMES_WALK, SERVO_LOOP_DELAY);
    delay(step_delay);
  }
}

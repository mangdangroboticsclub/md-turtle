#include <action.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Preferences.h>

#define TIMES_WALK 1
#define SERVO_LOOP_DELAY 1
#define STEP_DELAY 100

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
int ang[]   = { 0, 0, 0, 0, 0 };
int osang[] = { 0, -3, -3, 2, 0 };  //offset angle
int AngS[]  = { 0, 0, 0, 0, 0 };  // the current angle pos
int timewalk1 = 100;
int timewalk2 = 50;
int timeLT = 300;  // time for left turn
int timeST = 100;  // time for right turn
int angleOffset = 90;

// set servo#x to move to y position
int input_ang(int x, int y) {
  switch (x) {
    case 0:
      return 180 - angleOffset - y -osang[0];
    case 1:
      return angleOffset - y - osang[1];
    case 2:
      return angleOffset + y + osang[2];
    case 3:
      return 180 - angleOffset + y + osang[3];
    case 4:
      return 180 - y;
  }
}

// void Servo_Offset();
void MoveInit() {
  Serial.println("MoveInit start");
  preferences.begin("memory", false);

  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);
  servo3.attach(servo3Pin, minUs, maxUs);
  servo4.attach(servo4Pin, minUs, maxUs);
  servo5.attach(servo5Pin, minUs, maxUs);

  // get osang value from preferences
  // osang[0] = preferences.getInt("LFAngle", osang[0]);
  // osang[1] = preferences.getInt("LBAngle", osang[1]);
  // osang[2] = preferences.getInt("RFAngle", osang[2]);
  // osang[3] = preferences.getInt("RBAngle", osang[3]);
  // osang[4] = preferences.getInt("HDAngle", osang[4]);
  for (int i = 0; i < 5; i++) {
    AngS[i] = osang[i];
  }

  Serial.println("MoveInit end");
}

void MoveReset() {
  Serial.println("MoveReset start");

  servoLeftFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
  delay(STEP_DELAY);
  servoRightBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
  delay(STEP_DELAY);
  servoRightFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
  delay(STEP_DELAY);
  servoLeftBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
  delay(STEP_DELAY);
  servoHead(0, TIMES_WALK, SERVO_LOOP_DELAY);
  delay(STEP_DELAY);

  Serial.println("MoveReset end");
}

void servoLeftFront(int ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoLeftFront start: ang = ");
  Serial.println(ange);
  int angtmp = ange - AngS[0];
  for (int i = 1; i <= timewalk; i++) {
    servo1.write(input_ang(0, AngS[0] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[0] = ange;
  Serial.println("servoLeftFront end!");
}

void servoLeftBack(int ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoLeftBack start: ang = ");
  Serial.println(ange);
  int angtmp = ange - AngS[1];
  for (int i = 1; i <= timewalk; i++) {
    servo2.write(input_ang(1, AngS[1] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[1] = ange;
  Serial.println("servoLeftBack end!");
}

void servoRightFront(int ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoRightFront start: ang = ");
  Serial.println(ange);
  int angtmp = ange - AngS[2];
  for (int i = 1; i <= timewalk; i++) {
    servo3.write(input_ang(2, AngS[2] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[2] = ange;
  Serial.println("servoRightFront end!");
}

void servoRightBack(int ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoRightBack start: ang = ");
  Serial.println(ange);
  int angtmp = ange - AngS[3];
  for (int i = 1; i <= timewalk; i++) {
    servo4.write(input_ang(3, AngS[3] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[3] = ange;
  Serial.println("servoRightBack end!");
}

void servoHead(int ange, int timewalk, int servo_delay) {
  if (timewalk < 1) {
    Serial.println("servo timewalk error!");
    return;
  }
  Serial.print("servoHead start: ang = ");
  Serial.println(ange);
  int angtmp = ange - AngS[4];
  for (int i = 1; i <= timewalk; i++) {
    servo5.write(input_ang(4, AngS[4] + angtmp * i / timewalk));
    delay(servo_delay);
  }
  AngS[4] = ange;
  Serial.println("servoHead end!");
}

void MoveForward(int step_delay, int loop_num) {
  for (int i = 0; i < loop_num; i++) {
    Serial.print("MoveForward loop count:");
    Serial.println(i);
    servoLeftFront(15, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoRightBack(15, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoRightFront(-15, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoLeftBack(-15, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoHead(15, TIMES_WALK, SERVO_LOOP_DELAY);
    delay(step_delay);

    servoLeftFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoRightBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoRightFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoLeftBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    servoHead(0, TIMES_WALK, SERVO_LOOP_DELAY);
    // delay(step_delay);
    delay(step_delay);
  }
}

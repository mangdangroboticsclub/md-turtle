/*
# Copyright 2024 MangDang (www.mangdang.net) 
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Description: This Turtle Arduino project is based on MD Robot Starter Kit(ESP32S3 chipset), including 
#      Test single servo to move
#      Test all servoes to move
#      Two move forward functions
#
*/

#include <action.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Preferences.h>

#define TIMES_WALK 1
#define SERVO_LOOP_DELAY 1
#define STEP_DELAY 10
#define INTERPOLATION_NUM1 (2*15+1)
#define INTERPOLATION_NUM2 66

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

int angles3[2*100+1];

// set servo#x to move to y position
int input_ang(int x, int y) {
  switch (x) {
    case 0:
      return 90 - y -osang[0];
    case 1:
      return 90 - y - osang[1];
    case 2:
      return 90 + y + osang[2];
    case 3:
      return 90 + y + osang[3];
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
  servoRightBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
  servoRightFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
  servoLeftBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
  servoHead(0, TIMES_WALK, SERVO_LOOP_DELAY);

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

// simple forward movement
void MoveForward(int step_delay, int loop_num) {
  for (int i = 0; i < loop_num; i++) {
    Serial.print("MoveForward loop count:");
    Serial.println(i);
    servoLeftFront(15, TIMES_WALK, SERVO_LOOP_DELAY);
    servoRightBack(15, TIMES_WALK, SERVO_LOOP_DELAY);
    servoRightFront(-15, TIMES_WALK, SERVO_LOOP_DELAY);
    servoLeftBack(-15, TIMES_WALK, SERVO_LOOP_DELAY);
    servoHead(15, TIMES_WALK, SERVO_LOOP_DELAY);
    
    delay(step_delay);
    
    servoLeftFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
    servoRightBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
    servoRightFront(0, TIMES_WALK, SERVO_LOOP_DELAY);
    servoLeftBack(0, TIMES_WALK, SERVO_LOOP_DELAY);
    servoHead(0, TIMES_WALK, SERVO_LOOP_DELAY);
    
    delay(step_delay);
  }
}

// forward movement smoothly
void smoothMoveForward(int loopNum) { 
  unsigned char angle1_position_init[INTERPOLATION_NUM1] = {90, 90, 90, 90, 90, 90, 90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90};
  unsigned char angle2_position_init[INTERPOLATION_NUM1] = {90, 90, 91, 92, 93, 94, 94,  95,  96,  97,  98,  98,  99,  100, 101, 102, 102, 103, 104, 105, 106, 106, 107, 108, 109, 110, 111, 111, 112, 113, 114};
  unsigned char angle3_position_init[INTERPOLATION_NUM1] = {90, 91, 93, 95, 96, 98, 100, 102, 103, 105, 107, 109, 110, 112, 114, 115, 117, 119, 121, 122, 124, 126, 128, 129, 131, 133, 135, 136, 138, 140, 141};
  unsigned char angle4_position_init[INTERPOLATION_NUM1] = {90, 87, 85, 83, 81, 79, 77,  75,  73,  71,  69,  67,  65,  63,  60,  58,  56,  54,  52,  50,  48,  46,  44,  42,  40,  38,  36,  33,  31,  29,  27};
  
  unsigned char angles1_tem[INTERPOLATION_NUM2] = {90, 88, 86, 84, 82, 80, 78, 76, 74, 72, 70, 68, 66, 64, 62, 62, 62, 62, 62, 62, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 48, 49, 51, 52, 54, 55, 56, 58, 59, 60, 62, 63, 65, 66, 67, 69, 70, 72, 73, 74, 76, 77, 78, 80, 81, 83, 84, 85, 87, 88, 90};
  unsigned char angles2_tem[INTERPOLATION_NUM2] = {114, 115, 115, 116, 117, 118, 119, 119, 120, 121, 122, 123, 123, 124, 125, 126, 127, 127, 128, 129, 130, 131, 132, 132, 133, 134, 135, 136, 136, 137, 138, 138, 137, 137, 136, 136, 135, 135, 135, 134, 134, 133, 133, 132, 132, 131, 131, 131, 131, 131, 131, 131, 130, 129, 127, 126, 125, 124, 122, 121, 120, 119, 117, 116, 115, 114};
  unsigned char angles3_tem[INTERPOLATION_NUM2] = {141, 140, 138, 136, 135, 133, 131, 129, 128, 126, 124, 122, 121, 119, 117, 115, 114, 112, 110, 109, 107, 105, 103, 102, 100, 98, 96, 95, 93, 91, 90, 90, 91, 93, 95, 97, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 117, 117, 117, 117, 117, 117, 119, 121, 122, 124, 126, 128, 129, 131, 133, 135, 136, 138, 140, 141};
  unsigned char angles4_tem[INTERPOLATION_NUM2] = {27, 29, 30, 32, 33, 35, 36, 38, 39, 41, 42, 44, 45, 46, 48, 48, 48, 48, 48, 48, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 62, 61, 60, 58, 57, 56, 55, 54, 53, 51, 50, 49, 48, 47, 46, 45, 43, 42, 41, 40, 39, 38, 36, 35, 34, 33, 32, 31, 30, 28, 27};
  
  //from calibration position to walk position
  for (int j = 0; j < INTERPOLATION_NUM1; j++){
    servo1.write( int(angle1_position_init[j]) - osang[0]);
    servo2.write( int(angle2_position_init[j]) - osang[1]);
    servo3.write( int(angle3_position_init[j]) + osang[2]);
    servo4.write( int(angle4_position_init[j]) + osang[3]);
    delay(STEP_DELAY);
  }
  
  //forward walk loop
  for (int i = 0; i <= loopNum; i++){
    for (int j = 0; j < INTERPOLATION_NUM2 ; j++){
      servo1.write( int(angles1_tem[j]) - osang[0]);
      servo2.write( int(angles2_tem[j]) - osang[1]);
      servo3.write( int(angles3_tem[j]) + osang[2]);
      servo4.write( int(angles4_tem[j]) + osang[3]);
      delay(STEP_DELAY);
    }
  }
  
  //from walk position to calibration position
  for (int j = 0; j < INTERPOLATION_NUM1; j++){
    servo1.write(int(angle1_position_init[INTERPOLATION_NUM1 - 1 - j]) - osang[0]);
    servo2.write(int(angle2_position_init[INTERPOLATION_NUM1 - 1 - j]) - osang[1]);
    servo3.write(int(angle3_position_init[INTERPOLATION_NUM1 - 1 - j]) + osang[2]);
    servo4.write(int(angle4_position_init[INTERPOLATION_NUM1 - 1 - j]) + osang[3]);
    delay(STEP_DELAY);
  }
}

void MovementDance() {

  int timewalk1 = 8;
  int timewalk2 = 16;
  int loop_times = 6;
  int stepDelay = 30;

  unsigned char LF_angle_slow[] = {110, 109, 108, 107, 105, 104, 102, 100,  98,  95,  92,  89,  85,  81,76,  70,  63,  57,  51,  47,  43,  39,  37,  34,  32,  31,  29,  28,27,  27,  26,  26,  25};
  unsigned char LB_angle_slow[] = {160, 159, 157, 155, 153, 150, 148, 145, 142, 138, 134, 130, 126, 120,115, 109, 102,  96,  91,  87,  83,  81,  78,  77,  75,  74,  73,  72,71,  71,  70,  70,  70};
  unsigned char RF_angle_slow[] = {69,  70,  71,  72,  74,  75,  77,  79,  81,  84,  87,  90,  94,  98, 103, 109, 116, 122, 128, 132, 136, 140, 142, 145, 147, 148, 150, 151,152, 152, 153, 153, 154};
  unsigned char RB_angle_slow[] = {19,  20,  22,  24,  26,  29,  31,  34,  37,  41,  45,  49,  53,  59, 64,  70,  77,  83,  88,  92,  96,  98, 101, 102, 104, 105, 106, 107,108, 108, 109, 109, 109};

  unsigned char LF_angle_fast[] = {110, 109, 108, 105, 100,  93,  81,  61,  25};
  unsigned char LB_angle_fast[] = {160, 159, 157, 152, 145, 135, 121, 100,  70};
  unsigned char RF_angle_fast[] = {69,  70,  71,  74,  79,  86,  98,  118, 154};
  unsigned char RB_angle_fast[] = {19,  20,  22,  27,  34,  44,  58,  79,  109,};

  for (int i = 0; i < loop_times; i++) {
    for (int j = 0; j <= timewalk1; j++){
      servo1.write(int(LF_angle_fast[j]) - osang[0]);
      servo2.write(int(LB_angle_fast[j]) - osang[1]);
      servo3.write(int(RF_angle_fast[j]) + osang[2]);
      servo4.write(int(RB_angle_fast[j]) + osang[3]);
      delay(stepDelay);
    }

    for (int j = 0; j <= 2 * timewalk2; j++){
      servo1.write(int(LF_angle_slow[2 * timewalk2 - j]) - osang[0]);
      servo2.write(int(LB_angle_slow[2 * timewalk2 - j]) - osang[1]);
      servo3.write(int(RF_angle_slow[2 * timewalk2 - j]) + osang[2]);
      servo4.write(int(RB_angle_slow[2 * timewalk2 - j]) + osang[3]);
      delay(stepDelay);
    }
  }
}

void smoothAngle(float angle1,float angle2,int timewalk,float nonlinear_flag) {
  float delt[timewalk + 1];
  float angles1[timewalk + 1];
  float angles2[timewalk];
  float k;
  float temp[timewalk + 1];
  for (int i = 0; i <= timewalk; i++){
    delt[i] = - 1.0 + 1.0 / float(timewalk) * float(i);
  }
  //k is the parameter to control the nonlinear feature
  if (nonlinear_flag == 1.0){k = 2.0;} else{k = 0.0;}
  for (int i = 0; i <= timewalk; i++){
    temp[i] = (1.0 + delt[i]) * exp(k * delt[i]);
  }
  for (int i = 0; i <= timewalk; i++){
    angles1[i] = angle1 + temp[i] * (angle2 - angle1) / 2.0;

  }
  for (int i = 0; i <= timewalk - 1; i++){
    angles2[i] = angle1 + angle2 - angles1[timewalk - 1 - i];

  }
  for (int i = 0; i <= 2 * timewalk ; i++){
    if (i <= timewalk)
    {
      angles3[i] = angles1[i];

    }
    else
    {
      angles3[i] = angles2[i-timewalk-1];
    }
  }

}

// float an1, float an2, float an3, float an4, float an5,float an6,float an7,float an8, the main parameters to optimize
// float an9, float an10,  // init position for the front 2 legs and back legs
// int timewalk_8, interpolation point number
// float step_delay_input_1, int stoptime_input  NO need to optimize
void smoothMoveForwardDegug(float an1, float an2, float an3, float an4, float an5,float an6,float an7,float an8,float an9, float an10, int timewalk_8, float step_delay_input_1, int stoptime_input) {
  int loop_times = 6;
  int timewalk = timewalk_8;
  int half_timewalk = int(timewalk/2.0);
  int MovementGroup8_offset = 130;
  float step_delay_1 = step_delay_input_1;
  float step_delay_2 = 10;
  float init_front_angle = an9;
  float init_back_angle = an10;
  int nonlinear_flag_input = 0;
  int stoptime = stoptime_input;
  
  float LF_step1 = an1;
  float LF_step2 = an2;
  
  float LB_step1 = an3;
  float LB_step2 = an4;
  
  float RF_step1 = an5;
  float RF_step2 = an6;
  
  float RB_step1 = an7;
  float RB_step2 = an8;
  
  float nonlinear_flag = nonlinear_flag_input;
  
  float angle1_position_init[1 + 2 * timewalk];
  float angle2_position_init[1 + 2 * timewalk];
  float angle3_position_init[1 + 2 * timewalk];
  float angle4_position_init[1 + 2 * timewalk];
  
  float angle1_position_temp1[1 + 2 * half_timewalk];
  float angle2_position_temp1[1 + 2 * half_timewalk];
  float angle3_position_temp1[1 + 2 * half_timewalk];
  float angle4_position_temp1[1 + 2 * half_timewalk];
  
  float angle1_position_temp2[1 + 2 * timewalk];
  float angle2_position_temp2[1 + 2 * timewalk];
  float angle3_position_temp2[1 + 2 * timewalk];
  float angle4_position_temp2[1 + 2 * timewalk];
  
  float angle1_position_temp3[1 + 2 * half_timewalk];
  float angle2_position_temp3[1 + 2 * half_timewalk];
  float angle3_position_temp3[1 + 2 * half_timewalk];
  float angle4_position_temp3[1 + 2 * half_timewalk];
  
  float angle5_position_temp1[1 + 2 * timewalk];
  float angle5_position_temp2[1 + 2 * timewalk];
  
  float LF_init = (260 - MovementGroup8_offset - init_front_angle - osang[0])*180.0/260;
  float LF_position1 = (260 - MovementGroup8_offset - (init_front_angle + LF_step1) - osang[0])*180.0/260;
  float LF_position2 = (260 - MovementGroup8_offset - (init_front_angle + LF_step2) - osang[0])*180.0/260;
  
  float LB_init = (MovementGroup8_offset - init_back_angle - osang[1])*180.0/260;
  float LB_position1 = (MovementGroup8_offset - (init_back_angle + LB_step1) - osang[1])*180.0/260;
  float LB_position2 = (MovementGroup8_offset - (init_back_angle + LB_step2) - osang[1])*180.0/260;
  
  float RF_init = (MovementGroup8_offset + init_front_angle + osang[2])*180.0/260;
  float RF_position1 = (MovementGroup8_offset + (init_front_angle + RF_step1) + osang[2])*180.0/260;
  float RF_position2 = (MovementGroup8_offset + (init_front_angle + RF_step2) + osang[2])*180.0/260;
  
  float RB_init = (260 - MovementGroup8_offset + init_back_angle + osang[3])*180.0/260;
  float RB_position1 = (260 - MovementGroup8_offset + (init_back_angle + RB_step1) + osang[3])*180.0/260;
  float RB_position2 = (260 - MovementGroup8_offset + (init_back_angle + RB_step2) + osang[3])*180.0/260;
    
  //init
  smoothAngle(LF_init, LF_position2, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle1_position_init[i] = angles3[i];
  }
  
  smoothAngle(LB_init, LB_position1, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle2_position_init[i] = angles3[i];
  }
  
  smoothAngle(RF_init, RF_position1, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle3_position_init[i] = angles3[i];
  }
  
  smoothAngle(RB_init, RB_position2, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle4_position_init[i] = angles3[i];
  }
  
  smoothAngle(LF_init, LF_position1, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle1_position_temp1[i] = angles3[i];
  }
  
  smoothAngle(LF_position1, LF_position2, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle1_position_temp2[i] = angles3[i];
  }
  
  smoothAngle(LF_position2, LF_init, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle1_position_temp3[i] = angles3[i];
  }
  
  smoothAngle(LB_init, LB_position1, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle2_position_temp1[i] = angles3[i];
  }
  
  smoothAngle(LB_position1, LB_position2, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle2_position_temp2[i] = angles3[i];
  }
  
  smoothAngle(LB_position2, LB_init, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle2_position_temp3[i] = angles3[i];
  }
  
  smoothAngle(RF_init, RF_position1, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle3_position_temp1[i] = angles3[i];
  }
  
  smoothAngle(RF_position1, RF_position2, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle3_position_temp2[i] = angles3[i];
  }
  
  smoothAngle(RF_position2, RF_init, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle3_position_temp3[i] = angles3[i];
  }
  
  smoothAngle(RB_init, RB_position1, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle4_position_temp1[i] = angles3[i];
  }
  
  smoothAngle(RB_position1, RB_position2, timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * timewalk; i++){
    angle4_position_temp2[i] = angles3[i];
  }
  
  smoothAngle(RB_position2, RB_init, half_timewalk, nonlinear_flag);
  for (int i = 0 ; i <= 2 * half_timewalk; i++){
    angle4_position_temp3[i] = angles3[i];
  }
  
  int LF_loop_flag1 = 2 * half_timewalk;
  int LF_loop_flag2 = 2 * half_timewalk + stoptime;
  int LF_loop_flag3 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 ;
  int LF_loop_flag4 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 + 2*timewalk+1;
  float angles1_tem[LF_loop_flag4+1];
  
  for (int i = 0 ; i <= LF_loop_flag4; i ++){
    if (i<= LF_loop_flag1){
      angles1_tem[i] = angle1_position_temp3[i];

    } else if((LF_loop_flag1 < i) & (i <= LF_loop_flag2)){
      angles1_tem[i] = LF_init;

    } else if ((LF_loop_flag2 < i) & ( i <=LF_loop_flag3 )){
      angles1_tem[i] = angle1_position_temp1[i - LF_loop_flag2 - 1];

    } else if (( LF_loop_flag3 < i) & (i <=LF_loop_flag4 )){
      angles1_tem[i] = angle1_position_temp2[i - LF_loop_flag3 - 1];
    }
  }
  
  int LB_loop_flag1 = 2 * timewalk;
  int LB_loop_flag2 = 2 * timewalk + 2*half_timewalk+1 ;
  int LB_loop_flag3 = 2 * timewalk + 2*half_timewalk+1 + stoptime ;
  int LB_loop_flag4 = 2 * timewalk + 2*half_timewalk+1 + stoptime + 2*half_timewalk+1;
  float angles2_tem[LB_loop_flag4+1];
  
  for (int i = 0 ; i <= LB_loop_flag4; i ++){
    if (i<= LB_loop_flag1){
      angles2_tem[i] = angle2_position_temp2[i];

    } else if((LB_loop_flag1 < i) & (i <= LB_loop_flag2)){
      angles2_tem[i] = angle2_position_temp3[i - LB_loop_flag1 - 1];

    } else if ((LB_loop_flag2 < i) & ( i <=LB_loop_flag3 )){
      angles2_tem[i] = LB_init;

    } else if (( LB_loop_flag3 < i) & (i <=LB_loop_flag4 )){
      angles2_tem[i] = angle2_position_temp1[i - LB_loop_flag3 - 1];
    }
  }
  
  int RF_loop_flag1 = 2 * timewalk;
  int RF_loop_flag2 = 2 * timewalk + 2*half_timewalk+1 ;
  int RF_loop_flag3 = 2 * timewalk + 2*half_timewalk+1 + stoptime ;
  int RF_loop_flag4 = 2 * timewalk + 2*half_timewalk+1 + stoptime + 2*half_timewalk+1;
  float angles3_tem[RF_loop_flag4+1];
  
  for (int i = 0 ; i <= RF_loop_flag4; i ++){
    if (i<= RF_loop_flag1){
      angles3_tem[i] = angle3_position_temp2[i];
    
    } else if((RF_loop_flag1 < i) & (i <= RF_loop_flag2)){
      angles3_tem[i] = angle3_position_temp3[i - RF_loop_flag1 - 1];
    
    } else if ((RF_loop_flag2 < i) & ( i <=RF_loop_flag3 )){
      angles3_tem[i] = RF_init;
    
    } else if (( RF_loop_flag3 < i) & (i <=RF_loop_flag4 )){
      angles3_tem[i] = angle3_position_temp1[i - RF_loop_flag3 - 1];
    }
  }
  
  int RB_loop_flag1 = 2 * half_timewalk;
  int RB_loop_flag2 = 2 * half_timewalk + stoptime;
  int RB_loop_flag3 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 ;
  int RB_loop_flag4 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 + 2*timewalk+1;
  float angles4_tem[RB_loop_flag4+1];
  for (int i = 0 ; i <= RB_loop_flag4; i ++){
    if (i<= RB_loop_flag1){
      angles4_tem[i] = angle4_position_temp3[i];
    
    } else if ((RB_loop_flag1 < i) & (i <= RB_loop_flag2)){
      angles4_tem[i] = RB_init;
    
    } else if ((RB_loop_flag2 < i) & ( i <=RB_loop_flag3 )){
      angles4_tem[i] = angle4_position_temp1[i - RB_loop_flag2 - 1];
    
    } else if ((RB_loop_flag3 < i) & (i <=RB_loop_flag4 )){
      angles4_tem[i] = angle4_position_temp2[i - RB_loop_flag3 - 1];
    }
  }
  
  //init
  for (int j = 0; j <= 2 * timewalk; j++){
    servo1.write(angle1_position_init[j]);
    delay(step_delay_1);
    servo2.write(angle2_position_init[j]);
    delay(step_delay_1);
    servo3.write(angle3_position_init[j]);
    delay(step_delay_1);
    servo4.write(angle4_position_init[j]);
    delay(step_delay_1);
    
    delay(step_delay_2);
  }
  
  for (int i = 0; i <= loop_times; i++){
    for (int j = 0; j <= RB_loop_flag4 ; j++){
      servo1.write(angles1_tem[j]);
      delay(step_delay_1);
      servo2.write(angles2_tem[j]);
      delay(step_delay_1);
      servo3.write(angles3_tem[j]);
      delay(step_delay_1);
      servo4.write(angles4_tem[j]);
      delay(step_delay_1);
      
      delay(step_delay_2);
    }
  }
}

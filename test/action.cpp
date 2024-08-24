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


int angles3[31];

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

void smoothAngle(int angle1,int angle2,int timewalk,int nonlinear_flag) {
  float delt[timewalk + 1];
  float angles1[timewalk + 1];
  float angles2[timewalk];
  int k;
  float temp[timewalk + 1];
  for (int i = 0; i <= timewalk; i++){
    delt[i] = - 1 + 1 / float(timewalk) * float(i);
  }
  //k is the parameter to control the nonlinear feature
  if (nonlinear_flag == 1){k = 2;} else{k = 0;}
  for (int i = 0; i <= timewalk; i++){
    temp[i] = (1 + delt[i]) * exp(k * delt[i]);
  }

  for (int i = 0; i <= timewalk; i++){
    angles1[i] = angle1 + temp[i] * (angle2 - angle1) / 2;
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


void MoveForward() {
  int loop_times = 6;                        //loop times
  int timewalk = 15;                         //interplotation times
  int half_timewalk = int(timewalk / 2);
  int MovementGroup8_offset = 130;           //servo middle position
  int step_delay_1 = 2;                      //delay bewteen every servo
  int step_delay_2 = 10;                     //delay bewteen every four servos
  int init_front_angle = 40;                 //front leg middle position
  int init_back_angle = -60;                 //back leg middle position
  int nonlinear_flag = 0;                    //nonlinear flag,0 represent linear,1 represent nonlinear
  int stoptime = 5;                          //every gait loop leg pause time
  
  int LF_step1 = 20;                         //left front step forward angles
  int LF_step2 = -40;                        //left front step backward angles
  
  int LB_step1 = 25;
  int LB_step2 = -10;
  
  int RF_step1 = 35;
  int RF_step2 = -40;
  
  int RB_step1 = 20;
  int RB_step2 = -30;
  
  short angle1_position_init[1 + 2 * timewalk];
  short angle2_position_init[1 + 2 * timewalk];
  short angle3_position_init[1 + 2 * timewalk];
  short angle4_position_init[1 + 2 * timewalk];
  
  short angle1_position_temp1[1 + 2 * half_timewalk];
  short angle2_position_temp1[1 + 2 * half_timewalk];
  short angle3_position_temp1[1 + 2 * half_timewalk];
  short angle4_position_temp1[1 + 2 * half_timewalk];
  
  short angle1_position_temp2[1 + 2 * timewalk];
  short angle2_position_temp2[1 + 2 * timewalk];
  short angle3_position_temp2[1 + 2 * timewalk];
  short angle4_position_temp2[1 + 2 * timewalk];
  
  short angle1_position_temp3[1 + 2 * half_timewalk];
  short angle2_position_temp3[1 + 2 * half_timewalk];
  short angle3_position_temp3[1 + 2 * half_timewalk];
  short angle4_position_temp3[1 + 2 * half_timewalk];
  
  int LF_init = (260 - MovementGroup8_offset - init_front_angle - osang[0]) * 180 / 260;
  int LF_position1 = (260 - MovementGroup8_offset - (init_front_angle + LF_step1) - osang[0]) * 180 / 260;
  int LF_position2 = (260 - MovementGroup8_offset - (init_front_angle + LF_step2) - osang[0]) * 180 / 260;
  
  int LB_init = (MovementGroup8_offset - init_back_angle - osang[1]) * 180 / 260;
  int LB_position1 = (MovementGroup8_offset - (init_back_angle + LB_step1) - osang[1]) * 180 / 260;
  int LB_position2 = (MovementGroup8_offset - (init_back_angle + LB_step2) - osang[1]) * 180 / 260;
  
  int RF_init = (MovementGroup8_offset + init_front_angle + osang[2]) * 180 / 260;
  int RF_position1 = (MovementGroup8_offset + (init_front_angle + RF_step1) + osang[2]) * 180 / 260;
  int RF_position2 = (MovementGroup8_offset + (init_front_angle + RF_step2) + osang[2]) * 180 / 260;
  
  int RB_init = (260 - MovementGroup8_offset + init_back_angle + osang[3]) * 180 / 260;
  int RB_position1 = (260 - MovementGroup8_offset + (init_back_angle + RB_step1) + osang[3]) * 180 / 260;
  int RB_position2 = (260 - MovementGroup8_offset + (init_back_angle + RB_step2) + osang[3]) * 180 / 260;
  
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

  //left front
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
  
  //left back
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
  
  //right front
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
  
  //right back
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
  
  //left front
  int LF_loop_flag1 = 2 * half_timewalk;
  int LF_loop_flag2 = 2 * half_timewalk + stoptime;
  int LF_loop_flag3 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 ;
  int LF_loop_flag4 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 + 2*timewalk+1;
  short angles1_tem[LF_loop_flag4+1];
  
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
  
  //left back
  int LB_loop_flag1 = 2 * timewalk;
  int LB_loop_flag2 = 2 * timewalk + 2 * half_timewalk + 1 ;
  int LB_loop_flag3 = 2 * timewalk + 2 * half_timewalk + 1 + stoptime ;
  int LB_loop_flag4 = 2 * timewalk + 2 * half_timewalk + 1 + stoptime + 2 * half_timewalk + 1;
  short angles2_tem[LB_loop_flag4+1];
  
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
  
  //right front
  int RF_loop_flag1 = 2 * timewalk;
  int RF_loop_flag2 = 2 * timewalk + 2 * half_timewalk + 1 ;
  int RF_loop_flag3 = 2 * timewalk + 2 * half_timewalk + 1 + stoptime ;
  int RF_loop_flag4 = 2 * timewalk + 2 * half_timewalk + 1 + stoptime + 2 * half_timewalk + 1;
  short angles3_tem[RF_loop_flag4+1];
  
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
  
  //right back
  int RB_loop_flag1 = 2 * half_timewalk;
  int RB_loop_flag2 = 2 * half_timewalk + stoptime;
  int RB_loop_flag3 = 2 * half_timewalk + stoptime + 2 * half_timewalk + 1 ;
  int RB_loop_flag4 = 2 * half_timewalk + stoptime + 2 * half_timewalk + 1 + 2 * timewalk + 1;
  short angles4_tem[RB_loop_flag4 + 1];
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
  
  //init move
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

  //forward loop
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

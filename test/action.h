#ifndef ACTION_H
#define ACTION_H
#include <Arduino.h>


void MoveInit();
void MoveReset();
void MoveForward(int step_delay, int loop_num);
void smoothMoveForward(int loopNum);
void MovementDance();

void servoLeftFront(int ange, int timewalk, int servo_delay);
void servoLeftBack(int ange, int timewalk, int servo_delay);
void servoRightFront(int ange, int timewalk, int servo_delay);
void servoRightBack(int ange, int timewalk, int servo_delay);
void servoHead(int ange, int timewalk, int servo_delay);

void smoothMoveForwardDegug(float an1, float an2, float an3, float an4, float an5,float an6,float an7,float an8,float an9, float an10, int timewalk_8, float step_delay_input_1, int stoptime_input);

#endif
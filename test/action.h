#ifndef ACTION_H
#define ACTION_H
#include <Arduino.h>


void MoveInit();
void MoveReset();
void MoveForward();

void servoLeftFront(int ange, int timewalk, int servo_delay);
void servoLeftBack(int ange, int timewalk, int servo_delay);
void servoRightFront(int ange, int timewalk, int servo_delay);
void servoRightBack(int ange, int timewalk, int servo_delay);
void servoHead(int ange, int timewalk, int servo_delay);

#endif
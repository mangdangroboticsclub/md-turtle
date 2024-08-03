#ifndef ACTION_H
#define ACTION_H
#include <Arduino.h>


void MoveInit();
void MoveReset();
void MoveForward(int step_delay);

//just for debug
void servo(float AngE1, float AngE2, float AngE3, float AngE4, int timewalk, int servo_delay);
void servoLeftFront(float ange, int timewalk, int servo_delay);
void servoLeftBack(float ange, int timewalk, int servo_delay);
void servoRightFront(float ange, int timewalk, int servo_delay);
void servoRightBack(float ange, int timewalk, int servo_delay);

#endif  // TTS_H
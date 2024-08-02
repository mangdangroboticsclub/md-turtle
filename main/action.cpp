#include <Arduino.h>
#include <ESP32Servo.h>
#include <Preferences.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

Preferences preferences;

int minUs = 500;
int maxUs = 2500;
int servo1Pin = 35; // leftfront
int servo2Pin = 36; // leftback
int servo3Pin = 37; // rightfront
int servo4Pin = 38; // rightback
int servo5Pin = 39; // head
float ang[] = {0, 0, 0, 0, 0, 0};
float osang[] = {0, 0, 0, 0, 0, 0}; //offset angle
float AngS[] = {0, 0, 0, 0, 0, 0}; // the current angle pos
int timewalk1 = 100;
int timewalk2 = 50;
int timeLT = 300; // time for left turn
int timeST = 100; // time for right turn



float input_ang(int x, float y){
  float Aang, Bang; //0-180
  switch (x){
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
  Bang = 180*(Aang)/260;
  return Bang;
}

void servo(float AngE1, float AngE2, float AngE3, float AngE4, int timewalk){
  float AngTemp[] = {0, AngE1 - AngS[1], AngE2 - AngS[2], AngE3 - AngS[3], AngE4 - AngS[4]};
  for(int i = 0; i < timewalk; i++){
    servo1.write(input_ang(1, AngS[1] + AngTemp[1] * i/timewalk));
    servo2.write(input_ang(2, AngS[2] + AngTemp[2] * i/timewalk));
    servo3.write(input_ang(3, AngS[3] + AngTemp[3] * i/timewalk));
    servo4.write(input_ang(4, AngS[4] + AngTemp[4] * i/timewalk));

    delay(1);
  }
  AngS[1] = AngE1;
  AngS[2] = AngE2;
  AngS[3] = AngE3;
  AngS[4] = AngE4;
}



void Servo_Init() {
  preferences.begin("memory", false);
    
  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);
  servo3.attach(servo3Pin, minUs, maxUs);
  servo4.attach(servo4Pin, minUs, maxUs);
  servo5.attach(servo5Pin, minUs, maxUs);

  osang[1] = preferences.getInt("LFAngle", osang[1]);
  osang[2] = preferences.getInt("LBAngle", osang[2]);
  osang[3] = preferences.getInt("RFAngle", osang[3]);
  osang[4] = preferences.getInt("RBAngle", osang[4]);
  for(int i = 0; i < 4; i++){
    AngS[i] = osang[i];
  }

  servo1.write(180*(220 - osang[1])/260);
  servo2.write(180*(40 - osang[2])/260);
  servo3.write(180*(40 + osang[3])/260);
  servo4.write(180*(220 + osang[4])/260);
  servo5.write(input_ang(5, 0));
  delay(5000);
}

// void Servo_Offset() {
//   int end = 1;
//   int read_int;
//   while(end) {
//     int angle_text = Serial.read();
//       Serial.print("\n\n\nInputed word: ");
//       Serial.println(angle_text);
//       if (isDigit(angle_text)) {
//         switch (angle_text) {
//           case 0:
//             end = 0;
//             break;

//           case 1:
//             Serial.println("\nPlease input and angle for Servo 1(Left front): ");
//             read_int = Serial.read();
//             ang[1] = read_int;
//             break;
            
//           case 2:
//             Serial.println("\nPlease input and angle for Servo 2(Left back): ");
//             read_int = Serial.read();
//             ang[2] = read_int;
//             break;
            
//           case 3:
//             Serial.println("\nPlease input and angle for Servo 3(Right front): ");
//             read_int = Serial.read();
//             ang[3] = read_int;
//             break;
            
//           case 4:
//             Serial.println("\nPlease input and angle for Servo 4(Right back): ");
//             read_int = Serial.read();
//             ang[4] = read_int;
//             break;
//         }
//       } else {
//         Serial.println("Please Input A Int");
//       }
//     servo1.write(180*(220 - ang[1])/260);
//     servo2.write(180*(40 - ang[2])/260);
//     servo3.write(180*(40 + ang[3])/260);
//     servo4.write(180*(220 + ang[4])/260);
//   }
//   for(int i = 0; i <= 4; i++){
//       osang[i] = ang[i];
//       ang[i] = 0;
//     }
//   preferences.putInt("LFAngle", osang[1]);
//   preferences.putInt("LBAngle", osang[2]);
//   preferences.putInt("RFAngle", osang[3]);
//   preferences.putInt("RBAngle", osang[4]);
// }


void Servo_forward() {
  int walkcount = 1;
  int round = 0;
  while(round < 50) {
    switch (walkcount){
    case 1:
      servo(15, 0, 0, 0, timewalk1);
      servo(15, 0, 0, 15, timewalk1);
      walkcount++;
      break;
      
    case 2:
      servo(15, 0, -15, 15, timewalk2);
      servo(15, -15, -15, 15, timewalk2);
      walkcount++;
      break;
      
    case 3:
      servo(0, -15, -15, 15, timewalk2);
      servo(0, -15, -15, 0, timewalk2);
      walkcount++;
      break;
      
    case 4:
      servo(0, 0, 15, 0, timewalk1);
      servo(0, 15, 15, 0, timewalk1);
      walkcount++;
      break;
      
    case 5:
      servo(-15, 15, 15, 0, timewalk2);
      servo(-15, 15, 15, -15, timewalk2);
      walkcount++;
      break;
      
    case 6:
      servo(-15, 15, 0, -15, timewalk2);
      servo(-15, 0, 0, -15, timewalk2);
      walkcount = 1;
      break;
    }
    round++;
  }
  Servo_Init();
}
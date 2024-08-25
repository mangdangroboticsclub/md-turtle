#
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
# Description: create the interpolation arrays used in moveforward function of Turtle Arduino project.
#
# Test method: $python Turtle.Forward.Interpolation.py
#
#

import numpy as np

front_ang = 0
back_ang = 0

def smoothAngle(angle1,angle2,timewalk,nonlinear_flag):
  delt = np.zeros(timewalk + 1)
  angles1 = np.zeros(timewalk + 1)
  angles2 = np.zeros(timewalk)
  angles3 = np.zeros(2 * timewalk + 1)
  k = 0
  temp = np.zeros(timewalk + 1)
  for i in range(0,timewalk+ 1):
    delt[i] = - 1.0 + 1.0 / timewalk * i
  
  if (nonlinear_flag == 1.0):
    k = 2.0 
  else:
    k = 0.0
  
  for i in range(0,timewalk+ 1):
    temp[i] = (1.0 + delt[i]) * np.exp(k * delt[i])
  
  for i in range(0,timewalk+ 1):
    angles1[i] = angle1 + temp[i] * (angle2 - angle1) / 2.0

  for i in range(0,timewalk ):
    angles2[i] = angle1 + angle2 - angles1[timewalk - 1 - i]
  
  for i in range(0,2 * timewalk+1):
    if (i <= timewalk):    
      angles3[i] = angles1[i]
    
    else:    
      angles3[i] = angles2[i-timewalk-1]
      
  return angles3
    
  
def smoothAngle2(angle1,angle2,timewalk):
  delt = np.zeros(timewalk + 1)
  angles4 = np.zeros(timewalk + 1)
  temp = np.zeros(timewalk + 1)
  for i in range(0,timewalk+1):
    delt[i] = - 1.0 + 1.0 / timewalk * i
  
  for i in range(0,timewalk+1):
    temp[i] = (1.0 + delt[i]) * np.exp(4 * delt[i])

  for i in range(0,timewalk+1):
    angles4[i] = angle1 + temp[i] * (angle2 - angle1)

  return angles4


def Movement_get_angle(ang,front_offset,back_offset):
  movement_temp_angle = 0
  max_angle = np.pi - 0.5
  min_angle = np.pi - 1.85
  r = 5.5
  l = 5.8
  x1 = -l / 2.0 - r * np.sin(front_offset * 180.0 / 260.0 / 180.0 * np.pi)
  y1 = 0.0
  x2 = l / 2.0 + r * np.sin(back_offset * 180.0 / 260.0 / 180.0 * np.pi)
  y2 = 0.0
  k1 = x2 + r * np.cos(ang)
  k2 = y2 + r * np.sin(ang)
  k3 = k1 * k1 - x1 * x1 + k2 * k2 + r * r - l * l
  k4 = 2.0 * x1 - 2.0 * k1
  a = k4 * k4 + 4.0 * k2 * k2
  b = 2.0 * k3 * k4 - 8.0 * k2 * k2 * x1
  c = k3 * k3 - 4.0 * k2 * k2 * r * r + 4.0 * k2 * k2 * x1 * x1
  xx1 = (b + np.sqrt(b * b - 4.0 * a * c)) / (-2.0 * a)
  yy1 = np.sqrt(np.sqrt((r * r - (xx1 - x1) * (xx1 - x1))**2))
  beta = 0
  theta = np.arcsin((k2 - yy1) / l)

  if (xx1 > x1):
    beta = np.arcsin(yy1 / r)
  else:
    beta = np.pi - np.arcsin(yy1 / r)
  
  front_ang = ang - theta
  back_ang = np.pi - beta + theta
  return front_ang,back_ang


MovementDance_offset = 150
timewalk1 = 8
timewalk2 = 16
nonlinear_flag = 0  
loop_times = 12
step_delay = 1
max_angle = 2.35  #unit rad
min_angle = 1.55
LF_angle_slow = np.zeros(1+2*timewalk2)
LB_angle_slow = np.zeros(1+2*timewalk2)
RF_angle_slow = np.zeros(1+2*timewalk2)
RB_angle_slow = np.zeros(1+2*timewalk2)

LF_angle_fast = np.zeros(timewalk1+ 1)
LB_angle_fast = np.zeros(timewalk1+ 1)
RF_angle_fast = np.zeros(timewalk1+ 1)
RB_angle_fast = np.zeros(timewalk1+ 1)

angle1_position_temp = np.zeros(1+2*timewalk2)
angle2_position_temp = np.zeros(1+2*timewalk2)
angle3_position_temp = np.zeros(1+2*timewalk2)
angle4_position_temp = np.zeros(1+2*timewalk2)

temp_angleoffset1 = -10
temp_angleoffset2 = 20
angles4 = smoothAngle2(min_angle, max_angle, timewalk1)
for i in range(0,timewalk1+ 1):
  front_ang,back_ang = Movement_get_angle(angles4[i], 40, 20)
  LF_angle_fast[i] = int((260 - MovementDance_offset - ((front_ang/np.pi*180-90) /180.0*260  ))*180.0/260 - temp_angleoffset1) 
  LB_angle_fast[i] = int((MovementDance_offset - (-(back_ang/np.pi*180-90) /180.0*260) )*180/260.0 - temp_angleoffset2)
  RF_angle_fast[i] = int((MovementDance_offset + ((front_ang/np.pi*180-90) /180.0*260)  )*180/260.0 + temp_angleoffset1)
  RB_angle_fast[i] = int((260 - MovementDance_offset + (-(back_ang/np.pi*180-90) /180.0*260)  )*180.0/260 + temp_angleoffset2)

angles3 = smoothAngle(min_angle, max_angle, timewalk2, 1)
for i in range(0,2 * timewalk2+1):
  front_ang,back_ang = Movement_get_angle(angles3[i], 40 , 20 )
  LF_angle_slow[i] = int((260 - MovementDance_offset - ((front_ang/np.pi*180-90) /180*260  ))*180/260 - temp_angleoffset1) 
  LB_angle_slow[i] = int((MovementDance_offset - (-(back_ang/np.pi*180-90) /180*260) )*180/260 - temp_angleoffset2)
  RF_angle_slow[i] = int((MovementDance_offset + ((front_ang/np.pi*180-90) /180*260)  )*180/260 + temp_angleoffset1)
  RB_angle_slow[i] = int((260 - MovementDance_offset + (-(back_ang/np.pi*180-90) /180*260)  )*180/260 + temp_angleoffset2)

print(LF_angle_fast,"LF_angle_fast")
print(LB_angle_fast,"LB_angle_fast")
print(RF_angle_fast,"RF_angle_fast")
print(RB_angle_fast,"RB_angle_fast")

print(LF_angle_slow,"LF_angle_slow")
print(LB_angle_slow,"LB_angle_slow")
print(RF_angle_slow,"RF_angle_slow")
print(RB_angle_slow,"RB_angle_slow")
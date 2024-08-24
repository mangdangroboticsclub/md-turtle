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

def smoothForwardCalculate(an1, an2, an3, an4, an5, an6, an7, an8, an9, an10, timewalk_input, stoptime_input):

    loop_times = 6
    timewalk = timewalk_input
    half_timewalk = int(timewalk/2.0)
    offset = 130
    init_front_angle = an9
    init_back_angle = an10
    nonlinear_flag_input = 0
    stoptime = stoptime_input

    angles3 = np.zeros(1 + 2 * timewalk)

    LF_step1 = an1
    LF_step2 = an2

    LB_step1 = an3
    LB_step2 = an4

    RF_step1 = an5
    RF_step2 = an6

    RB_step1 = an7
    RB_step2 = an8

    nonlinear_flag = nonlinear_flag_input 

    angle1_position_init = np.zeros(1 + 2 * timewalk)
    angle2_position_init = np.zeros(1 + 2 * timewalk)
    angle3_position_init = np.zeros(1 + 2 * timewalk)
    angle4_position_init = np.zeros(1 + 2 * timewalk)

    angle1_position_temp1 = np.zeros(1 + 2 * half_timewalk)
    angle2_position_temp1 = np.zeros(1 + 2 * half_timewalk)
    angle3_position_temp1 = np.zeros(1 + 2 * half_timewalk)
    angle4_position_temp1 = np.zeros(1 + 2 * half_timewalk)

    angle1_position_temp2 = np.zeros(1 + 2 * timewalk)
    angle2_position_temp2 = np.zeros(1 + 2 * timewalk)
    angle3_position_temp2 = np.zeros(1 + 2 * timewalk)
    angle4_position_temp2 = np.zeros(1 + 2 * timewalk)

    angle1_position_temp3 = np.zeros(1 + 2 * half_timewalk)
    angle2_position_temp3 = np.zeros(1 + 2 * half_timewalk)
    angle3_position_temp3 = np.zeros(1 + 2 * half_timewalk)
    angle4_position_temp3 = np.zeros(1 + 2 * half_timewalk)

    angle5_position_temp1 = np.zeros(1 + 2 * timewalk)
    angle5_position_temp2 = np.zeros(1 + 2 * timewalk)

    middle_position = 90
    LF_init = (260 - offset - init_front_angle )*180.0/260
    LF_position1 = (260 - offset - (init_front_angle + LF_step1) )*180.0/260
    LF_position2 = (260 - offset - (init_front_angle + LF_step2) )*180.0/260

    LB_init = (offset - init_back_angle )*180.0/260
    LB_position1 = (offset - (init_back_angle + LB_step1))*180.0/260
    LB_position2 = (offset - (init_back_angle + LB_step2))*180.0/260

    RF_init = (offset + init_front_angle )*180.0/260
    RF_position1 = (offset + (init_front_angle + RF_step1) )*180.0/260
    RF_position2 = (offset + (init_front_angle + RF_step2) )*180.0/260

    RB_init = (260 - offset + init_back_angle )*180.0/260
    RB_position1 = (260 - offset + (init_back_angle + RB_step1) )*180.0/260
    RB_position2 = (260 - offset + (init_back_angle + RB_step2) )*180.0/260

    #init
    angles3 = smoothAngle(middle_position, LF_position2, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle1_position_init[i] = int(angles3[i])


    angles3 = smoothAngle(middle_position, LB_position1, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle2_position_init[i] = int(angles3[i])


    angles3 = smoothAngle(middle_position, RF_position1, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle3_position_init[i] = int(angles3[i])


    angles3 = smoothAngle(middle_position, RB_position2, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle4_position_init[i] = int(angles3[i])

    #loop
    angles3 = smoothAngle(LF_init, LF_position1, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle1_position_temp1[i] = int(angles3[i])

    angles3 = smoothAngle(LF_position1, LF_position2, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle1_position_temp2[i] = int(angles3[i])

    angles3 = smoothAngle(LF_position2, LF_init, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle1_position_temp3[i] = int(angles3[i])

    angles3 = smoothAngle(LB_init, LB_position1, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle2_position_temp1[i] = int(angles3[i])

    angles3 = smoothAngle(LB_position1, LB_position2, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle2_position_temp2[i] = int(angles3[i])  

    angles3 = smoothAngle(LB_position2, LB_init, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle2_position_temp3[i] = int(angles3[i])

    angles3 = smoothAngle(RF_init, RF_position1, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle3_position_temp1[i] = int(angles3[i])

    angles3 = smoothAngle(RF_position1, RF_position2, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle3_position_temp2[i] = int(angles3[i])

    angles3 = smoothAngle(RF_position2, RF_init, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle3_position_temp3[i] = int(angles3[i])

    angles3 = smoothAngle(RB_init, RB_position1, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle4_position_temp1[i] = int(angles3[i])

    angles3 = smoothAngle(RB_position1, RB_position2, timewalk, nonlinear_flag)
    for i in range(0,2 * timewalk+1):
      angle4_position_temp2[i] = int(angles3[i])

    angles3 = smoothAngle(RB_position2, RB_init, half_timewalk, nonlinear_flag)
    for i in range(0,2 * half_timewalk+1):
      angle4_position_temp3[i] = int(angles3[i])

    LF_loop_flag1 = 2 * half_timewalk
    LF_loop_flag2 = 2 * half_timewalk + stoptime
    LF_loop_flag3 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 
    LF_loop_flag4 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 + 2*timewalk+1
    angles1_tem = np.zeros(LF_loop_flag4+1)

    for i in range(0,LF_loop_flag4+1):
      if (i <= LF_loop_flag1):
        angles1_tem[i] = angle1_position_temp3[i]
      elif ((LF_loop_flag1 < i) & (i <= LF_loop_flag2)):
        angles1_tem[i] = int(LF_init)
      elif ((LF_loop_flag2 < i) & ( i <=LF_loop_flag3 )):
        angles1_tem[i] = angle1_position_temp1[i - LF_loop_flag2 - 1]
      elif (( LF_loop_flag3 < i) & (i <=LF_loop_flag4 )):
        angles1_tem[i] = angle1_position_temp2[i - LF_loop_flag3 - 1]    

    LB_loop_flag1 = 2 * timewalk
    LB_loop_flag2 = 2 * timewalk + 2*half_timewalk+1 
    LB_loop_flag3 = 2 * timewalk + 2*half_timewalk+1 + stoptime 
    LB_loop_flag4 = 2 * timewalk + 2*half_timewalk+1 + stoptime + 2*half_timewalk+1
    angles2_tem = np.zeros(LB_loop_flag4+1)

    for i in range(0,LB_loop_flag4+1):

      if (i<= LB_loop_flag1):
        angles2_tem[i] = angle2_position_temp2[i]
      elif ((LB_loop_flag1 < i) & (i <= LB_loop_flag2)):
        angles2_tem[i] = angle2_position_temp3[i - LB_loop_flag1 - 1]
      elif ((LB_loop_flag2 < i) & ( i <=LB_loop_flag3 )):
        angles2_tem[i] = int(LB_init)
      elif (( LB_loop_flag3 < i) & (i <=LB_loop_flag4 )):
        angles2_tem[i] = angle2_position_temp1[i - LB_loop_flag3 - 1]

    RF_loop_flag1 = 2 * timewalk
    RF_loop_flag2 = 2 * timewalk + 2*half_timewalk+1 
    RF_loop_flag3 = 2 * timewalk + 2*half_timewalk+1 + stoptime 
    RF_loop_flag4 = 2 * timewalk + 2*half_timewalk+1 + stoptime + 2*half_timewalk+1
    angles3_tem = np.zeros(RF_loop_flag4+1)

    for i in range(0,RF_loop_flag4+1):

      if (i<= RF_loop_flag1):
        angles3_tem[i] = angle3_position_temp2[i]
      elif ((RF_loop_flag1 < i) & (i <= RF_loop_flag2)):
        angles3_tem[i] = angle3_position_temp3[i - RF_loop_flag1 - 1]
      elif ((RF_loop_flag2 < i) & ( i <=RF_loop_flag3 )):
        angles3_tem[i] = int(RF_init)
      elif (( RF_loop_flag3 < i) & (i <=RF_loop_flag4 )):
        angles3_tem[i] = angle3_position_temp1[i - RF_loop_flag3 - 1]

    RB_loop_flag1 = 2 * half_timewalk
    RB_loop_flag2 = 2 * half_timewalk + stoptime
    RB_loop_flag3 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 
    RB_loop_flag4 = 2 * half_timewalk + stoptime + 2*half_timewalk+1 + 2*timewalk+1
    angles4_tem = np.zeros(RB_loop_flag4+1)
    for i in range(0,RB_loop_flag4+1):

      if (i<= RB_loop_flag1):
        angles4_tem[i] = angle4_position_temp3[i]
      elif ((RB_loop_flag1 < i) & (i <= RB_loop_flag2)):
        angles4_tem[i] = int(RB_init)
      elif ((RB_loop_flag2 < i) & ( i <=RB_loop_flag3 )):
        angles4_tem[i] = angle4_position_temp1[i - RB_loop_flag2 - 1]
      elif ((RB_loop_flag3 < i) & (i <=RB_loop_flag4 )):
        angles4_tem[i] = angle4_position_temp2[i - RB_loop_flag3 - 1]
        
    print(angle1_position_init)
    print("angle1_position_init")

    print(angle2_position_init)
    print("angle2_position_init")

    print(angle3_position_init)
    print("angle3_position_init")

    print(angle4_position_init)
    print("angle4_position_init")

    print(angles1_tem)
    print("angles1_tem")
    print(angles2_tem)
    print("angles2_tem")
    print(angles3_tem)
    print("angles3_tem")
    print(angles4_tem)
    print("angles4_tem")

    print(LB_loop_flag4+1,'size')
    
    return    
    
    
an1 = 20
an2 = -40
an3 = 25
an4 = -10
an5 = 35
an6 = -40
an7 = 20
an8 = -30
front_init = 40
back_init = -60
timewalk_input = 15
stoptime_input = 5

smoothForwardCalculate(an1, an2, an3, an4, an5, an6, an7, an8, front_init, back_init, timewalk_input, stoptime_input)




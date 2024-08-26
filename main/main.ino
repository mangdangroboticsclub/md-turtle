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
#      Generative AI
#      TTS: Text to Speach
#      STT: Speach to Text
#
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include "i2s_adc.h"
#include "cloud.h"
#include "tts.h"
#include "action.h"

const char* ssid = "Mangdang";
const char* password = "mangdang";

unsigned long cloud_start_time, gc_end_time, stt_end_time, ai_end_time, duration;  // for delay

void talk_loop() {
  Serial.println("=================================Record start!=================================");
  record();
  Serial.println("Record end!");

  uploadFile();

  String input_text = "";
  //speech to text
  input_text = speechToText();

  if (input_text != "") {  // ai response
    if (int(input_text.indexOf("come")) != -1) {
      tts("OK, my Guardian!");
      Serial.println("forward start");
        MoveForward(90, 6);
        Serial.println("\n\n forward end\n");
    } else if(int(input_text.indexOf("go")) != -1) {
      tts("OK, let's go!"); 
      Serial.println("smoothMoveForward");
      smoothMoveForward(6);
      Serial.println("\n\n smoothMoveForward end \n");
    }else if(int(input_text.indexOf("hand")) != -1) {
      tts("OK!"); 
      Serial.println("raise hand");
      servoLeftFront(60, 1, 1);
      Serial.println("\n\n raise hand end\n");
    } else if(int(input_text.indexOf("dance")) != -1) {
        tts("OK!,let's dance!"); 
        Serial.println("dance begin");
        MovementDance();
        Serial.println("\n\n dance end\n");
    }
	else {
      String ai_text = llm_response(input_text);
      if (ai_text != "") {  // text to speech
        tts(ai_text);
      }
    }
  }
  // tts("I  am doing well, thank you for asking!");
  delay(10);
}

void talk_task(void* args) {
  while (1) {
    talk_loop();
  }
  // delay(5000);
  // vTaskDelete(NULL);
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected");

  if (WiFi.status() == WL_CONNECTED) {
    // xTaskCreate(record_task, "record_task", 1024 * 8, NULL, 1, NULL);
    // record_task((void*)NULL);
  } else {
    Serial.println("WiFi Disconnected");
  }
  MoveInit();
  MoveReset();
}

void loop() {
  talk_loop();
}

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
# Description: This Turtle Arduino project is based on MD Robot Starter Kit(ESP32S3 chipset)
#              This file is focus unit test for servoes.
#
*/

#include <Audio.h>
#include <WiFi.h>
#include "Audio.h"
#include "action.h"

const char* ssid = "Mangdang";
const char* password = "mangdang";

#define I2S_DOUT 17  // Data Out
#define I2S_BCLK 16  // Bit Clock
#define I2S_LRC 15   // Left/Right Clock

// led
#include <Adafruit_NeoPixel.h>
#define PIN 47
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const String baseURL = "https://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&tl=en&q=";
// String audioFormat = "&c=mp3&f=16khz_8bit_mono";


bool isAlphaNumeric(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == ' ');
}

String encodeText(const String& text) {
  String encodedText = "";
  for (int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);
    if (isAlphaNumeric(c)) {
      if (c == ' ') {
        encodedText += "%20";
      } else {
        encodedText += c;
      }
    }
  }
  return encodedText;
}

int countWords(const String& text) {
  int count = 1;
  for (int i = 0; i < text.length(); i++) {
    if (text.charAt(i) == '+' || text.charAt(i) == ' ' || text.charAt(i) == '%') {
      count++;
    }
  }
  return count;
}

String removeNewlines(String text) {
  int index = 0;
  index = text.indexOf('\\n');
  Serial.println(index);
  while ((index = text.indexOf('\\n')) != -1) {
    text = text.substring(0, index) + text.substring(index + 1);
  }
  return text;
}

void testSingleServo(String text) {
  int index = text.indexOf(' ');
  int index1 = text.indexOf(' ', index + 1);
  Serial.println(index1);
  int index2 = text.indexOf(' ', index1 + 1);
  Serial.println(index2);
  int index3 = text.indexOf(' ', index2 + 1);
  Serial.println(index3);

  int ange = text.substring(index, index1).toInt();
  Serial.print("ange: ");
  Serial.println(ange);

  int walktime = text.substring(index1 + 1, index2).toInt();
  Serial.print("walktime: ");
  Serial.println(walktime);

  int loop_delay = text.substring(index2 + 1, index3).toInt();
  Serial.print("loop_delay: ");
  Serial.println(loop_delay);

  if (text.startsWith("leftfront"))
    servoLeftFront(ange, walktime, loop_delay);
  else if (text.startsWith("leftback"))
    servoLeftBack(ange, walktime, loop_delay);
  else if (text.startsWith("rightfront"))
    servoRightFront(ange, walktime, loop_delay);
  else if (text.startsWith("rightback"))
    servoRightBack(ange, walktime, loop_delay);
  else if (text.startsWith("head"))
    servoHead(ange, walktime, loop_delay);
  Serial.print(text);
  Serial.println("  end. ");
}

void init_led() {
  pixels.begin();
}

void reset_led() {
  pixels.clear();
  pixels.show();
}

void show_led(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  pixels.setPixelColor(n, r, g, b);
  pixels.show();
}

void testLed(String text) {
  int index = text.indexOf(' ');
  int index1 = text.indexOf(' ', index + 1);
  Serial.println(index1);
  int index2 = text.indexOf(' ', index1 + 1);
  Serial.println(index2);
  int index3 = text.indexOf(' ', index2 + 1);
  Serial.println(index3);
  int index4 = text.indexOf(' ', index3 + 1);
  Serial.println(index4);

  String cmd = text.substring(index, index1);
  Serial.print("cmd: ");
  Serial.println(cmd);

  uint8_t r = (uint8_t)(text.substring(index1 + 1, index2).toInt());
  Serial.print("r: ");
  Serial.println(r);

  uint8_t g = (uint8_t)(text.substring(index2 + 1, index3).toInt());
  Serial.print("g: ");
  Serial.println(g);

  uint8_t b = (uint8_t)(text.substring(index3 + 1, index4).toInt());
  Serial.print("b: ");
  Serial.println(b);

  if (text.startsWith("led init"))
    init_led();
  else if (text.startsWith("led reset"))
    reset_led();
  else if (text.startsWith("led show")) {
    for (int i = 0; i < NUMPIXELS; i++) {
      show_led(i, r, g, b);
    }
  }
  Serial.print(text);
  Serial.println("  end. ");
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");


  Serial.println("input cmd：");

  int internal = 5000;
  unsigned long split_time = millis();
  int file_duration, cur_audio_time;
  Audio* audio = nullptr;
  // MoveInit();
  // MoveReset();
  while (1) {
    if (Serial.available() > 0) {
      String text = Serial.readStringUntil('\n');
      text.trim();

      if (text.length() > 0) {
        if (text.startsWith("forward")) {
          int index = text.indexOf(' ');
          int index1 = text.indexOf(' ', index + 1);
          // Serial.println(index1);
          int index2 = text.indexOf(' ', index1 + 1);
          // Serial.println(index2);

          int step_delay = text.substring(index, index1).toInt();
          int loop_num = text.substring(index1 + 1, index2).toInt();

          MoveForward(step_delay, loop_num);
          continue;
        } else if (text.startsWith("smoothforward")) {
          int index = text.indexOf(' ');
          int index1 = text.indexOf(' ', index + 1);

          int loopNum = text.substring(index, index1).toInt();
          smoothMoveForward(loopNum);
          continue;
        } else if (text == "init") {
          MoveInit();
          continue;
        } else if (text == "reset") {
          MoveReset();
          continue;
        } else if (text == "movementdance") {
          MovementDance();
          continue;
        } else if (text.startsWith("smoothMoveForwardDegug")) {

          int index = text.indexOf(' ');
          int index1 = text.indexOf(' ', index + 1);
          Serial.println(index1);
          int index2 = text.indexOf(' ', index1 + 1);
          Serial.println(index2);
          int index3 = text.indexOf(' ', index2 + 1);
          Serial.println(index3);
          int index4 = text.indexOf(' ', index3 + 1);
          Serial.println(index4);
          int index5 = text.indexOf(' ', index4 + 1);
          Serial.println(index5);
          int index6 = text.indexOf(' ', index5 + 1);
          Serial.println(index6);
          int index7 = text.indexOf(' ', index6 + 1);
          Serial.println(index7);
          int index8 = text.indexOf(' ', index7 + 1);
          Serial.println(index8);
          int index9 = text.indexOf(' ', index8 + 1);
          Serial.println(index9);
          int index10 = text.indexOf(' ', index9 + 1);
          Serial.println(index10);
          int index11 = text.indexOf(' ', index10 + 1);
          Serial.println(index11);
          int index12 = text.indexOf(' ', index11 + 1);
          Serial.println(index12);
          int index13 = text.indexOf(' ', index12 + 1);
          Serial.println(index13);

          float an1 = 30;
          if (index > 0) an1 = text.substring(index + 1, index1).toFloat();
          Serial.print("an1: ");
          Serial.println(an1);

          float an2 = -30;
          if (index1 > 0) an2 = text.substring(index1 + 1, index2).toFloat();
          Serial.print("an2: ");
          Serial.println(an2);

          float an3 = 30;
          if (index2 > 0) an3 = text.substring(index2 + 1, index3).toFloat();
          Serial.print("an3: ");
          Serial.println(an3);

          float an4 = -30;
          if (index3 > 0) an4 = text.substring(index3 + 1, index4).toFloat();
          Serial.print("an4: ");
          Serial.println(an4);

          float an5 = -30;
          if (index4 > 0) an5 = text.substring(index4 + 1, index5).toFloat();
          Serial.print("an5: ");
          Serial.println(an5);

          float an6 = 30;
          if (index5 > 0) an6 = text.substring(index5 + 1, index6).toFloat();
          Serial.print("an6: ");
          Serial.println(an6);

          float an7 = -30;
          if (index6 > 0) an7 = text.substring(index6 + 1, index7).toFloat();
          Serial.print("an7: ");
          Serial.println(an7);

          float an8 = 30;
          if (index7 > 0) an8 = text.substring(index7 + 1, index8).toFloat();
          Serial.print("an8: ");
          Serial.println(an8);

          float an9 = 0;
          if (index8 > 0) an9 = text.substring(index8 + 1, index9).toFloat();
          Serial.print("an9: ");
          Serial.println(an9);

          float an10 = 0;
          if (index9 > 0) an10 = text.substring(index9 + 1, index10).toFloat();
          Serial.print("an10: ");
          Serial.println(an10);

          int timewalk_8 = 20;
          if (index10 > 0) timewalk_8 = text.substring(index10 + 1, index11).toInt();
          Serial.print("timewalk_8: ");
          Serial.println(timewalk_8);

          int step_delay_input_1 = 10;
          if (index11 > 0) step_delay_input_1 = text.substring(index11 + 1, index12).toInt();
          Serial.print("step_delay_input_1: ");
          Serial.println(step_delay_input_1);

          int stoptime_input = 10;
          if (index12 > 0) stoptime_input = text.substring(index12 + 1, index13).toInt();
          Serial.print("stoptime_input: ");
          Serial.println(stoptime_input);


          smoothMoveForwardDegug(an1, an2, an3, an4, an5, an6, an7, an8, an9, an10, timewalk_8, step_delay_input_1, stoptime_input);
          continue;
        } else if (text.startsWith("left") || text.startsWith("right") || text.startsWith("head")) {
          testSingleServo(text);
          continue;
        } else if (text.startsWith("led")) {
          testLed(text);
          continue;
        }
        if (audio) {
          audio->stopSong();
          delete audio;
          audio = nullptr;
        }
        audio = new Audio();
        audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
        //audio.setVolume(21); // Set volume level (0-100)
        // text = removeNewlines(text);
        text.replace("\\n", "");
        Serial.print("final text: ");
        Serial.println(text);
        String encodedText = encodeText(text);
        Serial.print("encodedText: ");
        Serial.println(encodedText);

        // String audioURL = baseURL + encodedText;
        String audioURL = text;
        Serial.println("URL: " + audioURL);
        audio->connecttohost(audioURL.c_str());
        file_duration = audio->getAudioFileDuration();
        Serial.print("File Duration: ");
        Serial.println(file_duration);
        split_time = millis();
        internal = 400 * countWords(text);
        Serial.print("internal: ");
        Serial.println(internal);
      }
    }

    if (audio)
      audio->loop();
    delay(1);

    unsigned long curtime = millis();
    if (curtime - split_time > internal) {
      split_time = curtime;
      // cur_audio_time = audio.getAudioCurrentTime();
      // Serial.print("cur_audio_time: ");
      // Serial.println(cur_audio_time);
      // if (audio)
      // audio->stopSong();
    }
  }
}

void loop() {
}

void audio_id3data(const char* info) {
  Serial.print("id3data: ");
  Serial.println(info);
}

void audio_eof_mp3(const char* info) {
  Serial.print("eof_mp3: ");
  Serial.println(info);
}

void audio_eof_speech(const char* info) {
  Serial.print("audio_eof_speech: ");
  Serial.println(info);
}

void audio_eof_stream(const char* info) {
  Serial.print("audio_eof_stream: ");
  Serial.println(info);
}

#include "tts.h"
#include <Arduino.h>
// #ifdef ESP_IDF_VERSION_MAJOR
// #undef ESP_IDF_VERSION_MAJOR
// #endif
// #define ESP_IDF_VERSION_MAJOR 5
#include <Audio.h>

#define I2S_DOUT 17  // Data Out
#define I2S_BCLK 16  // Bit Clock
#define I2S_LRC 15   // Left/Right Clock
const String baseURL = "https://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&tl=en&q=";


bool isAlphaNumeric(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == ' ') || (c == ',') || (c == '.') || (c == '?') || (c == '!');
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
    if (text.charAt(i) == ' ') {
      count++;
    }
  }
  return count;
}

void tts(String text) {
  Serial.println("tts start.");
  Audio* audio = new Audio();
  // Serial.println("new an Audio instance.");
  audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  //audio.setVolume(21); // 设置音量级别 (0-100)

  // 编码文本并过滤掉非字母数字字符
  text.replace("\\n", "");  // delete all "\n"
  // Serial.print("final text: ");
  // Serial.println(text);
  String encodedText = encodeText(text);
  // Serial.print("encoded text: ");
  // Serial.println(encodedText);

  // 生成音频 URL
  String audioURL = baseURL + encodedText;
  // Serial.print("生成的URL: ");
  // Serial.println(audioURL);

  // // 连接并播放音频
  audio->connecttohost(audioURL.c_str());
  unsigned long start_time = millis();
  int max_duration = 400 * countWords(text);
  // Serial.print("max_duration: ");
  // Serial.println(max_duration);

  while (1) {
    audio->loop();
    delay(1);
    if (millis() - start_time > max_duration) {
      // audio->I2Sstop(I2S_NUM_0);
      // Serial.println("speech end!");
      break;
    }
  }
  audio->stopSong();
  Serial.println("free Audio instance.");
  delete audio;
  audio = nullptr;
}

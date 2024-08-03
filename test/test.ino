#include <Audio.h>

#include <WiFi.h>
#include "Audio.h"
#include "action.h"

const char* ssid = "Mangdang";
const char* password = "mangdang";

#define I2S_DOUT 17  // Data Out
#define I2S_BCLK 16  // Bit Clock
#define I2S_LRC 15   // Left/Right Clock

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

  float ange = text.substring(index, index1).toFloat();
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
  Serial.print(text);
  Serial.println("  end. ");
}

void testAllServo(String text) {
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

  float ange1 = text.substring(index + 1, index1).toFloat();
  Serial.print("ange1: ");
  Serial.println(ange1);

  float ange2 = text.substring(index1 + 1, index2).toFloat();
  Serial.print("ange2: ");
  Serial.println(ange2);

  float ange3 = text.substring(index2 + 1, index3).toFloat();
  Serial.print("ange3: ");
  Serial.println(ange3);

  float ange4 = text.substring(index3 + 1, index4).toFloat();
  Serial.print("ange4: ");
  Serial.println(ange4);

  int walktime = text.substring(index4 + 1, index5).toInt();
  Serial.print("walktime: ");
  Serial.println(walktime);

  int loop_delay = text.substring(index5 + 1, index6).toInt();
  Serial.print("loop_delay: ");
  Serial.println(loop_delay);

  servo(ange1, ange2, ange3, ange4, walktime, loop_delay);
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


  Serial.println("请输入要转换成语音的文字，然后按回车键：");

  int internal = 5000;
  unsigned long split_time = millis();
  int file_duration, cur_audio_time;
  Audio* audio = nullptr;
  // Servo_Init();
  MoveInit();
  MoveReset();
  while (1) {
    if (Serial.available() > 0) {
      String text = Serial.readStringUntil('\n');
      text.trim();  // 移除输入字符串两端的空格和换行符

      if (text.length() > 0) {
        if (text.startsWith("forward")) {
          int index = text.indexOf(' ');
          int index1 = text.indexOf(' ', index + 1);
          int step_delay = 1000;
          if (index > 0) {
            step_delay = text.substring(index + 1, index1).toInt();
          }
          Serial.print("step_delay: ");
          Serial.println(step_delay);
          MoveForward(step_delay);
          continue;
        } else if (text == "init") {
          MoveInit();
          continue;
        } else if (text == "reset") {
          MoveReset();
          continue;
        } else if (text.startsWith("left") || text.startsWith("right")) {
          testSingleServo(text);
          continue;
        } else if (text.startsWith("servo")) {
          testAllServo(text);
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
        Serial.println("生成的URL: " + audioURL);
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
      audio->loop();  // 必须反复调用以保持音频播放
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

void audio_id3data(const char* info) {  // 当找到ID3数据时调用
  Serial.print("id3data: ");
  Serial.println(info);
}

void audio_eof_mp3(const char* info) {  // 当MP3文件播放结束时调用
  Serial.print("eof_mp3: ");
  Serial.println(info);
}

void audio_eof_speech(const char* info) {  // 当speech播放结束时调用
  Serial.print("audio_eof_speech: ");
  Serial.println(info);
}

void audio_eof_stream(const char* info) {  // 当stream播放结束时调用
  Serial.print("audio_eof_stream: ");
  Serial.println(info);
}

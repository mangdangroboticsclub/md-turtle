#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include "i2s_adc.h"
#include "cloud.h"
#include "tts.h"

const char* ssid = "Mangdang";
const char* password = "mangdang";

unsigned long cloud_start_time, gc_end_time, stt_end_time, ai_end_time, duration;  // for delay

void record_task(void* args) {
  while (1) {
    Serial.println("=================================Record start!=================================");
    record();
    Serial.println("Record end!");

    // upload to google cloud
    // cloud_start_time = millis();
    // uploadFile();
    // gc_end_time = millis();
    // duration = gc_end_time - cloud_start_time;
    // Serial.print("upload(), took: ");
    // Serial.println(duration);

    String input_text = "";
    //speech to text
    input_text = speechToText();
    stt_end_time = millis();
    duration = stt_end_time - gc_end_time;
    Serial.print("speechToText(), took: ");
    Serial.println(duration);

    if (input_text != "") {  // ai response
      String ai_text = llm_response(input_text);
      ai_end_time = millis();
      duration = ai_end_time - stt_end_time;
      Serial.print("ai(), took: ");
      Serial.println(duration);

      duration = ai_end_time - cloud_start_time;
      Serial.print("all took: ");
      Serial.println(duration);
      if (ai_text != "") {  // text to speech
        tts(ai_text);
      }
    } else {
      tts("Hello, I am Amy!");
    }
    // tts("I  am doing well, thank you for asking!");
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
}

void loop() {
  Serial.println("=================================Record start!=================================");
  record();
  Serial.println("Record end!");

  // upload to google cloud
  // cloud_start_time = millis();
  uploadFile();
  // gc_end_time = millis();
  // duration = gc_end_time - cloud_start_time;
  // Serial.print("upload(), took: ");
  // Serial.println(duration);

  String input_text = "";
  //speech to text
  input_text = speechToText();
  // stt_end_time = millis();
  // duration = stt_end_time - gc_end_time;
  // Serial.print("speechToText(), took: ");
  // Serial.println(duration);

  if (input_text != "") {  // ai response
    String ai_text = llm_response(input_text);
    // ai_end_time = millis();
    // duration = ai_end_time - stt_end_time;
    // Serial.print("ai(), took: ");
    // Serial.println(duration);

    // duration = ai_end_time - cloud_start_time;
    // Serial.print("all took: ");
    // Serial.println(duration);
    if (ai_text != "") {  // text to speech
      tts(ai_text);
    }
  }
  // tts("I  am doing well, thank you for asking!");
  delay(10);
}

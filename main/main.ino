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
      tts("OK, my master!");
      Serial.println("forward start");
      MoveForward(400);
      Serial.println("\n\nForwarding\n");
    } else {
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

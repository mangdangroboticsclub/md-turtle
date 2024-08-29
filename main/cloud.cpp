#include <Audio.h>

#include "cloud.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include "params.h"

//onst char* accessToken = "SAMPLE_TOKEN_HERE";

// STT Config
const char* bucketName = "mangdang_voice";
const char* audioContent = "gs://mangdang_voice/audio.wav";
const char* gcAdd = "https://storage.googleapis.com/upload/storage/v1/b/mangdang_voice/o?uploadType=media&name=audio.wav";

// Gemini API Config
const char* apiURL = "https://us-central1-aiplatform.googleapis.com/v1/projects/modern-rex-420404/locations/us-central1/publishers/google/models/gemini-1.5-flash:streamGenerateContent?alt=sse";


void uploadFile() {
  Serial.println("Opening file...");
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open file");
    file.close();
    return;
  }
  Serial.println("File opened successfully");

  HTTPClient _http;

  // String url = "https://storage.googleapis.com/upload/storage/v1/b/" + String(_bucketName) + "/o?uploadType=media&name=audio.wav";
  Serial.print("Connecting to URL: ");
  Serial.println(gcAdd);
  _http.begin(gcAdd);
  _http.addHeader("Authorization", "Bearer " + String(accessToken));
  _http.addHeader("Content-Type", "application/octet-stream");

  Serial.println("Sending POST request...");
  Serial.println(file.size());
  int httpCode = _http.sendRequest("POST", &file, file.size());
  Serial.println("POST request sent");

  if (httpCode > 0) {
    String response = _http.getString();
    // Serial.println("Response: " + response);
  } else {
    Serial.println("Error on file upload, HTTP code: " + String(httpCode));
  }

  file.close();
  _http.end();
  Serial.println("File closed and HTTP connection ended");
}

const String speechRequestData = "{\"config\": {\"encoding\":\"LINEAR16\",\"languageCode\":\"en-US\",\"enableWordTimeOffsets\":false},\"audio\":{\"uri\":\"gs://mangdang_voice/audio.wav\"}}";
String speechToText() {
  Serial.println("Speech to text start.");
  HTTPClient _http;

  _http.begin("https://speech.googleapis.com/v1/speech:recognize");
  _http.addHeader("Content-Type", "application/json");
  _http.addHeader("Authorization", "Bearer " + String(accessToken));
  _http.addHeader("User-Agent", "PostmanRuntime/7.40.0");
  _http.addHeader("Accept", "*/*");
  _http.addHeader("Accept-Encoding", "gzip, deflate, br");
  _http.addHeader("Connection", "keep-alive");
  _http.addHeader("x-goog-user-project", "modern-rex-420404");

  Serial.println("Speech request.");
  Serial.println("Speech request post start.");
  int httpResponseCode = _http.POST(speechRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode == 200) {
    String response = _http.getString();
    // Serial.println("Response: ");
    // Serial.println(response);

    int transcriptIndexStart = response.indexOf("\"transcript\": \"") + 15;
    int transcriptIndexEnd = response.indexOf("\"", transcriptIndexStart);

    if (transcriptIndexStart > 14 && transcriptIndexEnd > transcriptIndexStart) {
      String transcript = response.substring(transcriptIndexStart, transcriptIndexEnd);
      Serial.print("Transcript: ");
      Serial.println(transcript);
      return transcript;
    } else {
      Serial.println("Transcript not found.");
    }
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  _http.end();
  return "";
}

String generateJsonString(String text) {
  return "{"
         "\"contents\": ["
         "{"
         "\"role\": \"USER\","
         "\"parts\": { \"text\": \"Now, you are a small female robo turtle, your name is Amy. You will be a helpful AI assistent. Your LLM api is connected to STT and TTS models so you are able to hear the user.\" }"
         "},"
         "{"
         "\"role\": \"MODEL\","
         "\"parts\": { \"text\": \"OK, I know.\" }"
         "},"
         "{"
         "\"role\": \"USER\","
         "\"parts\": { \"text\": \""
         + text + "\" }"
                  "}"
                  "]"
                  ",\"safety_settings\": {"
                  "\"category\": \"HARM_CATEGORY_SEXUALLY_EXPLICIT\","
                  "\"threshold\": \"BLOCK_LOW_AND_ABOVE\""
                  "},"
                  "\"generation_config\": {"
                  "\"temperature\": 0.2,"
                  "\"topP\": 0.8,"
                  "\"topK\": 40,"
                  "\"maxOutputTokens\": 20"
                  "}"
                  "}";
}


String llm_response(String transcript) {
  HTTPClient _http;

  Serial.println("Start sending HTTP POST request...");
  _http.begin(apiURL);
  _http.addHeader("Content-Type", "application/json");
  _http.addHeader("Authorization", "Bearer " + String(accessToken));

  // Constrct JSON data
  // String jsonData = "{\"contents\": {\"role\": \"user\", \"parts\": [{\"text\": \"" + transcript + "\"}]}}";
  String jsonData = generateJsonString(transcript);
  // Serial.print("Sent JSON data: ");
  // Serial.println(jsonData);

  int httpResponseCode = _http.POST(jsonData);

  String geminiResponseText = "";
  if (httpResponseCode > 0) {
    String response = _http.getString();
    // Serial.print("HTTP repond code: ");
    // Serial.println(httpResponseCode);
    // Serial.print("Responded data: ");
    // Serial.println(response);

    int startIndex = 0;
    while (true) {
      startIndex = response.indexOf("\"text\": \"", startIndex);
      if (startIndex == -1) break;
      startIndex += 9;
      int endIndex = response.indexOf("\"", startIndex);
      if (endIndex == -1) break;
      geminiResponseText += response.substring(startIndex, endIndex) + " ";
      startIndex = endIndex + 1;
    }

    Serial.print("Combined text: ");
    Serial.println(geminiResponseText);
  } else {
    Serial.print("Request failed, HTTP respond code: ");
    Serial.println(httpResponseCode);
  }

  _http.end();
  return geminiResponseText;
}
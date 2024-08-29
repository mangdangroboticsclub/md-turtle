#ifndef CLOUD_H
#define CLOUD_H

#include <Arduino.h>

extern String accessToken;

void uploadFile();
String speechToText();
String llm_response(String transcript);

#endif
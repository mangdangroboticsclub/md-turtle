#ifndef CLOUD_H
#define CLOUD_H

#include <Arduino.h>

void uploadFile();
String speechToText();
String llm_response(String transcript);

#endif
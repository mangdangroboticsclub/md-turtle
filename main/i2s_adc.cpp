#include <Arduino.h>
#include <SPIFFS.h>
#include "i2s_adc.h"
#include <stdlib.h>
#include "params.h"
#include <driver/i2s_std.h>
#include <driver/gpio.h>

const int headerSize = 44;

void createWavHeader(byte* header, int channel_num, int sample_rate, int sample_bits, int audio_duration) {
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int wavSize = channel_num * sample_rate * sample_bits * audio_duration / 8;
  unsigned int fileSize = wavSize + headerSize - 8;
  header[4] = (byte)(fileSize & 0xFF);
  header[5] = (byte)((fileSize >> 8) & 0xFF);
  header[6] = (byte)((fileSize >> 16) & 0xFF);
  header[7] = (byte)((fileSize >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  header[20] = 0x01;
  header[21] = 0x00;
  header[22] = (byte)channel_num;
  header[23] = 0x00;
  header[24] = (byte)(sample_rate & 0xff);
  header[25] = (byte)((sample_rate >> 8) & 0xff);
  header[26] = (byte)((sample_rate >> 16) & 0xff);
  header[27] = (byte)((sample_rate >> 24) & 0xff);
  unsigned int byte_rate = channel_num * sample_rate * sample_bits / 8;
  header[28] = (byte)(byte_rate & 0xff);
  header[29] = (byte)((byte_rate >> 8) & 0xff);
  header[30] = (byte)((byte_rate >> 16) & 0xff);
  header[31] = (byte)((byte_rate >> 24) & 0xff);
  header[32] = (byte)(channel_num * sample_bits / 8);
  header[33] = 0x00;
  header[34] = (byte)sample_bits;
  header[35] = 0x00;
  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(wavSize & 0xFF);
  header[41] = (byte)((wavSize >> 8) & 0xFF);
  header[42] = (byte)((wavSize >> 16) & 0xFF);
  header[43] = (byte)((wavSize >> 24) & 0xFF);
}

void listSPIFFS(void) {
  Serial.println(F("\r\nListing SPIFFS files:"));
  static const char line[] PROGMEM = "=================================================";
  Serial.println(FPSTR(line));
  Serial.println(F("  File name                              Size"));
  Serial.println(FPSTR(line));
  fs::File root = SPIFFS.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }
  fs::File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in SPIFFS
      int spaces = 33 - fileName.length();
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String)file.size();
      spaces = 10 - fileSize.length();
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }
    file = root.openNextFile();
  }
  Serial.println(FPSTR(line));
  Serial.println();
  delay(1000);
}

//void i2s_adc_data_scale(uint8_t* d_buff, uint8_t* s_buff, uint32_t len) {
void i2s_adc_data_scale(uint16_t* buff, size_t len) {
  for (size_t i = 0; i < len; i++) {
    uint16_t val = buff[i] * 256 / 2048;
    buff[i] = (val << 8);
  }
}

void record() {
  // prepare file
  Serial.println("prepare file!");
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();
  }
  Serial.println("Removing file!");
  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("File is not available!");
  }
  byte header[headerSize];
  createWavHeader(header, I2S_CHANNEL_NUM, I2S_SAMPLE_RATE, I2S_SAMPLE_BITS, RECORD_TIME);

  file.write(header, headerSize);

  // config i2s
  Serial.println("config i2s!");
  i2s_chan_handle_t rx_handle;
  i2s_chan_config_t chan_cfg = {
    .id = I2S_NUM_0,
    .role = I2S_ROLE_MASTER,
    .dma_desc_num = DMA_BUFFER_COUNT,
    .dma_frame_num = I2S_FRAME_NUM,
    .auto_clear = true,
  };
  ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_handle));

  i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(I2S_SAMPLE_RATE),
    .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
    .gpio_cfg = {
      .mclk = I2S_GPIO_UNUSED,
      .bclk = GPIO_NUM_10,
      .ws = GPIO_NUM_11,
      .dout = I2S_GPIO_UNUSED,
      .din = GPIO_NUM_4,
      .invert_flags = {
        .mclk_inv = false,
        .bclk_inv = true,
        .ws_inv = true,
      },
    },
  };
  std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_RIGHT;
  ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));
  ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));

  // start record
  Serial.println("start record");
  int flash_wr_size = 0;
  size_t bytes_read = 0;


  // We apply a factor because one DMA buffer space is not enough.
  uint16_t i2s_read_buff[I2S_FRAME_NUM*BUFFER_SAFETY_FACTOR];

  Serial.println(" *** Recording Start *** ");
  
  esp_err_t read_error;
  while (flash_wr_size < FLASH_RECORD_SIZE) {
    read_error = i2s_channel_read(rx_handle, (void*)&i2s_read_buff, I2S_READ_LEN*BUFFER_SAFETY_FACTOR, &bytes_read, portMAX_DELAY);
    if(read_error != ESP_OK) {
      Serial.println(" Error while reading , this should not happen!");
      file.close();
      return;
    }
    i2s_adc_data_scale(i2s_read_buff, bytes_read/2);
    file.write((const byte*)i2s_read_buff, bytes_read);
    flash_wr_size += bytes_read;
  }
  Serial.println("flash_wr_size: " + String(flash_wr_size));
  Serial.println(" *** Recording Done *** ");
  file.close();
  Serial.println(" *** File closed *** ");

  i2s_channel_disable(rx_handle);
  i2s_del_channel(rx_handle);

  Serial.println(" *** Record function done *** ");
  // listSPIFFS();
}

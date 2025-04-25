#ifndef I2S_ADC_H
#define I2S_ADC_H

#define I2S_WS 11
#define I2S_SD 4
#define I2S_SCK 10
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE (16000)
#define I2S_SAMPLE_BITS (16)
#define I2S_FRAME_NUM (1024)
#define DMA_BUFFER_COUNT (64)
// At 16Khz, we take ~200 ms to write in the flash.
// It means, we must be able to store 3200 samples in the DMA buffers.
// One buffer of 1024 samples represent 64 ms at 16 khz.
// We have a 1024*64 samples capacity. So toward the end, we accumulate some late.
// This is why the buffer safety factor is necessary.
#define BUFFER_SAFETY_FACTOR (2)
#define I2S_READ_LEN (I2S_SAMPLE_BITS / 8 * I2S_FRAME_NUM)
#define RECORD_TIME (5)
#define I2S_CHANNEL_NUM (1)
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)


void record();

#endif

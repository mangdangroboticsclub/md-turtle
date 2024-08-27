# Google Gemini on ESP32-S3

This project integrates the Google Gemini model into the ESP32-S3, allowing users to interact with it through voice commands. The system controls the movement of servo motors based on the commands received. This setup could be used for a variety of applications, including robotics and interactive installations.

## Hardware Requirements

To run this project, you will need the following hardware components:

- **IMMP441 Microphone:** for capturing audio inputs.
- **MAX98357 Amplifier:** to drive the speaker.
- **Speaker:** for audio output.
- **5 Servo Motors:** for movement control.
- **ESP32-S3 (16MB) Board:** the main controller running the Google Gemini model.

## Software Requirements

- **Arduino IDE version 2.3.2 or higher**

## Installation

### Setting up the Arduino Environment

1. **Install the Arduino IDE:** Download and install Arduino IDE from [Arduino Official Website](https://www.arduino.cc/en/software).

### Installing the ESP32-S3 Board

2. **Board Installation:**
   - Follow the instructions on [Installing the ESP32 Board](https://docs.sunfounder.com/projects/umsk/en/latest/03_esp32/esp32_start/03_install_esp32.html) to add the ESP32-S3 to your Arduino IDE.

### Adding Required Libraries

3. **Install Audio and Servo Libraries:**
   - Go to `Sketch > Include Library > Add .ZIP Library` and select `ESP32-audioI2S-master.zip`.
   - Then, go to `Sketch > Include Library > Manage Libraries`, search for and install `ESP32Servo`.

### Configure Board Settings in Arduino IDE

4. **Board Configuration:**
   - Go to `Tools`, and set the following:
     - **Board:** `ESP32S3 Dev Module`
     - **Flash Size:** `16MB`
     - **Partition Scheme:** `8M with spiffs (3MB APP/1.5MB SPIFFS)`
     - **Upload Speed:** `115200`
     - **Port:** Select the corresponding port.

### Preparing the Code

5. **Setting Access Token:**
   - In the `cloud.cpp` file, replace the placeholder in `const char* accessToken = “SAMPLE TOKEN HERE”;` with your actual access token.
   - To obtain an access token, log in to the Google Cloud Platform console, open the `cloud shell` from the top left of the page, and execute:
     ```bash
     gcloud auth application-default print-access-token
     ```
   - Copy and paste the token back into your code.

### Uploading the Code

6. **Upload the Project:**
   - Once all settings are configured and the access token is set, upload the code to the ESP32-S3 board by clicking on the upload button in Arduino IDE.

## Running the Project

After uploading the code, the ESP32-S3 will process audio input through the microphone, interact via the Google Gemini model, and control the servo motors accordingly.

## Troubleshooting

If you encounter issues, please check the following:
- Ensure all library dependencies are correctly installed.
- Verify board settings match the requirements stated above.
- Check the serial monitor for any error messages after uploading the code.

## Contributing

Contributions to this project are welcome! Please fork the repository and submit a pull request with your improvements.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
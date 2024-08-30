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
   - Download this repository by pressing "<>Code" at this page them press "Download ZIP".
   - Extract the zip file from the downloaded package.
   - Go to `Sketch > Include Library > Add .ZIP Library` and select `ESP32-audioI2S-master.zip` from the "resource" folder in the zip file "md-turtle" you just extracted.
   - Delete `ESP32-audioI2S-master.zip` from the extracted folder after importing it.
   - Then, go to `Sketch > Include Library > Manage Libraries`, search for and install `ESP32Servo`, the version "3.0.5" is recommended.
   - If you see any notifications for updates, press "install all". 

### Configure Board Settings in Arduino IDE

4. **Board Configuration:**
   - Go to `Tools`, and set the following:
     - **Board:** `ESP32S3 Dev Module`
     - **Flash Size:** `16MB`
     - **Partition Scheme:** `8M with spiffs (3MB APP/1.5MB SPIFFS)`
     - **Upload Speed:** `115200`
     - **Port:** Select the corresponding port.

### Preparing the Code

5. **Setting Access Token (IF YOU ARE USING YOUR OWN ACCOUNT:)**
   - In the `cloud.cpp` file, remove "//" in the code  `const char* accessToken = “SAMPLE TOKEN HERE”;` then replace the placeholder in with your actual access token.
   - To obtain an access token, open the `cloud shell` from this link https://cloud.google.com/shell, press go to console and log into the account, then execute:
     ```bash
     gcloud auth application-default print-access-token
     ```
   - Copy and paste the token back into your code.
   - Press "Edit" in arduino ide top toolbar and then "Replace in Files", replace "mangdang_voice" with your own bucket name, then replace "modern-rex-420404 with your own project id of google cloud.

### Uploading the Code

6. **Upload the Project:**
   - Press "file" in arduino ide then "Open", look for the package you downloaded and extracted from the previous step, select "main" then "main.ino".
   - Once all settings are configured and the access token is set, upload the code to the ESP32-S3 board by clicking on the upload button in Arduino IDE. If you see the progress reaches 100%, you are done.
   - Press "Tools" in arudino ide and then "Serial Monitor" to checkout the output of the turtle.

## Running the Project

After uploading the code, the ESP32-S3 will process audio input through the microphone, interact via the Google Gemini model, and control the servo motors accordingly.
You can talk to the Gemini AI freely during when the serial monitor shows that recording is started, and if your speech contains words among "come", or "go", or "dance", or "hand", the motion of turtle will be triggered.

## Troubleshooting

If you encounter issues, please check the following:
- Ensure all library dependencies are correctly installed.
- Verify board settings match the requirements stated above.
- Check the serial monitor for any error messages after uploading the code.
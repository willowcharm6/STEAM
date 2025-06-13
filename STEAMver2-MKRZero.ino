#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>

const int buttonPins[] = {2, 3, 4, 5, 6};
const char* wavFiles[] = {
  "1.WAV",
  "2.WAV",
  "3.WAV",
  "4.WAV",
  "5.WAV"
};

const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);
const int speakerPin = A0;

File audioFile;

void setup() {
  Serial.begin(9600);

  // Setup buttons
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); // Use internal pull-up resistors
  }

  // Setup SD
  if (!SD.begin(SDCARD_SS_PIN)) {
    Serial.println("❌ SD card failed or not present!");
    while (1);
  }
  Serial.println("✅ SD card initialized.");

  // Setup Audio
  AudioZero.begin(22050);
}

void loop() {
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      Serial.print("Button ");
      Serial.print(i + 1);
      Serial.print(" pressed → Playing ");
      Serial.println(wavFiles[i]);

      playSound(wavFiles[i]);

      // Wait for button release
      while (digitalRead(buttonPins[i]) == LOW) {
        delay(10);
      }
      delay(200); // debounce
    }
  }
}

void playSound(const char* filename) {
  audioFile = SD.open(filename);
  if (!audioFile) {
    Serial.print("❌ Could not open ");
    Serial.println(filename);
    return;
  }

  AudioZero.play(audioFile);
  audioFile.close();
}

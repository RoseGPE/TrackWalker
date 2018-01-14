/*
 * This program does the following:
 *  - When the board is ready to begin operation, the pin on readyLight will be set HIGH.
 *  - When the switch on logSelectionPin is HIGH, logs datapoints to the SD card in form of "leftEncoderTicks,rightEncoderTicks" per line.
 *    - This occurs whenever either of the encoders goes an additional sampleTol ticks from the last datapoint.
 *    - While the logSelectionPin is HIGH, the pin on loggingLight will be set HIGH to indicate logging.
 *  - When the switch on logSelectionPin is toggled from HIGH to LOW, the connection to the SD card will be closed.
 */


#include <Encoder.h>

Encoder knobLeft(20, 21);
Encoder knobRight(18, 19);

#include <SPI.h>
#include <SD.h>
const int chipSelect = 53;

const int readyLight = 15;
const int loggingLight = 16;

const int logSelectionPin = 17;
const int sampleTol = 10;

void setup() {
  Serial.begin(9600);

  pinMode(loggingLight, OUTPUT);
  digitalWrite(loggingLight,LOW);
  pinMode(readyLight, OUTPUT);
  digitalWrite(readyLight,LOW);
  pinMode(logSelectionPin, INPUT_PULLUP);


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // Just quit because there's nothing else to do
    while (1);
  }
  Serial.println("card initialized.");

  digitalWrite(loggingLight,LOW);
  digitalWrite(readyLight,HIGH);
}

long positionLeft  = -999;
long positionRight = -999;

void loop() {
  File dataFile;
  bool startup = 1;
  while(digitalRead(logSelectionPin)) {
    if (startup) {
      Serial.println("Lets go");
      char filename[] = "xxxxxxxx.log";
      long i;
      for (i=0;i<=1000;i++) {
        sprintf(filename, "run%04d.log", i);
        if(!SD.exists(filename)) break;
      }
      Serial.println(filename);
      dataFile = SD.open(filename, FILE_WRITE);
      knobLeft.write(0);
      knobRight.write(0);
      positionLeft  = -999;
      positionRight = -999;
      startup = 0;
    }
    digitalWrite(loggingLight,HIGH);
    long newLeft, newRight;
    newLeft = knobLeft.read();
    newRight = knobRight.read();
    if (abs(newLeft-positionLeft)>sampleTol || abs(newRight-positionRight)>sampleTol) {
      //Log this datapoint: "newLeft, newRight"
      String dataString = "";
      dataString += String(newLeft);
      dataString += ",";
      dataString += String(newRight);
      dataFile.println(dataString);
      Serial.println(dataString);
      
      positionLeft = newLeft;
      positionRight = newRight;
    }
    delay(30);
  }
  if (!startup) {
    Serial.println("Done.");
    dataFile.close();
  }
  digitalWrite(loggingLight,LOW);
  delay(100);
}

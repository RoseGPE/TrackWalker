/*
 * This program does the following:
 *  - When the board is ready to begin operation, the pin on readyLight will be set HIGH.
 *  - When the switch on logSelectionPin is HIGH, logs datapoints to the SD card in form of "leftEncoderTicks,rightEncoderTicks" per line.
 *    - This occurs whenever either of the encoders goes an additional sampleTol ticks from the last datapoint.
 *    - While the logSelectionPin is HIGH, the pin on loggingLight will be set HIGH to indicate logging.
 *  - When the switch on logSelectionPin is toggled from HIGH to LOW, the connection to the SD card will be closed.
 */


#include <Encoder.h>

Encoder knobLeft(2, 3);
Encoder knobRight(18, 19);

#include <SPI.h>
#include <SD.h>
const int chipSelect = 53;

const int readyLight = 4;
const int loggingLight = 5;

const int logSelectionPin = 6;
const int sampleTol = 10;

void setup() {
  Serial.begin(9600);

  digitalWrite(loggingLight,LOW);
  digitalWrite(readyLight,LOW);


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
  while(digitalRead(logSelectionPin) && dataFile) {
    if (startup) {
      dataFile = SD.open("walkerlog.txt", FILE_WRITE);
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
      
      positionLeft = newLeft;
      positionRight = newRight;
    }
  }
  if (!startup) {
    dataFile.close();
  }
  digitalWrite(loggingLight,LOW);
  delay(10);
}

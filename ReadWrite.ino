//CanSat Design Challenge 
//Group Members: Bhagat, Mackenzie, Malin
const int sucessPin = 4; //green
const int failedPin = 5; // red
const int infoCapturingPin = 3; // blue
const int initializingPin = 2; //white
const int BuzzerPin = 6; //buzzer

int x;
int launchInterval = 10000;
int lightsInterval = 2000;
int otherInterval = 1000;
int sucessInterval = 4000;

//SD Card Pin Configurations:
// CS SD Pin: Digital Pin 10
//SCK SD Pin: Digital Pin 13
//MOSI SD Pin: Digital Pin 11
//MISO SD Pin: Digital Pin 12
//VCC SD Pin: 5V Pin
// GND SD Pin: GND Pin

//Notice: SCL Pin also A5 and SDA Pin also A4

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <MQ135.h> 
//essential variable declaration and initialization
File myFile;
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
MQ135 mq135_sensor(A0);
float temperature;
float humidity = 25.0;

void setup() {
  //Initialization
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //LED Initialization
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  pinMode(sucessPin, OUTPUT);
  pinMode(failedPin, OUTPUT);
  pinMode(infoCapturingPin, OUTPUT);
  pinMode(initializingPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(10, OUTPUT);
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //Serial Initialization
  //--------------------------------------------------------------
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:

  noTone(BuzzerPin);
  digitalWrite(initializingPin, LOW);
  digitalWrite(failedPin, LOW);
  digitalWrite(sucessPin, LOW);
  digitalWrite(infoCapturingPin, LOW);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
//--------------------------------------------------------------
//SD Card
//--------------------------------------------------------------
  Serial.println("ISC");
  digitalWrite(initializingPin, HIGH);
  delay(lightsInterval);
  if (!SD.begin(10)) {
    Serial.println("IF");
    digitalWrite(initializingPin, LOW);
    digitalWrite(failedPin, HIGH);
    while (!SD.begin(10)){
      tone(BuzzerPin, 1000);
      delay(otherInterval);
      if (SD.begin(10)){
        break;
      }
    }
    noTone(BuzzerPin);
    digitalWrite(failedPin, LOW);
    digitalWrite(initializingPin, HIGH);
    delay(lightsInterval);
  }
  digitalWrite(initializingPin, LOW);
  Serial.println("ID");
  digitalWrite(sucessPin, HIGH);
  delay(sucessInterval);
  digitalWrite(sucessPin, LOW);
  //--------------------------------------------------------------
  //BMP280 Sensor
  //--------------------------------------------------------------
    unsigned status;
    status = bmp.begin();
    digitalWrite(initializingPin, HIGH);
    delay(lightsInterval);
    if (!status) {
      Serial.println("No BMP280");
      digitalWrite(initializingPin, LOW);
      digitalWrite(failedPin, HIGH);
      while (!bmp.begin()){
        tone(BuzzerPin, 1000);
        delay(otherInterval);
        status = bmp.begin();
        if (status){
          break;
        }
      }
      noTone(BuzzerPin);
      digitalWrite(failedPin, LOW);
      digitalWrite(initializingPin, HIGH);
      delay(lightsInterval);
    }
    digitalWrite(initializingPin, LOW);
    Serial.println("bmp_setupSucess");
    digitalWrite(sucessPin, HIGH);
    delay(sucessInterval);
    digitalWrite(sucessPin, LOW);
  //--------------------------------------------------------------
  //MQ135
  //--------------------------------------------------------------
  digitalWrite(initializingPin, HIGH);
  delay(lightsInterval);
  int checkValue  = analogRead(A0);
  if (checkValue==0 || checkValue ==1023){
    digitalWrite(initializingPin, LOW);
    digitalWrite(failedPin, HIGH);
    while (checkValue==0 || checkValue ==1023){
      tone(BuzzerPin, 1000);
      if (!(checkValue==0 || checkValue ==1023)){
        noTone(BuzzerPin);
        break;
      }
      digitalWrite(initializingPin, HIGH);
      delay(lightsInterval);
    }
  }
  digitalWrite(initializingPin, LOW);
  Serial.println("MQ_setupSucess");
  digitalWrite(sucessPin, HIGH);
  delay(sucessInterval);
  digitalWrite(sucessPin, LOW);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Actions
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//bmp280 sensor
//--------------------------------------------------------------
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
//--------------------------------------------------------------
//sdcard
//--------------------------------------------------------------
  // open the file. note that only one file can be open at a time,
  mainLoop();
  digitalWrite(infoCapturingPin, HIGH);
  digitalWrite(sucessPin, HIGH);
  tone(BuzzerPin, 4000);
  delay(60000);
  noTone(BuzzerPin);
  Serial.println("done");
  for (int m = 200; m<=1000;m=+500){
    
  }
  //--------------------------------------------------------------
}
static void mainLoop(){
  //Main Loop
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  delay(3000);
  digitalWrite(initializingPin, HIGH);
  digitalWrite(infoCapturingPin, HIGH);
  delay(launchInterval);
  digitalWrite(initializingPin, LOW);
  digitalWrite(infoCapturingPin, LOW);
  for (x = 1; x<=10;x++){
    myFile = SD.open("MYFILE.txt", FILE_WRITE);
    if (myFile){
      mainText();
      myFile.close();
    }
    else{
      // if the file didn't open, print an error:
      Serial.println("no file");
      while (true){
        tone(BuzzerPin, 1000);
        digitalWrite(failedPin, HIGH);
        delay(1000);
        digitalWrite(failedPin, LOW);
        delay(1000);
      }
    }
  }
}
static void mainText(){
    sensors_event_t temp_event, pressure_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);

    // sensors_event_t a, g, temp;
    // mpu.getEvent(&a, &g, &temp);
    temperature = temp_event.temperature;
    myFile.print("Test #");
    myFile.print(x);
    myFile.println(": ");
    digitalWrite(infoCapturingPin, HIGH);
    myFile.println("");
    myFile.print("BMP Temperature: ");
    myFile.print(temperature);
    myFile.println(" °C");

    myFile.print("Pressure = ");
    myFile.print(pressure_event.pressure);
    myFile.println(" hPa");
    float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
    myFile.print("Parts per Million (Air Quality Measurement): ");
    myFile.print(correctedPPM);
    myFile.println(" ppm");
    delay(otherInterval);
    digitalWrite(infoCapturingPin, LOW);
    delay(otherInterval);
    myFile.println();
    myFile.println();
    myFile.println();
}

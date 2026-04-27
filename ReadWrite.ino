//CanSat Design Challenge 
//Group Members: Bhagat, Mackenzie, Malin
const int sucessPin = 4; //green
const int failedPin = 5; // red
const int infoCapturingPin = 3; // blue
const int initializingPin = 2; //white
const int BuzzerPin = 6; //buzzer

int x;

//SD Card Pin Configurations:
// CS SD Pin: Digital Pin 10
//SCK SD Pin: Digital Pin 13
//MOSI SD Pin: Digital Pin 11
//MOSO SD Pin: Digital Pin 12
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
//essential variable declaration and initialization
File myFile;
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
Adafruit_MPU6050 mpu;

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
  while (!Serial){
    delay(5000);
  }

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
  delay(2000);
  if (!SD.begin(10)) {
    Serial.println("IF");
    digitalWrite(initializingPin, LOW);
    digitalWrite(failedPin, HIGH);
    while (!SD.begin(10)){
      tone(BuzzerPin, 1000);
      delay(1000);
      if (SD.begin(10)){
        break;
      }
    }
    noTone(BuzzerPin);
    digitalWrite(failedPin, LOW);
    digitalWrite(initializingPin, HIGH);
    delay(2000);
  }
  digitalWrite(initializingPin, LOW);
  Serial.println("ID");
  digitalWrite(sucessPin, HIGH);
  delay(5000);
  digitalWrite(sucessPin, LOW);
  //--------------------------------------------------------------
  //BMP280 Sensor
  //--------------------------------------------------------------
    unsigned status;
    status = bmp.begin();
    digitalWrite(initializingPin, HIGH);
    delay(2000);
    if (!status) {
      Serial.println("No BMP280");
      digitalWrite(initializingPin, LOW);
      digitalWrite(failedPin, HIGH);
      while (!bmp.begin()){
        tone(BuzzerPin, 1000);
        delay(1000);
        status = bmp.begin();
        if (status){
          break;
        }
      }
      noTone(BuzzerPin);
      digitalWrite(failedPin, LOW);
      digitalWrite(initializingPin, HIGH);
      delay(2000);
    }
    digitalWrite(initializingPin, LOW);
    Serial.println("bmp_setupSucess");
    digitalWrite(sucessPin, HIGH);
    delay(5000);
    digitalWrite(sucessPin, LOW);
  //--------------------------------------------------------------
  //MPU6050 Sensor
  //--------------------------------------------------------------
  unsigned status2;
    status2 = mpu.begin();
    digitalWrite(initializingPin, HIGH);
    delay(2000);
    if (!status2) {
      Serial.println("No MPU6050");
      digitalWrite(initializingPin, LOW);
      digitalWrite(failedPin, HIGH);
      while (!mpu.begin()){
        tone(BuzzerPin, 1000);
        delay(1000);
        status2 = mpu.begin();
        if (status2){
          break;
        }
      }
      noTone(BuzzerPin);
      digitalWrite(failedPin, LOW);
      digitalWrite(initializingPin, HIGH);
      delay(2000);
    }
    digitalWrite(initializingPin, LOW);
    Serial.println("mpu_setupSucess");
    digitalWrite(sucessPin, HIGH);
    delay(5000);
    digitalWrite(sucessPin, LOW);
  //--------------------------------------------------------------
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
//mpu6050
//--------------------------------------------------------------
mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
mpu.setGyroRange(MPU6050_RANGE_500_DEG);
mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//--------------------------------------------------------------
//sdcard
//--------------------------------------------------------------
  // open the file. note that only one file can be open at a time,
  mainLoop();
  tone(BuzzerPin, 4000);
  delay(5000);
  noTone(BuzzerPin);
  digitalWrite(infoCapturingPin, HIGH);
  digitalWrite(sucessPin, HIGH);
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
  delay(10000);
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

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    myFile.print("Test #");
    myFile.print(x);
    myFile.println(": ");
    digitalWrite(infoCapturingPin, HIGH);
    myFile.println("");
    myFile.print("BMP Temperature: ");
    myFile.print(temp_event.temperature);
    myFile.println(" *C");

    myFile.print("Pressure = ");
    myFile.print(pressure_event.pressure);
    myFile.println(" hPa");
    myFile.print("Acceleration X: ");
    myFile.print(a.acceleration.x);
    myFile.print(", Y: ");
    myFile.print(a.acceleration.y);
    myFile.print(", Z: ");
    myFile.print(a.acceleration.z);
    myFile.println(" m/s^2");

    myFile.print("Rotation X: ");
    myFile.print(g.gyro.x);
    myFile.print(", Y: ");
    myFile.print(g.gyro.y);
    myFile.print(", Z: ");
    myFile.print(g.gyro.z);
    myFile.println(" rad/s");

    myFile.print("MPU Temperature: ");
    myFile.print(temp.temperature);
    myFile.println(" degC");
    myFile.println();
    delay(2000);
    digitalWrite(infoCapturingPin, LOW);
    delay(2000);
    myFile.println();
    myFile.println();
    myFile.println();
}

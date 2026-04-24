const int sucessPin = 4; //green
const int failedPin = 5; // red
const int infoCapturingPin = 3; // blue
const int initializingPin = 2; //white
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SPI.h>
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
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //Serial Initialization
  //--------------------------------------------------------------
  // Open serial communications and wait for port to open:
  Serial.println("5");
  delay(1000);
  Serial.println("4");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
  Serial.println("IPB");
  Serial.begin(115200);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial){
    delay(100);
  }
//--------------------------------------------------------------
//SD Card
//--------------------------------------------------------------
  Serial.print("ISC");
  digitalWrite(initializingPin, HIGH);
  delay(100);
  if (!SD.begin(10)) {
    Serial.println("IF");
    digitalWrite(initializingPin, LOW);
    digitalWrite(failedPin, HIGH);
    while (!SD.begin(10)){

    }
    digitalWrite(failedPin, LOW);
    digitalWrite(initializingPin, HIGH);
    delay(100);
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
    delay(100);
    if (!status) {
      Serial.println("No BMP280");
      digitalWrite(initializingPin, LOW);
      digitalWrite(failedPin, HIGH);
      while (!bmp.begin()){
        delay(100);
        status = bmp.begin();
      }
      digitalWrite(failedPin, LOW);
      digitalWrite(initializingPin, HIGH);
      delay(100);
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
    delay(100);
    if (!status2) {
      Serial.println("No MPU6050");
      digitalWrite(initializingPin, LOW);
      digitalWrite(failedPin, HIGH);
      while (!mpu.begin()){
        delay(100);
        status2 = mpu.begin();
      }
      digitalWrite(failedPin, LOW);
      digitalWrite(initializingPin, HIGH);
      delay(100);
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

  bmp_temp->printSensorDetails();
//--------------------------------------------------------------
//mpu6050
//--------------------------------------------------------------
mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
//--------------------------------------------------------------
//sdcard
//--------------------------------------------------------------
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("MyFile.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    mainLoop();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("no file");
    while (true){
      digitalWrite(failedPin, HIGH);
      delay(1000);
      digitalWrite(failedPin, LOW);
      delay(1000);
    }
  }
  //--------------------------------------------------------------
}
static void mainLoop(){
  //Main Loop
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  int x;
  for (x = 1; x<=10;x++){
    if (myFile){
      myFile = SD.open("MyFile.txt", FILE_WRITE);
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
      myFile.println("BMP Temperature: ");
      myFile.print(temp_event.temperature);
      myFile.println(" *C");

      myFile.println("Pressure = ");
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
      digitalWrite(infoCapturingPin, LOW);
      digitalWrite(sucessPin, HIGH);
      delay(2000);
      digitalWrite(sucessPin, LOW);
      myFile.print("Test #");
      myFile.print(x);
      myFile.println(" completed");
      myFile.println();
      myFile.println();
      Serial.println("done");
      myFile.close();
    }
  }
}

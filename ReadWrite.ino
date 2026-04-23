const int sucessPin = 4; //green
const int failedPin = 5; // red
const int infoCapturingPin = 10; // blue
const int initializingPin = 4; //white
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
  Initialization
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //Serial Initialization
  //--------------------------------------------------------------
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial){
    delay(1000);
  }
//--------------------------------------------------------------
//SD Card
//--------------------------------------------------------------
  Serial.print("Initializing SD card...");
  digitalWrite(initializingPin, HIGH);
  delay(100);
  if (!SD.begin(10)) {
    Serial.println("Initialization failed");
    digitalWrite(initializingPin, LOW);
    digitalWrite(failedPin, HIGH);
    while (!SD.begin(10)){

    }
    digitalWrite(failedPin, LOW);
    digitalWrite(initializingPin, HIGH);
    delay(100);
  }
  digitalWrite(initializingPin, LOW);
  Serial.println("initialization done.");
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
      Serial.println("Could not find a valid BMP280 sensor, check wiring or try a different adress");
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
      Serial.println("Failed to find MPU6050 chip");
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
  Serial.print("Accelerometer range set to: ");
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
  Serial.print("Gyro range set to: ");
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
  Serial.print("Filter bandwidth set to: ");
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
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
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
  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  int x;
  for (x = 1; x<=10;x++){
    if (myFile){
      myFile.println("Test #"+x+": ");
      myFile.println("");
      myFile.println("Temperature: ");
      myFile.print(temp_event.temperature);
      myFile.println(" *C");
      myFile.println("Pressure = ");
      myFile.print(pressure_event.pressure);
      myFile.println(" hPa");
      myFile.println();
      myFile.println();
      myFile.println();
      myFile.println();
    }
  }
}

/*
  Project: Transmitter for Flight parameter project.
  Project Description:
  This sketch sends readings read from various sensors to receiver using LoRa
  This sketch involves GPS , BMP , MPU sensors.
  Many serial print lines have been commented out to avoid extra space being used up in the program memory. They have still been kept since they can help in debugging in future.


  Author: STEMVentor Educonsulting
  
  This code is copyrighted. Please do not reuse or share for any purpose other than for learning with subscribed STEMVentor programs.
  This code is for educational purposes only and is not for production use.
*/

// LIBRARIES
//for I2C communication 
#include "Wire.h" 
//to read value from BMP280 sensor
//one change has been made to the .h file that the default address of the bmp is set to 0x76 since the BMP being used in this project has that as its address.
//address of the BMP was found with the help of I2C address finder code
#include <Adafruit_Sensor.h> 
#include <Adafruit_BMP280.h>
//to read values from the MPU6050 sensor
#include "MPU6050_6Axis_MotionApps20.h"
//to read values from the NEO7M GPS sensor 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//to transmit data using sx1278 LoRa module
#include <SPI.h>
#include <LoRa.h>

// PIN DEFINITIONS
//No need to physically connect these pins in this use case but need to be defined for the LoRa to transmit

#define nss 10 //LoRa module NSS pin
#define rst 9 //LoRa module RST pin
#define dio0 2 //LoRa module dio0 pin

/* INITIALIZE OBJECTS
 * Libraries usually follow an object-oriented approach that requires
 * an instance of the class to call its methods.
 */
TinyGPSPlus gps;
Adafruit_BMP280 bmp;
MPU6050 mpu;

//to configure tx abnd rx pins for the GPS
static const int RXPin = 5, TXPin = 6;
SoftwareSerial ss(RXPin, TXPin);
// SoftwareSerial "variable_name"(GPS_TXPin, GPS_RXPin);
//GPS module baud rate , usually 9600
static const uint32_t GPSBaud = 9600;

// GLOBAL VARIABLES
String BMP_data ;
String MPU_data ;
String GPS_data ; 
String outgoing ;

//GLOBAL VARIABLE FOR MPU READINGS

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


//individual setup functions for each sensor and module
void bmp_setup()
{
    if (bmp.begin()) //starts the bmp 
    {
        // Serial.println("BMP180 initialized successfully.");
    }
    else
    {
        Serial.println("BMP180 initialization failed.");
    }
}

void mpu_setup()
{

  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // verify connection
  // Serial.println(F("Testing device connections..."));
  // Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  // Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // // make sure it worked (returns 0 if so)
  if (devStatus == 0) 
  {
      // Calibration Time: generate offsets and calibrate our MPU6050
      mpu.CalibrateAccel(6);
      mpu.CalibrateGyro(6);
      mpu.PrintActiveOffsets();
      // turn on the DMP, now that it's ready
      Serial.println(F("Enabling DMP..."));
      mpu.setDMPEnabled(true);

      //set our DMP Ready flag so the main loop() function knows it's okay to use it
      dmpReady = true;
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      Serial.print(F("DMP Initialization failed (code "));
  }



}

void lora_setup()
{
    // Serial.println("Starting LoRa Communication");
    LoRa.setPins(nss, rst, dio0); //These pins need not be connected but have to be defined

    if (!LoRa.begin(433E6)) { //starts LoRa at 433MHz frequency
      Serial.println("Starting LoRa failed!");
      while (10); // runs only once
    }
    // LoRa.setSyncWord(0xFF); // isolates network , the receiver also needs to have the same syncword. More different the syncwords  , better the isolation.
}

void gps_setup()
{
  ss.begin(GPSBaud); //setup GPS communication channel
}


/*
individual read functions for each sensor
Nine data variables are being measured and each of those has been seperated by a , so as to make indentifaction easier at the receiver.
order of transmitting the data:-
"Yaw,Pitch,Roll,Altitude,Pressure(commented for now),Temperature,Latitude,Longitude,Speed"
Each sensors data is being sent separately , so that there are less errors and identification is easier at the receiver end
Each data packet has a letter as a prefix indicating which sensors data is it
Index:-
a-MPU
b-BMP
c-GPS
*/

void readMPU6050() //Reads the Yaw , Pitch and Roll from the MPU using the library functions
{
    // if programming failed, don't try to do anything
    if (!dmpReady) return;
    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            // Serial.print("ypr\t");
            // Serial.print(ypr[0] * 180/M_PI);
            // Serial.print("\t");
            // Serial.print(ypr[1] * 180/M_PI);
            // Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);

            MPU_data = "a"; //identifier for MPU data
            MPU_data += ",";
            MPU_data += String(ypr[0] * 180/M_PI);
            MPU_data += ",";
            MPU_data += String(ypr[1] * 180/M_PI);
            MPU_data += ",";
            MPU_data += String(ypr[2] * 180/M_PI);
            // Serial.println(MPU_data);

    }
}
void readBMP180() //Reads the Temperature , Altitude and Pressure(commented for now ) , from the BMP 280 sensor using the library
{
    uint8_t temperature = bmp.readTemperature(); //gets temperature in degree celcius
    // uint8_t pressure = bmp.readPressure(); //gets pressure
    uint8_t altitude = bmp.readAltitude(1004); //gets altitude in metres  , 1004 is the sea level pressure of Mumbai
    // altitude = altitude - 35; //35m is the calibration factor

    // Serial.print("BMP280 - Altitude: ");
    Serial.println(altitude);
    // Serial.print(" [m] \tTemperature: ");
    // // Serial.print(pressure / 100.0);
    // // Serial.print(" [hPa] \tTemperature: ");
    // Serial.print(temperature);
    // Serial.println(" [°C]");

    BMP_data ="b"; //identifier for BMP data
    BMP_data +=",";
    BMP_data +=String(altitude);
    BMP_data +=",";
    // BMP_data +=String(pressure);
    // BMP_data +=",";
    BMP_data +=String(temperature);
    


}

void readGPS() //reads the latitude , longitude and the speed from the GPS sensor with the help of library functions
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    if (gps.encode(ss.read()))
    {
      // Serial.print(F("Location: ")); 
      if (gps.location.isValid())
      {
        delay(200);
        // Serial.print(gps.location.lat(), 6); //gets latitude upto 6 decimal places
        // Serial.print(F(","));
        // Serial.print(gps.location.lng(), 6); //gets longitude upto 6 decimal places

        GPS_data = "c"; //identifier for GPS data
        GPS_data+= ",";
        GPS_data += gps.location.lat();
        GPS_data+= ",";
        GPS_data += gps.location.lng();

        // Serial.print(F("Speed: ")); 
        if (gps.speed.isValid())
        {
          delay(200);
          // Serial.println(gps.speed.kmph(),4); //gets speed upto 4 decimal places
          GPS_data+= ",";
          GPS_data += gps.speed.kmph();
        }
        // else
        // {
        //   // Serial.println(F("INVALID"));
        // }
      }
      // else
      // {
      //   // Serial.println(F("INVALID"));
      // }
      
    }
  }


  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    // Serial.println(F("No GPS detected: check wiring."));
  }
}

void sendMessage(String outgoing) //sends the data as a string over LoRa
{
  // Serial.println("Sending packet id " + outgoing.substring(0,1)) ; 
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  Serial.println("Sent packet id " + outgoing.substring(0,1));               
}

//The Main Setup function which sets up all the modules
void setup() 
{
    Serial.begin(9600); 
    while(!Serial); //waits for serial comm to start
    Wire.begin(); //to enable I2C serial communication which is common for MPU and BMP.
    bmp_setup();
    mpu_setup();
    gps_setup();
    lora_setup();
    Serial.println("All setup done");
}

//The Main loop fucntion which calls all the read functions of the sensor and then sends the data via LoRa.
void loop() 
{
    delay(100);
    readMPU6050();
    sendMessage(MPU_data);
    delay(100);
    readBMP180();
    sendMessage(BMP_data);
    delay(100);
    readGPS();
    sendMessage(GPS_data);
}



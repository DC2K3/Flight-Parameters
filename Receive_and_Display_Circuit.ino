/*
  Project: Receiver for Flight tracker
  Project Description:
  This sketch sends receiving data from the transmitter via LoRa
  This sketch involves readings from GPS , BMP , MPU sensors and is also responsible to send it to processing app.

  Author: STEMVentor Educonsulting
  
  This code is copyrighted. Please do not reuse or share for any purpose other than for learning with subscribed STEMVentor programs.
  This code is for educational purposes only and is not for production use.
*/

// LIBRARIES
//to transmit data using sx1278 LoRa module
#include <SPI.h>
#include <LoRa.h>

#include <Wire.h> //used for communicating with I2C devices

// install the adafruit ssd1306 library
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>

//for bluetooth
#include <BluetoothSerial.h>



// PIN DEFINITIONS
// No need to physically connect these pins in this use case but need to be defined for the LoRa to transmit
#define ss 15 //LoRa module NSS pin
#define rst 16 //LoRa module RST pin
#define dio0 4 //LoRa module  DIO0 pin


// GLOBAL VARIABLES
String Lat;
String Long;
String Temperature;
String Altitude;
String Speed;
String data = ""; //for the incoming data

// Define the display size
const byte SCREEN_WIDTH = 128;// OLED display width, in pixels
const byte SCREEN_HEIGHT = 64;// OLED display height, in pixels
bool check = false;
bool data1_check = false;
bool data2_check = false;

// Timing variables
unsigned long lastUpdateTime = 0; // Tracks the last update time
const unsigned long updateInterval = 5000; // 5 seconds interval

/* INITIALIZE OBJECTS
 * Libraries usually follow an object-oriented approach that requires
 * an instance of the class to call its methods.
 */
/* 
 * All I2C components have an address, the default is usually 0x27
 * If that doesn't work, see this:https://playground.arduino.cc/Main/I2cScanner/
 * The init statement accepts the address and the number of columns and rows.
 */

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //-1 indicates that the library should not use a hardware reset pin
//bluetooth object
BluetoothSerial SerialBT;

//Function to receive and serial print the incoming data packet
//Serial printing the data will make it available/readable for the processing app via the comm port. 
void onReceive(int packetSize) 
{
   
  if (packetSize == 0) return;          // if there's no packet, return
  data = "";

  while (LoRa.available()) {
    data += (char)LoRa.read();
  }
  Serial.println(data);
  SerialBT.println(data); //send via bluetooth as well

}


//to display data every 5 seconds onto the OLED
void seggregateData()
{
  char dataType = data[0];
  data = data.substring(2);  // Remove the identifier and the comma
  
  // Split the remaining data by commas
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);

  switch (dataType) {
    case 'a': {
      float altitude_value = data.substring(0, firstComma).toFloat();
      float temp = data.substring(firstComma + 1).toFloat();
      Altitude = "Altitude:";
      Altitude+= String(altitude_value);
      Altitude+= " m";
      Temperature = " Temperature:";
      Temperature+= String(temp);
      Temperature+=" deg C";
      data1_check = true;
      break;
    }
    case 'b': {
      float latitude = data.substring(0, firstComma).toFloat();
      float longitude = data.substring(firstComma + 1, secondComma).toFloat();
      float speed_value = data.substring(secondComma + 1).toFloat();
      Lat = "Lat:";
      Lat+= String(latitude);
      Long= " Long:";
      Long+= String(longitude);
      Speed= " Speed:";
      Speed+= String(speed_value);
      Speed+=" Km/hr";
      data2_check = true;
      break;
    }
    default:
      Serial.println("Unknown data type");
      break;
  }
  if (data1_check == true && data2_check == true )
  {
    check = true;
  }
}

//setup function for OLED
void OLED_setup() {
  // Initializing wire
  Wire.begin();
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }

  oled.clearDisplay(); // clear display
  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
}

void setupBluetooth() {
  SerialBT.begin("Stemventor_project"); // Set the name for the Bluetooth device
}

//setup function for LoRa
void lora_setup()
{
    Serial.println("Starting LoRa Communication");
    LoRa.setPins(ss, rst, dio0); //These pins need not be connected but have to be defined

    if (!LoRa.begin(433E6)) { //starts LoRa at 433MHz frequency
      Serial.println("Starting LoRa failed!");
      while (1); // runs only once
    }
    // LoRa.setSyncWord(0xFF); // isolates network , the receiver also needs to have the same syncword. More different the syncwords  , better the isolation.
}

void setup() 
{
  // Start the serial communication with baud rate suitable for your components.
  Serial.begin(9600);
  while (!Serial);
  lora_setup();
  setupBluetooth();
  OLED_setup();

}

void loop() 
{ 

  onReceive(LoRa.parsePacket()); //constantly listen for incoming data

  unsigned long currentTime = millis(); //checks if 5 seconds have passed by since the last update

  if (currentTime - lastUpdateTime >= updateInterval) 
  {
    // Update the OLED with the data
    seggregateData();


    if (check == true)
    {
      oled.clearDisplay(); // clear display
      oled.setCursor(0, 0);       // set position to display (x,y)
      oled.println(Lat); // set text
      oled.setCursor(0, 12);       // set position to display (x,y)
      oled.println(Long); // set text
      oled.setCursor(0, 24);       // set position to display (x,y)
      oled.println(Temperature); // set text
      oled.setCursor(0, 36);       // set position to display (x,y)
      oled.println(Altitude); // set text
      oled.setCursor(0, 48);       // set position to display (x,y)
      oled.println(Speed); // set text
      oled.display();              // display on OLED
      data1_check = false;
      data2_check = false;
      check = false;
      lastUpdateTime = currentTime; // Update the last update time
    }
  }

}



  




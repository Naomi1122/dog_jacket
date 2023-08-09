
#define SDA_PIN D2
#define SCL_PIN D1
#define USE_NODE_MCU_BOARD

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM6DSO32.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ezTime.h>
#define FIREBASE_PROJECT_ID "doggydynamic"
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>


// Date and time
Timezone GB;

//#define DATABASE_URL "doggydynamic.europe-west1.firebasedatabase.app"

#define API_KEY "AIzaSyDZAiahjyaU4il1wpxvhVLmQE6pYWpkCEI"
// Define Firebase Data objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;
// Replace with authorized email and corresponding password
#define USER_EMAIL "ucfnjc0@ucl.ac.uk"
#define USER_PASSWORD "WHcjq0815"

#include "DFRobot_GDL.h"
#include "DFRobot_UI.h"
/*M0*/
#define TFT_DC  D4
#define TFT_CS  D8
#define TFT_RST D3


/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 * @param bl  Screen backlight pin
 */
DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

/**
 * @brief Construct a function
 * @param gdl Screen object
 * @param touch Touch object
 */
DFRobot_UI ui(&screen, NULL);

float voltage;
int bat_percentage;
int analogInPin  = A0;    // Analog input pin
int sensorValue;
float calibration = 0.36; // Check Battery voltage using multimeter & add/subtract the value 

//TinyGPSPlus gps;
//SoftwareSerial SerialGPS(32, 15); 


float LSM6_AX;
float LSM6_AY;
float LSM6_AZ;
float LSM6_GX;
float LSM6_GY;
float LSM6_GZ;
float LSM6_temp;
float latitude , longitude;
const int buttonPin = 10;


// Wifi and MQTT
#include "secret.h" 

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
/*
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

//ESP8266WebServer server(80);
const char* mqtt_server = "mqtt.cetools.org";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
*/
//Adafruit_MMA8451 mma = Adafruit_MMA8451();
//Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM6DSO32 dso32;


void setup(void) {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  

  ui.begin();
  ui.setTheme(DFRobot_UI::MODERN);

  //Create a progress bar control
  DFRobot_UI::sBar_t &bar1 = ui.creatBar();
  /** User-defined progress bar parameters */
  bar1.setStyle(DFRobot_UI::COLUMN);
  bar1.fgColor = COLOR_RGB565_GREEN;
  bar1.setCallback(barCallback1);
  ui.draw(&bar1,/*x=*/33,/*y=*/screen.height()/5*3);
  ui.drawString(/*x=*/33,/*y=*/screen.height()/5*2.5,"Battery Capacity",COLOR_RGB565_WHITE,ui.bgColor,/*fontsize =*/2,/*Invert=*/0);


  configF.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback;
  configF.max_token_generation_retry = 5;
  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);

  if (!dso32.begin_I2C()) {

    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DSO32 Found!");

  dso32.setAccelRange(LSM6DSO32_ACCEL_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (dso32.getAccelRange()) {
  case LSM6DSO32_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case LSM6DSO32_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case LSM6DSO32_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  case LSM6DSO32_ACCEL_RANGE_32_G:
    Serial.println("+-32G");
    break;
  }

  // dso32.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("Gyro range set to: ");
  switch (dso32.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    break; // unsupported range for the DSO32
  }

  // dso32.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Accelerometer data rate set to: ");
  switch (dso32.getAccelDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  // dso32.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Gyro data rate set to: ");
  switch (dso32.getGyroDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  //SerialGPS.begin(115200);

  // run initialisation functions
  startWifi();
  //startWebserver();


  // start MQTT server
  //client.setServer(mqtt_server, 1884);
  //client.setCallback(callback);
  syncDate();


}

void loop() {

  if (digitalRead(buttonPin) == HIGH) {
    Serial.print("deep sleep mode");
    ESP.deepSleep(0);
  } else {
      //server.handleClient();
      //sendMQTT();
      //client.loop();
      //screenTime();
      batteryCapacity();
      ui.refresh();
      firebasepush();
      screenTime();
  }
}

void startWifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
void sendMQTT() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  dso32.getEvent(&accel, &gyro, &temp);

  LSM6_temp = temp.temperature;
  snprintf (msg, 50, "%.2f", LSM6_temp);
  Serial.print("Temperature ");
  Serial.print(msg);
  Serial.println(" deg C");
  client.publish("student/ucfnjc0/accel/lsm6D_temp", msg);

  LSM6_AX = accel.acceleration.x;
  snprintf (msg, 50, "%.2f", LSM6_AX);
  Serial.print("Publish message for lsm6D AX: ");
  Serial.print(msg);
  Serial.println(" m/s^2");
  client.publish("student/ucfnjc0/accel/lsm6D_acceleration_x", msg);

  LSM6_AY = accel.acceleration.y;
  snprintf (msg, 50, "%.2f", LSM6_AY);
  Serial.print("Publish message for lsm6D AY: ");
  Serial.print(msg);
  Serial.println(" m/s^2");
  client.publish("student/ucfnjc0/accel/lsm6D_acceleration_y", msg);

  LSM6_AZ = accel.acceleration.z;
  snprintf (msg, 50, "%.2f", LSM6_AZ);
  Serial.print("Publish message for lsm6D AZ: ");
  Serial.print(msg);
  Serial.println(" m/s^2");
  client.publish("student/ucfnjc0/accel/lsm6D_acceleration_z", msg);

  LSM6_GX = gyro.gyro.x;
  snprintf (msg, 50, "%.2f", LSM6_GX);
  Serial.print("Publish message for lsm6D GX: ");
  Serial.print(msg);
  Serial.println(" radians/s");
  client.publish("student/ucfnjc0/accel/lsm6D_gyro_x", msg);

  LSM6_GY = gyro.gyro.y;
  snprintf (msg, 50, "%.2f", LSM6_GY);
  Serial.print("Publish message for lsm6D GY: ");
  Serial.print(msg);
  Serial.println(" radians/s");
  client.publish("student/ucfnjc0/accel/lsm6D_gyro_y", msg);

  LSM6_GZ = gyro.gyro.z;
  snprintf (msg, 50, "%.2f", LSM6_GZ);
  Serial.print("Publish message for lsm6D GZ: ");
  Serial.print(msg);
  Serial.println(" radians/s");
  client.publish("student/ucfnjc0/accel/lsm6D_gyro_z", msg);
/*
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        snprintf (msg, 50, "%.6f", latitude);
        Serial.print("Publish message for GPS: ");
        Serial.print(msg);
        client.publish("student/ucfnjc0/gps/latitude", msg);
        longitude = gps.location.lng();
        snprintf (msg, 50, "%.6f", longitude);
        Serial.print("Publish message for GPS: ");
        Serial.print(msg);
        client.publish("student/ucfnjc0/gps/longitude", msg);
      }
    }  

  

  delay(10000);
}
*/
void barCallback1(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 in each time, it enters the callback function.
  delay(50);
  obj.setValue(bat_percentage);
	//if(value1 < 100) value1++;
}

void screenTime(){
  while(1){
    screen.setTextColor(0x30FF);
    screen.setTextSize(3);
    screen.setCursor(30, 30);
    String Date = String(GB.dateTime("Y-m-d"));
    screen.println(Date);
    screen.setCursor(30, 60);
    String Time = String(GB.dateTime("H:i"));
    screen.println(Time);
    delay(10);
  }
  
}
void syncDate() {
  // get real date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());
}

void batteryCapacity(){
  sensorValue = analogRead(analogInPin);
  voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); //multiply by two as voltage divider network is 100K & 100K Resistor
  bat_percentage = mapfloat(voltage, 4.2, 2.8, 0, 100); //2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage
  if (bat_percentage >= 100)
  {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0)
  {
    bat_percentage = 1;
  }
  
  
  //Print data on serial monitor

  Serial.print("Analog Value = ");
  Serial.println(sensorValue);
  Serial.print("Output Voltage = ");
  Serial.println(voltage);
  Serial.print("Battery Percentage = ");
  Serial.print(bat_percentage);
  Serial.println(" %");
  Serial.println();
  Serial.println("****************************");
  Serial.println();
  delay(1000);
 
       if (bat_percentage <=30)
    {
      Serial.println("Battery level below 30%, Charge battery on time");
      delay(500);
    }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/*
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect with clientID, username and password
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("student/ucfnjc0/accel/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
*/
void firebasepush(void){

  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 1000U) {
    printTimepoint = millis();
    

    String documentPath = String(GB.dateTime("Y-m-d")) + "/" + String(GB.dateTime("H:i:s"));
    FirebaseJson content;

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    dso32.getEvent(&accel, &gyro, &temp);
    LSM6_temp = temp.temperature;
    content.set("fields/temp/doubleValue", String(LSM6_temp).c_str());
    LSM6_AX = accel.acceleration.x;
    content.set("fields/AX/doubleValue", String(LSM6_AX).c_str());
    LSM6_AY = accel.acceleration.y;
    content.set("fields/AY/doubleValue", String(LSM6_AY).c_str());
    LSM6_AZ = accel.acceleration.z;
    content.set("fields/AZ/doubleValue", String(LSM6_AZ).c_str());
    LSM6_GX = gyro.gyro.x;
    content.set("fields/GX/doubleValue", String(LSM6_GX).c_str());
    LSM6_GY = gyro.gyro.y;
    content.set("fields/GY/doubleValue", String(LSM6_GY).c_str());
    LSM6_GZ = gyro.gyro.z;
    content.set("fields/GZ/doubleValue", String(LSM6_GZ).c_str());
    content.set("fields/battery_Capacity/doubleValue", String(bat_percentage).c_str());
    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "battery_Capacity,temp,AX,AY,AZ,GX,GY,GZ")) {
      // Serial.print("ok\n%s\n\n", fbdo.payload().c_str());
      return;
    } else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())) {
      // Serial.print("ok\n%s\n\n", fbdo.payload().c_str());
      return;
    } else {
      Serial.println(fbdo.errorReason());
    }




    delay(500);  // Wait for 1 second before publishing again
  }
}

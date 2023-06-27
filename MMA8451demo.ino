

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM6DSO32.h>


float X;
float Y;
float Z;
float MMA_AX;
float MMA_AY;
float MMA_AZ;
float LSM3_AX;
float LSM3_AY;
float LSM3_AZ;
float LSM6_AX;
float LSM6_AY;
float LSM6_AZ;
float LSM6_GX;
float LSM6_GY;
float LSM6_GZ;
float LSM6_temp;

// Wifi and MQTT
#include "secret.h" 

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

ESP8266WebServer server(80);
const char* mqtt_server = "mqtt.cetools.org";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

Adafruit_MMA8451 mma = Adafruit_MMA8451();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM6DSO32 dso32;

void setup(void) {
  Serial.begin(115200);
  
  Serial.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");

  #ifndef ESP8266
    while (!Serial)
      ; // will pause Zero, Leonardo, etc until serial console opens
  #endif
    Serial.begin(9600);
    Serial.println("Accelerometer Test");
    Serial.println("");

    /* Initialise the sensor */
    if (!accel.begin()) {
      /* There was a problem detecting the ADXL345 ... check your connections */
      Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
      while (1)
        ;
    }

    /* Display some basic information on this sensor */
    displaySensorDetails();

    accel.setRange(LSM303_RANGE_4G);
    Serial.print("Range set to: ");
    lsm303_accel_range_t new_range = accel.getRange();
    switch (new_range) {
    case LSM303_RANGE_2G:
      Serial.println("+- 2G");
      break;
    case LSM303_RANGE_4G:
      Serial.println("+- 4G");
      break;
    case LSM303_RANGE_8G:
      Serial.println("+- 8G");
      break;
    case LSM303_RANGE_16G:
      Serial.println("+- 16G");
      break;
    }

    accel.setMode(LSM303_MODE_NORMAL);
    Serial.print("Mode set to: ");
    lsm303_accel_mode_t new_mode = accel.getMode();
    switch (new_mode) {
    case LSM303_MODE_NORMAL:
      Serial.println("Normal");
      break;
    case LSM303_MODE_LOW_POWER:
      Serial.println("Low Power");
      break;
    case LSM303_MODE_HIGH_RESOLUTION:
      Serial.println("High Resolution");
      break;
    }
    /*
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit LSM6DSO32 test!");
*/
  if (!dso32.begin_I2C()) {
    // if (!dso32.begin_SPI(LSM_CS)) {
    // if (!dso32.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    // Serial.println("Failed to find LSM6DSO32 chip");
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


  // run initialisation functions
  startWifi();
  startWebserver();


  // start MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);

}

void loop() {
  server.handleClient();
  sendMQTT();
  client.loop();
  
  
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

void sendMQTT() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  mma.read();

  X = mma.x; // Gets the values of the x
  snprintf (msg, 50, "%.2f", X);
  Serial.print("Publish message for x: ");
  Serial.println(msg);
  client.publish("student/ucfnjc0/accel/x", msg);

  Y = mma.y; // Gets the values of the y
  snprintf (msg, 50, "%.2f", Y);
  Serial.print("Publish message for y: ");
  Serial.println(msg);
  client.publish("student/ucfnjc0/accel/y", msg);

  Z = mma.z; // Gets the values of the z
  snprintf (msg, 50, "%.2f", Z);
  Serial.print("Publish message for z: ");
  Serial.println(msg);
  client.publish("student/ucfnjc0/accel/z", msg);


  sensors_event_t event; 
  mma.getEvent(&event);


  MMA_AX = event.acceleration.x; // Gets the values of the acceleration x
  snprintf (msg, 50, "%.2f", MMA_AX);
  Serial.print("Publish message for AX: ");
  Serial.print(msg);
  Serial.println("m/s^2 ");
  client.publish("student/ucfnjc0/accel/mma_acceleration_x", msg);

  MMA_AY = event.acceleration.y; // Gets the values of the acceleration y
  snprintf (msg, 50, "%.2f", MMA_AY);
  Serial.print("Publish message for AY: ");
  Serial.print(msg);
  Serial.println("m/s^2 "); 
  client.publish("student/ucfnjc0/accel/mma_acceleration_y", msg);

  MMA_AZ = event.acceleration.z; // Gets the values of the acceleration z
  snprintf (msg, 50, "%.2f", MMA_AZ);
  Serial.print("Publish message for AZ: ");
  Serial.print(msg);
  Serial.println("m/s^2 ");
  client.publish("student/ucfnjc0/accel/mma_acceleration_z", msg);

 uint8_t o = mma.getOrientation();
  
  switch (o) {
    case MMA8451_PL_PUF: 
      Serial.println("Portrait Up Front");
      client.publish("student/ucfnjc0/accel/mma_direction", "Portrait Up Front");
      break;
    case MMA8451_PL_PUB: 
      Serial.println("Portrait Up Back");
      client.publish("student/ucfnjc0/accel/mma_direction", "Portrait Up Back");
      break;    
    case MMA8451_PL_PDF: 
      Serial.println("Portrait Down Front");
      client.publish("student/ucfnjc0/accel/mma_direction", "Portrait Down Front");
      break;
    case MMA8451_PL_PDB: 
      Serial.println("Portrait Down Back");
      client.publish("student/ucfnjc0/accel/mma_direction", "Portrait Down Back");
      break;
    case MMA8451_PL_LRF: 
      Serial.println("Landscape Right Front");
      client.publish("student/ucfnjc0/accel/mma_direction", "Landscape Right Front");
      break;
    case MMA8451_PL_LRB: 
      Serial.println("Landscape Right Back");
      client.publish("student/ucfnjc0/accel/mma_direction", "Landscape Right Back");
      break;
    case MMA8451_PL_LLF: 
      Serial.println("Landscape Left Front");
      client.publish("student/ucfnjc0/accel/mma_direction", "Landscape Left Front");
      break;
    case MMA8451_PL_LLB: 
      Serial.println("Landscape Left Back");
      client.publish("student/ucfnjc0/accel/mma_direction", "Landscape Left Back");
      break;
    }
  Serial.println();

  // sensors_event_t event;
  accel.getEvent(&event);

  LSM3_AX = event.acceleration.x; // 获取x轴加速度值
  snprintf (msg, 50, "%.2f", LSM3_AX);
  Serial.print("Publish message for lsm303 AX: ");
  Serial.print(msg);
  Serial.println(" m/s^2");
  client.publish("student/ucfnjc0/accel/lsm303_acceleration_x", msg);

  LSM3_AY = event.acceleration.y; // 获取y轴加速度值
  snprintf (msg, 50, "%.2f", LSM3_AY);
  Serial.print("Publish message for lsm303 AY: ");
  Serial.print(msg);
  Serial.println(" m/s^2");
  client.publish("student/ucfnjc0/accel/lsm303_acceleration_y", msg);

  LSM3_AZ = event.acceleration.z; // 获取z轴加速度值
  snprintf (msg, 50, "%.2f", LSM3_AZ);
  Serial.print("Publish message for lsm303 AZ: ");
  Serial.print(msg);
  Serial.println(" m/s^2");
  client.publish("student/ucfnjc0/accel/lsm303_acceleration_z", msg);

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  dso32.getEvent(&accel, &gyro, &temp);

  LSM6_temp = temp.temperature;
  snprintf (msg, 50, "%.2f", LSM3_AZ);
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

  delay(1000);
}

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

void startWebserver() {
  // when connected and IP address obtained start HTTP server  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");  
}

void handle_OnConnect() {
  X = mma.x; // Gets the values of the temperature
  Y= mma.y;
  Z = mma.z;
  server.send(200, "text/html", SendHTML(X, Y, Z));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat, float Humiditystat, int Moisturestat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 DHT22 Report</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 Huzzah DHT22 Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += (int)Temperaturestat;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Humiditystat;
  ptr += "%</p>";
  ptr += "<p>Moisture: ";
  ptr += Moisturestat;
  ptr += "</p>";


  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void displaySensorDetails(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" m/s^2");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" m/s^2");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
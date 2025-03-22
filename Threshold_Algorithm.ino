#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include <WiFi.h>

const char *WIFI_SSID = "Galaxy M02c141";
const char *WIFI_PWD = "thengavellam";

const char *MQTT_SERVER = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Adafruit_MPU6050 mpu;

const size_t BUFFER_SIZE = 100;
const char *BUFFER_FORMAT = "%1.3f";
char buffer[BUFFER_SIZE + 1];
String str="";

void CallbackMqtt(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void SetupMqtt()
{
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

}

/*void ConnectToMqtt()
{
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected())
  {
    char clientId[100] = "\0";
    sprintf(clientId, "ESP32Client-%04X", random(0xffff));
    Serial.println(clientId);

    if (mqttClient.connect(clientId))
    {
      Serial.println("Connected to MQTT broker.");
    }
    else
    {
      Serial.print("Failed to connect to MQTT, state: ");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}*/

void ConnectToWiFi()
{
  Serial.print("Connecting to WiFi ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PWD, 6);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nConnected to ");
  Serial.println(WIFI_SSID);
}

void SetupMpu6050()
{
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  

  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth())
  {
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
}

float anet,gnet,prevanet;

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup begin");

  ConnectToWiFi();
  //SetupMqtt();
  SetupMpu6050();


  Serial.println("Setup end");
}

void loop()
{
  /*if (!mqttClient.connected())
  {
    ConnectToMqtt();
  }
  mqttClient.loop();*/

  const float F12 = 10.8;  // 1.95Upper acceleration fall threshold (g)
  const float FT3 = 1.96;
  
  static uint64_t last_time;
  uint64_t now = millis();
  if (now - last_time > 1000)
  {
    // Get new sensor events with the readings.
    sensors_event_t a, g,temp;
    mpu.getEvent(&a, &g,&temp);

    float ax = a.acceleration.x;
    float ay = a.acceleration.y ;
    float az = a.acceleration.z ;
    
    float gx = g.gyro.x;
    float gy = g.gyro.y;
    float gz = g.gyro.z;

    anet=sqrt(ax*ax+ay*ay+az*az);
    gnet=sqrt(gx*gx+gy*gy+gz*gz);

    if(anet>F12 && gnet<FT3)
    {
          str="";
          Serial.println(anet);
          Serial.println(gnet);
          Serial.println("SENDING");
          Serial.println("SENDING");
          Serial.println("SENDING");
          Serial.println("SENDING");

          snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, (a.acceleration.x*2048)/9.81);
          Serial.print("Acceleration XX: ");
          Serial.print(buffer);
          str+=String(buffer)+",";

          snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, (a.acceleration.y*2048)/9.81);
          Serial.print(", YT: ");
          Serial.print(buffer);
          Serial.print(", ZZ: ");
          str+=String(buffer)+",";
 
          snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, (a.acceleration.z*2048)/9.81);
          Serial.print(buffer);
          Serial.println(" m/s^2");
          str+=String(buffer)+",";

          snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, g.gyro.x*16.4);
          Serial.print("Rotation XX: ");
          Serial.print(buffer);
          str+=String(buffer)+",";

          snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, g.gyro.y*16.4);
          Serial.print(", YY: ");
          Serial.print(buffer);
          str+=String(buffer)+",";

          snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, g.gyro.z*16.4);
          Serial.print(", ZZ: ");
          Serial.print(buffer);
          Serial.println(" rad/s");
          str+=String(buffer)+";";
          //mqttClient.publish("sensors/mpug12fd", str.c_str());
    }

    snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, a.acceleration.x);
    Serial.print("Acceleration X: ");
    Serial.print(buffer);

    snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, a.acceleration.y);
    Serial.print(", Y: ");
    Serial.print(buffer);
    Serial.print(", Z: ");

 
    snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, a.acceleration.z);
    Serial.print(buffer);
    Serial.println(" m/s^2");


    snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, g.gyro.x);
    Serial.print("Rotation X: ");
    Serial.print(buffer);


    snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, g.gyro.y);
    Serial.print(", Y: ");
    Serial.print(buffer);


    snprintf(buffer, BUFFER_SIZE, BUFFER_FORMAT, g.gyro.z);
    Serial.print(", Z: ");
    Serial.print(buffer);
    Serial.println(" rad/s");

    prevanet=anet;

    last_time = now;
  }
}
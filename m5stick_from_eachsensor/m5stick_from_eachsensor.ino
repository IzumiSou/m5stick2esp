

#include <M5Unified.h>
#include <Wire.h>
#include <SensirionI2CScd4x.h>
#include <M5_ENV.h>
#include <esp_now.h>
#include <WiFi.h>

// I2C
#define PCA9548A_ADDR 0x70
//CO2
#define SCD40_CH 0
// light
// #define BH1750_CH 1
// kiatu
// #define QMP6988_CH 2

class PCA9548A {
  public:
    bool begin() {
      Wire.beginTransmission(PCA9548A_ADDR);
      if (!Wire.endTransmission()) return 0;
      else return 1;
    }

    void selectChannel(uint8_t channel) {
      Wire.beginTransmission(PCA9548A_ADDR);
      Wire.write(1 << channel);
      Wire.endTransmission();
    }
};

SensirionI2CScd4x scd40;//I2C
PCA9548A pca9548a;//CO2
bool isI2C = false;
bool isCO2 = false;
bool isMoist = false;

// esp_MAC_address
// const uint8_t broadcastAddress[] = { 0x94, 0xE6, 0x86, 0x11, 0x65, 0x30 }; //device 1
const uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0xC0, 0x60, 0xFC }; //device 4


void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  return;
}

  typedef struct sensorData{
    float temperature = 0.0f;
    float humidity = 0.0f;
  } sensorData;

sensorData sensordata;
esp_now_peer_info_t peerSend;

void setup() {
  M5.begin();
  Wire.begin(32,33);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  //initialize ESP_NOW
  if(esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //register peer
  memset(&peerSend, 0 , sizeof(esp_now_peer_info_t));
  memcpy(peerSend.peer_addr, broadcastAddress, 6);
  peerSend.channel = 0;
  peerSend.encrypt = false;
  
  if(esp_now_add_peer(&peerSend) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  // send_cb
  esp_now_register_send_cb(onDataSend);
  
  // I2CBus Begin
  if(pca9548a.begin()) 
  {
    Serial.println("Failed to Begin I2C Module");
    isI2C = false;
  }
  else 
  {
    Serial.println("Successed to Begin I2C Module");
    isI2C = true;
  }

  // CO2 Begin
  pca9548a.selectChannel(SCD40_CH);
  scd40.begin(Wire);
  if (scd40.stopPeriodicMeasurement() || scd40.startPeriodicMeasurement()) {
    Serial.println("Failed to get CO2 data");
    isCO2 = false;
  } else {
    Serial.println("Successed to get CO2 data");
    isCO2 = true;
  }

  Serial.println("Press A Button");
  while (true) {
    M5.update();
    if (M5.BtnA.wasPressed()) 
      break;
    delay(500);
  }
  Serial.println("A Button was pressed!");

}

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;  // 送信するタイミングの設定

float temperature = 0.0f;
float humidity = 0.0f;
uint16_t co2 = 0;

float lux = 0.0f;
float pressure = 0.0f;
uint8_t moist = 0;

void loop()
{
    if(isI2C == false)
    {
      return;
    }

    if(isCO2 == true)
    {
        pca9548a.selectChannel(SCD40_CH);
        bool isDataReady  = false;
        while (!isDataReady) {
          scd40.getDataReadyFlag(isDataReady);
          delay(5000);
        }
        scd40.readMeasurement(co2, temperature, humidity);
        sensordata.temperature = temperature;
        sensordata.humidity = humidity;   

    }

    if ((millis() - lastTime) > timerDelay) {  // 2000ms毎にデータを送信する
    // データを送信する
    // バイト列に変換
    Serial.println("----------------------------");
    Serial.println("time: " + String(millis()) + "[ms]");
    Serial.println("co2: " + String(co2) + "[ppm]");
    Serial.println("hum: " + String(humidity) + "[%%]");
    Serial.println("tmp: " + String(temperature) + "[c]");
    
    esp_err_t result = esp_now_send(
      broadcastAddress,
      (uint8_t *) &sensordata,
      sizeof(sensorData)
    );

    if(result == ESP_OK)
    {
      Serial.println("Sent with success");
    }
    else
    {
      Serial.println("Error sending the data");
    }

      // uint8_t send_data[sizeof(sensorVal)];
      // memcpy(send_data, &sensorval, sizeof(sensorVal));
      // esp_now_send(peerSend.peer_addr, (uint8_t *)&send_data, sizeof(send_data));
    lastTime = millis();
  }

  
}

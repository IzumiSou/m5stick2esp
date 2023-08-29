#ifdef ARDUINO_M5Stick_C_PLUS
#include <M5StickCPlus.h>
#include <esp_now.h>
#elif ARDUINO_M5STACK_CORES3
#include <M5CoreS3.h>
#include <esp_now.h>
#elif ESP32
#include <esp_now.h>
#else
#endif
#include <WiFi.h>

esp_now_peer_info_t peerReceive;

typedef struct sensorData{
  float temperature = 0.0f;
  float humidity = 0.0f;
}sensorData;

sensorData sensordata;

const uint8_t mac_device1[] = {0xD4, 0xD4, 0xDA, 0x83, 0x81, 0xFC};
const uint8_t mac_device4[] = {0xD4, 0xD4, 0xDA, 0x85, 0x30, 0xA8};

void OnDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {

  //データ格納
  memset(&sensordata, 0 , sizeof(sensorData));
  memcpy(&sensordata, incomingData, sizeof(sensorData));

  Serial.println("----------------------");
  if (memcmp(mac, mac_device1, sizeof(mac)) == 0) {
    Serial.print("device1");
    Serial.println();
  }
  if (memcmp(mac, mac_device4, sizeof(mac)) == 0) {
    Serial.print("device4");
    Serial.println();
  }
  Serial.println("data received");
  Serial.print("temp: ");
  Serial.println(sensordata.temperature);
  Serial.print("humid: ");
  Serial.println(sensordata.humidity);
}

void setup() {
  Serial.begin(115200);

  // WiFiのステーションとしてデバイスを設定
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // init_esp
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataReceived);
}

void loop() {

}

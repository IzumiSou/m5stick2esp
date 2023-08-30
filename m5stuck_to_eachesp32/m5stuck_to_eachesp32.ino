#include <M5Unified.h>
#include <Wire.h>
#include <M5_ENV.h>
#include <esp_now.h>
#include <WiFi.h>

// esp_MAC_address
const uint8_t Water_esp_macAddress[] = { 0x94, 0xE6, 0x86, 0x11, 0x65, 0x30 }; //esp device 1
const uint8_t Motor_esp_macAddress[] = {0x78, 0x21, 0x84, 0xC0, 0x60, 0xFC }; //esp device 4

isAutoMode = true;
isWaterStop = true;
isMotorStop = ture;

typedef struct buttonData{
  bool isAutoMode = true;
  bool isWatetStop = true;
  bool isMotorStop = true;
} buttonData;

buttonData buttondata;

void autoModeButtonPressed_cb()
{
  esp_err_t result;

  if(isAutoMode)
  {
    buttondata.isAutoMode = true;
    buttondata.isWatetStop = true;
    buttondata.isMotorStop = true;
  }  
  else
  {
    buttondata.isAutoMode = false;
    buttondata.isWatetStop = true;
    buttondata.isMotorStop = true;
  }
  result = esp_now_send(
    NULL,
    (uint8_t *) &buttondata,
    sizeof(buttonData)
  );

  if(result == ESP_OK)
    Serial.println("Sent with success");
  else
    Serial.println("Error sending the data");
}
void waterButtonPressed_cb()
{
  if(isWatetStop)
  {
    buttondata.isAutoMode = false;
    buttondata.isWatetStop = true;
  }  
  else
  {
    buttondata.isAutoMode = false;
    buttondata.isWatetStop = false;
  }

  esp_err_t result = esp_now_send(
    Water_esp_macAddress,
    (uint8_t *) &buttondata,
    sizeof(buttonData)
  );

  if(result == ESP_OK)
    Serial.println("Sent with success");
  else
    Serial.println("Error sending the data");
}

void motorButtonPressed_cb()
{
  if(isMotorStop)
  {
    buttondata.isAutoMode = false;
    buttondata.isMotorStop = true;
  }  
  else
  {
    buttondata.isAutoMode = false;
    buttondata.isMotorStop = false;
  }

  esp_err_t result = esp_now_send(
    Motor_esp_macAddress,
    (uint8_t *) &buttondata,
    sizeof(buttonData)
  );

  if(result == ESP_OK)
    Serial.println("Sent with success");
  else
    Serial.println("Error sending the data");
}

void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.println("-----------------------");
  Serial.print("mac : ");
  Serial.println(*mac_addr);
  Serial.print("AutoMode : ");
  Serial.println(buttondata.isAutoMode);
  Serial.print("WaterStop : ");
  Serial.println(buttondata.isWaterStop);
  Serial.print("MotorStop : ");
  Serial.println(buttondata.isMotorStop);
  return;
}

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

  // Initialize peerSend for WaterPeer
  memset(&peerSend, 0, sizeof(esp_now_peer_info_t));
  memcpy(peerSend.peer_addr, Water_esp_macAddress, 6);
  peerSend.channel = 0;
  peerSend.encrypt = false;

  // Register WaterPeer
  if (esp_now_add_peer(&peerSend) != ESP_OK) {
    Serial.println("Failed to add Water peer");
    return;  // Or handle the error as needed
  }

  // Initialize peerSend for MotorPeer
  memset(&peerSend, 0, sizeof(esp_now_peer_info_t));
  memcpy(peerSend.peer_addr, Motor_esp_macAddress, 6);
  peerSend.channel = 0;
  peerSend.encrypt = false;

  // Register MotorPeer
  if (esp_now_add_peer(&peerSend) != ESP_OK) {
    Serial.println("Failed to add Motor peer");
    return;  // Or handle the error as needed
  }

  // send_cb
  esp_now_register_send_cb(onDataSend);

}

void loop()
{
      
}

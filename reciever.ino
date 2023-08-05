// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <WiFiClient.h>

char serData[7]={'0','0','0','0','0'};

typedef struct struct_message {
  int DOOR1_STATE;
  int DOOR2_STATE;
  int DOOR3_STATE;
  int PIR_STATE;
  int SENSOR_ID;
} door_state;
 
door_state myData;
 
// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  serData[0]=myData.SENSOR_ID+'0';
  serData[1]=myData.DOOR1_STATE+'0';
  serData[2]=myData.DOOR2_STATE+'0';
  serData[3]=myData.DOOR3_STATE+'0';
  serData[4]=myData.PIR_STATE+'0';
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
  
}
 
void loop() {
 Serial.write(serData,6);
 //Serial.println(serData);
 delay(500);
}

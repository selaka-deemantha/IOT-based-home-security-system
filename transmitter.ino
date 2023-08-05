
#define BUTTON_PIN_BITMASK 0x306000000 //bitmask for pin 33,32,25,26

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
 
// Variables for test data
int door1=32;
int door2=33;
int door3=25;
int pir=26;

 
uint8_t broadcastAddress[] = {0xE0, 0x5A, 0x1B, 0x60, 0x44, 0xD4};
 
// Define a data structure
typedef struct struct_message {
  int DOOR1_STATE;
  int DOOR2_STATE;
  int DOOR3_STATE;
  int PIR_STATE;
  int SENSOR_ID;
} door_state;
 
// Create a structured object
door_state Data;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("hhh"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : send_sensor_data(); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}


void setup(){
  Serial.begin(115200);
  pinMode(door1,INPUT);
  pinMode(door2,INPUT);
  pinMode(door3,INPUT);
  pinMode(pir,INPUT);
  WiFi.mode(WIFI_STA);

 if (esp_now_init() != ESP_OK) {
    return;
  }
 
  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    return;
  }
  
  print_wakeup_reason();
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  esp_deep_sleep_start();
}

void loop(){
}
void send_sensor_data(){
  while(digitalRead(door1)==1 || digitalRead(door2)==1 || digitalRead(door3)==1 || digitalRead(pir)==1){
  Data.DOOR1_STATE = digitalRead(door1);
  Data.DOOR2_STATE = digitalRead(door2);
  Data.DOOR3_STATE = digitalRead(door3);
  Data.PIR_STATE   = digitalRead(pir);
  Data.SENSOR_ID = 0;
  Serial.println("data sending");
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Data, sizeof(Data));
  delay(500);
  }
  Data.DOOR1_STATE = 0;
  Data.DOOR2_STATE = 0;
  Data.DOOR3_STATE = 0;
  Data.PIR_STATE   = 0;
  Data.SENSOR_ID = 0;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Data, sizeof(Data)); 
  Serial.println("going to sleep");
}

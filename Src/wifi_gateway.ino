#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//#define BLYNK_PRINT Serial

// Enter your Auth token
#define BLYNK_TEMPLATE_ID "TMPL6SCe7mFAY"
#define BLYNK_TEMPLATE_NAME "UI"
#define BLYNK_AUTH_TOKEN "1Qk0-o5Y7aJEWQfEKlGgONMJlgvcNctm"

//Enter your WIFI SSID and password
char ssid[] = "SLT-4G_1F5A96";
char pass[] = "ACD475C5";

char mystr[10]; 

int state=1;
int buzz=27;
int buzz_state=1;
int safe_led=32;

BLYNK_WRITE(V4){
  state=param.asInt();
}

BLYNK_WRITE(V5){
  buzz_state=param.asInt();
}


void send_to_blynk(){
    if(mystr[1]=='1'){
      Blynk.virtualWrite(V0,1);
    }
    if(mystr[1]=='0'){
      Blynk.virtualWrite(V0,0);
    }

    if(mystr[2]=='1'){
      Blynk.virtualWrite(V1,1);
    }
    if(mystr[2]=='0'){
      Blynk.virtualWrite(V1,0);
    }

    if(mystr[3]=='1'){
      Blynk.virtualWrite(V2,1);
    }
    if(mystr[3]=='0'){
      Blynk.virtualWrite(V2,0);
    }
    if(mystr[4]=='1'){
      Blynk.virtualWrite(V3,1);
    }
    if(mystr[4]=='0'){
      Blynk.virtualWrite(V3,0);
    }
    if(buzz_state==1){
      if(mystr[1]=='0' && mystr[2]=='0' && mystr[3]=='0' && mystr[4]=='0'){
        digitalWrite(buzz,LOW);
      }
      if(mystr[1]=='1' || mystr[2]=='1' || mystr[3]=='1' || mystr[4]=='1'){
        Blynk.logEvent("sensor_trigger","Your security has been breached!");
        digitalWrite(buzz,HIGH);
      }
    }
    if(buzz_state==0){
      digitalWrite(buzz,LOW);
    }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(14,OUTPUT);
  pinMode(safe_led,OUTPUT);
  digitalWrite(14,LOW);
  pinMode(buzz,OUTPUT);
  digitalWrite(buzz,LOW);
  digitalWrite(buzz,HIGH);
  digitalWrite(safe_led,HIGH);
  delay(1000);
  digitalWrite(buzz,LOW);
  digitalWrite(safe_led,LOW);
  
}

void loop() {

  Blynk.run();
  if(state==1){
    digitalWrite(safe_led,HIGH);
  }
  if(state==0){
    digitalWrite(safe_led,LOW);
  }
  Serial.readBytes(mystr,6);
  Serial.println(mystr);
  if(state){       
      send_to_blynk();   
    }
  else{
      Blynk.virtualWrite(V0,0);
      Blynk.virtualWrite(V1,0);
      Blynk.virtualWrite(V2,0);
      Blynk.virtualWrite(V3,0);
    }    
}

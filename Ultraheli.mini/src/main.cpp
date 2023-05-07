#include <Arduino.h>
#include <ittiot.h>
 
#define WIFI_NAME "TalTech"
#define WIFI_PASSWORD ""
 
const int triggerPin[2] = {15, 0}; 
const int echoPin[2] = {13, 4}; 
int newDistance[2];
int counter;
int distance[2] = {0, 0};

void setup()
{
  Serial.begin(115200); // setting up serial connection parameter
  iot.setConfig("wname", WIFI_NAME);
  iot.setConfig("wpass", WIFI_PASSWORD);
  iot.setConfig("msrv", "193.40.245.72");
  iot.setConfig("mport", "1883");
  iot.setConfig("muser", "test");
  iot.setConfig("mpass", "test");
  iot.printConfig(); // print json config to serial
  iot.setup(); // Initialize IoT library
  for (int i = 0; i < 2; i++){
    pinMode(triggerPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  };
}

void count(int distance1, int distance2){
  if (distance1 > newDistance[0] - 5 && distance1 < newDistance[0] + 5  && distance2 > newDistance[1] + 5 || distance2 < newDistance[1] - 5)
    counter++;
  else if (distance2 > newDistance[1] - 5 && distance2 < newDistance[1] + 5  && distance1 > newDistance[0] + 5 || distance1 < newDistance[0] - 5)
    counter--;
  
  if (counter < 0) counter = 0;

  Serial.println(distance[0]);
  Serial.println(distance[1]);
  Serial.println(newDistance[0]);
  Serial.println(newDistance[1]);
}
 
void loop()
{
  iot.handle();
  delay(10); // wait for 0.01 second
  int duration[2] = {0, 0};

  for (int i = 0; i < 2; i++){ 
    digitalWrite(triggerPin[i], LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin[i], LOW);
    newDistance[i] = distance[i];
    duration[i] = pulseIn(echoPin[i], HIGH);
    distance[i] = duration[i]/58.2;
  }
  delay(1000);
  count(distance[0], distance[1]);

  Serial.print("Count: ");
  Serial.println(counter);

  String msg = String(counter);
  iot.publishMsg("ultrasound", msg.c_str());
}
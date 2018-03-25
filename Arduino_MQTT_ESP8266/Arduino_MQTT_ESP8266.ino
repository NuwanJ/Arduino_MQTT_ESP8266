#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "defines.h"

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);
char msg[600];
/*topics*/
String SUB_TOPIC       = String ("+/" + DEVICE_SERIAL + "/" + DEVICE_BRAND + "/" + DEVICE_TYPE + "/" + DEVICE_VERSION + "/sub");
String EVENT_TOPIC     = String (DEVICE_BRAND + "/" + DEVICE_TYPE + "/" + DEVICE_VERSION + "/common");

/* definitions for this example */
#include <Servo.h>
Servo myservo;


void setup() {

  Serial.begin(115200);
  setup_wifi();
  /* configure the MQTT server with IP address and port */
  client.setServer("52.221.141.22", 1883);
  /* this receivedCallback function will be invoked
    when client received subscribed topic */
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void callback(char* topic, byte* payload, long length) {
  Serial.print("Message arrived [");
  Serial.print(SUB_TOPIC);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  do_actions(msg);
  for (int i = 0; i < length; i++) {
    msg[i] = NULL;
  }
}

void reconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    //client ID
    String clientId = DEVICE_SERIAL;
    /* Attempt to connect */
    if (client.connect(clientId.c_str(), server_username, server_password)) {
      Serial.println("connected");
      SUB_TOPIC.c_str();
      /* subscribe topic with default QoS 0*/
      client.subscribe(SUB_TOPIC.c_str());
      Serial.print("subscribed to topic: ");
      Serial.print(SUB_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }

  /* if client was disconnected then try to reconnect again */
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
    reconnect();
  }

  /* this function will listen for incomming
    subscribed topic-process-invoke receivedCallback */
  client.loop();
}

void publish_message(const char* message) {
  client.publish(EVENT_TOPIC.c_str(), message);
}


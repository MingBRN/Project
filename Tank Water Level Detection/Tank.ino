#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <Adafruit_HDC1000.h>

#define wifi_ssid "****"
#define wifi_password "********"

#define mqtt_server "********************************"
#define mqtt_user "****"
#define mqtt_password "******"

///// Ultrasonic /////

const int trigPin = D4; 
const int echoPin = D5; 

long duration;
int distance;

//////////////////////

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  ///// Ultrasonic /////

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //////////////////////
  delay(500);
}
  
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void loop() {
  if (!client.connected()) {
    reconnect();
  }

  ///// Ultrasonic /////

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  Serial.print(duration);
  // Calculating the distance
  distance = duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(2000);


  //////////////////////

  client.loop();
  if (distance < 400 && distance >= 0) {
    client.publish("sensor/ultrasonic", String(distance).c_str(), true);
    Serial.println("go");
    delay(5000);
  }
  Serial.println("finished");
}
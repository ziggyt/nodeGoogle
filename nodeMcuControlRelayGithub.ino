//Not used right now
//#include <DS3231.h>
//#include <Wire.h>
//DS3231 current;
#include <ESP8266WiFi.h>



const char* ssid = "SSID";
const char* password = "PASSWORD";
int relayPin = D3;

String currentState = "";


WiFiServer server(8085);


void setup() {
 // rtc.begin();
 
  Serial.begin(115200);
  delay(10);
  pinMode(relayPin, OUTPUT);
  
  //Initialize relay to "OFF"
  digitalWrite(relayPin, HIGH); 

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  //Serial.println(current.hour);

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  controlRelay(request); //Send URL in String

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

//If no state has been set, display nothing
  if (currentState != "") { 
    client.print(currentState);
  }

  client.println("<br><br>");
  client.println("<a href=\"/RelayOn\"\"><button>Turn On Relay </button></a>");
  client.println("<a href=\"/RelayOff\"\"><button>Turn Off Relay </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}


//Evaluate string to see if it matches desired value
void controlRelay(String request) {

  if (request.indexOf("/RelayOn") != -1 ) {
    relayOn();
  }
  if (request.indexOf("/RelayOff") != -1 ) {
    relayOff();
  }
}


void relayOn() {
  digitalWrite(relayPin, LOW);
  currentState = "Relay is on";
}
void relayOff() {
  digitalWrite(relayPin, HIGH);
  currentState = "Relay is off";
}


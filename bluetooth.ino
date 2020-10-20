#include <SPI.h>
#include <WizFi250.h>
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);
int getInt(String input);

#define VARID "650063e5f65fa583e398931611dba9a6"

char ssid[] = "안녕디지몬";       // your network SSID (name)
char pass[] = "ki980624";        // your network password
int status = WL_IDLE_STATUS;       // the Wifi radio's status

char server[] = "api.openweathermap.org";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 1000L; // delay between updates, in milliseconds

boolean readingVal;
boolean getIsConnected = false;
//String valString;
int val, temp;
float tempVal;

String rcvbuf;

// Initialize the Ethernet client object
WiFiClient client;

void httpRequest();
void printWifiStatus();


void setting(int a){
    if(a =='1')
  { 
   digitalWrite(A0,LOW);
   delay(1000);
   digitalWrite(A1,HIGH);
   delay(1000);
   digitalWrite(A2,HIGH);
   delay(1000);
   digitalWrite(A3,HIGH);
  
  }

 else if(a=='2')
  {
   digitalWrite(A0,LOW);
   delay(1000);
   digitalWrite(A1,LOW);
   delay(1000);
   digitalWrite(A2,HIGH);
   delay(1000);
   digitalWrite(A3,HIGH);
  
 
  }
 else if(a=='3'){
   digitalWrite(A0,LOW);
  delay(1000);
   digitalWrite(A1,LOW);
  delay(1000);
   digitalWrite(A2,LOW);
   delay(1000);
   digitalWrite(A3,HIGH);

 
 }
 else if(a=='4'){
   digitalWrite(A0,LOW);
   delay(1000);
   digitalWrite(A1,LOW);
   delay(1000);
   digitalWrite(A2,LOW);
   delay(1000);
   digitalWrite(A3,LOW);

 }
  }


void setup()
{
  // initialize serial for debugging
  BTSerial.begin(9600);
  Serial.begin(9600);
  Serial.println(F("\r\nSerial Init"));
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);

  WiFi.init();

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  
  Serial.println("You're connected to the network");

  printWifiStatus();
}

void loop() {

  if(BTSerial.available()){
  char  a = char(BTSerial.read());
  Serial.write(BTSerial.read());
  setting(a);
  }

  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  String valString;
  while (client.available()) {

    if ( rcvbuf.endsWith("\"temp\":")) {
      readingVal = true;
      valString = "";
    }

    char c = client.read();

    if ( c != NULL ) {
      if (rcvbuf.length() > 30)
        rcvbuf = "";
      rcvbuf += c;
      //Serial.write(c);
    }

    if (readingVal) {
      if (c != ',' ) {
        valString += c;
        //Serial.write(c);
      }
      else {
        readingVal = false;
        //Serial.println(valString);
        tempVal = valString.toFloat() - 273.0;
        Serial.println(tempVal);
      }
    }
  }
  if (millis() - lastConnectionTime > postingInterval) {
    
    if (getIsConnected) {
      Serial.println(valString);
      Serial.println(F("==================="));
      Serial.print(F("Temperature : "));
      Serial.println(tempVal);
      int c = tempVal;
      if(c<0){
        setting(1);
      }

      else if(c>=0&&c<10){
        setting(2);
      }

      else if(c>=10&&c<20){
        setting(3);
      }

      else{
        setting(4);
        
      }
      Serial.println(F("==================="));
    }
    httpRequest();
  }
  rcvbuf = "";
}

// this method makes a HTTP connection to the server
void httpRequest() {
  Serial.println();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");

    // send the HTTP PUT request
    client.print("GET /data/2.5/weather?q=Seoul,kr&appid=");
    client.print(VARID);
    client.println(" HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
    getIsConnected = true;
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
    getIsConnected = false;
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


int getInt(String input) {
  char carray[20];
  //Serial.println(input);
  input.toCharArray(carray, sizeof(carray));
  //Serial.println(carray);
  temp = atoi(carray);
  return temp;
}

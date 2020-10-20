#include <SPI.h>
#include <WizFi250.h>
#include "Adafruit_WS2801.h"
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif


#define VARID      "650063e5f65fa583e398931611dba9a6"


char ssid[] = "MeLong";       // 와이파이SSID
char pass[] = "tkdlaekd0";        // 와이파이 비밀번호
int status = WL_IDLE_STATUS;
int count = 150;
int count2 = 30;
char server[] = "api.openweathermap.org";

unsigned long lastConnectionTime = 0;         // 서버에 접속한 마지막 시간 (MilliSecond)
const unsigned long postingInterval = 1000L; // 서버에 데이터를 요청할 Delay (MilliSecond)

boolean readingVal;
boolean getIsConnected = false;
int val, temp;
int tempVal;

String rcvbuf;

int layer[4] = {A0, A1, A2, A3}; 
int room[14] = {A4, A5, 0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int m_layer = 4;  // 층 수
int m_room = 14; // 호실 수


unsigned int layerPattern1[16] ={ //ㅂ층 패턴
  0B11111111111111,
  0B10011001101001,
  0B11111111111111,
  0B10011001101001
};
unsigned int layerPattern2[16] ={ //ㄱ층 패턴
  0B11111111111111,
  0B00010001000001
};
unsigned int layerPattern3[16] ={ //ㄴ층 패턴
  0B10001000101000,
  0B11111111111111,
};

unsigned int layerPattern4[16] ={ //ㅎ층 패턴
  0B00100100010100,
  0B10111011101011,
  0B11100110010110,
  0B01010101010101 

};

unsigned int layerPattern5[16] ={ //LOVE층 패턴
  0B10001000101000,//L
  0B11111111111111,
  0B11111111111111,//O
  0B10011001101001,
  0B10011001101001,//V
  0B01100110010110,
  0B11111111111111,//E
  0B10001000101000

};


int m_pattern = 16; //패턴수


// WiFiClient 오브젝트 선언
WiFiClient client;

void httpRequest();
void printWifiStatus();


    

void setup()
{
   for(int i=0;i<m_layer;i++){
    pinMode(layer[i], OUTPUT); // 층 선언
    digitalWrite(layer[i], HIGH);  // 각층 닫기
  }
  
  for(int i=0;i<m_room;i++){
    pinMode(room[i], OUTPUT); //호실 선언
  }
  
  Serial.begin(115200);

  WiFi.init();

  if (WiFi.status() == WL_NO_SHIELD) {   // WiFi 보드에 문제가 있다면
    Serial.println("WiFi board error"); // 시리얼 모니터에 WiFi board error  출력
  }

  while ( status != WL_CONNECTED) {  // WiFi보드에 문제가 없어서 Wifi연결 시도
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);          // 연결 할 WiFi의 SSID를 출력

    status = WiFi.begin(ssid, pass);     // WPA/WPA2 WiFi에 연결
  }

  Serial.println("You're connected to the network");

  printWifiStatus();

       digitalWrite(1, HIGH); 
      digitalWrite(5, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(13, HIGH); 
      digitalWrite(layer[0],LOW);      
      delay(100);

      digitalWrite(0,  HIGH); 
      digitalWrite(4,  HIGH);
      digitalWrite(8,  HIGH);
      digitalWrite(12, HIGH); 
      digitalWrite(layer[1], LOW);      
      delay(100);

      digitalWrite(A5, HIGH); 
      digitalWrite(7,  HIGH);
      digitalWrite(11,  HIGH); 
      digitalWrite(layer[2], LOW);      
      delay(100);

      digitalWrite(A4,  HIGH); 
      digitalWrite(6,  HIGH);
      digitalWrite(10,  HIGH); 
      digitalWrite(layer[3], LOW);      
      delay(500);//기준

      digitalWrite(A4, LOW); 
      digitalWrite(6,  LOW);
      digitalWrite(10,  LOW); 
      digitalWrite(layer[3], HIGH);      
      delay(100);//기준

      digitalWrite(A5, LOW); 
      digitalWrite(7,  LOW);
      digitalWrite(11,  LOW); 
      digitalWrite(layer[2], HIGH);      
      delay(100);

      digitalWrite(0, LOW); 
      digitalWrite(4, LOW); 
      digitalWrite(8,  LOW);
      digitalWrite(12,  LOW); 
      digitalWrite(layer[1], HIGH);      
      delay(100);

      digitalWrite(1, LOW); 
      digitalWrite(5, LOW); 
      digitalWrite(9,  LOW);
      digitalWrite(13,  LOW); 
      digitalWrite(layer[0], HIGH);      
      delay(100);
     

      for(int j=0;j < count2 ;j++){  //m_delay 시간만큼 해당 패턴을 유지한다.

      LEDSetting(layer[0], layerPattern5[1]);  //L       
      LEDSetting(layer[1], layerPattern5[0]);        
      LEDSetting(layer[2], layerPattern5[0]);        
      LEDSetting(layer[3], layerPattern5[0]);
    }
    delay(50);
    

    for(int j=0;j < count2 ;j++){  //m_delay 시간만큼 해당 패턴을 유지한다.

      LEDSetting(layer[0], layerPattern5[2]);  //O       
      LEDSetting(layer[1], layerPattern5[3]);        
      LEDSetting(layer[2], layerPattern5[3]);        
      LEDSetting(layer[3], layerPattern5[2]);
    }
    delay(50);

    for(int j=0;j < count2 ;j++){  //m_delay 시간만큼 해당 패턴을 유지한다.

      LEDSetting(layer[0], layerPattern5[5]);  //V       
      LEDSetting(layer[1], layerPattern5[4]);        
      LEDSetting(layer[2], layerPattern5[4]);        
      LEDSetting(layer[3], layerPattern5[4]);
    }
    delay(50);

    for(int j=0;j < count2 ;j++){  //m_delay 시간만큼 해당 패턴을 유지한다.

      LEDSetting(layer[0], layerPattern5[6]);  //E       
      LEDSetting(layer[2], layerPattern5[6]);        
      LEDSetting(layer[1], layerPattern5[7]);        
      LEDSetting(layer[3], layerPattern5[6]);
    }
    delay(50);

      digitalWrite(1, HIGH); 
      digitalWrite(5, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(13, HIGH); 
      digitalWrite(layer[0],LOW);      
      delay(100);

      digitalWrite(0,  HIGH); 
      digitalWrite(4,  HIGH);
      digitalWrite(8,  HIGH);
      digitalWrite(12, HIGH); 
      digitalWrite(layer[1], LOW);      
      delay(100);

      digitalWrite(A5, HIGH); 
      digitalWrite(7,  HIGH);
      digitalWrite(11,  HIGH); 
      digitalWrite(layer[2], LOW);      
      delay(100);

      digitalWrite(A4,  HIGH); 
      digitalWrite(6,  HIGH);
      digitalWrite(10,  HIGH); 
      digitalWrite(layer[3], LOW);      
      delay(500);//기준

      digitalWrite(A4, LOW); 
      digitalWrite(6,  LOW);
      digitalWrite(10,  LOW); 
      digitalWrite(layer[3], HIGH);      
      delay(100);//기준

      digitalWrite(A5, LOW); 
      digitalWrite(7,  LOW);
      digitalWrite(11,  LOW); 
      digitalWrite(layer[2], HIGH);      
      delay(100);

      digitalWrite(0, LOW); 
      digitalWrite(4, LOW); 
      digitalWrite(8,  LOW);
      digitalWrite(12,  LOW); 
      digitalWrite(layer[1], HIGH);      
      delay(100);

      digitalWrite(1, LOW); 
      digitalWrite(5, LOW); 
      digitalWrite(9,  LOW);
      digitalWrite(13,  LOW); 
      digitalWrite(layer[0], HIGH);      
      delay(100);
     

}
void loop() {
  String valString;
  while (client.available()) {


    // {"id": 뒤에 오는 문자열을 날씨 코드로 저장
    if ( rcvbuf.endsWith("{\"id\":")) {
      readingVal = true;
      valString = "";
    }

    char c = client.read();

    if (c != NULL) {
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
        tempVal = valString.toFloat(); // 날씨 코드를 String에서 Integer로 변환
        Serial.print("날씨코드 : ");
        Serial.println(tempVal);  // 시리얼 모니터에 날씨 코드 출력
      }
    }
  }

  if (tempVal > 299 && tempVal < 532) {  //비
    for(int j=0;j < count ;j++){  
      LEDSetting(layer[0], layerPattern1[0]);  //1층 i번째 패턴        
      LEDSetting(layer[1], layerPattern1[1]);  //2층 i번째 패턴        
      LEDSetting(layer[2], layerPattern1[0]);  //3층 i번째 패턴        
      LEDSetting(layer[3], layerPattern1[1]);
      }}

  
  else if (tempVal > 599 && tempVal < 623) { //눈
    for(int j=0;j < count ;j++){  
      LEDSetting(layer[0], layerPattern3[1]);  //1층 i번째 패턴        
      LEDSetting(layer[1], layerPattern3[0]);  //2층 i번째 패턴        
      LEDSetting(layer[2], layerPattern3[0]);  //3층 i번째 패턴        
      LEDSetting(layer[3], layerPattern3[0]);
      
    }
  }

  
  else if (tempVal > 700 && tempVal < 782) { //안개(흐림)
    for(int j=0;j < count ;j++){  
      LEDSetting(layer[0], layerPattern2[1]);  //1층 i번째 패턴        
      LEDSetting(layer[1], layerPattern2[1]);  //2층 i번째 패턴        
      LEDSetting(layer[2], layerPattern2[1]);  //3층 i번째 패턴        
      LEDSetting(layer[3], layerPattern2[0]);
      
    }
  }
  
  else if (tempVal > 799 && tempVal < 802) { //맑음
    for(int j=0;j < count ;j++){  
       LEDSetting(layer[0], layerPattern4[0]);  //1층 i번째 패턴        
      LEDSetting(layer[1], layerPattern4[1]);  //2층 i번째 패턴        
      LEDSetting(layer[2], layerPattern4[2]);  //3층 i번째 패턴        
      LEDSetting(layer[3], layerPattern4[3]);
      
    }
  }

  
  else if (tempVal > 801 && tempVal < 805) { //구름많음
    for(int j=0;j < count ;j++){  
      LEDSetting(layer[0], layerPattern2[1]);  //1층 i번째 패턴        
      LEDSetting(layer[1], layerPattern2[1]);  //2층 i번째 패턴        
      LEDSetting(layer[2], layerPattern2[1]);  //3층 i번째 패턴        
      LEDSetting(layer[3], layerPattern2[0]);
      
    }
  
  }
  

  //날씨 코드 초기화
  tempVal = 0;

  if (millis() - lastConnectionTime > postingInterval) { // interval 시간이 충족되었다면
    httpRequest(); //데이터 호출
  }
  rcvbuf = "";
}

// 서버에 날씨 데이터 호출 함수
void httpRequest() {
  Serial.println();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // 제대로 서버에 연결되었을 경우
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");

    // HTTP 요청을 보냄
    client.print("GET /data/2.5/weather?q=Wonju,kr&appid=");
    client.print(VARID);
    client.println(" HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
    getIsConnected = true;

  }

  // 제대로 서버에 연결이 되지 않았으면 Connection failed 메세지와 함께 neopixel LED에 적색불 점멸
  else {
    Serial.println("Connection failed");
    getIsConnected = false;
  }
}


// WiFi 연결 상태 정보 출력 함수
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


void LEDSetting(int layer, unsigned int state){
  digitalWrite(layer, LOW); //층 개방
  for(int i=0;i<14;i++){    
    digitalWrite(room[i],bitRead(state, i)); //호실 개방     
  }
  delay(5);
  for(int i=0;i<14;i++){     
    digitalWrite(room[i], LOW);   //호실 닫힘   
  }
  digitalWrite(layer, HIGH); //층 닫힘    
}
    

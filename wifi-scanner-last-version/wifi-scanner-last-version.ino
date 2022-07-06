#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "mati lampu guys"; //Nama SSID AP/Hotspot
const char* password = "ayam12345"; //Password Wifi

ESP8266WebServer server(80); //Menyatakan Webserver pada port 80

void setup() {
  Serial.begin(9600);
  delay(10);

// Connect ke WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

// Mengatur WiFi 
  WiFi.mode(WIFI_STA); // Mode Station
  WiFi.begin(ssid, password); // Mencocokan SSID dan Password
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Tampilkan status Connected
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// Membuat tampilan Web
  server.on("/", []() {
    server.send(200, "text/plain", "Hello Para Warrior IoT");
  });

// Bagian ini untuk membuat halaman web lain
  server.on("/status", []() {
    int n = WiFi.scanNetworks();
    
    Serial.println("Scan done");

  String response = "";
  if (n == 0)
  {
    Serial.println("No Networks Found");
    response = "[]";
  }
  else
  {
    Serial.print(n);
    Serial.println(" Networks found");

//    response = "{";
    response+="[";
    for (int i = 0; i < n; ++i)
    {   
      response+="{'SSID':'";
      response+=WiFi.SSID(i);
      response+="',";
      response+="'RSSI':'";
      response+=WiFi.RSSI(i);
      response+="',";
      response+="'MAC':'";
      response+=WiFi.BSSIDstr(i);
      response+="',";
      response+="'isSecured':";
      response+=(WiFi.encryptionType(i) == ENC_TYPE_NONE)?"'Unsecured'":"'Secured'";
      response+="}";
      response+=(n-i != 1 ? ",":"");
    }
    response+="]";
//    response+="}";
  }
    server.send(200, "application/json", response);
  });

  server.begin();
  Serial.println("Webserver dijalankan");
}

void loop() {
   server.handleClient();
}

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
WiFiClient wifiClient;
const char *ssid     = "NDAK WIFI PERAI KO DO";
const char *password = "sayangamak";

//Web/Server address to read/write from 
const char *host = "http://192.168.100.87/mlx-data";   //your IP/web server address
//double temperature = 0;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  while (!Serial);

  Serial.println("Adafruit MLX90614 test");

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
}

void loop() {
  String a,t,s,d,postData;
  // ======= insert data ======
  // Declare object of class HTTPClient
  HTTPClient http;
  a = String(mlx.readAmbientTempC());
  t = String(mlx.readObjectTempC());
  s = String(0);
  d = String(0);

  //prepare request
  postData = "temperature=" + t;
  
  http.begin(wifiClient, host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
  
  // ======= end insert data ======
  

   Serial.println("&temperature=" + t);
   Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
//   Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
   Serial.print("*C\tObject = " + String(mlx.readObjectTempC())); Serial.println("*C");
   Serial.println("");
   delay(200);
}

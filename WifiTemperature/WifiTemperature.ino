#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHTesp.h"
#include "WifiCredentials.h"

int pin = 2;
DHTesp dht;

WiFiServer server(80);

void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.println();
  WifiCredentials credentials = WifiCredentials("Vk1BMTczMkU4Om1Kc3J6c3Mybnp1Vg==");
  
  dht.setup(pin, DHTesp::DHT11); // Connect DHT sensor to GPIO 2

  // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.println();
 
  Serial.println("Credentials valid: " + (String)credentials.IsValid()); 
  Serial.println("User: " + (String)credentials.SSID());
  Serial.println("Password: " + (String)credentials.Password());

  WiFi.begin(credentials.SSID(), credentials.Password());
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
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1500);

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  SendTemperatureAndHumidity(temperature, humidity);

  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 3");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
  client.println("<head>\n<meta charset='UTF-8'>");
  client.println("<title>ESP8266 Temperature & Humidity DHT11 Sensor</title>");
  client.println("</head>\n<body>");
  client.println("<H2>ESP8266 & DHT11 Sensor</H2>");
  client.println("<H3>Humidity / Temperature</H3>");
  client.println("<pre>");
  client.print("Humidity(%): ");
  client.println(humidity, 2);
  client.print("Temperature (°C): ");
  client.println(temperature, 2);
  client.println("</pre>");
  client.print("</body>\n</html>");
}

//WiFiClient clientOut;
void SendTemperatureAndHumidity(float temperature, float humidity) {
  if (temperature == NAN || humidity == NAN)
  {
    Serial.print("NAN values on DHT Sensor: Temperature = ");
    Serial.print(temperature);
    Serial.print(" Humidity = ");
    Serial.print(humidity);
    return;
  }

  int temperatureInt = (int)temperature;
  int humidityInt = (int)humidity;

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidityInt, 1);
  Serial.print("\t\t");
  Serial.print(temperatureInt, 1);
  Serial.println("");

  //const int httpPort = 80;
  const String protocol = "http";
  const String endpoint = "/api/iot/posttemperature";
  const String host = "mhiot.somee.com";
  //const String host = "192.168.0.73";
  const int httpPort = 80;
  const String hostName = host + ":" + (String)httpPort;
  const String url = protocol + "://" + hostName + endpoint;

  String jsonData = GetTemperatureAndHumidityAsJson(temperature, humidity);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(jsonData);
  String payload = http.getString();

  Serial.println("httpCode: " + (String)httpCode);
  Serial.println("payload: " + payload);
  http.end();
}

String GetTemperatureAndHumidityAsJson(int temperature, int humidity) {
  String data = "{\"temperature\":\"";
  data +=  (String)temperature + "\",";
  data += "\"humidity\":\"";
  data +=  (String)humidity + "\"}";
  return data;
}

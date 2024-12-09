#include <Arduino.h>
#include <PromLokiTransport.h>
#include <PrometheusArduino.h>
#include <DHT.h>

#include "certificates.h"
#include "config.h"

// DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

// Prometheus client and transport
PromLokiTransport transport;
PromClient client(transport);

// Create a write request for 3 series (when changing update buffers used to serialize the data)
WriteRequest req(3, 1537);

// Define a TimeSeries which can hold up to 5 samples, has a name of `temperature/humidity/...` and uses the above labels of which there are 2
TimeSeries ts1(5, "temperature_celsius", "{monitoring_type=\"room_comfort\",board_type=\"firebeetle\",room=\"office\"}");
TimeSeries ts2(5, "humidity_percent",  "{monitoring_type=\"room_comfort\",board_type=\"firebeetle\",room=\"office\"}");
TimeSeries ts3(5, "heat_index_celsius",  "{monitoring_type=\"room_comfort\",board_type=\"firebeetle\",room=\"office\"}");

int loopCounter = 0;

// Function to set up Prometheus client
void setupClient() {
  Serial.println("Setting up client...");
  
  uint8_t serialTimeout;
  while (!Serial && serialTimeout < 50) {
    delay(100);
    serialTimeout++;
  }
  
  // Configure and start the transport layer
  transport.setUseTls(true);
  transport.setCerts(grafanaCert, strlen(grafanaCert));
  transport.setWifiSsid(WIFI_SSID);
  transport.setWifiPass(WIFI_PASSWORD);
  transport.setDebug(Serial);  // Remove this line to disable debug logging of the client.
  if (!transport.begin()) {
      Serial.println(transport.errmsg);
      while (true) {};
  }

  // Configure the client
  client.setUrl(GC_PROM_URL);
  client.setPath(GC_PROM_PATH);
  client.setPort(GC_PORT);
  client.setUser(GC_PROM_USER);
  client.setPass(GC_PROM_PASS);
  client.setDebug(Serial);  // Remove this line to disable debug logging of the client.
  if (!client.begin()) {
      Serial.println(client.errmsg);
      while (true) {};
  }

  // Add our TimeSeries to the WriteRequest
  req.addTimeSeries(ts1);
  req.addTimeSeries(ts2);
  req.addTimeSeries(ts3);
  req.setDebug(Serial);  // Remove this line to disable debug logging of the write request serialization and compression.
}


// ========== MAIN FUNCTIONS: SETUP & LOOP ========== 
// SETUP: Function called at boot to initialize the system
void setup() {
  // Start the serial output at 115,200 baud
  Serial.begin(115200);

  // Set up client
  setupClient();

  // Start the DHT sensor
  dht.begin();
}


// LOOP: Function called in a loop to read from sensors and send them do databases
void loop() {
  int64_t time;
  time = transport.getTimeMillis();

  // Read temperature and humidity
  float hum = dht.readHumidity();
  float cels = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(hum) || isnan(cels)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(cels, hum, false);

  if (loopCounter >= 5) {
    //Send
    loopCounter = 0;
    PromClient::SendResult res = client.send(req);
    if (!res == PromClient::SendResult::SUCCESS) {
            Serial.println(client.errmsg);
    }
    
    // Reset batches after a succesful send.
    ts1.resetSamples();
    ts2.resetSamples();
    ts3.resetSamples();
  } else {
    if (!ts1.addSample(time, cels)) {
      Serial.println(ts1.errmsg);
    }
    if (!ts2.addSample(time, hum)) {
      Serial.println(ts2.errmsg);
    }
    if (!ts3.addSample(time, hic)) {
      Serial.println(ts3.errmsg);
    }
    loopCounter++;
  }
  // wait INTERVAL seconds and then do it again
  delay(INTERVAL * 1000);
}

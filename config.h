#define WIFI_SSID     "wifi_name" // Add wifi name
#define WIFI_PASSWORD "wifi_password" // Add wifi passowrd

#define ID "weather" // Add unique name for this sensor
#define INTERVAL 10 //seconds

#define DHTPIN 4    // Which pin is DHT 11 connected to
#define DHTTYPE DHT11 // Type DHT 11

#define GC_URL "prometheus-us-central1.grafana.net" // Url to Prometheus instance 
#define GC_PATH "/api/prom/push" // Path
#define GC_PORT 443
#define GC_USER "" // Username
#define GC_PASS "" // API key
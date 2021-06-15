# (WIP) Room confort monitoring with Grafana stack

The weather monitoring let's you see the current and previous temperature and humidity data of your room (or any place you set up monitoring in). This project was part of **[GrafanaCONline 2021 Easy DIY IoT projects with Grafana](https://grafana.com/go/grafanaconline/2021/diy-iot/)** session.

<img src="imgs/monitor.jpg" width="700" title="Image of how the system looks">

## Hardware

To build this system, you will need:

- **1 [DHT11](https://components101.com/sensors/dht11-temperature-sensor)** sensor to measure temperature and humidity
- **1 [ESP32 DEVKIT DOIT](https://randomnerdtutorials.com/getting-started-with-esp32/)** development board
- **3 F-F Dupont cables**
- **1 micro USB cable**
- **1 USB charger**

![Image of hardware](imgs/parts.jpg)

## Libraries:

- **DHT sensor library by** Adafruit
- **Adafruit Unified Sensor** by Adafruit
- **ArduinoBearSSL** by Arduino
- **ArduinoHttpClient** by Arduino
- **PrometheusArduino** by Ed Welch
- **PromLokiTransport** by Ed Welch
- **SnappyProto** by Ed Welch

## Circuit & Wiring diagrams

<img src="imgs/wire_diagram.png" width="700" title="Wire diagram">

## Data storage and visualisation

We are using free tier of [Grafana Cloud](https://grafana.com/products/cloud/) that comes with hosted [Loki](https://grafana.com/oss/loki/) (logs), [Prometheus](https://grafana.com/oss/prometheus/) (metrics) and [Grafana](https://grafana.com/oss/grafana/) (visualisation). As soon as your account is all set up, you can see the portal with the hosted Grafana, Loki, and Prometheus instances. Created API keys for Loki and Prometheus to publish metrics from the monitoring system to these databases. Update API keys in the config file.

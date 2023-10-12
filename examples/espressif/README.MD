# Espressif Example
This example shows you how to connect and read the first values from your solar inverter using an esp32.

## Building
Before building some variables at the beginning of `main.cpp` need to be changed to match your setup:

| variable      | Explanation                         | Source     |
| ------------- | ----------------------------------- | ---------- |
| ip            | The inverters local IP address      | Scan network for new IPs or check your routers web interface.  |
| port          | The inverters port                  | The port should usually be 8899. |
| serial_number | The inverters unique serial number  | Can be found on the back of the inverter or on the inverters web interface. |
| wifi_ssid | Your wifi SSID |  ... |
| wifi_pwd | Your wifi password  | ... |

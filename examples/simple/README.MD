# Simple Example
This simple example shows you how to connect and read the first values from your solar inverter.

## Dependencies
Make sure that [Boost](https://www.boost.org/) with the development headers is installed on your system.

## Building
Before building some variables at the beginning of `main.cpp` need to be changed to match your setup:

| variable      | Explanation                         | Source     |
| ------------- | ----------------------------------- | ---------- |
| ip            | The inverters local IP address      | Scan network for new IPs or check your routers web interface.  |
| port          | The inverters port                  | The port should usually be 8899. |
| serial_number | The inverters unique serial number  | Can be found on the back of the inverter or on the inverters web interface.


```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running

```bash
$ ./deye_simple_example
Production Today (Watt hours): 100 Wh
PV1 Voltage (Volts): 31.8 V
PV1 Current (Ampere): 0.4 A
Radiator Temperature (degrees Ceclius): 24 C°
```

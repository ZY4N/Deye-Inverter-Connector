# InfluxDB Example
This example shows you how to log data from your solar inverter to an influxdb database for use with Grafana, etc..

## Dependencies
Make sure that [Boost](https://www.boost.org/) with the development headers, [InfluxDB](https://docs.influxdata.com/influxdb/v2/install/) aswell as [influxdb-cxx](https://www.influxdata.com/blog/getting-started-c-influxdb/) is installed on your system.

## Building
Before building some variables at the beginning of `main.cpp` need to be changed to match your setup:

| variable      | Explanation                         | Source     |
| ------------- | ----------------------------------- | ---------- |
| ip            | The inverters local IP address      | Scan network for new IPs or check your routers web interface.  |
| port          | The inverters port                  | The port should usually be 8899. |
| serial_number | The inverters unique serial number  | Can be found on the back of the inverter or on the inverters web interface.
| influxdb_url | The url of your influx database dataset | The databse will be created if it doesn't already exist. |

```bash
mkdir build
cd build
cmake ..
cmake --build .
```


## Flags

You can change the debug level using the following flags:

| flags      | log level |
| ---------- | --------- |
| `-e`, `-E` | error     |
| `-w`, `-W` | warn      |
| `-i`, `-I` | info      |
| `-l`, `-L` | log       |
| `-d`, `-D` | debug     |

## Running

```bash
$ ./deye_influxdb_example -l
[log][inverter1][tick]: 1 # day time
[log][inverter1][tick]: 2
[log][inverter1][tick]: 3
[log][inverter1][tick]: 4
...
[error][inverter1][connect][system]: No route to host # night time
```
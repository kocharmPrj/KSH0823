

# KSH0823 prj

using 2 arduino and 1 rasberry pi 4, control servo motor following CDS sensor

## Arduino #1 with CDS sensor 
- upload .ino file in /arduino/cdsWithBluetooth/cdsWithBluetooth.ino to arduino #1
- connect CDS sensor to Arduino analog port #1 with pull-up
- connect bluetooth module(hc-06) tx to arduino d10, rx to arduino d11 port

## Arduino #2 with Servo motor
- upload .ino file in /arduino/servoWithWifi/servoWithWifi.ino to arduino #2

## Raspberry pi

- upload files in /arduino
- ![enter image description here](https://ibb.co/CMz9Fc0)
- ![enter image description here](https://ibb.co/qmdgHy6)
- ![enter image description here](https://ibb.co/2jrbvBd)

- refer the port Number, you can change port num in runClient.sh

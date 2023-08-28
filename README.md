

# KSH0823 prj

using 2 arduino and 1 rasberry pi 4, control servo motor based on CDS sensor value

## Arduino #1 with CDS sensor 
- upload .ino file in /arduino/cdsWithBluetooth/cdsWithBluetooth.ino to arduino #1
- connect CDS sensor to Arduino analog port #1 with pull-up
- connect bluetooth module(hc-06) tx to arduino d10, rx to arduino d11 port

## Arduino #2 with Servo motor
- upload .ino file in /arduino/servoWithWifi/servoWithWifi.ino to arduino #2

## Raspberry pi
- upload files in /arduino
- Run server
  
<a href="https://imgbb.com/"><img src="https://i.ibb.co/RB96WFH/server.png" alt="server" border="0"></a>
-  Run Client for bluetooth connection
  
 <a href="https://imgbb.com/"><img src="https://i.ibb.co/j3TwC6t/client2.png" alt="client2" border="0"></a>

OR

 <a href="https://imgbb.com/"><img src="https://i.ibb.co/sF4BKrq/client1.png" alt="client1" border="0"></a>

- confer the port Number in server & client and you can change port num in runClient.sh also

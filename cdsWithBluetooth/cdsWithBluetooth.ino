/*
  blue test: 
  http://www.kccistc.net/
  작성일 : 2022.12.19
  작성자 : IoT 임베디드 KSH
*/
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MsTimer2.h>

//#define DEBUG
#define CDS_PIN1 A0
#define CDS_PIN2 A1

#define ARR_CNT 5
#define CMD_SIZE 60
char sendBuf[CMD_SIZE];
char recvId[10] = "ARD_MOT";  // SQL 저장 클라이이언트 ID
bool timerIsrFlag = false;
unsigned int secCount;
int cds1=0;
int cds2=0;
bool cdsFlag = false;
int getSensorTime;
SoftwareSerial BTSerial(10, 11); // RX ==>BT:TXD, TX ==> BT:RXD

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("setup() start!\n");
#endif

  BTSerial.begin(9600); // set the data rate for the SoftwareSerial port
  MsTimer2::set(5000, timerIsr); // 5s period
  MsTimer2::start();
}

void loop()
{
  if (BTSerial.available()) bluetoothEvent();

  if (timerIsrFlag){
    timerIsrFlag = false;
    cds1 = map(analogRead(CDS_PIN1), 0, 1023, 0, 100);
    cds2 = map(analogRead(CDS_PIN2), 0, 1023, 0, 100);

    
    //ADDED LINE
    sprintf(sendBuf,"[%s]SENSOR@%d\r\n",recvId,cds1);
    BTSerial.write(sendBuf);   
    Serial.write(sendBuf);
    
  }
#ifdef DEBUG
  if (Serial.available())
    BTSerial.write(Serial.read());
#endif
}
void bluetoothEvent() {
  int i = 0;
  char * pToken;
  char * pArray[ARR_CNT] = {0};
  char recvBuf[CMD_SIZE] = {0};
  int len = BTSerial.readBytesUntil('\n', recvBuf, sizeof(recvBuf) - 1);

  pToken = strtok(recvBuf, "[@]");
  while (pToken != NULL)
  {
    pArray[i] =  pToken;
    if (++i >= ARR_CNT)
      break;
    pToken = strtok(NULL, "[@]");
    // NO USE OF pARR
  }
#ifdef DEBUG
  Serial.print("Recv : ");
  Serial.println(recvBuf);
#endif
  //recvBuf : [XXX_BTM]LED@ON
  //pArray[0] = "XXX_LIN"   : 송신자 ID
  //pArray[1] = "LED"
  //pArray[2] = "ON"
  //pArray[3] = 0x0
}

void timerIsr()
{
  timerIsrFlag = true;
  secCount++;
  secCount%=10000;
}

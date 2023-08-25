/*
  blue test: 
  http://www.kccistc.net/
  작성일 : 2022.12.19
  작성자 : IoT 임베디드 KSH
*/
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>
#include <MsTimer2.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DEBUG
#define CDS_PIN A0

#define LED_BUILTIN_PIN 13
#define SERVO_PIN 5
#define ARR_CNT 5
#define CMD_SIZE 60
char lcdLine1[17] = "Smart IoT By KJH";
char lcdLine2[17] = "";
char sendBuf[CMD_SIZE];
char recvId[10] = "KJH_SQL";  // SQL 저장 클라이이언트 ID

bool timerIsrFlag = false;
unsigned int secCount;
unsigned int myservoTime = 0;
int cds=0;

bool cdsFlag = false;
int getSensorTime;

SoftwareSerial BTSerial(10, 11); // RX ==>BT:TXD, TX ==> BT:RXD

Servo myservo;

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("setup() start!");
#endif
  lcd.init();
  lcd.backlight();
  lcdDisplay(0, 0, lcdLine1);
  lcdDisplay(0, 1, lcdLine2);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
 
  BTSerial.begin(9600); // set the data rate for the SoftwareSerial port
  MsTimer2::set(1000, timerIsr); // 1000ms period
  MsTimer2::start();

  myservo.attach(SERVO_PIN);
  myservo.write(0);
  myservoTime = secCount;
}

void loop()
{
  if (BTSerial.available())
    bluetoothEvent();

  if (timerIsrFlag)
  {
    timerIsrFlag = false;
    cds = map(analogRead(CDS_PIN), 0, 1023, 0, 180);


    sprintf(lcdLine2,"C:%d",cds);
#ifdef DEBUG
    Serial.println(lcdLine2);
#endif
    if (myservo.attached() && myservoTime + 2 == secCount)
    {
      myservo.detach();
    }
    lcdDisplay(0, 1, lcdLine2);

    if(getSensorTime != 0 && !(secCount % getSensorTime)) {
      sprintf(sendBuf,"[%s]SENSOR@%d\n",recvId,cds);
      BTSerial.write(sendBuf);   
    }    
    
    if ((cds >= 90) && cdsFlag)
    {
      cdsFlag = false;
      sprintf(sendBuf, "[%s]CDS@%d\n", recvId, cds);
      BTSerial.write(sendBuf, strlen(sendBuf));
    } 
    else if ((cds < 90) && !cdsFlag)
    {
      cdsFlag = true;
      sprintf(sendBuf, "[%s]CDS@%d\n", recvId, cds);
      BTSerial.write(sendBuf, strlen(sendBuf));
    }
  }
  
}
void bluetoothEvent()
{
  int i = 0;
  char * pToken;
  char * pArray[ARR_CNT] = {0};
  char recvBuf[CMD_SIZE] = {0};
  int len = BTSerial.readBytesUntil('\n', recvBuf, sizeof(recvBuf) - 1);

#ifdef DEBUG
  Serial.print("Recv : ");
  Serial.println(recvBuf);
#endif

  pToken = strtok(recvBuf, "[@]");
  while (pToken != NULL)
  {
    pArray[i] =  pToken;
    if (++i >= ARR_CNT)
      break;
    pToken = strtok(NULL, "[@]");
  }
  //recvBuf : [XXX_BTM]LED@ON
  //pArray[0] = "XXX_LIN"   : 송신자 ID
  //pArray[1] = "LED"
  //pArray[2] = "ON"
  //pArray[3] = 0x0
  if ((strlen(pArray[1]) + strlen(pArray[2])) < 16)
  {
    sprintf(lcdLine2, "%s %s", pArray[1], pArray[2]);
    lcdDisplay(0, 1, lcdLine2);
  }
  
  if(!strcmp(pArray[1],"GETSENSOR"))
  {
    if(pArray[2] == NULL){
      getSensorTime = 0;
    }else {
      getSensorTime = atoi(pArray[2]);
      strcpy(recvId,pArray[0]);
    }
    sprintf(sendBuf,"[%s]SENSOR@%d\n",pArray[0],cds);
  }
  else if (!strcmp(pArray[1], "SERVO"))
  {
    myservo.attach(SERVO_PIN);
    myservoTime = secCount;
    if (!strcmp(pArray[2], "ON"))
      myservo.write(180);
    else
      myservo.write(0);
    sprintf(sendBuf, "[%s]%s@%s\n", pArray[0], pArray[1], pArray[2]);

  }
  else if (!strncmp(pArray[1], " New", 4)) // New Connected
  {
    return ;
  }
  else if (!strncmp(pArray[1], " Alr", 4)) //Already logged
  {
    return ;
  }

#ifdef DEBUG
  Serial.print("Send : ");
  Serial.print(sendBuf);
#endif
  BTSerial.write(sendBuf);
}
void timerIsr()
{
  timerIsrFlag = true;
  secCount++;
}
void lcdDisplay(int x, int y, char * str)
{
  int len = 16 - strlen(str);
  lcd.setCursor(x, y);
  lcd.print(str);
  for (int i = len; i > 0; i--)
    lcd.write(' ');
}


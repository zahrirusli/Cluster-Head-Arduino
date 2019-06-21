#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_INA219.h> // You will need to download this library

Adafruit_INA219 sensor219; // Declare and instance of INA219


RF24 radio(7, 8);

const int nodeId=2;
int CH_ID=0;
int ch_status=0;
const int rxAddr = 5000;
const int rxAddr1 = 2001;
const int rxAddr2 = 2002;
const int rxAddr3 = 2003;
const int rxAddr4 = 2004;
const int sinkAddr = 1000;
const int bcAddr = 7000;
int energi1 = 30;
int energi2 = 70;
int energi3 = 50;
int energi4 = 90;
int ChAddr = 0;
int sinkStatus = 0;
float busVoltage = 0;
float current = 0; // Measure in milli amps
float power = 0;


int a = 0, xMili = 0, xSecond = 0, xMinute = 0, xHour = 0, currentSecond = 0, currentMilis=0;
unsigned long wTime;
int start=0;
int flag_reset_time = 0;
void setup()
{

  xMili = 0; xSecond = 0; xMinute = 0; xHour = 0;


  while (!Serial);
  Serial.begin(9600);
  
  sensor219.begin();
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();

}

void getPower(){
  busVoltage = sensor219.getBusVoltage_V();
  current = sensor219.getCurrent_mA();
  power = busVoltage * current; // Calculate the Power
  
  
  Serial.print("Bus Voltage:   "); 
  Serial.print(busVoltage); 
  Serial.println(" V");  
  
  Serial.print("Current:       "); 
  Serial.print(current); 
  Serial.println(" mA");
  
  Serial.print("Power:         "); 
  Serial.print(power); 
  Serial.println(" W");  
  
  Serial.println("");  

  delay(2000);

  
}
  
void countTime() {
  ++xMili;
  if (xMili == 999) {
    ++xSecond;
    xMili = 0;
  }
  if ((xSecond > 59)) {
    ++xMinute;
    xSecond = 0;
    currentSecond = -1;
  }
  if (xMinute > 59) {
    ++xHour;
    xMinute = 0;

  }
  
}



void tapListening() {

  // radio.openReadingPipe(0, rxAddr);
  // radio.startListening();

  if (radio.available())
  {
    char rtext[25];
    radio.read(&rtext, sizeof(rtext));
       if(rtext[0] == 'R'){
          xSecond = 0;
          xMinute = 0;
          xHour = 0;
          xMili = 0;
          flag_reset_time = 1;
          if(rtext[2]=='1'){
          ChAddr = rxAddr1 ;
          CH_ID = 1;
          }
      }
      
    Serial.println(rtext);
   }
  
}

void tapListening(int addres) {

   radio.openReadingPipe(0, addres);
   radio.startListening();
  
  if (radio.available())
  {
     char rtext[25];
    radio.read(&rtext, sizeof(rtext));
    if(rtext[0]=='S'){
         ch_status = 0;
         CH_ID = 0;
     }
     else if(rtext[0]=='B'){
      
          if(rtext[2]=='1'){
            energi1 = rtext+3;   
          }
          else if(rtext[2]=='2'){
            energi2 = rtext+3;   
          }
          else if(rtext[2]=='3'){
            energi3 = rtext+3;   
          }
          else if(rtext[2]=='4'){
            energi4 = rtext+3;   
          }
          delay(100);
          if (energi1<energi2 && energi1<energi3 && energi1<energi4){
                CH_ID = 1;
                ChAddr= rxAddr1;
                if(nodeId==1){
                  ch_status=1;
                   char text[25];
                   String sendTosink = "I#";
                   sendTosink.concat(nodeId);
                   transmit(sinkAddr,sendTosink);
                }else{
                  ch_status=0;
                 }
            }else if (energi2<energi1 && energi2<energi3 && energi2<energi4){
                CH_ID = 2;
                ChAddr= rxAddr2;
                if(nodeId==2){
                  ch_status=1;
                      char text[25];
                      String sendTosink = "I#";
                      sendTosink.concat(nodeId);
                      transmit(sinkAddr,sendTosink);
               
                }else{
                  ch_status=0;
                 }
            }else if (energi3<energi1 && energi3<energi3 && energi3<energi4){
                CH_ID = 3;
                ChAddr= rxAddr3;
                if(nodeId==3){
                  ch_status=1;
                      char text[25];
                      String sendTosink = "I#";
                      sendTosink.concat(nodeId);
                      transmit(sinkAddr,sendTosink);
               
                }else{
                  ch_status=0;
                 }
            }else if (energi4<energi1 && energi4<energi2 && energi4<energi2){
                CH_ID = 4;
                ChAddr= rxAddr4;
                if(nodeId==4){
                  ch_status=1;
                       char text[25];
                       String sendTosink = "I#";
                       sendTosink.concat(nodeId);
                       transmit(sinkAddr,sendTosink);
                }else{
                  ch_status=0;
                 }
            }

            delay(5);
      }
      else if(rtext[0]=='I'){
          String msg = "CH ADALAH NODE :";
          msg.concat(rtext[2]);
          Serial.println(msg);
       }
        else if(rtext[0]=='D'){
           transmit(sinkAddr,rtext);
           Serial.println(rtext);
        }
     
      
    Serial.println(rtext);
  }
}

void transmit(String Message)
{

  radio.setRetries(15, 15);
  radio.stopListening();
  radio.openWritingPipe(rxAddr);

  String c = Message;
  char pesan[25];
  c.toCharArray(pesan, 25);

  int  rNumber  = random(1,5);
  delay(rNumber);
  
  radio.write(&pesan, sizeof(pesan));
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
   Serial.println(pesan[0]);
   
   if(pesan[0] == 'R'){
        xSecond = 0;
        xMinute = 0;
        xHour = 0;
        xMili = 0;
        flag_reset_time = 1;
        ch_status =1;
        CH_ID = 1;
        ChAddr = rxAddr1;          
      }

}




void transmit(int sendAddr, String Message)
{

  radio.setRetries(15, 15);
  radio.stopListening();
  radio.openWritingPipe(sendAddr);

  String c = Message;
  char pesan[25];
  c.toCharArray(pesan, 25);

  int  rNumber  = random(1,5);
  delay(rNumber);
  
  radio.write(&pesan, sizeof(pesan));
  radio.openReadingPipe(0, sendAddr);
  radio.startListening();
     if (pesan[0] == 'S') {
          ch_status = 0;
          CH_ID = 0;                 
        }
   
}


void loop()
{


    getPower();
    power+=power ; 
  currentSecond = xSecond;
  countTime();

  //node saat waktu akan di restart
  if(flag_reset_time==0) {
      tapListening();
        if((xSecond == 15) && (xMili==1)&&(nodeId==1))
            {
              char text[25];
              String ResetTime = "R#";
              ResetTime.concat(nodeId);
              transmit(ResetTime);
              Serial.println(ResetTime) ;
            }  
    }

    //node saat stop sebelum pemilihan cluster head
  else if ((flag_reset_time == 1)&& (CH_ID==0)){
      tapListening(bcAddr);
      radio.setPALevel(RF24_PA_LOW);
      if (((xMinute % 3)==0) && (xSecond == 10) && (xMili==1) )
          {
          char text[25];
          String bcEnergy = "B#";
          bcEnergy.concat(nodeId);
          bcEnergy.concat(power);
          transmit(bcAddr,bcEnergy);
          Serial.println(bcEnergy) ;
          }
    }
    // node saat menjadi cluster head
  else if ((ch_status==1)&&(flag_reset_time ==1) && (CH_ID!=0)){
      tapListening(ChAddr);
      radio.setPALevel(RF24_PA_HIGH);
          //pesan untuk berhenti menjadi cluster head
          if (((xMinute % 3)==0) && (xSecond == 1) && (xMili==1) )
          {
              char text[25];
              String stopCH = "S#";
              stopCH.concat(nodeId);
              transmit(rxAddr,stopCH);
              Serial.println(stopCH) ;
          }
          //pesan kepada sink kalau dia cluster head
          else if (((xMinute % 1)==0) && (xSecond == 1) && (xMili==1) )
          {
              char text[25];
              String sendTosink = "I#";
              sendTosink.concat(nodeId);
              transmit(sinkAddr,sendTosink);
              Serial.println(sendTosink) ;
          }
          
    }
    // node biasa saat terdpat cluster head
  else if ((ch_status==0)&&(flag_reset_time ==1)&& (CH_ID!=0)){
      tapListening(rxAddr);
      radio.setPALevel(RF24_PA_LOW);
          if  (((xSecond %25) ==0) && (xMili==1) )
          {
              char text[25];
              String sendtoCH = "DATA#";
              sendtoCH.concat(nodeId);
              transmit(ChAddr,sendtoCH);
              Serial.println(sendtoCH) ;
          }
      
    }
    //coordinator
  else if (sinkStatus == 1){
        tapListening(sinkAddr);
      }
 
  
   
  delay(1);
}

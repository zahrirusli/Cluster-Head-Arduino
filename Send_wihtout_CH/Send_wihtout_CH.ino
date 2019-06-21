#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const int nodeID=2;
const int chId=1;
const int statusCH=0;
const int rxAddr = 5000;
const int sinkAddr = 7000;
int sendAddr3 = 5000;
int counter =0;
int coordinator_status = 0;
int a = 0, xMili = 0, xSecond = 0, xMinute = 0, xHour = 0, currentSecond = 0, currentMilis=0;
unsigned long wTime;
int flag = 1; int start=0;
void setup()
{

  xMili = 0; xSecond = 0; xMinute = 0; xHour = 0;


  while (!Serial);
  Serial.begin(9600);


  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);     
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();

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

 //  radio.openReadingPipe(0, addres);
 //  radio.startListening();

  if (radio.available())
  {
     char rtext[25];
    radio.read(&rtext, sizeof(rtext));
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


}


void loop()
{



  currentSecond = xSecond;
  countTime();

  if(coordinator_status == 0){
    tapListening();
  }
  else{
    tapListening(sinkAddr);
    }

   
    if (((xSecond % 15) == 0)&& (flag==1) && (xMili ==1 )) {

      char text[25];
      counter++;
      String myMessage = "MSG#" ;
      myMessage.concat(nodeID);
      myMessage.concat("#");
      myMessage.concat(counter);
      transmit(sinkAddr,myMessage);
             
       Serial.println(myMessage);
       
    }       
 
  
   
  delay(1);
}

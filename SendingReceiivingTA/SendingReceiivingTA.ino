#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const int nodeId=4;
const int chId=1;
int ch_status=0;
const int rxAddr = 5000;
const int rxAddr1 = 2001;
const int rxAddr2 = 2002;
const int rxAddr3 = 2003;
const int rxAddr4 = 2004;
const int shinkAddr = 1000;
const int bcAddr = 7000;
int ChAddr = 0;



int a = 0, xMili = 0, xSecond = 0, xMinute = 0, xHour = 0, currentSecond = 0, currentMilis=0;
unsigned long wTime;
int start=0;
int flag_reset_time = 0;
void setup()
{

  xMili = 0; xSecond = 0; xMinute = 0; xHour = 0;


  while (!Serial);
  Serial.begin(9600);


  radio.begin();
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
          if(rtext[2]==1)
          ChAddr = rxAddr1 ;
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
     if(pesan[0] == "R"){
        xSecond = 0;
        xMinute = 0;
        xHour = 0;
        xMili = 0;
        flag_reset_time = 1;
        ChAddr = sendAddr ;
        ch_status =1; 
      }
   
}


void loop()
{



  currentSecond = xSecond;
  countTime();

  if(flag_reset_time ==0)
    {
      tapListening();
    }
  else {
      tapListening(ChAddr);
    }
    
    if((flag_reset_time==0) && (xSecond == 15) && (xMili==1)&&(nodeId==1))
        {
          char text[25];
          String ResetTime = "R#";
          ResetTime.concat(nodeId);
          transmit(ResetTime);
          Serial.println(ResetTime) ;
        }
         
 
  
   
  delay(1);
}

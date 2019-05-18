#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const int nodeId=2;
const int chId=1;
const int statusCH=0;
const int rxAddr = 5000;
const int rxAddr1 = 2000;
const int rxAddr2 = 3000;
const int rxAddr3 = 4000;
int sendAddr3 = 5000;
int flag_reset_time = 0;


int a = 0, xMili = 0, xSecond = 0, xMinute = 0, xHour = 0, currentSecond = 0, currentMilis=0;
unsigned long wTime;
int flag = 1; int start=0;
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
    // int a = 0;
     char rtext[25];
    radio.read(&rtext, sizeof(rtext));
    //Serial.println("Pesannya ?");
    
    //main IF
    // kode pesan = rText[1];
  //  jika pesan == C
   //  (adress kirim diganti ke 
    Serial.println(rtext);
  }
}

void tapListening(int addres) {

   radio.openReadingPipe(0, addres);
   radio.startListening();
  
  if (radio.available())
  {
   Serial.println("Listening");
   Serial.println(addres);
    // int a = 0;
     char rtext[25];
    radio.read(&rtext, sizeof(rtext));
    //Serial.println("Pesannya ?");
     if(strcmp(rtext,"R")==0){

          Serial.println(rtext);
          Serial.println("INI");
          xMili = 0 ; 
          xSecond = 0; 
          xMinute = 0; 
          xHour = 0;
          flag_reset_time =1;
         
        }
    //main IF
    // kode pesan = rText[1];
  //  jika pesan == C
   //  (adress kirim diganti ke 
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
  radio.stopListening();
  radio.openReadingPipe(0, sendAddr);
  radio.startListening();

   if(strcmp(pesan,"R")==0){
      Serial.println(pesan);
      Serial.println("masuk sini");
          flag_reset_time =1;
          xMili = 0 ; 
          xSecond = 0; 
          xMinute = 0; 
          xHour = 0;
    }


}
//----------------------------------------------- Blok Start TIme--------------------------------------------//
void startTime_send(int sendAddr, String Message)
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
  radio.stopListening();
  radio.openReadingPipe(0, sendAddr);
  radio.startListening();

       flag_reset_time =1;
          xMili = 0 ; 
          xSecond = 0; 
          xMinute = 0; 
          xHour = 0;

}
void startTime_listen(int addres) {

   radio.openReadingPipe(0, addres);
   radio.startListening();
  //  Serial.println("IF1");
    if (radio.available())
  {
     Serial.println("Listening");
     Serial.println(addres);

    // int a = 0;
     char rtext[25];
    radio.read(&rtext, sizeof(rtext));
    //Serial.println("Pesannya ?");
          xMili = 0 ; 
          xSecond = 0; 
          xMinute = 0; 
          xHour = 0;
          flag_reset_time =1;
 
    //main IF
    // kode pesan = rText[1];
  //  jika pesan == C
   //  (adress kirim diganti ke 
    Serial.println(rtext);
  }
}

//----------------------------------------------- Blok Start TIme--------------------------------------------//
void loop()
{



  currentSecond = xSecond;
  countTime();

  if(flag_reset_time==0){
      startTime_listen(rxAddr1);
     
    }
  else{ 
    tapListening();
    }
    if (((xSecond % 3) == 0)&& (flag==1) && (xMili ==1 )) {

      char text[25];
      String myMessage = String(xHour, DEC) + ":" + String(xMinute, DEC) + ":" + String(xSecond, DEC);
      
       myMessage.concat("#Jalak#" );
       transmit(myMessage);
       myMessage.concat("Sent:" );
       Serial.println(myMessage);

        if((nodeId==1)&& (flag_reset_time == 0)&&(xSecond ==15)){
          char text[25]= "Restart Time";
          startTime_send(rxAddr1,text);
          Serial.println("Masuk If");
          }
    }       
 
  
   
  delay(1);
}

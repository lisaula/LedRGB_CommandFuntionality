#include <BlinkTask.h>
#include <Debouncer.h>
#include <DelayRun.h>
#include <Dimmer.h>
#include <FrequencyTask.h>
#include <Heartbeat.h>
#include <Rotary.h>
#include <SoftPwmTask.h>
#include <SoftTimer.h>
#include <Task.h>
#include <TonePlayer.h>

int state=2;
int r=0;
int g=0;
int b=0;

int rpin = 11;
int gpin = 9;
int bpin = 10;

int index=0;
char newChar=-1;
char command[20];

void processCommand(Task * me);
void checkState(Task * me);
void clearCommand();

Task taskProcessCommand(100,processCommand);
Task taskCheckState(100,checkState);

void setup() {
    Serial.begin(9600);
    pinMode(rpin,OUTPUT);
    pinMode(gpin,OUTPUT);
    pinMode(bpin,OUTPUT);

    SoftTimer.add(&taskProcessCommand);
    SoftTimer.add(&taskCheckState);
    clearCommand();
}

void clearCommand(){
  for(int i=0; i<20; i++){
    command[i]=0;
  }
  index=0;
}

void saveColors(){
  int indexColor=9;
  char colorC[4];
  for(int i=0;i<3;i++){
  int indexNumero=0;
    while(command[indexColor]!=','){
      colorC[indexNumero] = command[indexColor];
      indexNumero++;
      indexColor++;
    }
    indexColor++;
    colorC[indexNumero] = '\0';
    switch(i){
      case 0: r = atoi(colorC); Serial.print("r: "); Serial.println(colorC);break;
      case 1: g = atoi(colorC); Serial.print("g: "); Serial.println(colorC);break;
      case 2: b = atoi(colorC); Serial.print("b: "); Serial.println(colorC);break;
    }
  }
}

void processCommand(Task * me) {
  clearCommand();
  while(Serial.available()>0){
      newChar = Serial.read();
      command[index] = newChar;
      index++;
    }
    if(index>0){
      command[index] = '\0';
      Serial.println(command);
      char commandTag[9];
      int indexTag=0;
      while(indexTag<8){
        commandTag[indexTag]=command[indexTag];
        indexTag++;
      }
      commandTag[indexTag]='\0';

      if(strcmp(commandTag,"setcolor")==0){
        state = 0;
        saveColors();
      }else if(strcmp(commandTag,"turnoff")==0){
        state = 1;
      }else if(strcmp(commandTag,"cycle")==0){
        state = 2;
      }else{
        Serial.println("Commando no aceptado");
      }
    }
  }
void checkState(Task * me){
  switch(state){
    case 0: //setcolor 
      analogWrite(rpin,r);
      analogWrite(gpin,g);
      analogWrite(bpin,b);
    break;
    case 1: //turnoff
      analogWrite(rpin,255);
      analogWrite(gpin,255);
      analogWrite(bpin,255);
    break;
    case 2: //cycle
      analogWrite(rpin,random(0,254));
      analogWrite(gpin,random(0,254));
      analogWrite(bpin,random(0,254));
      delay(1000);
    break;
  }
}

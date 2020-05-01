#include <TroykaOLED.h>

#define S1 7
#define S2 6
#define PAD_1 0
#define PAD_4 5
#define PAD_2 1
#define PAD_3 4

TroykaOLED myDisp(0x3C);

bool mainMenu;
bool needUpdate;
int pos;
int myNum[4];
int ardNum[4];
String answer;
bool win;

void setup() {
    randomSeed(analogRead(A0)); 
    resetGame();
}

void resetGame() {
    
    myDisp.begin();
    myDisp.setFont(alphabet6x8);
    myDisp.print("Press S1 to Start", 12, 5);
    myDisp.print("Bulls & Cows", 25, 20);
    myDisp.print("(c) 2020 @pazhukov", 5, 50);
    memset(myNum, 0, sizeof(myNum));
    memset(ardNum, -1, sizeof(ardNum)); 
    win = false;
    numCPU();
    pos=0;
    answer=""; 
}

void loop() {
  int btn_s1=digitalRead(S1);
  int btn_s2=digitalRead(S2);
  int btn_pad_1=digitalRead(PAD_1);
  int btn_pad_4=digitalRead(PAD_4);
  int btn_pad_2=digitalRead(PAD_2);
  int btn_pad_3=digitalRead(PAD_3);
 
  if(btn_s1==0 && win) {
    resetGame();  
  }

  if(btn_s1==0 && !win) {
    mainMenu=true;  
  }  

  if(btn_s2==0 && !win) {
    chectkBullsAndCows();
    drawGameMenu();  
  }  

  if(mainMenu) {
    pos=0;
    drawGameMenu();
    mainMenu=false; 
  } 

  if(btn_pad_1==0 && !win){
    pos++;
    if(pos>3){
      pos=0;
    }
    needUpdate=true; 
  } 

  if(btn_pad_2==0){
    for(int i=0; i<4; i++){
      myNum[i]=ardNum[i];          
    }
    answer="!GAME  OVER!";
    drawGameMenu();
    win=true;
  }

  if(btn_pad_3==0 && !win){
    myNum[pos]++;
    if(myNum[pos]>9){
      myNum[pos]=0;  
    }
    needUpdate=true;    
  }

  if(btn_pad_4==0 && !win){
    myNum[pos]--;
    if(myNum[pos]<0){
      myNum[pos]=9;  
    }
    needUpdate=true;    
  }
  
  delay(500);
  if(needUpdate && !win){
    drawGameMenu();
    needUpdate=false;  
  }
}

void drawGameMenu() {
  myDisp.clearDisplay();
  myDisp.invertText(false);
  myDisp.setFont(alphabet6x8);
  myDisp.print("Enter your number", 14, 5); 
  myDisp.setFont(font12x10);
  if(win) {
    myDisp.print("WIN", 48, 24);   
  } else {
    for(int i=0; i<4; i++)
    {
      myDisp.invertText(false);
      if(i==pos){
        myDisp.invertText(true);   
      }
      myDisp.print(myNum[i], 24+i*24, 24);
    }
  }
  myDisp.invertText(false);
  myDisp.setFont(alphabet6x8);
  myDisp.print(answer, 30, 54); 
}

void numCPU() {
  ardNum[0]=int(random(1, 10));
  for(int i=1; i<4; i++){
    int k=-1;
    do{
      k=int(random(0, 10));
      int exit;
      for(int j=0; j<4; j++) {
        if(ardNum[j]==k){
          k=-1;
        }
      }    
    } while(k==-1);
    ardNum[i]=k;
  }
}

void chectkBullsAndCows(){
  int c=0;
  int b=0;
  for(int i=0; i<4; i++){
    if(myNum[i]==ardNum[i]){
      b++;
    }
  }
  for(int i=0; i<4; i++){
    for(int j=0; j<4; j++){
      if(i==j){
        continue;
      }
      if(myNum[i]==ardNum[j]){
        c++;
      }
    }
  }

  answer = String(myNum[0]) + String(myNum[1]) + String(myNum[2]) + String(myNum[3]) + " - B" + String(b) + " C" + String(c); 
  win=b==4;
}

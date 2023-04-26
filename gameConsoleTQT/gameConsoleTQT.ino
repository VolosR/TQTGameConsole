#include "menu.h"
#include "ship.h"
#include "comet.h"
#include "gameOver.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define left 0
#define right 47

// snake game variables :::::::::::::::::::::::::::::::::::::::::::::::::::::::
int size=1;
int y[120]={0};
int x[120]={0};
unsigned long currentTime=0;
int period=200;
int deb,deb2=0;
int dirX=1;
int dirY=0;
bool taken=0;
unsigned short colors[2]={0x48ED,0x590F}; //terain colors
unsigned short snakeColor[2]={0x9FD3,0x38C9};
bool chosen=0;
bool gOver=0;
int moves=0;
int playTime=0;
int foodX=0;
int foodY=0;
int howHard=0;
bool ready=1;
long readyTime=0;
int change=0;

//brakeout variables:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
float playerX=30;
int playerY=120;
int playerW=24;
int playerH=6;
float ballX=80;
float ballY=50;
float ballS=0.4;
int bXd=1;
int bYd=1;
float amount[4]={0.2,0.3,0.4,0.5};
float xs=0.4;
int remaining=16;
int level=1;


int enx[16]={4,29,54,79,104,4,29,54,79,104,18,43,68,93,43,68};
int enx2[16]={4,29,54,79,104,4,29,54,79,104,18,43,68,93,43,68};
int eny[16]={17,17,17,17,17,25,25,25,25,25,33,33,33,33,41,41};
int enc[16]={TFT_RED,TFT_RED,TFT_RED,TFT_RED,TFT_RED,TFT_GREEN,TFT_GREEN,TFT_GREEN,TFT_GREEN,TFT_GREEN,TFT_ORANGE,TFT_ORANGE,TFT_ORANGE,TFT_ORANGE,0x02F3,0x02F3};

//space game variables ::::::::::::::::::::::::::::::::::::::::::::::::::::::::
float shipX=60;  //ship X
int shipW=20;  //ship wifth
int shipH=36;   //ship Heigth
bool shipD=1;   //ship direction
int directions[2]={-1,1};
float shipS=0.5;  //ship speed
int score=0;
int health=30;

int enemyX[5]={0};
float enemyY[5]={30,0,-39,-64,-116,};
int enemyHe[5]={2,2,2,2,2};
int enemyW=16;
int enemyH=16;
float enemyS=0.3;
float hard=0;

int dotX[30];
float dotY[30];

int bulX[10]={160};
int bulY[10]={130};
int bCount=0;

int game=0;  //  0 is snake, 1 is brakeout, 2 is space


void setup() { 
    pinMode(left,INPUT_PULLUP);
    pinMode(right,INPUT_PULLUP);
   
    tft.init();
    tft.setRotation(2);
    sprite.createSprite(128,128);
    tft.setSwapBytes(true);
    sprite.setSwapBytes(true);
    tft.pushImage(0,0,128,128,menu);
    tft.fillCircle(16,70+(game*16),4,0xC018);


    while(digitalRead(right)==1)
    {
        if(digitalRead(left)==0){
          if(deb==0)
          {deb=1;
          game++;
          if(game==3) game=0;
          tft.pushImage(0,0,128,128,menu);
          tft.fillCircle(16,70+(game*16),4,0xC018);
          }
        }   else deb=0;

    }

    y[0]=random(5,14);
    getFood();
      
    dirX=1;
    dirY=0;

    for(int i=0;i<5;i++)
    enemyX[i]=(random(0,6)*20)+1;

    for(int i=0;i<30;i++){
    dotX[i]=random(10,120);
    dotY[i]=random(10,120);
    }
}

void getFood()//.....................getFood -get new position of food
{
    foodX=random(0,16);
    foodY=random(0,16);
    taken=0;
    for(int i=0;i<size;i++)
    if(foodX==x[i] && foodY==y[i])
    taken=1;
    if(taken==1)
    getFood();
}

void runSnake()//...............................run function
{
      for(int i=size;i>0;i--)
       {x[i]=x[i-1];    
       y[i]=y[i-1];}    

     x[0]=x[0]+dirX;
     y[0]=y[0]+dirY;

    if(x[0]==foodX && y[0]==foodY)
    {size++; getFood();  period=period-1;}
     
     sprite.fillSprite(TFT_BLACK);
  
     checkGameOver();
     if(gOver==0){
     sprite.drawRect(0,0,128,128,0x02F3);     
     for(int i=0;i<size;i++){
     sprite.fillRoundRect(x[i]*8,y[i]*8,8,8,2,snakeColor[0]); 
     sprite.fillRoundRect(2+x[i]*8,2+y[i]*8,5,5,2,snakeColor[1]); 
     }
     sprite.fillRoundRect(foodX*8+1,foodY*8+1,6,6,1,TFT_RED); 
     sprite.fillRoundRect(foodX*8+3,foodY*8+3,3,3,1,0xFE18); 
    }
else    
{sprite.fillSprite(TFT_BLACK);
 sprite.pushImage(0,0,128,128,gameOver);
 sprite.drawString("SCORE: "+String(size),40,96,2);}    
 sprite.pushSprite(0,0);
}     

void checkGameOver()//..,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,check game over
{
if(x[0]<0 || x[0]>=16 || y[0]<0 || y[0]>=16 )
gOver=true;
for(int i=1;i<size;i++)
if(x[i]==x[0] && y[i]==y[0])
gOver=true;
}

void runBrakeout() //.............................brakeout game run........................
{
  if(gOver==0){
  sprite.fillSprite(TFT_BLACK);
  sprite.drawRect(0,0,128,129,0x02F3); 
  sprite.drawString("SCO: "+String(score),4,4);
  sprite.drawString("LVL:"+String(level),90,4);
  for(int i=0;i<30;i++)
   {
    dotY[i]=dotY[i]+0.2;
    if(dotY[i]>130)
    {dotX[i]=random(10,120);
    dotY[i]=random(10,200)*-1;}
    sprite.drawPixel(dotX[i],dotY[i],0x5AEB);  
   } 

  for(int j=0;j<16;j++){
  if(ballX>enx[j] && ballX<enx[j]+20 && ballY>eny[j] && ballY<eny[j]+5) 
  {enx[j]=-30; score++; bXd=bXd*-1; bYd=bYd*-1; xs=amount[random(0,4)]; 
  if(score%16==0) brakeNewLevel();}
  sprite.fillRect(enx[j],eny[j],20,4,enc[j]);
  }
  sprite.fillRect(playerX,playerY,playerW,playerH,TFT_GREEN);

  ballX=ballX+(bXd*(xs+(level*0.1)));
  ballY=ballY+(bYd*(ballS+(level*0.1)));

  if(ballX<2 || ballX>126)
  bXd=bXd*-1;

  if(ballY<1)
  bYd=bYd*-1;

  if(ballY>128)
  gOver=1; 

  if(ballY>=playerY && ballY<playerY+6 && ballX>=playerX && ballX<playerX+20)
  bYd=bYd*-1;

  sprite.fillCircle(ballX,ballY,2,TFT_WHITE);
  }
  else
  {sprite.pushImage(0,0,128,128,gameOver);
  sprite.drawString("SCORE: "+String(score),40,96,2);}
sprite.pushSprite(0,0);
}

void brakeNewLevel()
{
 level++;
 for(int i=0;i<16;i++)
 enx[i]=enx2[i];
 ballX=random(30,60);
 ballY=60;
 delay(2000);
}

// spae game run........................................................................
void runSpace()
{
if(gOver==0){  
sprite.fillSprite(TFT_BLACK);
for(int i=0;i<30;i++)
{
  dotY[i]=dotY[i]+0.2;
  if(dotY[i]>130)
  {dotX[i]=random(10,120);
  dotY[i]=random(10,200)*-1;}
  sprite.drawPixel(dotX[i],dotY[i],0x5AEB);  
}

for(int i=0;i<10;i++)
{
  if(bulY[i]<128) 
  bulY[i]--;
  if(bulY[i]<0)
    {
    bulY[i]=130;
    }
  for(int j=0;j<5;j++)
  {
    if(bulX[i]>enemyX[j] && bulX[i]<enemyX[j]+16 && bulY[i]<enemyY[j]+16 && bulY[i]>enemyY[j]) //colision buton with enemy
    {bulY[i]=130;
    bulX[i]=200;
    score++;
    enemyHe[j]--;
    if(enemyHe[j]==0)
    {score=score+5; enemyX[j]=(random(0,6)*20)+1; enemyY[j]=(random(10,240)*-1); enemyHe[j]=2;}
    }
  }  
  sprite.fillCircle(bulX[i],bulY[i],2,TFT_RED);  
}

sprite.pushImage(shipX,96,shipW,shipH,ship);

for(int i=0;i<5;i++)
{
  enemyY[i]=enemyY[i]+enemyS+(score/800.00);
  sprite.pushImage(enemyX[i],enemyY[i],enemyW,enemyH,comet); 
  if(enemyY[i]>130) {enemyX[i]=(random(0,6)*20)+1; enemyY[i]=(random(10,240)*-1);}
  if(shipX>=enemyX[i] && shipX<enemyX[i]+16 && enemyY[i]+16>96 && enemyY[i]<126)
  {health=health-6; enemyX[i]=(random(0,6)*20)+1; enemyY[i]=(random(10,240)*-1);
  if(health<=0) gOver=1;}
}

sprite.fillRect(90,4,health,7,TFT_GREEN);
sprite.fillRect(90+health,4,30-health,7,TFT_RED);
sprite.drawString(String(score),8,2,2);
}
else
{sprite.pushImage(0,0,128,128,gameOver);
sprite.drawString("SCORE: "+String(score),40,96,2);}
sprite.pushSprite(0,0);
}


void loop() {

if(game==1){ 
  if(millis()>currentTime+period) 
  {runSnake(); currentTime=millis();} 
  if(millis()>readyTime+100 && ready==0) 
  {ready=1;} 

  if(ready==1){
  if(digitalRead(left)==0){

  if(deb==0)
  {deb=1;
  if(dirX==1 && change==0) {dirY=dirX*-1; dirX=0; change=1;}
  if(dirX==-1 && change==0) {dirY=dirX*-1; dirX=0;change=1; }
  if(dirY==1 && change==0) {dirX=dirY*1; dirY=0; change=1;}
  if(dirY==-1 && change==0) {dirX=dirY*1; dirY=0; change=1;}
  change=0;
  ready=0;
  readyTime=millis();
  }}else{ deb=0;}}

  if(ready==1){
    if(digitalRead(right)==0)
    {
    if(deb2==0)
    {deb2=1;
    if(dirX==1 && change==0) {dirY=dirX*1; dirX=0; change=1;}
    if(dirX==-1 && change==0) {dirY=dirX*1; dirX=0;change=1; }
    if(dirY==1 && change==0) {dirX=dirY*-1; dirY=0; change=1;}
    if(dirY==-1 && change==0) {dirX=dirY*-1; dirY=0; change=1;}
      change=0;
      ready=0;
    readyTime=millis();
  }}else {deb2=0;}}}

if(game==0)
{
  if(digitalRead(left)==0 && playerX>2) playerX=playerX-0.6; 
  if(digitalRead(right)==0 && playerX<126) playerX=playerX+0.6; 
  runBrakeout();
}  

if(game==2)
{
  if(digitalRead(left)==0){
    if(deb==0)
    {deb=1;
    shipD=!shipD;}
  }else deb=0;

  if(digitalRead(right)==0){
    if(deb2==0)
    {deb2=1;
    bCount++;
  if(bCount==10)
  bCount=0;
  bulX[bCount]=shipX+10;
  bulY[bCount]=90;
  }}else deb2=0;

  shipX=shipX+(shipS*directions[shipD]);
  if(shipX<=0 || shipX>=128-shipW)
  shipD=!shipD;  
  runSpace();
}

}

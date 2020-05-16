#include"TFT_eSPI.h"
TFT_eSPI tft;

float time = 0;
const float delta_t = 0.3;

float y = 0;
const float x = 300;
float pre_y = 0;
float velocity = 0;
float velocity_a = 0;
float force = 0;
int score = 0;

float k_air = 0.1;
float gravity = 9.8;
int flytime = 0;
float ext_force = 0;

float attack_y = 0;
float attack_x = 0;
float attack_width = 80;
float attack_height = 50;
float attack_speed = 5;
float attack_none = 100;

boolean gamestart = false;
const int OPENING = 0;
const int GAME = 1;
const int ENDING = 2;

int gamemode = OPENING;

const int width = 320;
const int height = 240;

void setup() {
    tft.begin();
    tft.setRotation(3);
 
    tft.fillScreen(TFT_WHITE); // fills entire the screen with colour red

    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
}
 

void reset(){
  y = height/2;
  flytime = 0;
  ext_force = 0;
  velocity = 0;
  velocity_a = 0;
  force = 0;
  attack_x = -200;
  attack_y = random(attack_none, height - attack_none);  
  score = 0;
}


void opening(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.drawString("Please Click! Click! Click!", width/2 - 80, height/2);
}

void ending(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.drawString("Game Over", width/2 - 20, height/2);
}


void game(){
//  background(255);

  time += delta_t;

  // move nekoman (by verlet method)
  force = gravity - k_air*velocity + ext_force;
  velocity_a = velocity + 0.5*force*delta_t;
  y +=  velocity_a * delta_t;
  force = gravity - k_air*velocity_a + ext_force;
  velocity = velocity_a + 0.5*force*delta_t;

  // move target
  attack_x += attack_speed;
  if(attack_x > width){
    score++;
    attack_x = 0;
    attack_y = random(attack_none, height - attack_none);
  }

  // draw objects
  tft.fillScreen(TFT_WHITE);
  tft.drawCircle(x, y, 10, TFT_BLACK);

  tft.drawRect(attack_x - attack_width/2, -1, attack_width, attack_y - attack_height, TFT_BLACK);
  tft.drawRect(attack_x - attack_width/2, attack_y + attack_height, attack_width, height, TFT_BLACK);

  // ext force
  if(flytime > 0){
    flytime--;
    ext_force = -(float)(flytime) * 30;
  }else{
    ext_force = 0;
  }

  // detect
  if(y < 0){
    gamemode = ENDING;
  }

  if(y > height){
    gamemode = ENDING;
  }

  if(x > attack_x - attack_width/2 && x < attack_x + attack_width/2){
    if(y > attack_y + attack_height || y < attack_y - attack_height){
      gamemode = ENDING;
    }
  }  
}

void check_switch(){
  // put your main code here, to run repeatedly:
  if (digitalRead(WIO_KEY_A) == LOW || digitalRead(WIO_KEY_B) == LOW || digitalRead(WIO_KEY_C) == LOW) {
    if(gamemode == GAME){
      flytime = 2;
    }

    if(gamemode == OPENING){
      gamemode = GAME;
      reset();
    }

    if(gamemode == ENDING){
      gamemode = GAME;
      reset();
    }
  }
}

void loop() {
  check_switch();

  switch (gamemode){
    case OPENING:
      opening();
      break;
    case GAME:
      game();
      break;
    case ENDING:
      ending();
      break;
    default:
      break;
  }
}

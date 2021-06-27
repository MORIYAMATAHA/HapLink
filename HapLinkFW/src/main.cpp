#include <Arduino.h>
#include "AllLib.h"
#include "PinConfig.h"

Encoder L_R_ENC(L_R_enc_A, L_R_enc_B);
Encoder L_L_ENC(L_L_enc_A, L_L_enc_B);
Encoder R_R_ENC(R_R_enc_A, R_R_enc_B);
Encoder R_L_ENC(R_L_enc_A, R_L_enc_B);


/*------モータ駆動関数------------*/
void motor_L_R(float p1)
{
  p1 = constrain(p1, -1.0f, 1.0f);
  if(p1 > 0)
  {
    ledcWrite(1, 4095);
    ledcWrite(0, (float)((1 - p1) * 4095.0));  
  }
  else if (p1 < 0)
  {
    ledcWrite(0, 4095);
    ledcWrite(1, (float)((1 + p1) * 4095.0));  
  }
  else if( p1 == 0)
  {
    ledcWrite(1, 0);
    ledcWrite(0, 0);  
  }
}

void motor_L_L(float p2)
{
  p2 = constrain(p2, -1.0f, 1.0f);
  if(p2 > 0)
  {
    ledcWrite(3, 4095);
    ledcWrite(2, (float)((1 - p2) * 4095.0));  
  }
  else if(p2 < 0)
  {
    ledcWrite(2, 4095);
    ledcWrite(3, (float)((1 + p2) * 4095.0));  
  }
  else if(p2 == 0)
  {
    ledcWrite(2, 0);
    ledcWrite(3, 0);    
  }
}

void motor_R_R(float p3)
{
  p3 = constrain(p3, -1.0f, 1.0f);
  if(p3 > 0)
  {
    ledcWrite(4, 4095);
    ledcWrite(5, (float)((1 - p3) * 4095.0));  
  }
  else if( p3 < 0)
  {
    ledcWrite(5, 4095);
    ledcWrite(4, (float)((1 + p3) * 4095.0));  
  }
  else if( p3 == 0)
  {
    ledcWrite(4, 0);
    ledcWrite(5, 0);    
  }
}

void motor_R_L(float p4)
{
  p4 = constrain(p4, -1.0f, 1.0f);
  if(p4 > 0)
  {
    ledcWrite(7, 4095);
    ledcWrite(6, (float)((1 - p4) * 4095.0));  
  }
  else if(p4 < 0)
  {
    ledcWrite(6, 4095);
    ledcWrite(7, (float)((1 + p4) * 4095.0));  
  }
  else if(p4 == 0)
  {
    ledcWrite(6, 0);
    ledcWrite(7, 0);    
  }
}

void encoder_debug()
{
  float newPosition_L_R = L_R_ENC.read();
  float newPosition_L_L = L_L_ENC.read();
  float newPosition_R_R = R_R_ENC.read();
  float newPosition_R_L = R_L_ENC.read();

  Serial.println(newPosition_L_R);
  Serial.println(newPosition_L_L);
  Serial.println(newPosition_R_R);
  Serial.println(newPosition_R_L);
  Serial.println(" ");

  delay(500);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  ledcSetup(0, 20000, 12);
  ledcAttachPin(L_R_FIN, 0);
  ledcSetup(1, 20000, 12);
  ledcAttachPin(L_R_RIN, 1);
  ledcSetup(2, 20000, 12);
  ledcAttachPin(L_L_FIN, 2);
  ledcSetup(3, 20000, 12);
  ledcAttachPin(L_L_RIN, 3);
  ledcSetup(4, 20000, 12);
  ledcAttachPin(R_R_FIN, 4);
  ledcSetup(5, 20000, 12);
  ledcAttachPin(R_R_RIN, 5);
  ledcSetup(6, 20000, 12);
  ledcAttachPin(R_L_FIN, 6);
  ledcSetup(7, 20000, 12);
  ledcAttachPin(R_L_RIN, 7);
}

byte incomingbyte[4];
int incomingbyte_ = 0;
float newPosition_L_R ;
float newPosition_L_L ;
float newPosition_R_R ;
float newPosition_R_L ;

float goal_R_L = 0;
float goal_R_R = 0;
float goal_L_L = 0;
float goal_L_R = 0;

float newPosition_L_R_p;
float newPosition_L_L_p;
float newPosition_R_R_p;
float newPosition_R_L_p;

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available() > 3){
    Serial.readBytes(incomingbyte, 4);
      goal_R_L = incomingbyte[0] * 100;
      goal_R_R = incomingbyte[1] * 100;
      goal_L_L = incomingbyte[2] * 100;
      goal_L_R = incomingbyte[3] * 100;
  }
  /*
  
    if(Serial.available() > 0){
      Serial.read();
    incomingbyte_ += 10000;
  }
*/  
  
  float newPosition_L_R = L_R_ENC.read();
  float newPosition_L_L = L_L_ENC.read();
  float newPosition_R_R = R_R_ENC.read();
  float newPosition_R_L = R_L_ENC.read();

//encoder_debug();

/*
      goal_R_L = incomingbyte_;
      goal_R_R = incomingbyte_;
      goal_L_L = incomingbyte_;
      goal_L_R = incomingbyte_;
*/
/*
      goal_R_L = incomingbyte[0];
      goal_R_R = incomingbyte[1];
      goal_L_L = incomingbyte[2];
      goal_L_R = incomingbyte[3];

  float p_gain = 0.0005;
  float d_gain = 1000;

/*横ズレ*/

/*
  float p_gain = 0.00002;
  float d_gain = 0.001;
*/ 

  float p_gain = 0.0005;
  float d_gain = 1000;
  float answer1 = p_gain * (goal_R_L - newPosition_R_L) - d_gain * (newPosition_R_L - newPosition_R_L_p);
  float answer2 = p_gain * (-goal_R_R - newPosition_R_R) - d_gain * (newPosition_R_R - newPosition_R_R_p);
  float answer3 = p_gain * (goal_L_L - newPosition_L_L) - d_gain * (newPosition_L_L - newPosition_L_L_p);
  float answer4 = p_gain * (-goal_L_R - newPosition_L_R) - d_gain * (newPosition_L_R - newPosition_L_R_p);
  
  motor_R_L(answer1);
  motor_R_R(answer2);
  motor_L_L(answer3);
  motor_L_R(answer4);

  newPosition_L_R_p = L_R_ENC.read();
  newPosition_L_L_p = L_L_ENC.read();
  newPosition_R_R_p = R_R_ENC.read();
  newPosition_R_L_p = R_L_ENC.read();

}
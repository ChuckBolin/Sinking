/************************************************************************
CSpeed.h - Written by Chuck Bolin, March 2008
Revised April 11, 2008
  //initialization
  CSpeed g_Speed;
  g_Speed.SetLimits(-6, 22);
  g_Speed.SetMaxAcceleration(0.35);

  //change speed...2 ways
  g_Speed.SetSpeed(4);                   //sets speed in knots
  g_Speed.SetMOT(CSPEED_STOP);           //sets Motor Order Telegraph 
  g_Speed.SetMOT(CSPEED_AHEAD_STANDARD);
************************************************************************/

#ifndef CSPEED_H
#define CSPEED_H

#include "CLog.h"

//motor order telegraph commands for SetMOT(...)
const int CSPEED_STOP = 0;
const int CSPEED_AHEAD1 = 1;
const int CSPEED_AHEAD2 = 2;
const int CSPEED_AHEAD_STANDARD = 3;
const int CSPEED_AHEAD_FULL = 4;
const int CSPEED_AHEAD_FLANK = 5;
const int CSPEED_BACK1 = 6;
const int CSPEED_BACK2 = 7;

class CSpeed{
public:
  CSpeed();
  void SetMaxAcceleration(float acc){m_maxAcc = acc;}
  void SetLimits(float min, float max);
  void SetSpeed(float speed);    //knots used during game play
  void SetInitialSpeed(float speed){m_speed = speed; m_commandSpeed = speed;}
  void SetMOT(int speedCommand); //motor order telegraph
  void Update(double timeDifference);
  float GetSpeed(){return m_speed;}
  float GetCommandSpeed(){return m_commandSpeed;}
  void SetMaxSpeed(float maxSpeed){m_maxSpeed = maxSpeed;}

  
private:
  float m_commandSpeed;
  float m_speed;
  float m_maxAcc;
  float m_maxSpeed;
  float m_minSpeed;
  float m_stepForward; //calculated to determine MOT speed equivalents
  float m_stepReverse;

};

#endif
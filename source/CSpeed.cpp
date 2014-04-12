#include "..\includes\CSpeed.h"

CSpeed::CSpeed(){
  m_commandSpeed = 0;
  m_speed = 0;
  m_maxSpeed = 25;
  m_minSpeed = -25;
  m_maxAcc = 0.25;

  m_stepForward = m_maxSpeed / 5;
  m_stepReverse = m_minSpeed / 2;
}

void CSpeed::Update(double timeDifference){
  
  static double timeAcc = 0;
  timeAcc += timeDifference;
  if(timeAcc > 0.1){

  float speedDiff = m_commandSpeed - m_speed;

  if(speedDiff > 0.05){
    m_speed += timeDifference * m_maxAcc;
    if(m_speed > m_maxSpeed)
      m_speed = m_maxSpeed;
  }
  else if(speedDiff < -.05){
    m_speed -= timeDifference * m_maxAcc;
    if(m_speed < m_minSpeed)
      m_speed = m_minSpeed;
  }
  else
  
  m_speed = m_commandSpeed;
  
  //CLog *pLog = CLog::Instance();
  //pLog->Log("UpdatespeedDiff", speedDiff);
 // pLog->Log("Update m_speed", m_speed);
  }
}

void CSpeed::SetLimits(float min, float max){
  if(min >= max)
    return;

  m_maxSpeed = max;
  m_minSpeed = min;
  m_stepForward = m_maxSpeed / 5;
  m_stepReverse = m_minSpeed / 2;
}

void CSpeed::SetSpeed(float speed){
  if(speed > m_maxSpeed)
    speed = m_maxSpeed;
  else if(speed < m_minSpeed)
    speed = m_minSpeed;

  m_commandSpeed = speed;
  //CLog *pLog = CLog::Instance();
  //pLog->Log("CSpeedSetSpeed", speed);
  //pLog->Log("CSpeedCmdSpeed", m_commandSpeed);
}

void CSpeed::SetMOT(int speedCommand){
  switch(speedCommand){
    case CSPEED_STOP:
      SetSpeed(0);
      break;
    case CSPEED_AHEAD1:
      SetSpeed(m_stepForward);
      break;
    case CSPEED_AHEAD2:
      SetSpeed(m_stepForward * 2);
      break;
    case CSPEED_AHEAD_STANDARD:
      SetSpeed(m_stepForward * 3);
      break;
    case CSPEED_AHEAD_FULL:
      SetSpeed(m_stepForward * 4);
      break;
    case CSPEED_AHEAD_FLANK:
      SetSpeed(m_stepForward * 5);
      break;
    case CSPEED_BACK1:
      SetSpeed(m_stepReverse);
      break;
    case CSPEED_BACK2:
      SetSpeed(m_stepReverse * 2);
      break;
  }

}
/************************************************************************
CHeading.h - Written by Chuck Bolin, March 2008
Revised April 11, 2008
************************************************************************/
#ifndef CHEADING_H
#define CHEADING_H

#include <cmath>
#include "CLog.h"

const int CHEADING_AMIDSHIPS = 0;
const int CHEADING_LEFT_EASY = 1;
const int CHEADING_LEFT_HARD = 2;
const int CHEADING_RIGHT_EASY = 3;
const int CHEADING_RIGHT_HARD = 4;
const float CHEADING_PI = 3.141592654;

class CHeading{
public:
  CHeading();
  void SetHeading(float heading);
  void SetRudder(int rudder);
  void SetMaxTurnrate(float turnrate);
  void Update(double timeDifference, float speed);  
  float GetHeading(){return m_heading;}
  float GetRudderAngle(){return m_rudderAngle;}
  float GetCommandHeading(){return m_commandHeading;}  
  double GetPositionX(){return m_posX;}
  double GetPositionY(){return m_posY;}
  void SetPosition(double x, double y){m_posX = x; m_posY = y;}
  //utility functions -
  float ConvertDegreesToRadians(float deg);
  float ConvertRadiansToDegrees(float rad);
private:
  float m_heading;
  float m_headingRadians;
  float m_commandHeading;
  float m_maxTurnRate;
  int m_commandRudder;
  float m_rudderAngle; 
  float m_actualRudderAngle;
  double m_posX;
  double m_posY;
  float m_maxRudder;
  bool m_bRudder;



};

#endif

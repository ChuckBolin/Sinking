#ifndef CTORPEDO_H
#define CTORPEDO_H

#include <cmath>
const int CMAX_DISTANCE = 4000;

class CTorpedo{
public:
  CTorpedo(float cmdHeading, float heading, float x, float y);
  void Update(double timeDifference);
  bool IsRunning(){return m_bRunning;}
  float GetDistance(){return m_distance;}
  float GetSpeed(){return m_speed;}
  float GetCmdHeading(){return m_cmdHeading;}
  float GetHeading(){return m_heading;}
  void Kill(){m_bRunning = false;}
  float GetPositionX(){return m_posX;}
  float GetPositionY(){return m_posY;}
  float ConvertDegreesToRadians(float deg);
  float ConvertRadiansToDegrees(float rad);
  float m_posX;
  float m_posY;
private:
  float m_speed;
  float m_distance;
  bool m_bRunning;
  float m_cmdHeading;
  float m_heading;

};

#endif
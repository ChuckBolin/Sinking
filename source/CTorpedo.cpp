#include "..\includes\CTorpedo.h"

CTorpedo::CTorpedo(float cmdHeading, float heading, float x, float y){
  m_cmdHeading = cmdHeading;
  m_heading = heading;
  m_bRunning = true;
  m_speed = 46;
  m_posX = x;
  m_posY = y;
}

void CTorpedo::Update(double timeDifference){
  float diff = m_cmdHeading - m_heading;

  double radians = 0;
  //double yards = (m_speed * 2000)/3600;
  
  static double timeAcc = 0;
  timeAcc += timeDifference;
  if(timeAcc > 0.1){

    if(m_bRunning == true){
      //turn torpedo towards target angle
      if(diff > 0.1)
        m_heading += timeAcc * 8;
      else if(diff < -0.1)
        m_heading -= timeAcc * 8;
      else
        m_heading = m_cmdHeading;

      //update position
      radians = ConvertDegreesToRadians(m_heading);
      m_posX += m_speed * timeAcc  * cos(radians);
      m_posY += m_speed * timeAcc  * sin(radians);
      m_distance = m_speed * (2000 / 3600) * timeAcc;
    }
    timeAcc = 0;
  }

  //torpedo ran out of fuel
  if(m_bRunning == true && m_distance > CMAX_DISTANCE)
    m_bRunning = false;
}

//compass degrees...math radians
float CTorpedo::ConvertDegreesToRadians(float deg){
    float rad = 450 - deg;
    rad = rad * 3.141592654/180;

    return rad;  
}

//compass degrees...math radians
float CTorpedo::ConvertRadiansToDegrees(float rad){
  float deg = rad  * 180/3.141592654;
  deg = 450 - deg;
  if (deg > 360)
    deg = deg - 360;
  if (deg < 0)
    deg = deg + 360;

  return deg;
}
  
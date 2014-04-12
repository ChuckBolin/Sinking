#include "..\includes\CHeading.h"

CHeading::CHeading(){
  m_heading = 0;
  m_commandHeading = 0;
  m_maxTurnRate = 1;
  m_rudderAngle = 0; 
  m_maxRudder = 30;
  m_bRudder = true;
  m_commandRudder = 0;
  m_actualRudderAngle = 0;
}

void CHeading::SetHeading(float heading){
  m_commandHeading = heading;
  m_bRudder = false;
  m_heading = heading;
  m_rudderAngle = 0;
 // CLog *pLog = CLog::Instance();
 // pLog->Log("setheading", m_commandHeading);
}

void CHeading::SetRudder(int rudder){
  if(rudder < 0 || rudder >4)
    return;
  m_bRudder = true;
  m_commandRudder = rudder;  

  /*if(rudder == 0){
    m_bRudder = false;
    m_commandHeading = m_heading;
  }*/
}

void CHeading::SetMaxTurnrate(float turnrate){
  m_maxTurnRate = turnrate;
}

void CHeading::Update(double timeDifference, float speed){
  static double timeAcc = 0;
  timeAcc += timeDifference;
  if(timeAcc > 0.1){
    //rudder command supercedes SetHeading function
    float rudderMult = 1;

    //determine new heading
    if(speed >= 0)
      m_heading += timeAcc * speed * m_rudderAngle * 0.025;//rudderMult;
    else
      m_heading += timeAcc * -speed * m_rudderAngle * 0.025;//rudderMult;

    if (m_heading > 360)
      m_heading = m_heading - 360;
    if (m_heading < 0)
      m_heading = m_heading + 360;

   m_posX += speed * timeAcc * cos(ConvertDegreesToRadians(m_heading));
   m_posY -= speed * timeAcc * sin(ConvertDegreesToRadians(m_heading));
  }
 // m_headingRadians = ConvertDegreesToRadians(m_heading);

  //2000 yards in 1 nm...3600 seconds per minute...timeDifference means part of a second
  /*
  float distance = speed * 2000/3600;
  float dx = (distance * cos(m_headingRadians)) * timeDifference ;
  float dy = (distance * sin(m_headingRadians)) * timeDifference ;
  m_posX += dx;
  m_posY += dy;
  */
/*
  //determine rudder
  float rudderDiff = 0;
  if(m_bRudder == true){
    if(m_commandRudder == CHEADING_AMIDSHIPS)
      m_actualRudderAngle = 0;
    else if(m_commandRudder == CHEADING_LEFT_EASY)
      m_actualRudderAngle = -10;
    else if(m_commandRudder == CHEADING_LEFT_HARD)
      m_actualRudderAngle = -25;
    else if(m_commandRudder == CHEADING_RIGHT_EASY)
      m_actualRudderAngle = 10;
    else if(m_commandRudder == CHEADING_RIGHT_HARD)
      m_actualRudderAngle = 25;

    //move rudder
    rudderDiff = m_rudderAngle - m_actualRudderAngle;
    if(rudderDiff < 0)      
      m_rudderAngle += timeDifference * 8.0f;
    else if(rudderDiff > 0)
      m_rudderAngle -= timeDifference * 8.0f;

  }
  else{
   // m_rudderAngle = headDiff / speed;
    
    if( m_rudderAngle > m_maxRudder)
      m_rudderAngle = m_maxRudder;
    else if(m_rudderAngle < -m_maxRudder)
      m_rudderAngle = -m_maxRudder;
    
  }
  }
  */
}

//compass degrees...math radians
float CHeading::ConvertDegreesToRadians(float deg){
    float rad = 450 - deg;
    rad = rad * CHEADING_PI/180;

    return rad;  
}

//compass degrees...math radians
float CHeading::ConvertRadiansToDegrees(float rad){
  float deg = rad  * 180/CHEADING_PI;
  deg = 450 - deg;
  if (deg > 360)
    deg = deg - 360;
  if (deg < 0)
    deg = deg + 360;

  return deg;
}

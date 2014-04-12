#include "..\includes\CVessel.h"

CVessel::CVessel(){

  m_maxSpeed = 25;
  m_minSpeed = -25;
  m_maxAcc = 0.25;
  m_bSubmerged = true;
  m_bAlive = true;
  m_damage = 0;
  m_bdropDepthCharge = false;
  m_TimerDepthCharge.initialize();

}

void CVessel::LoadVessel(float speed, float depth, float heading,
          int vesselType, std::string name, long posX, long posY,
          long destPosX, long destPosY, int numFwdTorpedos, int numAftTorpedos,
          float damage, unsigned int damageStatus, float distance){

  //load data regarding vessel
  m_speed = speed;
  m_depth = depth;
  m_cmdDepth = depth;
  m_heading = heading;
  m_cmdRudder = 0;
  m_vesselType = vesselType;
  m_shipName = name;
  m_posX = posX;
  m_posY = posY;
  m_destPosX = destPosX;
  m_destPosY = destPosY;
  m_numFwdTorpedos = numFwdTorpedos;
  m_numAftTorpedos = numAftTorpedos;
  m_damage = damage;
  m_damageGoal = 0;
  m_damageStatus = damageStatus;
  m_distance = distance;
  m_maxSpeed = 25;
  m_minSpeed = -25;
  m_maxAcc = 0.25;
  if(depth <= 25)
    m_bSubmerged = false;
  else
    m_bSubmerged = true;
  m_timerDamage.initialize();
  
  //particle effects
  m_bExplode = false;
  m_bSmoke = false;
  m_smokeDelay = 0;
  //read catalog and load 'class of ship' standard data

}

void CVessel::LoadCatalog(std::string className, float length, float beam,
          float draft, float range, float testDepth, float crushDepth,
          float maxSpeedSurface, float maxSpeedSubmerged, float displacement){

  m_className = className;
  m_length = length;
  m_beam = beam;
  m_draft = draft;
  m_range = range;
  m_testDepth = testDepth;
  m_crushDepth = crushDepth;
  m_maxSpeedSurface = maxSpeedSurface;
  m_maxSpeedSubmerged = maxSpeedSubmerged;
  m_displacement = displacement;
}


void CVessel::Update(double timeDifference){
  /*CLog *pLog = CLog::Instance();
  static int count = 0;
  count++;
  if(count > 100){
    pLog->Log("t",timeDifference);
    count = 0;
  }*/

  static double timeAcc = 0;
  timeAcc += timeDifference;
  if(timeAcc > 0.1){
    
    //update heading based upon rudder, speed, and depth
    UpdateDepth(timeAcc);
    UpdateSpeed(timeAcc);
    UpdateHeading(timeAcc);

    //update position
    double radians = ConvertDegreesToRadians(m_heading);
    double yards = (m_speed * 2000)/3600;
    m_posX += yards * timeAcc  * cos(radians);
    m_posY += yards * timeAcc  * sin(radians);

    timeAcc = 0;

    //particle stuff for damage
    if(m_bExplode == true)
      m_smokeDelay++;

    if(m_bExplode == true && m_smokeDelay > 5){
      m_bExplode = false;
      m_bSmoke = true;
    }

    if(m_damageGoal > m_damage){
      m_damage += 1;
     // if(m_damage >= 100.0f)
     //   m_bAlive = false;
    }
  }

  if(m_state == VESSEL_STATE_ATTACK && m_bdropDepthCharge == false && m_TimerDepthCharge.getTimer(1) == true)
    m_bdropDepthCharge= true;

}

void CVessel::UpdateSpeed(double timeAcc){
  float speedDiff = m_cmdSpeed - m_speed;

  if(speedDiff < -0.5){
    m_speed -= timeAcc * m_maxAcc;
  }
  else if(speedDiff > .5){
    m_speed += timeAcc * m_maxAcc;
  }
  else  
    m_speed = m_cmdSpeed;
}

void CVessel::UpdateDepth(double timeAcc){
  float depthDiff = m_cmdDepth - m_depth;

  if(depthDiff > 0.05)
    m_depth += timeAcc * m_speed * .25;// + .001);
  else if(depthDiff < -.05)
    m_depth -= timeAcc * m_speed * .25;// + .001);
  else
    m_depth = m_cmdDepth;

  if(m_depth > 26)
    m_bSubmerged = true;
  else
    m_bSubmerged = false;

  if(m_bSubmerged == true && m_cmdSpeed > 6)//auto adjust speed down to max of 6 submerged
    m_cmdSpeed = 6;
  else if(m_bSubmerged == true && m_cmdSpeed < -6)//auto adjust speed if max reverse of 6 
    m_cmdSpeed = -6;

}

void CVessel::UpdateHeading(double timeAcc){

  //determine rudder
  float rudderDiff = 0;

  //move rudder
  rudderDiff = m_cmdRudder - m_rudder;
  if(rudderDiff < -0.3)      
    m_rudder -= timeAcc * 8.0f;
  else if(rudderDiff > 0.3)
    m_rudder += timeAcc * 8.0f;
  else
    m_rudder = m_cmdRudder;

  if(m_rudder > 0){
    m_heading -= timeAcc * -m_speed * m_rudder * 0.025;
  }
  else if(m_rudder < 0){
    m_heading += timeAcc * m_speed * m_rudder * 0.025;
  }

  /*
  //determine new heading
  if(m_speed >= 0)
    m_heading += timeAcc * m_speed * m_rudder * 0.025;//rudderMult;
  else
    m_heading += timeAcc * -m_speed * m_rudder * 0.025;//rudderMult;
*/
  if (m_heading > 360)
    m_heading = m_heading - 360;
  if (m_heading < 0)
    m_heading = m_heading + 360;

}


void CVessel::SetSpeed(float speed){
  m_cmdSpeed = speed;
}

void CVessel::SetDepth(float depth){
  m_cmdDepth = depth;
}

void CVessel::SetRudder(float rudder){
  m_cmdRudder = rudder;
}



//compass degrees...math radians
float CVessel::ConvertDegreesToRadians(float deg){
    float rad = 450 - deg;
    rad = rad * CHEADING_PI/180;

    return rad;  
}

//compass degrees...math radians
float CVessel::ConvertRadiansToDegrees(float rad){
  float deg = rad  * 180/CHEADING_PI;
  deg = 450 - deg;
  if (deg > 360)
    deg = deg - 360;
  if (deg < 0)
    deg = deg + 360;

  return deg;
}


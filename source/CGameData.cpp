//CGameData.cpp
#include "..\includes\cgamedata.h"

CGameData *CGameData::pInstance = 0;

CGameData::CGameData(){
}

CGameData *CGameData::Instance(){
  if(CGameData::pInstance == 0)
    CGameData::pInstance = new CGameData;

  //else
  return CGameData::pInstance;
}

CGameData::~CGameData(){
  delete pInstance;
}

/******************************************************
  All lines must have one of the following codes:
  $M = This is a line in the Sub Orders...add as many
       as you want.
  $S = This is USS Tautog data. Only one allowed.
  $T = This is a target...can have many of them.

******************************************************/
bool CGameData::LoadGameData(std::string sFile){

  CFileReader* cfr = new CFileReader;
  std::string sValue;
  CLog *pLog = CLog::Instance();
  cfr->SupressSpaceRemoval(true);
  cfr->LoadFile("data\\" + sFile);
  CVessel vessel;
  int missionNumber;
  int fwdTorp;
  int aftTorp;

  STRUCT_TARGET temp;

  //clear
  m_targets.clear();
  
  if(cfr->IsValid()== true){

    //clear out existing data
//    m_target.clear();
    m_orders.clear();
    m_objective = "";

    for(int j=0; j< cfr->GetNumberOfLines();j++){
      pLog->Log(j,cfr->GetLineFromFile(j));
      //first term of line is codeID
      sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 1);

      //load USS Tautog (SS-199) Data
      if(sValue == "$S"){

        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 2);
        missionNumber = atoi(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 3);
        vessel.m_vesselType = atoi(sValue.c_str());

        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 4);
        vessel.m_posX =  atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 5);
        vessel.m_posY = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 6);
        vessel.m_speed = atof(sValue.c_str());
        vessel.m_cmdSpeed = vessel.m_speed;
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 7);
        vessel.m_heading = atof(sValue.c_str());
        vessel.m_cmdRudder = 0;
        vessel.m_rudder = 0;
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 8);
        vessel.m_depth = atof(sValue.c_str());
        vessel.m_cmdDepth = vessel.m_depth;
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 9);
        fwdTorp = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 10);
        aftTorp = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 11);
        vessel.m_damage = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 12);
        vessel.m_damageStatus = atoi(sValue.c_str());      
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 13);
        vessel.m_distance = atof(sValue.c_str());       
        m_Player = vessel;

        /*
        m_Player.LoadVessel(vessel.m_speed, vessel.m_depth,
          vessel.m_heading, vessel.m_vesselType, "USS Tautog",
          vessel.m_posX, vessel.m_posY, vessel.m_destPosX, vessel.m_destPosY,
          fwdTorp, aftTorp,vessel.m_damage, vessel.m_damageStatus,vessel.m_distance);
          */
      }
      else if(sValue == "$O"){
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 2);//message part of orders
        m_objective = sValue;
      }

      else if(sValue == "$E"){//environmental
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 2);//seastate
        m_seaState = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 3);//time in minutes measured from midnight
        m_time = atoi(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 4);//goal type 0, 1
        m_goalType = atoi(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 5);//goal amount
        m_goal = atoi(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 6);//goal time minutes
        m_goalTime = atoi(sValue.c_str());
        m_accGoal = 0;
        m_accTime = 0;

        /*
        int m_goalType; //0=sink number of ships, 1=sink amount of tonnage
        int m_goal;     //number of ships or amount of tonnage
        int m_accGoal;  //accumulated number of ships or tonnage
        int m_goalTime; //number of minutes to achieve goal
        int m_accTime;  //accumulated time in minutes
        */

      }

      else if(sValue == "$M"){//mission orders
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 2);//message part of orders
        m_orders.push_back(sValue);
        //pLog->Log("$M",sValue);
      }
      else if(sValue == "$T"){//targets
  
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 2);//vessel type
        vessel.m_vesselType = atoi(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 3);
        vessel.m_posX =  atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 4);
        vessel.m_posY = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 5);
        vessel.m_speed = atof(sValue.c_str());
        vessel.m_cmdSpeed = vessel.m_speed;
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 6);
        vessel.m_heading = atof(sValue.c_str());
        vessel.m_cmdRudder = 0;
        vessel.m_rudder = 0;
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 7);
        vessel.m_depth = atof(sValue.c_str());
        vessel.m_cmdDepth = vessel.m_depth;
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 8);
        vessel.m_destPosX = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 9);
        vessel.m_destPosY = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 10);
        vessel.m_damage = atof(sValue.c_str());
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 11);
        vessel.m_damageStatus = atoi(sValue.c_str());      
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 12);
        vessel.m_distance = atof(sValue.c_str());

        //find catalog data
        for(int i = 0; i  < m_catalog.size(); ++i){
          if(vessel.m_vesselType == m_catalog[i].vesselID){
            vessel.LoadCatalog(m_catalog[i].name, m_catalog[i].length,
              m_catalog[i].beam, m_catalog[i].draft, m_catalog[i].range,
              m_catalog[i].testDepth, m_catalog[i].crushDepth, m_catalog[i].maxSpeedSurface,
              m_catalog[i].maxSpeedSubmerged, m_catalog[i].displacement);
            break;
          }
        }
        //pLog->Log("HDG",vessel.m_Heading.GetHeading());
        //pLog->Log("SPD",vessel.m_Speed.GetSpeed());

        //this is best managed from a data file...if time permits
        if(vessel.m_vesselType == 1){//carrier
          vessel.m_AOBsprite[0] = 83;
          vessel.m_AOBsprite[1] = 84;
          vessel.m_AOBsprite[2] = 85;
          vessel.m_AOBsprite[3] = 86;
          vessel.m_AOBsprite[4] = 87;
          vessel.m_AOBsprite[5] = 88;
          vessel.m_AOBsprite[6] = 89;
          vessel.m_AOBsprite[7] = 90;
        }
        else if(vessel.m_vesselType == 2){//destroyer
          vessel.m_AOBsprite[0] = 58;
          vessel.m_AOBsprite[1] = 59;
          vessel.m_AOBsprite[2] = 60;
          vessel.m_AOBsprite[3] = 61;
          vessel.m_AOBsprite[4] = 62;
          vessel.m_AOBsprite[5] = 63;
          vessel.m_AOBsprite[6] = 64;
          vessel.m_AOBsprite[7] = 65;
        }
        else if(vessel.m_vesselType == 3){//oiler
          vessel.m_AOBsprite[0] = 91;
          vessel.m_AOBsprite[1] = 92;
          vessel.m_AOBsprite[2] = 93;
          vessel.m_AOBsprite[3] = 94;
          vessel.m_AOBsprite[4] = 95;
          vessel.m_AOBsprite[5] = 96;
          vessel.m_AOBsprite[6] = 97;
          vessel.m_AOBsprite[7] = 98;
        }
        else if(vessel.m_vesselType == 4){//cruiser
          vessel.m_AOBsprite[0] = 99;
          vessel.m_AOBsprite[1] = 100;
          vessel.m_AOBsprite[2] = 101;
          vessel.m_AOBsprite[3] = 102;
          vessel.m_AOBsprite[4] = 103;
          vessel.m_AOBsprite[5] = 104;
          vessel.m_AOBsprite[6] = 105;
          vessel.m_AOBsprite[7] = 106;
        }

        vessel.m_state = VESSEL_STATE_TRANSIT;
        vessel.m_bdropDepthCharge = true;


        vessel.m_bAlive = true;
        vessel.m_damage = 0;
        vessel.m_damageGoal = 0;
        m_targets.push_back(vessel);
      }
    }
  }
  else{
    delete cfr;
    return false;
  }
  delete cfr;
  //******************************************************* initialize game data
  //*******************************************************
  //*******************************************************
  //load periscope information
  m_scopePosition = 0;
  m_scopeMaxPosition = 1135;
  m_scopeMinPosition = 0;
  m_bScopeUp = false;
  m_bScopeDown = false;
  m_bRotateLeft = false;
  m_bRotateRight = false;
  m_scopeRotateAngle = 0;
  m_scopeZoom = .5;//was 1
  //m_seaState = 1;
  m_nightColor = 0;
  m_lightTimer.initialize();
  
  //shock and vibration
  m_shockTimer.initialize();
  m_shockTime = 0;
  m_maxShockX = 20;
  m_maxShockY = 20;
  m_shockX = 0;
  m_shockY = 0; 
  m_shockCount = 0;
  m_shockAngle = 0;
  m_shockDuration = 0;

  m_bAttack = false;
  m_posError = 50000;

  return true;
}

bool CGameData::LoadCatalogData(std::string sFile){
  CFileReader* cfr = new CFileReader;
  std::string sValue;
  int codeID;
  STRUCT_CATALOG temp;
  CLog *pLog = CLog::Instance();

 

  cfr->LoadFile("data\\catalog.dat");

  if(cfr->IsValid()== true){

    //clear out existing data
    m_catalog.clear();

    for(int j=0; j< cfr->GetNumberOfLines();j++){
      //pLog->Log(j, cfr->GetNumberOfTermsFromLine(j));

      if(cfr->GetNumberOfTermsFromLine(j) == 11){//11 terms
        
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 1);//vessel ID
        temp.vesselID = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 2);//Name of class
        temp.name = sValue;
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 3);//length in feet
        temp.length = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 4);//beam in feet
        temp.beam = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 5);//keel depth in feet
        temp.draft = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 6);//range in feet
        temp.range = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 7);//test depth (subs)
        temp.testDepth = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 8);//crush depth (subs)
        temp.crushDepth = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 9);//max speed surfaced
        temp.maxSpeedSurface = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 10);//max speed submerged (subs)
        temp.maxSpeedSubmerged = atoi(sValue.c_str()); 
        sValue = cfr->GetTerm(cfr->GetLineFromFile(j), 11);//displacement tons
        temp.displacement = atoi(sValue.c_str()); 
        m_catalog.push_back(temp);

      }
    }
    delete cfr;
  }//if
  else{
    delete cfr;
    return false;
  }

  pLog->Log("Number of targets****************************",m_catalog.size());
  return true;
}

//updates game data
void CGameData::Update(double timeDifference){
  //update submarine
  CLog *pLog = CLog::Instance();
  float distance = 0;
  m_missionOver = MISSION_OVER_NOT;
  float deltaY = 0, deltaX = 0;
  m_bDropped = false;
  //update all targets that are still alive
  for(int i = 0; i < m_targets.size();++i){
    m_targets[i].Update(timeDifference);

    //distance to ship and target
    distance = sqrt((m_Player.m_posX - m_targets[i].m_posX) * (m_Player.m_posX - m_targets[i].m_posX) +
                      (m_Player.m_posY - m_targets[i].m_posY) * (m_Player.m_posY - m_targets[i].m_posY));


    //check for collision...sub less than 40 feet depth and X,Y within 50 yards
    if(m_Player.m_depth < 40){
      if(distance < 50){
        m_missionOver = MISSION_OVER_COLLISION;
        m_Player.m_damageGoal = 100;

      }
    }

    if(m_targets[i].m_vesselType == 2 &&  distance < 8000 && m_Player.m_depth <= 35)
      m_bAttack = true;

    //determine AI data
    if(m_targets[i].m_vesselType == 2 &&  distance < 8000 && m_bAttack == true){

      if(m_targets[i].m_bdropDepthCharge == true){
        m_targets[i].m_cmdSpeed = m_targets[i].m_maxSpeed;

        m_posError = 1000;
        if(distance < 4000)
          m_posError = 400;
        if(distance < 3000)
          m_posError = 300;
        if(distance < 2000 && m_scopePosition < m_scopeMaxPosition) //scope down
          m_posError = 200;
        if(distance < 2000 && m_scopePosition == m_scopeMaxPosition)//less than 2000 yards, scope up..uh oh
          m_posError = 100;
        if(distance < 750)
          m_posError = 50;

        m_posAngle = (rand() % 628) * 0.01; //get random angle

        m_estX = m_Player.m_posX + (cos(m_posAngle) * m_posError);
        m_estY = m_Player.m_posY + (sin(m_posAngle) * m_posError);
        deltaY = m_estY - m_targets[i].m_posY;
        deltaX = m_estX - m_targets[i].m_posX;
        
        //m_attackHeading = m_targets[i].m_relativeBearing + 180; 
        m_attackHeading = GetBearing(deltaY, deltaX);

        if(m_attackHeading > 360)
          m_attackHeading = 0;
        else if(m_attackHeading < 0)
          m_attackHeading = 360;
             
        m_targets[i].m_heading = m_attackHeading;

        if(distance < 300){
        if(m_posError < 50){
          CreateShock(100, 1.8, rand()% 628 * 0.01);
          m_Player.m_damageGoal += 25;
          m_bDropped = true;
        }
        else if(m_posError < 100){
          CreateShock(75, 1.8, rand()% 628 * 0.01);
          m_Player.m_damageGoal += 20;
          m_bDropped = true;
        }
        else if(m_posError < 150){
          CreateShock(50, 1.8, rand()% 628 * 0.01);
          m_Player.m_damageGoal += 15;
          m_bDropped = true;
        }
        else if(m_posError < 200){
          CreateShock(25, 1.8, rand()% 628 * 0.01);
          m_Player.m_damageGoal += 10;
          m_bDropped = true;
        }
        else if(m_posError < 250){
          CreateShock(15, 1.8, rand()% 628 * 0.01);
          m_Player.m_damageGoal += 5;
          m_bDropped = true;
        }
        m_targets[i].m_bdropDepthCharge = false;
        }
      }
    }
  }

  //update all torpedos that are still running
  for(int i = 0; i < m_torpedos.size();++i){
    m_torpedos[i].Update(timeDifference);
  }

  //torpedo impact

  for(int i = 0; i < m_targets.size();++i){
    for(int j = 0; j < m_torpedos.size();++j){
      if(m_torpedos[j].IsRunning() == true && m_targets[i].m_bAlive == true){
        distance = sqrt((m_torpedos[j].m_posX - m_targets[i].m_posX) * (m_torpedos[j].m_posX - m_targets[i].m_posX) +
                       (m_torpedos[j].m_posY - m_targets[i].m_posY) * (m_torpedos[j].m_posY - m_targets[i].m_posY));

        if(distance < 100){ //100 yards
          //m_targets[i].m_damage = 25 + rand() % 100; //random number 0 - 125%
          m_targets[i].m_damageGoal = m_targets[i].m_damage + 25 + rand() % 100; //random number 0 - 125%
          m_targets[i].m_bExplode = true;//allow for explosion...followed in 0.5 sec by smoke
          m_targets[i].m_smokeDelay = 0;
          m_torpedos[j].Kill();
          m_bAttack = true; //uh-oh...sub out there

          //change state of all destroyers
          for(int k = 0; k < m_targets.size(); ++k){
            if(m_targets[k].m_bAlive == true && m_targets[k].m_vesselType == 2)//destroyer escore
              m_targets[k].m_state = VESSEL_STATE_ATTACK;
          }
        }
      }    

      //kill torpedo if > 8000 yards
      if(m_torpedos[j].GetDistance() > 8000)
        m_torpedos[j].Kill();
    }

  }

  //update damage results
  for(int i = 0; i < m_targets.size();++i){
    if(m_targets[i].m_bAlive == true){// && m_targets[i].m_timerDamage.getTimer(0.1) == true){
     // if(m_targets[i].m_damage < m_targets[i].m_damageGoal)
     //   m_targets[i].m_damage += 0.5;

      if(m_targets[i].m_damage > 99){
        m_targets[i].m_bAlive = false;
        if(m_goalType == 0)//number of ships
          m_accGoal++;
        else if(m_goalType == 1)//tonnage
          m_accGoal += m_targets[i].m_displacement;

      }
    }
  }

  //update player data
  m_Player.Update(timeDifference);

  //update relative information about all targets relative to the sub
  //float m_trueBearing;//
  //float m_relativeBearing;//
  //float m_rangeToTarget;//
  //float m_AOB; //angle on the bow//
  //float m_AOBindex; //current index, 0 to 7 0=front aspect, 2= port aspect, 4=aft aspect, 6 = starboard aspect//
  //float m_gyroAngle; //angle calculated for torpedo
  //double deltaX = 0, deltaY = 0;
  float deflAngle = 0;
  for(int i = 0; i < m_targets.size();++i){
    deltaX = m_targets[i].m_posX - m_Player.m_posX;
    deltaY = m_targets[i].m_posY - m_Player.m_posY;
    m_targets[i].m_trueBearing = ConvertRadiansToDegrees(GetBearing(deltaY, deltaX)); //ok

    //determine relative bearing
    m_targets[i].m_relativeBearing = m_targets[i].m_trueBearing - ConvertTo180(m_Player.m_heading);//ok

    //determine range to target
    m_targets[i].m_rangeToTarget = sqrt((deltaX * deltaX) + (deltaY * deltaY));//ok

    //calculate AOB
    //- AOB is port, + AOB is starboard
    m_targets[i].m_AOB = ConvertTo180(m_targets[i].m_trueBearing - m_targets[i].m_heading - 180);// + 180;

    //calculate AOB index
    if(m_targets[i].m_AOB >= -20 && m_targets[i].m_AOB <= 0)
      m_targets[i].m_AOBindex = 0;
    else if(m_targets[i].m_AOB < -20 && m_targets[i].m_AOB >= - 70)
      m_targets[i].m_AOBindex = 1;
    else if(m_targets[i].m_AOB < -70 && m_targets[i].m_AOB >= - 110)
      m_targets[i].m_AOBindex = 2;
    else if(m_targets[i].m_AOB < -110 && m_targets[i].m_AOB >= - 160)
      m_targets[i].m_AOBindex = 3;
    else if(m_targets[i].m_AOB < -160 && m_targets[i].m_AOB >= - 180)
      m_targets[i].m_AOBindex = 4;
    else if(m_targets[i].m_AOB > 20 && m_targets[i].m_AOB <= 70)
      m_targets[i].m_AOBindex = 7;
    else if(m_targets[i].m_AOB > 70 && m_targets[i].m_AOB <= 110)
      m_targets[i].m_AOBindex = 6;
    else if(m_targets[i].m_AOB > 110 && m_targets[i].m_AOB <= 160)
      m_targets[i].m_AOBindex = 5;
    else if(m_targets[i].m_AOB > 160 && m_targets[i].m_AOB <= 180)
      m_targets[i].m_AOBindex = 4;
    else if(m_targets[i].m_AOB < 20 && m_targets[i].m_AOB >= 0)
      m_targets[i].m_AOBindex = 0;

    //calculate gyro angle
    deflAngle = (m_targets[i].m_speed/m_Player.m_speed) * sin(ConvertDegreesToRadians(m_targets[i].m_AOB));
    m_targets[i].m_gyroAngle = m_Player.m_heading - m_targets[i].m_relativeBearing - ConvertRadiansToDegrees(deflAngle);

    m_targets[i].m_bRangeFound = false;
    m_targets[i].m_rangePosition = 0;
  }

  //sort all targets by range. Number 0 is farthest away
  float range = 0;
  int index = 0;
  int farthest = 0;

  for(int i = 0; i < m_targets.size();++i){

    //iterate through all ranges...find farthest not yet found
    for(int j = 0; j < m_targets.size();++j){
      if(i != j && m_targets[j].m_bRangeFound == false){
        if(m_targets[j].m_rangeToTarget > range){
          range = m_targets[j].m_rangeToTarget;
          farthest = j;
        }
      }
    }

    m_targets[farthest].m_rangePosition = index;
    m_targets[farthest].m_bRangeFound = true;
    index++;
    farthest = 0;
    range = 0;
  }

  //periscope updating
  static double scopeTimeCount = 0;
  scopeTimeCount += timeDifference;
  if(scopeTimeCount > 0.05){
    scopeTimeCount = 0;

    //periscope animation - raise lower, rotate
    if(m_bScopeUp == true){
      m_scopePosition += 10;//++;
      if(m_scopePosition >= m_scopeMaxPosition){
        m_scopePosition = m_scopeMaxPosition;
        m_bScopeUp = false;
      }
    }
    else if(m_bScopeDown == true){
      m_scopePosition -= 10;//-;
      if(m_scopePosition <= m_scopeMinPosition){
        m_scopePosition = m_scopeMinPosition;
        m_bScopeDown = false;
      }
    }

    if(m_bRotateLeft == true){
      m_scopeRotateAngle += m_scopeControlValue;// 10;
      if(m_scopeRotateAngle < 0)
        m_scopeRotateAngle = 2047;
    }
    else if(m_bRotateRight == true){
      m_scopeRotateAngle += m_scopeControlValue;//10;
      if(m_scopeRotateAngle > 2047)
        m_scopeRotateAngle = 0;
    } 
   
  }

  //update shock
  if(m_shockTimer.getTimer(0.1) == true && m_shockCount > 0){
    m_shockTime += m_shockDuration;//controls time of sine wave
   
    m_maxShockX =  cos(m_shockAngle);
    m_maxShockY =  sin(m_shockAngle);
    m_shockX = m_shockCount * m_maxShockX * cos(m_shockTime);
    m_shockY = m_shockCount * m_maxShockY * sin(m_shockTime);
    if(m_shockCount > 10)
      m_shockCount -= 8.5;
    else
      m_shockCount -= 1.0;
  }



  //update time and lighting
  m_nightColor = abs(720 - m_time)/3;  
  if(m_lightTimer.getTimer(60.0) == true){ //60 seconds equals 1 minute
    m_time++;
    if(m_time > 1440)
      m_time = 0;
    
    //increment game clock
    m_accTime++;
    if(m_accTime >= m_goalTime)
      m_missionOver = MISSION_OVER_TIMEOVER;
  }

  if(m_accGoal >= m_goal)
    m_missionOver = MISSION_OVER_WIN;

  if(m_Player.m_damage >= 100)
    m_missionOver = MISSION_OVER_DEAD;

}

//triggers a shock event
//************************************************************************
void CGameData::CreateShock(float magnitude, float duration, float angle){
  m_shockCount = magnitude;
  m_shockAngle = angle;
  m_shockDuration = duration;
}

void CGameData::LoadDefaultData(){

}

float CGameData::GetDegrees(long num){
  return (num / 2000)/60 ;
}

float CGameData::GetMinutes(long num){
  long a = num/2000;
  
  a =  a - GetDegrees(num) * 60;
  return a;
}

//compass degrees...math radians
float CGameData::ConvertDegreesToRadians(float deg){
    float rad = 450 - deg;
    rad = rad * CHEADING_PI/180;

    return rad;  
}

//compass degrees...math radians
float CGameData::ConvertRadiansToDegrees(float rad){
  float deg = rad  * 180/CHEADING_PI;
  deg = 450 - deg;
  if (deg > 360)
    deg = deg - 360;
  if (deg < 0)
    deg = deg + 360;

  return deg;
}

float CGameData::GetBearing(float dy, float dx){
  float angle = 0;

  if(dx == 0){
    angle = atan(dy/.001);
  }
  else{
    angle = atan(dy/dx);
  }

  if(dy >= 0 && dx >= 0){  //top-right OK
    //angle = CHEADING_PI/2 - angle;
  }
  else if(dy >= 0 && dx < 0) {//top-left OK
    angle = 1 * CHEADING_PI + angle;
  }
  else if(dy < 0 && dx < 0){ //bottom-left 
    angle = CHEADING_PI - angle;
  }
  else if(dy < 0 && dx >= 0){ //top-right
    angle = 2 * CHEADING_PI + angle;
  }

  if(angle > 2 * CHEADING_PI)
    angle = 0.0f;
  if(angle < 0.0f)
    angle = 2 * CHEADING_PI;

  return angle;
}

float CGameData::ConvertTo180(float heading){
  if(heading > 360)
    heading = heading - 360;
  else if(heading < -360)
    heading = heading + 360;

  if(heading > 180)
    heading = heading - 360;
  else if(heading < -180)
    heading = 360 + heading;

  return heading;
}

/*
  CGameData.h - Singleton design pattern. Written by Chuck Bolin, March 25, 2008
  Purpose: Contains all game data and supporting methods.
  Initialization:

    CGameData *pData = CGameData::Instance();
*/

#ifndef CGAMEDATA_H
#define CGAMEDATA_H

#include <fstream>
#include <cstdarg>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include "CDateTime.h"
#include "CFileReader.h"
#include "CLog.h"
#include "CVessel.h"
#include "CTorpedo.h"
#include "CTimer.h"
#include "CAudioManager.h"

//m_missionOver
const int MISSION_OVER_NOT = 0;
const int MISSION_OVER_WIN = 1;
const int MISSION_OVER_DEAD = 2;
const int MISSION_OVER_TIMEOVER = 3;
const int MISSION_OVER_COLLISION = 4;

//structure for USS Tautog's data
struct STRUCT_SUB{
  //loaded from mission file
  int missionNumber; //0,1,2 = Training  4,5,6,7 = War Patrols 10 - 13
  int vesselType; //0 = sub
  long xPos;
  long yPos;
  long xDestPos;
  long yDestPos;
  float speed;
  float heading;
  float depth;
  int numFwdTorpedos;
  int numAftTorpedos;

  //calculated
  float damage;

  unsigned int damageStatus;
  std::ostringstream latitude;
  std::ostringstream longitude;
  
  //loaded
  unsigned int vesselConfig;
  float length;
  float beam;
  float draft;
  float range;
  float testDepth;
  float crushDepth;
  float maxSpeedSurface;
  float maxSpeedSubmerged;
  float displacement;

};

//struct for all targets
struct STRUCT_TARGET{
 
  
  //loaded from mission file
  int missionNumber; //0,1,2 = Training  4,5,6,7 = War Patrols 10 - 13
  int vesselType; //0 = sub
  std::string name;
  long xPos;
  long yPos;
  long xDestPos;
  long yDestPos;
  float speed;
  float heading;
  float depth;
  int numFwdTorpedos;
  int numAftTorpedos;

  //calculated
  float damage;

  unsigned int damageStatus;
  float distance;
  //std::ostringstream latitude;
  //std::ostringstream longitude;

};

struct STRUCT_CATALOG{
  int vesselID;
  std::string name;
  float length;
  float beam;
  float draft;
  float range;
  float testDepth;
  float crushDepth;
  float maxSpeedSurface;
  float maxSpeedSubmerged;
  float displacement;
};

class CGameData{
public:
  static CGameData *Instance();
  bool LoadGameData(std::string sFile);
  void LoadDefaultData();
  bool LoadCatalogData(std::string sFile);
  float GetDegrees(long num);
  float GetMinutes(long num);
  void Update(double timeDifference);
  float ConvertDegreesToRadians(float deg);
  float ConvertRadiansToDegrees(float rad);
  float GetBearing(float dy, float dx);

  //std::vector<STRUCT_TARGET> m_target;
  CVessel m_Player;
  std::vector<STRUCT_CATALOG> m_catalog;
  std::vector<std::string> m_orders;
  std::string m_objective;
  std::vector<CVessel> m_targets;  

  //torpedo
  std::vector<CTorpedo> m_torpedos;

  //periscope information
  int m_scopePosition;
  int m_scopeMaxPosition;
  int m_scopeMinPosition;
  bool m_bScopeUp;
  bool m_bScopeDown;
  int m_scopeRotateAngle; //0 to 2047  5.688 pixels = 1 degree
  bool m_bRotateLeft;
  bool m_bRotateRight;
  float m_scopeZoom;
  float m_scopeControlValue;

  //sensor stuff
  float m_seaState; //1 to 7, 7 is bad
  int m_time;//0 (midnight) to 720 (noon)
  int m_nightColor; //0 = noon 255 = midnight
  CTimer m_lightTimer;
  
  //goal stuff
  int m_goalType; //0=sink number of ships, 1=sink amount of tonnage
  int m_goal;     //number of ships or amount of tonnage
  int m_accGoal;  //accumulated number of ships or tonnage
  int m_goalTime; //number of minutes to achieve goal
  int m_accTime;  //accumulated time in minutes
  int m_missionOver; //0 = no it's not, 1 = objectives met, 2 = destroyed, 3 = failed to meet time
  
  //shock and vibration controls
  CTimer m_shockTimer;
  float m_shockTime;
  float m_maxShockX;
  float m_maxShockY;
  float m_shockX;
  float m_shockY;
  float m_shockCount;
  float m_shockAngle;
  float m_shockDuration;
  void CreateShock(float magnitude, float duration, float angle);

  //AI attack
  float m_estX;
  float m_estY;
  float m_attackHeading;
  float m_posError;
  float m_posAngle;
  bool m_bAttack;
  bool m_bDropped;

protected:
  CGameData();
  ~CGameData();

private:
  static CGameData* pInstance;
  float ConvertTo180(float heading);
};

#endif



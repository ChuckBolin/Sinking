/*
CVessel.h - Written by Chuck Bolin, April 11, 2008
*/
#ifndef CVESSEL_H
#define CVESSEL_H

#include <vector>
#include <string>
#include "CLog.h"
#include <cmath>
#include "CTimer.h"

const int CHEADING_AMIDSHIPS = 0;
const int CHEADING_LEFT_EASY = 1;
const int CHEADING_LEFT_HARD = 2;
const int CHEADING_RIGHT_EASY = 3;
const int CHEADING_RIGHT_HARD = 4;
const float CHEADING_PI = 3.141592654;
const int VESSEL_STATE_TRANSIT = 0;
const int VESSEL_STATE_ATTACK = 1;


class CVessel {
public:
  CVessel();
  void LoadVessel(float speed, float depth, float heading,
          int vesselType, std::string name, long posX, long posY,
          long destPosX, long destPosY, int numFwdTorpedos, int numAftTorpedos,
          float damage, unsigned int damageStatus, float distance);
  void LoadCatalog(std::string className, float length, float beam,
          float draft, float range, float testDepth, float crushDepth,
          float maxSpeedSurface, float maxSpeedSubmerged, float displacement);
  void Update(double timeDifference);
  float GetSpeed(){return m_speed;}
  float GetDepth(){return m_depth;}
  float GetHeading(){return m_heading;}
  void SetSpeed(float speed);
  void SetDepth(float depth);
  void SetRudder(float rudder);
  void UpdateSpeed(double timeDifference);
  void UpdateDepth(double timeDifference);
  void UpdateHeading(double timeDifference);
  int m_vesselType; 
  std::string m_shipName;
  double m_posX;
  double m_posY;
  double m_destPosX;
  double m_destPosY;
  int m_numFwdTorpedos;
  int m_numAftTorpedos;
  float m_depth;
  double m_heading;
  double m_speed;
  float m_rudder;
  float m_cmdDepth;
  float m_cmdRudder;
  float m_cmdSpeed;

  //calculated
  float m_damage;
  float m_damageGoal;
  CTimer m_timerDamage;
  bool m_bAlive;
  unsigned int m_damageStatus;
  float m_distance;

  //information loaded in the catalog
  std::string m_className;
  float m_length;
  float m_beam;
  float m_draft;
  float m_range;
  float m_testDepth;
  float m_crushDepth;
  float m_maxSpeedSurface;
  float m_maxSpeedSubmerged;
  float m_displacement;
  float ConvertDegreesToRadians(float deg);
  float ConvertRadiansToDegrees(float rad);
  bool m_bSubmerged;

  //calculated data for sensor utilization
  float m_trueBearing;
  float m_relativeBearing;
  float m_rangeToTarget;
  float m_AOB; //angle on the bow
  int m_AOBindex; //current index, 0 to 7 0=front aspect, 2= port aspect, 4=aft aspect, 6 = starboard aspect
  float m_gyroAngle; //angle calculated for torpedo
  int m_AOBsprite[8]; //stores 8 sprite indexes
  int m_rangePosition; //0 means farthest away
  bool m_bRangeFound;  //used only for sorting

  //particle effects
  bool m_bExplode;
  bool m_bSmoke;
  int m_smokeDelay;

  int m_state;//0 OR 1, TRANSIT OR ATTACK
  bool m_bdropDepthCharge;
  CTimer m_TimerDepthCharge;
  float m_maxAcc;
  float m_maxSpeed;
  float m_minSpeed;
private:

};

#endif
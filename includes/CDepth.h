/************************************************************************
CDepth.h - Written by Chuck Bolin, March 2008
Revised April 11, 2008
************************************************************************/
#ifndef CDEPTH_H
#define CDEPTH_H

const int CDEPTH_LEVEL = 0;
const int CDEPTH_GO_PD = 1;
const int CDEPTH_SURFACE = 2;
const int CDEPTH_GO_DEEP = 3;
const int CDEPTH_GO_BELOW_ISOTHERMAL = 4;
const int CDEPTH_DIVE = 5;

//sub is in one of four states
const int CDEPTH_STATE_SURFACED = 0;
const int CDEPTH_STATE_SURFACING = 1;
const int CDEPTH_STATE_DIVING = 2;
const int CDEPTH_STATE_SUBMERGED = 3;

class CDepth{
public:
  CDepth();
  void SetDepth(float depth);  //depth in feet
  void SetDepthCommand(int depth);    //depth - predefined
  void Update(double timeDifference, float speed);
  float GetDepth(){return m_depth;}
  float GetCommandDepth(){return m_commandDepth;}
  
private:
  float m_depth;
  float m_commandDepth;
  float m_testDepth;
  float m_crushDepth;
  float m_periscopeDepth;
  float m_surfaceDepth;
  int m_state;
  float m_diveRate;

};

#endif
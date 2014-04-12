#include "..\includes\CDepth.h"

CDepth::CDepth(){
  m_depth = 60;
  m_commandDepth = 60;
  m_testDepth = 250;
  m_crushDepth = 450;
  m_periscopeDepth = 60;
  m_surfaceDepth = 15;
  m_diveRate = 1;
  m_state = CDEPTH_STATE_SUBMERGED;
}
  
void CDepth::SetDepth(float depth){
  if(depth < m_surfaceDepth || depth > m_crushDepth)
    return;

  m_commandDepth = depth;
}

void CDepth::SetDepthCommand(int depth){
  if(depth < 0 || depth > 5)
    return;

  if(depth == CDEPTH_GO_PD)
    m_commandDepth = m_periscopeDepth;
  else if(depth == CDEPTH_GO_DEEP)
    m_commandDepth = m_testDepth;
  else if(depth == CDEPTH_LEVEL)
    m_commandDepth = m_depth; 

}

void CDepth::Update(double timeDifference, float speed){
  float depthDiff = m_commandDepth - m_depth;

  if(depthDiff > 0.05)
    m_depth += timeDifference * m_diveRate * (speed + .1);
  else if(depthDiff < -.05)
    m_depth -= timeDifference * m_diveRate * (speed + .1);
  else
    m_depth = m_commandDepth;
}

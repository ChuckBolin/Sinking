#ifndef GAME_STATE_CONTROL_H
#define GAME_STATE_CONTROL_H
#include <iostream>
using std::cout;
using std::endl;

#include "cgamestateobject.h"
#include "Graphics.h"
#include "CAudioManager.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "..\resource.h"
#include "CGameData.h"

class cGameStateConn :  public cGameStateObject{
public:
  cGameStateConn(void);
  ~cGameStateConn(void);
  void initialize();
  void activate();
  cGameStateObject* update(double timeDifference);
  void render(Graphics &g_con);
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  bool m_bDone;
  int m_event;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;

  //periscope
  int m_scopePosition;
  int m_scopeMaxPosition;
  int m_scopeMinPosition;
  bool m_bScopeUp;
  bool m_bScopeDown;
  int m_scopeRotateAngle; //0 to 2047  5.688 pixels = 1 degree
  bool m_bRotateLeft;
  bool m_bRotateRight;

  //gauges
  float m_fHeading;
  float m_fDepth;
  float m_fSpeed;
};

#endif

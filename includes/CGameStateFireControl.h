#ifndef GAME_STATE_FIRECONTROL_H
#define GAME_STATE_FIRECONTROL_H
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
#include "CGameData.h"
#include "CTorpedo.h"

class cGameStateFireControl :  public cGameStateObject{
public:
  cGameStateFireControl(void);
  ~cGameStateFireControl(void);
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
};

#endif

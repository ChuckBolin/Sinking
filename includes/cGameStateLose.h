#ifndef GAME_STATE_LOSE_H
#define GAME_STATE_LOSE_H
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

class cGameStateLose :  public cGameStateObject{
public:
  cGameStateLose(void);
  ~cGameStateLose(void);
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
  bool m_bSplashComplete;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;

};

#endif

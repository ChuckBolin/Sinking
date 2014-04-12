#ifndef GAME_STATE_SELECT_H
#define GAME_STATE_SELECT_H
#include <iostream>
using std::cout;
using std::endl;

#include <sstream>
#include <string>
#include "cgamestateobject.h"
#include "Graphics.h"
#include "CAudioManager.h"
#include "CTimer.h"
#include "keystatus.h"
#include "CLog.h"
#include "cgamedata.h"
#include "..\resource.h"

class cGameStateSelect :  public cGameStateObject{
public:
  cGameStateSelect(void);
  ~cGameStateSelect(void);
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
  int m_event;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;
  //menu selection
  int m_selection;

  //prevents ENTER key bleed through to other states
  bool m_bActivated;
  CTimer m_enableTimer;

};

#endif

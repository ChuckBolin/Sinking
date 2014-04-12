#ifndef GAME_STATE_MAIN_H
#define GAME_STATE_MAIN_H

#include "cgamestateobject.h"
#include "CTimer.h"
#include "Graphics.h"
#include "CAudioManager.h"
#include <iostream>
#include <mmsystem.h>
#include "CGlobal.h"
#include <windows.h>
#include "..\resource.h"

class cGameStateMain :  public cGameStateObject{
public:
  cGameStateMain(void);
  ~cGameStateMain(void);
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
  int m_event;
  CTimer m_timer;
  int m_selection;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;
  
  //prevents ENTER key bleed through to other states
  bool m_bActivated;
  CTimer m_enableTimer;
};

#endif
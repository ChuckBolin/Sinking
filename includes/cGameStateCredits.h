#ifndef GAME_STATE_CREDITS_H
#define GAME_STATE_CREDITS_H
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

struct SCROLL_MSG{
  std::string msg;
  int red;
  int green;
  int blue;
};

class cGameStateCredits :  public cGameStateObject{
public:
  cGameStateCredits(void);
  ~cGameStateCredits(void);
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
  std::vector<SCROLL_MSG> m_msg;
  int m_scrollValue;

  //used to flip through images
  CTimer m_secondTimer;
  int m_secondCount;

  CTimer m_fadeTimer;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;
};

#endif

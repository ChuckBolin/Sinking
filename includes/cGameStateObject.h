//cGameStateObject.h
//base class...all other game states are derived from here
#ifndef GAME_STATE_OBJECT_H
#define GAME_STATE_OBJECT_H

#include <windows.h>
#include <vector>
#include "Graphics.h"

//constants for game state transition events
const int EVENT_GO_NO_WHERE = 0;
const int EVENT_GO_INTRO = 1;
const int EVENT_GO_MAIN = 2;
const int EVENT_GO_PLAY = 3;
const int EVENT_GO_END = 4;
const int EVENT_GO_HISTORY = 5;
const int EVENT_GO_RELOAD = 6;
const int EVENT_GO_CREDITS = 7;
const int EVENT_GO_SELECT = 8;
const int EVENT_GO_AWARDS = 9;
const int EVENT_GO_WIN = 10;
const int EVENT_GO_LOSE = 11;
const int EVENT_GO_STATUS = 12;
const int EVENT_GO_CHART = 13;
const int EVENT_GO_CONTROL = 14;
const int EVENT_GO_BRIDGE = 15;
const int EVENT_GO_SCOPE = 16;
const int EVENT_GO_SONAR = 17;
const int EVENT_GO_FIRECONTROL = 18;
const int EVENT_GO_RADAR = 19;


class cGameStateObject;//forward initialization

struct TRANSITION_EVENT{
  int event;
  cGameStateObject* p_gso;
};

class cGameStateObject{
public:
  cGameStateObject();
  virtual ~cGameStateObject();

  virtual void initialize();
  virtual void activate();
  virtual cGameStateObject* update(double timeDifference);
  virtual void render(Graphics &g_con);
  virtual void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  virtual void deactivate();
  virtual void resume();
  virtual void pause();
  virtual void save();
  virtual void addTransitionEvent(int event, cGameStateObject* p_Next);

protected:

private:
  
  

};

#endif //GAME_STATE_OBJECT_H
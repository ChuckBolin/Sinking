#include "..\includes\cGameStateHistory.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateHistory::cGameStateHistory(){
}

cGameStateHistory::~cGameStateHistory()
{
	m_TE.clear();
}

void cGameStateHistory::initialize(){
  m_red = 255;
  m_green = 255;
  m_blue = 255;
}

void cGameStateHistory::activate(){
}

cGameStateObject* cGameStateHistory::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;

  if(keyDown(VK_SPACE) && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_MAIN;
    ::ShowCursor(true);
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateHistory::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();

  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(10);
  con.RenderGraphicModulate(con.GetScreenCenterX() -  (float)(gi.width/2) , con.GetScreenCenterY() - (gi.height/2), gi, m_red, m_green, m_blue);
  
  con.Draw2DText("History state - Press SPACE to return.", F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() + (gi.height/2) + 20, 0,255,255);

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  con.EndDrawing ();
  con.Present();
}

void cGameStateHistory::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateHistory::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateHistory::resume(){
}
void cGameStateHistory::pause(){
}
void cGameStateHistory::save(){
}

void cGameStateHistory::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

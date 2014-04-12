#include "..\includes\cGameStateReload.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateReload::cGameStateReload(void){
}

cGameStateReload::~cGameStateReload(void)
{
	m_TE.clear();
}

void cGameStateReload::initialize(){

}

void cGameStateReload::activate(){
}

cGameStateObject* cGameStateReload::update(double timeDifference){
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

void cGameStateReload::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();

  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(107);
  con.RenderGraphicModulate(con.GetScreenCenterX() -  (float)(gi.width/2) , con.GetScreenCenterY() - (gi.height/2), gi, 255,255,255);//);

  con.Draw2DText("Reload state unavailable.", F_V20, con.GetScreenCenterX() + 256, con.GetScreenCenterY() - 10, 0,255,255);
  con.Draw2DText("Press SPACE to return!", F_V20, con.GetScreenCenterX() + 256, con.GetScreenCenterY() + 10, 0,255,255);

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  con.EndDrawing ();
  con.Present();
}

void cGameStateReload::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateReload::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateReload::resume(){
}
void cGameStateReload::pause(){
}
void cGameStateReload::save(){
}

void cGameStateReload::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

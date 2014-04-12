#include "..\includes\cGameStateAwards.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateAwards::cGameStateAwards(void){
}

cGameStateAwards::~cGameStateAwards(void)
{
	m_TE.clear();
}

void cGameStateAwards::initialize(){

}

void cGameStateAwards::activate(){
}

cGameStateObject* cGameStateAwards::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;

  if(keyDown(VK_SPACE) && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_SELECT;
    ::ShowCursor(true);
  }    

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateAwards::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();
  //display flag
  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(109);
  con.RenderGraphicModulate(con.GetScreenCenterX() - gi.width/2,  con.GetScreenCenterY() - gi.height/2 - 100 ,gi, m_red, m_green, m_blue);

  con.Draw2DText("Not programmed. Press SPACE to continue!", F_V20, con.GetScreenCenterX() - 190, con.GetScreenCenterY() + 180, 0,255,255);

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  con.EndDrawing ();
  con.Present();
}

void cGameStateAwards::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateAwards::deactivate(){
//  CAudioManager *pAudio = CAudioManager::Instance();
//  pAudio->KillAll();
 
}
void cGameStateAwards::resume(){
}
void cGameStateAwards::pause(){
}
void cGameStateAwards::save(){
}

void cGameStateAwards::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

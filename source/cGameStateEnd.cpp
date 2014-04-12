#include "..\includes\cgamestateend.h"
#include "..\includes\CGlobal.h"

//objects
//extern Graphics con;
//extern CAudioManager g_Audio;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateEnd::cGameStateEnd(void){
}

cGameStateEnd::~cGameStateEnd(void)
{
	m_TE.clear();
}

void cGameStateEnd::initialize(){

}

void cGameStateEnd::activate(){
}

cGameStateObject* cGameStateEnd::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;

  if((keyDown(VK_SPACE)|| g_Global.g_bLeftMouseDown == true) && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_END;
    ::ShowCursor(true);
  }
    
  
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateEnd::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();

  /*
  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(0);
  gi.scale = (float)con.GetScreenWidth() / (float)gi.width;// 1273;// .75;
  con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) , con.GetScreenCenterY() - (gi.height * gi.scale), gi, m_red, m_green, m_blue);
    con.Draw2DText("End state - Press SPACEBAR to continue...", F_V20, con.GetScreenCenterX() - 150, con.GetScreenHeight() - 50, 130, 130, 130);

  con.DrawCircle(300, 100, 255, 0, 0, 20);
  con.DrawLine(100, 700, 400, 700, 0, 255, 0);
  con.DrawCircle(800, 600, 0, 0, 255, 25);
  //con.DrawRect(200, 200, 500, 500, 0, 0, 255);
  con.Draw2DText("End", F_V20, 0, 0, 0,0,255);
*/
  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  con.EndDrawing ();
  con.Present();
}

void cGameStateEnd::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateEnd::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateEnd::resume(){
}
void cGameStateEnd::pause(){
}
void cGameStateEnd::save(){
}

void cGameStateEnd::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

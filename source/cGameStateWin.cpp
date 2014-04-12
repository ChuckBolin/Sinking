#include "..\includes\cGameStateWin.h"
#include "..\includes\CGlobal.h"

//objects
//extern Graphics con;
//extern CAudioManager g_Audio;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateWin::cGameStateWin(){
}

cGameStateWin::~cGameStateWin()
{
	m_TE.clear();
}

void cGameStateWin::initialize(){

}

void cGameStateWin::activate(){
}

cGameStateObject* cGameStateWin::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  if(keyDown(VK_SPACE) && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_SELECT;
    ::ShowCursor(true);
  }
 
  CAudioManager *pAudio = CAudioManager::Instance();
  CGameData *pData = CGameData::Instance();

  if(pData->m_missionOver == MISSION_OVER_WIN){//collided with surface ship
    if(pAudio->IsPlaying(SOUND_ANTHEM) == false)
      pAudio->PlaySoundClip(SOUND_ANTHEM, 1);
  }

  if(pAudio->IsPlaying(SOUND_PING) == true)
    pAudio->StopSoundClip(SOUND_PING);

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateWin::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();
  
  //display flag
  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(108);
  con.RenderGraphicModulate(con.GetScreenCenterX() - gi.width/2,  con.GetScreenCenterY() - gi.height/2 - 100 ,gi, m_red, m_green, m_blue);


  CGameData *pData = CGameData::Instance();
  if(pData->m_missionOver == MISSION_OVER_WIN){
    con.Draw2DText("Congratulations Captain!  You have met all objectives for this mission.", F_V20, con.GetScreenCenterX() - 300, con.GetScreenCenterY() + 60, 255,255,255);
    con.Draw2DText("Press SPACE to continue!", F_V20, con.GetScreenCenterX() - 110, con.GetScreenCenterY() + 100, 255,255,255);
  }

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  con.EndDrawing ();
  con.Present();
}

void cGameStateWin::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateWin::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateWin::resume(){
}
void cGameStateWin::pause(){
}
void cGameStateWin::save(){
}

void cGameStateWin::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

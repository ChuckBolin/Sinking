#include "..\includes\cGameStateLose.h"
#include "..\includes\CGlobal.h"

//objects
//extern Graphics con;
//extern CAudioManager g_Audio;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateLose::cGameStateLose(void){
}

cGameStateLose::~cGameStateLose(void)
{
	m_TE.clear();
}

void cGameStateLose::initialize(){

}

void cGameStateLose::activate(){
}

cGameStateObject* cGameStateLose::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  CAudioManager *pAudio = CAudioManager::Instance();
  CGameData *pData = CGameData::Instance();

  //if(pAudio->IsPlaying(SOUND_AMBIENCE1) == false)
  //  pAudio->PlaySoundClip(SOUND_AMBIENCE1, 1);
  static bool bFirstTime = true;
  if(pData->m_missionOver == MISSION_OVER_COLLISION){//collided with surface ship
    if(pAudio->IsPlaying(SOUND_SINKING)== false)//explosion done playing
      pAudio->PlaySoundClip(SOUND_SINKING, 0);
    if(pAudio->IsPlaying(SOUND_TAPS) == false)
      pAudio->PlaySoundClip(SOUND_TAPS, 1);
      bFirstTime = false;
  }
  else if(pData->m_missionOver == MISSION_OVER_DEAD){//sunk by depth charges
    if(pAudio->IsPlaying(SOUND_SINKING) == false)//explosion done playing
      pAudio->PlaySoundClip(SOUND_SINKING, 0);
    if(pAudio->IsPlaying(SOUND_TAPS) == false )//explosion done playing
      pAudio->PlaySoundClip(SOUND_TAPS, 1);
      bFirstTime = false;
  }
  else if(pData->m_missionOver == MISSION_OVER_TIMEOVER){//unfit to command
    if(pAudio->IsPlaying(SOUND_FUNERAL) == false )
      pAudio->PlaySoundClip(SOUND_FUNERAL, 1);
  }

  if(pAudio->IsPlaying(SOUND_PING) == true)
      pAudio->StopSoundClip(SOUND_PING);

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

void cGameStateLose::render(Graphics &con){


  CGameData *pData = CGameData::Instance();
  
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 88, 0);
  con.BeginDrawing();

  if(pData->m_missionOver == MISSION_OVER_COLLISION){//collided with surface ship
    con.Draw2DText("Due to your recklessness on the high seas you will go to your grave", F_V20, con.GetScreenCenterX() - 300, con.GetScreenCenterY() - 10, 255,255,255);
    con.Draw2DText("along with 55 officers and crewmembers.  If only you had paid attention...", F_V20, con.GetScreenCenterX() - 330, con.GetScreenCenterY() + 10, 255,255,255);
    con.Draw2DText("Press SPACE to continue!", F_V20, con.GetScreenCenterX() - 110, con.GetScreenCenterY() + 100, 255,255,255);
  }
  else if(pData->m_missionOver == MISSION_OVER_DEAD){//sunk by depth charges
    con.Draw2DText("You and your crew gave it your very best. You will NEVER be forgotten.", F_V20, con.GetScreenCenterX() - 300, con.GetScreenCenterY() - 10, 0,255,255);
    con.Draw2DText("You took on the Japanese Imperial Navy...and for that we are forever grateful. ", F_V20, con.GetScreenCenterX() - 320, con.GetScreenCenterY() + 10, 0,255,255);
    con.Draw2DText("Press SPACE to continue!", F_V20, con.GetScreenCenterX() - 110, con.GetScreenCenterY() + 100, 255,255,255);
  }
  else if(pData->m_missionOver == MISSION_OVER_TIMEOVER){//unfit to command
    con.Draw2DText("To the Commanding Officer of the USS Tautog. You are immediately", F_V20, con.GetScreenCenterX() - 300, con.GetScreenCenterY() - 10, 0,255,255);
    con.Draw2DText("relieved.  Lt. Bolin, you are ordered to assume command and return", F_V20, con.GetScreenCenterX() - 280, con.GetScreenCenterY() + 10, 0,255,255);
    con.Draw2DText("your boat to Midway for refueling and resupply.", F_V20, con.GetScreenCenterX() - 200, con.GetScreenCenterY() + 30, 0,255,255);
    con.Draw2DText("Press SPACE to continue!", F_V20, con.GetScreenCenterX() - 110, con.GetScreenCenterY() + 100, 255,255,255);
  }
  
  
  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  con.EndDrawing ();
  con.Present();
}

void cGameStateLose::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateLose::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateLose::resume(){
}
void cGameStateLose::pause(){
}
void cGameStateLose::save(){
}

void cGameStateLose::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

#include "..\includes\cgamestateintro.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateIntro::cGameStateIntro(void){
}

cGameStateIntro::~cGameStateIntro(void)
{
	m_TE.clear();
}

void cGameStateIntro::initialize(){
  //moves flag
  m_motionTimer.initialize();
  m_motionCount = 0;
  m_motionStep = 0;
}

void cGameStateIntro::activate(){
   ::ShowCursor(true);
   ::SetCursor(::LoadCursor(NULL, (LPCSTR)IDC_CURSOR3));
}

cGameStateObject* cGameStateIntro::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;

  
 // if(keyDown(VK_SPACE) && ::GetActiveWindow() == g_hWnd){
 //   m_event = EVENT_GO_MAIN;
 //   ::ShowCursor(true);
 // }

  //loads mouse position into global variable.
  //loads mouse position into global variable.
  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y - rct.top;

  if(m_motionTimer.getTimer(0.005) == true){
    m_motionCount += m_motionStep;
  }

  if(g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd) 
      m_event = EVENT_GO_MAIN;

  //play music
  CAudioManager *pAudio = CAudioManager::Instance();
  if(pAudio->IsPlaying(SOUND_REMEMBER) == false)
    pAudio->PlaySoundClip(SOUND_REMEMBER, 0);


  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateIntro::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();

  //render splash screen
  GRAPHIC_IMAGE gi;
  
  //flag is advanced by this amount
  m_motionStep = 1;//con.GetScreenWidth() / 256;

  //render splash - Tautog at sea
  gi = g_Sprite.GetSpriteData(1);
  gi.scale = 0.5;
  con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, 255, 255, 255);// m_red, m_green, m_blue);

  //TIGRS Rating
  gi = g_Sprite.GetSpriteData(31);  
  con.RenderGraphicModulate(con.GetScreenWidth() - 180, con.GetScreenHeight() - 100, gi, 255, 255, 255);// m_red, m_green, m_blue);

  //move flag
  gi = g_Sprite.GetSpriteData(11);
  gi.scale = 1.0;
  con.RenderGraphicModulate(m_motionCount, m_motionCount * 2, gi, m_red, m_green, m_blue);

  if(m_motionCount > 200){
    gi = g_Sprite.GetSpriteData(12);
    gi.scale = 1.0;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2), 100, gi, m_red, m_green, m_blue);
    con.Draw2DText("Left click to continue...", F_V20, con.GetScreenCenterX() - 120, con.GetScreenHeight() - 50, 200, 0, 0);
  }

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  con.EndDrawing ();
  con.Present();
}

void cGameStateIntro::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateIntro::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  
}
void cGameStateIntro::resume(){
}
void cGameStateIntro::pause(){
}
void cGameStateIntro::save(){
}

void cGameStateIntro::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
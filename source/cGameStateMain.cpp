/*
  cGameStateMain.cpp - Controls the user interface and display for the main menu. 

*/
#include "..\includes\cgamestatemain.h"
#include "..\includes\keystatus.h"

extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateMain::cGameStateMain(void)
{
}

cGameStateMain::~cGameStateMain(void)
{
	m_TE.clear();
}

void cGameStateMain::initialize(){
  m_selection = 0;
  m_timer.initialize();
  m_red = 255;
  m_green = 255;
  m_blue = 255;

}

void cGameStateMain::activate(){
  //::ShowCursor(false);
  m_bActivated = false;
  m_enableTimer.initialize();
  m_enableTimer.ResetTimer();

  ::ShowCursor(true);
  ::SetCursor(::LoadCursor(NULL, (LPCSTR)IDC_CURSOR3));
}

cGameStateObject* cGameStateMain::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;

  CAudioManager *pAudio = CAudioManager::Instance();

  if(m_enableTimer.getTimer(.2) == true)
    m_bActivated = true;

  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y - rct.top;

  
  static double timeDiffTotal = 0;
  static int last_selection = -1;

  timeDiffTotal += timeDifference;
  if(timeDiffTotal > .1){
    timeDiffTotal = 0;
    /*
    if(keyDown(VK_UP)==true && ::GetActiveWindow() == g_hWnd){
      m_selection--;
      if(m_selection < 0)
        m_selection = 3;
      if(pAudio->IsPlaying(SOUND_BEEP) == false)
        pAudio->PlaySoundClip(SOUND_BEEP, 0);
    }

    if(keyDown(VK_DOWN)==true && ::GetActiveWindow() == g_hWnd){
      m_selection++;
      if(m_selection > 3)
        m_selection = 0;
      if(pAudio->IsPlaying(SOUND_BEEP) == false)
        pAudio->PlaySoundClip(SOUND_BEEP, 0);
    }*/

    if(::GetActiveWindow() == g_hWnd){
      //if(g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
      if(g_Global.g_mouse.y > 140 && g_Global.g_mouse.y <= 232){
        m_selection = 0;
      }
      else if(g_Global.g_mouse.y > 232 && g_Global.g_mouse.y <= 295){
        m_selection = 1;
      }
      else if(g_Global.g_mouse.y > 295 && g_Global.g_mouse.y <= 395){
        m_selection = 2;
      }
      else if(g_Global.g_mouse.y > 395){// && g_Global.g_mouse.y <= 472){
        m_selection = 3;
      }
    }
    if(m_selection != last_selection){
      if(pAudio->IsPlaying(SOUND_BEEP) == false)
        pAudio->PlaySoundClip(SOUND_BEEP, 0);
    }
    last_selection = m_selection;
  }

  //must calculate scale first since screen size may change and therefore the menu too
  //float scale = (float)g_con.GetScreenWidth() / 1024; //width game is designed to be played
 float scale = 1.0f;
 if( (keyDown(VK_RETURN)== true || g_Global.g_bLeftMouseDown == true) 
    && m_bActivated == true 
    && ::GetActiveWindow() == g_hWnd){//|| g_Global.g_bLeftMouseDown == true){
    switch(m_selection){
      case 0:
        m_event = EVENT_GO_SELECT;
        break;
      case 1:
        m_event = EVENT_GO_RELOAD;
        break;
      case 2:
        m_event = EVENT_GO_HISTORY;
        break;
      case 3:
        m_event = EVENT_GO_CREDITS;
        break;
    }
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
        return m_TE[i].p_gso;
    }
  }

  return 0;
}

void cGameStateMain::render(Graphics &con){

  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();
  
  //float scale = 1.0;//(float)g_con.GetScreenWidth() / 1024;
  //float scaleY = (float)g_con.GetScreenHeight()/768;
  //GRAPHIC_IMAGE gi;

  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(13);
  con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi,255, 255, 255);// m_red, m_green, m_blue);
  
  gi = g_Sprite.GetSpriteData(23);
  switch(m_selection){
    case 0:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 315 , con.GetScreenCenterY() - 247, gi, m_red, m_green, m_blue);
      break;
    case 1:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 312 , con.GetScreenCenterY() - 152, gi, m_red, m_green, m_blue);
      break;
    case 2:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 333 , con.GetScreenCenterY() - 86, gi, m_red, m_green, m_blue);
      break;
    case 3:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 230 , con.GetScreenCenterY() + 14, gi, m_red, m_green, m_blue);
      break;
  }
 
  //con.Draw2DTextValue("X", (float)g_Global.g_mouse.x, F_V20, 0, 20, 255, 0, 0);
  //con.Draw2DTextValue("Y", (float)g_Global.g_mouse.y, F_V20, 0, 40, 255, 0, 0);

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true){
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);
    con.Draw2DTextValue("X", (float)g_Global.g_mouse.x, F_V20, 0, 180, 255, 0, 0);
    con.Draw2DTextValue("Y", (float)g_Global.g_mouse.y, F_V20, 0, 200, 255, 0, 0);
  }

  con.EndDrawing ();
  con.Present();
}

void cGameStateMain::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}

void cGameStateMain::deactivate(){

}
void cGameStateMain::resume(){
}
void cGameStateMain::pause(){
}
void cGameStateMain::save(){
}

void cGameStateMain::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
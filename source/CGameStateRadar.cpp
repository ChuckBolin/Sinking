#include "..\includes\cGameStateRadar.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateRadar::cGameStateRadar(void){
}

cGameStateRadar::~cGameStateRadar(void)
{
	m_TE.clear();
}

void cGameStateRadar::initialize(){

}

void cGameStateRadar::activate(){
}

cGameStateObject* cGameStateRadar::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  //loads mouse position into global variable.
  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y + rct.top;
  
  static double timeDiffTotal = 0;
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > .1){
    timeDiffTotal = 0;

    //process short cut buttons
    if(g_Global.g_mouse.y > 10 && g_Global.g_mouse.y < 130 
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
      if(g_Global.g_mouse.x > 10 && g_Global.g_mouse.x < 110)
        m_event = EVENT_GO_SONAR;
      //else if(g_Global.g_mouse.x > 120 && g_Global.g_mouse.x < 220)
      //  m_event = EVENT_GO_RADAR;
      else if(g_Global.g_mouse.x > 230 && g_Global.g_mouse.x < 330)
        m_event = EVENT_GO_CONTROL;
      else if(g_Global.g_mouse.x > 694 && g_Global.g_mouse.x < 794)
        m_event = EVENT_GO_FIRECONTROL;
      else if(g_Global.g_mouse.x > 804 && g_Global.g_mouse.x < 904)
        m_event = EVENT_GO_STATUS;
      else if(g_Global.g_mouse.x > 914 && g_Global.g_mouse.x < 1014)
        m_event = EVENT_GO_CHART;
    }      
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateRadar::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();
  GRAPHIC_IMAGE gi;

  //display short cuts
  gi = g_Sprite.GetSpriteData(36);//sonar
  con.RenderGraphicModulate(10,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(43);//radar
  con.RenderGraphicModulate(120,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(38);//conn selected
  con.RenderGraphicModulate(230,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(39);//fire control
  con.RenderGraphicModulate(694,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(40);//damage control
  con.RenderGraphicModulate(804,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(41);//chart
  con.RenderGraphicModulate(914,10,gi,m_red, m_green, m_blue);

  //draw tool tips for short cuts
  if(g_Global.g_mouse.y > 10 && g_Global.g_mouse.y < 130){
    if(g_Global.g_mouse.x > 10 & g_Global.g_mouse.x < 104)
      con.Draw2DText("Sonar", F_V20, 20, 10, 0,255,255);
    //if(g_Global.g_mouse.x > 120 & g_Global.g_mouse.x < 207)
    //  con.Draw2DText("Radar", F_V20, 130, 10, 0,255,255);
    else if(g_Global.g_mouse.x > 230 & g_Global.g_mouse.x < 310)
      con.Draw2DText("Conn", F_V20, 240, 10, 0,255,255);
    else if(g_Global.g_mouse.x > 694 & g_Global.g_mouse.x < 794)
      con.Draw2DText("Torpedos", F_V20, 704, 10, 0,255,255);
    else if(g_Global.g_mouse.x > 804 & g_Global.g_mouse.x < 904)
      con.Draw2DText("Damage", F_V20, 814, 10, 0,255,255);
    else if(g_Global.g_mouse.x > 914 & g_Global.g_mouse.x < 1014)
      con.Draw2DText("Chart", F_V20, 924, 10, 0,255,255);
  }

  con.Draw2DText("Radar state", F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() - 10, 0,255,255);

  // SMJ - 11/13/2007 Draw FPS on screen
  //if(g_Global.g_bDisplayFramerate == true)
  con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 180, 0,0,255);
  con.Draw2DTextValue("X", (float)g_Global.g_mouse.x, F_V20, 0, 140, 255, 0, 0);
  con.Draw2DTextValue("Y", (float)g_Global.g_mouse.y, F_V20, 0, 160, 255, 0, 0);

  con.EndDrawing ();
  con.Present();
}

void cGameStateRadar::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateRadar::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateRadar::resume(){
}
void cGameStateRadar::pause(){
}
void cGameStateRadar::save(){
}

void cGameStateRadar::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

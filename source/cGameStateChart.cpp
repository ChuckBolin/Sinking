#include "..\includes\cGameStateChart.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateChart::cGameStateChart(void){
}

cGameStateChart::~cGameStateChart(void)
{
	m_TE.clear();
}

void cGameStateChart::initialize(){
  m_chartScale[0] = 20000;
  m_chartScale[1] = 10000;
  m_chartScale[2] = 5000;
  m_chartScale[3] = 2000;
  m_chartScale[4] = 1000;
  m_chartScaleIndex = 1; //10000 yards

  m_red = 255;
  m_green = 255;
  m_blue = 255;
}

void cGameStateChart::activate(){
}

cGameStateObject* cGameStateChart::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  //loads mouse position into global variable.
  
  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y - rct.top;
  
  CGameData *pData = CGameData::Instance();
  //start ambient sound loop
  CAudioManager *pAudio = CAudioManager::Instance();
  if(pAudio->IsPlaying(SOUND_AMBIENCE1) == false)
    pAudio->PlaySoundClip(SOUND_AMBIENCE1, 1);

  if(pData->m_bAttack == true){
    if(pAudio->IsPlaying(SOUND_PING) == false)
      pAudio->PlaySoundClip(SOUND_PING,0);
  }

  if(pData->m_bDropped == true && pAudio->IsPlaying(SOUND_DEPTH_CHARGE2) == false)
        pAudio->PlaySoundClip(SOUND_DEPTH_CHARGE2, 0);

  //update all targets, sub, torpedos, and depth charges
  pData->Update(timeDifference);

  static double timeDiffTotal = 0;
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > .11){
    timeDiffTotal = 0;

    if(keyDown(VK_SPACE) == true && ::GetActiveWindow() == g_hWnd){
      CLog *pLog = CLog::Instance();
      pLog->Log("Tautog ...............");
      pLog->Log("subX", (long)pData->m_Player.m_posX);
      pLog->Log("subY", (long)pData->m_Player.m_posY);
      pLog->Log("subH", pData->m_Player.m_heading);
      pLog->Log("subS", pData->m_Player.m_speed);
      pLog->Log("..............");

      for(int i = 0; i < pData->m_targets.size(); ++i){
        pLog->Log("target", i);
        pLog->Log("targetX", (long)pData->m_targets[i].m_posX);
        pLog->Log("targetY", (long)pData->m_targets[i].m_posY);
        pLog->Log("targetH", pData->m_targets[i].m_heading);
        pLog->Log("targetS", pData->m_targets[i].m_speed);
        pLog->Log("targetTB", pData->m_targets[i].m_trueBearing);
        pLog->Log("targetRB", pData->m_targets[i].m_relativeBearing);
        pLog->Log("targetAOB",(int)pData->m_targets[i].m_AOB);
        pLog->Log("targetAOBindex", pData->m_targets[i].m_AOBindex);
        pLog->Log(".......................");
      }
    }

    //process short cut buttons
    if(g_Global.g_mouse.y > 10 && g_Global.g_mouse.y < 130 
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
      //if(g_Global.g_mouse.x > 10 && g_Global.g_mouse.x < 110)
      //  m_event = EVENT_GO_SONAR;
      //else if(g_Global.g_mouse.x > 120 && g_Global.g_mouse.x < 220)
      //  m_event = EVENT_GO_RADAR;
      if(g_Global.g_mouse.x > 230 && g_Global.g_mouse.x < 330)
        m_event = EVENT_GO_CONTROL;
      //else if(g_Global.g_mouse.x > 694 && g_Global.g_mouse.x < 794)
      //  m_event = EVENT_GO_FIRECONTROL;
      //else if(g_Global.g_mouse.x > 804 && g_Global.g_mouse.x < 904)
      //  m_event = EVENT_GO_STATUS;
    } 
    else if(g_Global.g_mouse.x > 806 && g_Global.g_mouse.x < 1006 
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
        if(g_Global.g_mouse.y > 134 && g_Global.g_mouse.y < 249){//zoom in
          m_chartScaleIndex++;
          if(m_chartScaleIndex > 4)
            m_chartScaleIndex = 4;
        }
        else if(g_Global.g_mouse.y > 269 && g_Global.g_mouse.y < 384){//zoom out
          m_chartScaleIndex--;
          if(m_chartScaleIndex < 0)
            m_chartScaleIndex = 0;
        }
    }
  }
  
  //this controls game states
  if(pData->m_missionOver == MISSION_OVER_WIN  )
    m_event = EVENT_GO_WIN;
  else if(pData->m_missionOver == MISSION_OVER_DEAD || pData->m_missionOver == MISSION_OVER_TIMEOVER || pData->m_missionOver == MISSION_OVER_COLLISION )
    m_event = EVENT_GO_LOSE;

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateChart::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();
  GRAPHIC_IMAGE gi;

  CGameData *pData = CGameData::Instance();

  //draw chart stuff
  gi = g_Sprite.GetSpriteData(48);//white square for chart background
  gi.scale = 5.05;
  con.RenderGraphicModulate(262 + pData->m_shockX, 134 + pData->m_shockX, gi, m_red, m_green, m_blue);

  //chart highlights and reference lines
  con.DrawRect(262, 134, 762, 634,255, 0, 0);
  con.DrawLine(512, 134, 512, 634, 0, 0, 120);
  con.DrawLine(262, 384, 762, 384, 0, 0, 120);
  
  //draw user interface on right side of chart
  con.DrawLine(780, 134, 800, 134, 0, 255, 0);//horizontal tick marks
  con.DrawLine(780, 249, 800, 249, 0, 255, 0);
  con.DrawLine(780, 269, 800, 269, 0, 255, 0);
  con.DrawLine(780, 384, 800, 384, 0, 255, 0);
  con.DrawLine(790, 134, 790, 249, 0, 255, 0);//vertical lines
  con.DrawLine(790, 269, 790, 384, 0, 255, 0);

  //draw range
  con.Draw2DTextValue("SCALE", (float)m_chartScale[m_chartScaleIndex], F_V20, 780, 249, 0, 255, 0);

  //zoom in and zoom out
  if(g_Global.g_mouse.x > 806 && g_Global.g_mouse.x < 1006 
    && ::GetActiveWindow() == g_hWnd){
    if(g_Global.g_mouse.y > 134 && g_Global.g_mouse.y < 249 && m_chartScaleIndex < 4){//zoom in
      gi = g_Sprite.GetSpriteData(30);
      con.RenderGraphicModulate(811,139,gi,m_red, m_green, m_blue);
    }
    else if(g_Global.g_mouse.y > 269 && g_Global.g_mouse.y < 384 && m_chartScaleIndex > 0){//zoom out
      gi = g_Sprite.GetSpriteData(29);
      con.RenderGraphicModulate(811,274,gi,m_red, m_green, m_blue);
    }
  }

  //display ship and sub data
//  CGameData *pData = CGameData::Instance();
  float posX = 0, posY = 0;
  float range = (float)m_chartScale[m_chartScaleIndex];

  //ships
  for(int i = 0; i < pData->m_targets.size(); ++i){
    posX = (pData->m_targets[i].m_posX - pData->m_Player.m_posX) * 250/range;
    posY = (pData->m_targets[i].m_posY - pData->m_Player.m_posY) * 250/range;

    if(pData->m_targets[i].m_bAlive == true &&  posX >= -250 && posX <= 250 && posY >= -250 && posY <= 250)//{
      con.DrawCircle(512 + posX,384 - posY, 255,0,0,(pData->m_targets[i].m_length/500) * (20000/(range * 2) ));
  }

  //torpedo
  for(int i = 0; i < pData->m_torpedos.size(); ++i){
    posX = (pData->m_torpedos[i].GetPositionX()- pData->m_Player.m_posX) * 250/range;
    posY = (pData->m_torpedos[i].GetPositionY() - pData->m_Player.m_posY) * 250/range;

    if(pData->m_torpedos[i].IsRunning() == true &&  posX >= -250 && posX <= 250 && posY >= -250 && posY <= 250)//{
      con.DrawCircle(512 + posX,384 - posY, 0, 0, 0,2 * (20000/(range * 8)));
  }

  //draw scope bearing line to indicate which target is being viewed
  float s = 0, t = 0;
  s = 512 + 250 * cos(pData->ConvertDegreesToRadians( pData->m_Player.m_heading + pData->m_scopeRotateAngle/5.689));
  t = 384 - 250 * sin(pData->ConvertDegreesToRadians( pData->m_Player.m_heading + pData->m_scopeRotateAngle/ 5.689));
  con.DrawLine(512,384,s,t, 0, 255,0);

  //draw sub in center of chart
  gi = g_Sprite.GetSpriteData(57);
  gi.rotationAngle =  pData->m_Player.m_heading * 3.141592654/180;
  gi.scale = 1;// (1000/range);
  con.RenderGraphicModulate(512 - (gi.width/2)* gi.scale, 384 - (gi.height/2) * gi.scale, gi, m_red, m_green, m_blue);

  //display short cuts
  gi = g_Sprite.GetSpriteData(36);//sonar
  con.RenderGraphicModulate(10,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(37);//radar
  con.RenderGraphicModulate(120,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(38);//conn selected
  con.RenderGraphicModulate(230,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(39);//fire control
  con.RenderGraphicModulate(694,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(40);//damage control
  con.RenderGraphicModulate(804,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(47);//chart
  con.RenderGraphicModulate(914,10,gi,m_red, m_green, m_blue);

  //draw tool tips for short cuts
  if(g_Global.g_mouse.y > 10 && g_Global.g_mouse.y < 130){
    //if(g_Global.g_mouse.x > 10 & g_Global.g_mouse.x < 104)
    //  con.Draw2DText("Sonar", F_V20, 20, 10, 0,255,255);
    //else if(g_Global.g_mouse.x > 120 & g_Global.g_mouse.x < 207)
    //  con.Draw2DText("Radar", F_V20, 130, 10, 0,255,255);
    if(g_Global.g_mouse.x > 230 & g_Global.g_mouse.x < 310)
      con.Draw2DText("Conn", F_V20, 240, 10, 0,255,255);
    //else if(g_Global.g_mouse.x > 694 & g_Global.g_mouse.x < 794)
    //  con.Draw2DText("Torpedos", F_V20, 704, 10, 0,255,255);
    //else if(g_Global.g_mouse.x > 804 & g_Global.g_mouse.x < 904)
    //  con.Draw2DText("Damage", F_V20, 814, 10, 0,255,255);
  }

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
      con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 720, 0,255,255);
  //con.Draw2DTextValue("subX", (long)pData->m_Player.m_posX, F_V20, 0, 140, 255, 0, 0);
  //con.Draw2DTextValue("subY", (long)pData->m_Player.m_posY, F_V20, 0, 160, 255, 0, 0);
  //con.Draw2DTextValue("T", (long)pData->m_torpedos.size(), F_V20, 0, 200, 255, 0, 0);
  
  con.Draw2DTextValue("BRG", (long)( (float)pData->m_scopeRotateAngle/5.689),F_V20,484, 640, 255, 255, 0);
  con.Draw2DTextValue("Damage", (long)( (float)pData->m_Player.m_damage),F_V20,474, 660, 255, 0, 0);

  //display target data
  //for(int i = 0; i < pData->m_targets.size(); ++i){
  ///  con.Draw2DTextValue("X", (long)pData->m_targets[i].m_posX, F_V20, 0, 220 + (i * 20), 255, 0, 0);
  //  con.Draw2DTextValue("Y", (long)pData->m_targets[i].m_posY, F_V20, 120, 220 + (i * 20), 255, 0, 0);
  //}

  con.EndDrawing ();
  con.Present();
}

void cGameStateChart::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateChart::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateChart::resume(){
}
void cGameStateChart::pause(){
}
void cGameStateChart::save(){
}

void cGameStateChart::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

#include "..\includes\cGameStateScope.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateScope::cGameStateScope(){
}

cGameStateScope::~cGameStateScope()
{
	m_TE.clear();
}

void cGameStateScope::initialize(){
  //periscope
  //button animation
  m_bButtonPressed = false;
  m_buttonOffset = 0;
  m_timerButton.initialize();

  //rolling submarine
  m_timerRoll.initialize();
  m_rollAngle = 0;
  m_rollDirection = 0;

  m_bEnableTorpedo = true;
  m_TimerTorpedo.initialize();

}

void cGameStateScope::activate(){
   ::ShowCursor(true);
   ::SetCursor(::LoadCursor(NULL, (LPCSTR)IDC_CURSOR3));
}

//***************************************************************
//                         U P D A T E
//***************************************************************
cGameStateObject* cGameStateScope::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  //if(keyDown(VK_SPACE) && ::GetActiveWindow() == g_hWnd){
  //  m_event = EVENT_GO_CONTROL;
  //  ::ShowCursor(false);
 // }

  //loads mouse position into global variable.
  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y - rct.top;
 
  //start ambient sound loop
  CAudioManager *pAudio = CAudioManager::Instance();
  if(pAudio->IsPlaying(SOUND_AMBIENCE1) == false)
    pAudio->PlaySoundClip(SOUND_AMBIENCE1, 1);

  CGameData *pData = CGameData::Instance();
  pData->Update(timeDifference);

  if(pData->m_bAttack == true){
    if(pAudio->IsPlaying(SOUND_PING) == false)
      pAudio->PlaySoundClip(SOUND_PING,0);
  }

  if(pData->m_bDropped == true && pAudio->IsPlaying(SOUND_DEPTH_CHARGE2) == false)
        pAudio->PlaySoundClip(SOUND_DEPTH_CHARGE2, 0);

  //exit scope to conn if scope is going down
  if(pData->m_bScopeUp == false && pData->m_bScopeDown == true && (pData->m_scopePosition == pData->m_scopeMinPosition))
    m_event = EVENT_GO_CONTROL;

  //code to animate roll
  //********************************************
  if(m_timerRoll.getTimer(0.1) == true){
    m_maxRollAngle = pData->m_seaState * 6;

    if(m_rollDirection == 0){//roll right
      m_rollAngle += pData->m_seaState/3;//2;
      if(m_rollAngle > m_maxRollAngle)
        m_rollDirection = 1;
    }
    else{  //roll left
      m_rollAngle -= pData->m_seaState/3;//2;
      if(m_rollAngle < -m_maxRollAngle)
        m_rollDirection = 0;
    }

    m_rollDeltaY = sin(m_rollAngle * 3.14/180) * 50;
    m_rollDeltaX = cos(m_rollAngle * 3.14/180) * 50;
  }

  //code to animate button and change states
  if(m_timerButton.getTimer(0.03) == true && m_bButtonPressed == true){
    m_buttonOffset++;
    if(m_buttonOffset > 6){
      m_bButtonPressed = false;
      m_buttonOffset = 0;
          //CTorpedo torpedo((float)pData->m_scopeRotateAngle/5.689, (float)pData->m_scopeRotateAngle/5.689, pData->m_Player.m_posX, pData->m_Player.m_posY);
          //CTorpedo torpedo((float)pData->m_Player.m_trueBearing, (float)pData->m_Player.m_trueBearing, pData->m_Player.m_posX, pData->m_Player.m_posY);
          CTorpedo torpedo( (float)(pData->m_Player.m_heading + pData->m_scopeRotateAngle/5.689), (float)(pData->m_Player.m_heading + pData->m_scopeRotateAngle/5.689), pData->m_Player.m_posX, pData->m_Player.m_posY);
          pData->m_torpedos.push_back(torpedo);       

          //there are 2 torpedo sounds...play 1..if busy play 2
          if(pAudio->IsPlaying(SOUND_TORPEDO1) == false)
            pAudio->PlaySoundClip(SOUND_TORPEDO1, 0);
          else{
            if(pAudio->IsPlaying(SOUND_TORPEDO2) == false)
              pAudio->PlaySoundClip(SOUND_TORPEDO2, 0);
          }

    }
  }

  //monitors mouse buttons and key presses
  static double timeDiffTotal = 0;
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > .1){
    timeDiffTotal = 0;

    if(keyDown(VK_UP) == true){
      pData->m_nightColor++;
      if(pData->m_nightColor > 255)
        pData->m_nightColor = 255;
    }
    if(keyDown(VK_DOWN) == true){
      pData->m_nightColor--;
      if(pData->m_nightColor < 0)
        pData->m_nightColor = 0;
    }

    //zoom in
    if(g_Global.g_mouse.x > 389 && g_Global.g_mouse.x < 655 
            && g_Global.g_mouse.y > 431 && g_Global.g_mouse.y < 542 
            && g_Global.g_bLeftMouseDown == true
            && ::GetActiveWindow() == g_hWnd){
      pData->m_scopeZoom = 2.5;//+= 0.1;
    }
    //zoom out
    else if(g_Global.g_mouse.x > 389 && g_Global.g_mouse.x < 675 
            && g_Global.g_mouse.y > 27 && g_Global.g_mouse.y < 119
            && g_Global.g_bLeftMouseDown == true
            && ::GetActiveWindow() == g_hWnd){
      pData->m_scopeZoom = .5;//-= 0.1;
    }

    //fire torpedo
    if(m_bEnableTorpedo == true){
      if(g_Global.g_mouse.x > 662 && g_Global.g_mouse.x < 755 
        && g_Global.g_mouse.y > 425 && g_Global.g_mouse.y < 517 
        && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
          if(IsInside(g_Global.g_mouse.x, g_Global.g_mouse.y, 707, 472, 40) == true){
            if(pAudio->IsPlaying(SOUND_BUTTON_CLICK) == false) 
              pAudio->PlaySoundClip(SOUND_BUTTON_CLICK,0);
            m_bButtonPressed = true;
            m_bEnableTorpedo = false;
            m_TimerTorpedo.ResetTimer();
          }
      }
    }

    //enable torpedo firing after 2 seconds
    if(m_bEnableTorpedo == false && m_TimerTorpedo.getTimer(2.0) == true)
      m_bEnableTorpedo = true;

    //return to conn
    else if(g_Global.g_mouse.x > 291 && g_Global.g_mouse.x < 391 
      && g_Global.g_mouse.y > 441 && g_Global.g_mouse.y < 536
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
        m_event = EVENT_GO_CONTROL;     
    }

    //lower scope
    else if(g_Global.g_mouse.x > 291 && g_Global.g_mouse.x < 391
      && g_Global.g_mouse.y > 48 && g_Global.g_mouse.y < 148 
      && g_Global.g_bLeftMouseDown == true
      && ::GetActiveWindow() == g_hWnd){
        pData->m_bScopeUp = false;
        pData->m_bScopeDown = true;
        if(pAudio->IsPlaying(SOUND_PERISCOPE) == false)
          pAudio->PlaySoundClip(SOUND_PERISCOPE, 0);
      }

  }
    //rotate left
    if(g_Global.g_mouse.x > 256 && g_Global.g_mouse.x < 315
                                && g_Global.g_mouse.y > 150 && g_Global.g_mouse.y < 421  
                                && g_Global.g_bLeftMouseDown == true
                                && ::GetActiveWindow() == g_hWnd){
        pData->m_bRotateLeft = true;
        pData->m_bRotateRight = false;
        pData->m_scopeControlValue = g_Global.g_mouse.x - 315;
      }

    //rotate right
    else if(g_Global.g_mouse.x > 710 && g_Global.g_mouse.x < 777 
                                    && g_Global.g_mouse.y > 150 && g_Global.g_mouse.y < 421 
                                    && g_Global.g_bLeftMouseDown == true
                                    && ::GetActiveWindow() == g_hWnd){  
        pData->m_bRotateLeft = false;
        pData->m_bRotateRight = true;
        pData->m_scopeControlValue = g_Global.g_mouse.x - 710;
    }

    //no rotation
    else{
      pData->m_bRotateLeft = false;
      pData->m_bRotateRight = false;
    }

    //explosion sounds
    for(int i = 0; i < pData->m_targets.size(); ++i){
      if(pData->m_targets[i].m_bExplode == true){
        if(pAudio->IsPlaying(SOUND_DEPTH_CHARGE1) == false)
          pAudio->PlaySoundClip(SOUND_DEPTH_CHARGE1, 0);
      }
    }

//  pData->Update(timeDifference);
  //this controls game states
  if(pData->m_missionOver == MISSION_OVER_WIN)
    m_event = EVENT_GO_WIN;
  else if(pData->m_missionOver == MISSION_OVER_DEAD || pData->m_missionOver == MISSION_OVER_TIMEOVER  || pData->m_missionOver == MISSION_OVER_COLLISION )
    m_event = EVENT_GO_LOSE;

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateScope::render(Graphics &con){
  
  CGameData *pData = CGameData::Instance();

  //Clear buffer and draw graphics
  con.ClearBuffer(0, 255 - pData->m_nightColor, 255 - pData->m_nightColor, 0);
  con.BeginDrawing();

  POINT pt;
  pt.x = g_Global.g_mouse.x;
  pt.y = g_Global.g_mouse.y;
  GRAPHIC_IMAGE gi;

  //render all targets within range
  float angle = 0;
  float pos = 0;
  float delta = 0;
  float rangeFactor = 1;
  float magFactor = 1;

  //water background
  gi = g_Sprite.GetSpriteData(66);
  gi.alpha = 255;//80;
  gi.rotationAngle = m_rollAngle * 3.14/180;
  con.RenderGraphicModulate(512 - gi.rotationX, 377 - gi.rotationY,gi, m_red, m_green, m_blue);

  int target = 0; //displays 0 first
  CLog *pLog = CLog::Instance();
  int effectType = 1; //smoke
  float tx = 0, ty = 0;
  float width = 0;
  float shipHeight = 0;
  static bFirstTime = true;

  for(int j = 0; j < pData->m_targets.size(); ++j){

    //find and render farthest ship
    for(int i = 0; i < pData->m_targets.size(); ++i){
      if(pData->m_targets[i].m_rangePosition == target && pData->m_targets[i].m_bAlive == true){
        
        //only render ships within 10000 yards
        if(pData->m_targets[i].m_rangeToTarget <= 10000){
        
          angle = pData->m_targets[i].m_relativeBearing * 5.689;
          delta = pData->m_scopeRotateAngle - angle;            //linear difference in pixels
          gi = g_Sprite.GetSpriteData(pData->m_targets[i].m_AOBsprite[pData->m_targets[i].m_AOBindex]);
          
          width = (BearingWidth(pData->m_targets[i].m_rangeToTarget,
                              pData->m_targets[i].m_length,
                              pData->m_targets[i].m_beam,
                              pData->m_targets[i].m_AOBindex,
                              pData->m_scopeZoom) * 10 /* pixels per degree */);
          
          gi.scale = width/gi.width;          
          shipHeight = gi.height * gi.scale;

          gi.rotationAngle = m_rollAngle * 3.14/180;
  
          pos = delta + (gi.width * gi.scale)/2;

          ty = ((10000 - pData->m_targets[i].m_rangeToTarget)/10000.0f) * (gi.height * gi.scale)/2;
          if(abs(delta) < 1600)
          //if(delta >= 0)//bs(delta) > 1)
            ty += delta * sin(m_rollAngle * 3.14/180);
          //else
          //  ty -= delta * sin(m_rollAngle * 3.14/180);
          
          con.RenderGraphicModulate(con.GetScreenWidth()/2 - pos - 2048, 260 - ty, gi, m_red, m_green, m_blue);
          con.RenderGraphicModulate(con.GetScreenWidth()/2 - pos, 260 - ty, gi, m_red, m_green, m_blue);
          con.RenderGraphicModulate(con.GetScreenWidth()/2 - pos + 2048, 260 - ty, gi, m_red, m_green, m_blue);

          if(bFirstTime == true && i == 0){
            bFirstTime = false;
            pLog->Log("i", i);
            pLog->Log("angle", angle);
            pLog->Log("delta", angle);
            pLog->Log("width", width);
            pLog->Log("scale", gi.scale);
            pLog->Log("gi.width", gi.width);
            pLog->Log("gi.height", gi.height);
            pLog->Log("ty", ty);
            pLog->Log("pos", pos);
            pLog->Log("range",pData->m_targets[i].m_rangeToTarget);
            pLog->Log("length", pData->m_targets[i].m_length);
            pLog->Log("beam", pData->m_targets[i].m_beam);
            pLog->Log("aobindex", pData->m_targets[i].m_AOBindex);
            pLog->Log("mag",  pData->m_scopeZoom);
          }

          //draw water overlay
          if(pData->m_targets[i].m_damage > 0){

            if(pData->m_targets[i].m_bExplode == true)
              effectType = 0;
            else if(pData->m_targets[i].m_bExplode == true)
              effectType = 1;

            con.BeginPyro(con.GetScreenWidth()/2 - pos + (gi.width * gi.scale)/2, 260 - ty, effectType);
            con.EndPyro();

          
          }//if(pData
        }

        target++;
        break;
      }
    }
  }

  //water splashing
  if(abs(m_rollAngle) > 22){
    gi = g_Sprite.GetSpriteData(66);
    gi.alpha = 255;
    gi.rotationAngle = m_rollAngle * 3.14/180;
    con.RenderGraphicModulate(512 - gi.width/2, 390 - ((abs(m_rollAngle)-22) * 12) ,gi, 0, 135, 135);
  }

  //water covering scope
  if(pData->m_scopePosition < pData->m_scopeMaxPosition){
    if(pData->m_scopeMaxPosition - pData->m_scopePosition <= 300){
      gi = g_Sprite.GetSpriteData(66);
      gi.alpha = 255;
      con.RenderGraphicModulate(512 - gi.width/2, 420 - (pData->m_scopeMaxPosition - pData->m_scopePosition) ,gi, 0, 135, 135);
    }
    else if(pData->m_scopeMaxPosition - pData->m_scopePosition > 300){
      gi = g_Sprite.GetSpriteData(66);
      gi.alpha = 255;
      con.RenderGraphicModulate(512 - gi.width/2, 120,gi, 0, 135, 135);
    }
  }

  //background image
  gi = g_Sprite.GetSpriteData(28); //room layout with square hole for image
  con.RenderGraphicModulate(pData->m_shockX, pData->m_shockY ,gi, m_red, m_green, m_blue);

  //night covering
  gi = g_Sprite.GetSpriteData(27);
  gi.alpha = pData->m_nightColor;
  con.RenderGraphicModulate(312, 84 ,gi, m_red, m_green, m_blue);

  //reticule
  gi = g_Sprite.GetSpriteData(56);
  con.RenderGraphicModulate(310 + pData->m_shockX, 100 + pData->m_shockY ,gi, m_red, m_green, m_blue);

  //zoomed scope
  gi = g_Sprite.GetSpriteData(25);
  gi.scale = 0.75;
  con.RenderGraphicModulate(250 + pData->m_shockX, 10 + pData->m_shockY, gi, m_red, m_green, m_blue);//162, 34

  //brg button on top
  gi = g_Sprite.GetSpriteData(26);
  gi.scale = 0.75;
  if(m_buttonOffset < 5)
    con.RenderGraphicModulate(660 + m_buttonOffset, 422 + m_buttonOffset, gi, m_red, m_green, m_blue);//162, 34
  else if(m_buttonOffset == 5)
    con.RenderGraphicModulate(660 + 3, 422 + 3, gi, m_red, m_green, m_blue);//162, 34
  else if(m_buttonOffset == 6)
    con.RenderGraphicModulate(660 + 1, 422 + 1, gi, m_red, m_green, m_blue);//162, 34

  //lower scope
  gi = g_Sprite.GetSpriteData(21);//down arrow
  con.RenderGraphicModulate(270, 42, gi, m_red, m_green, m_blue);
  con.Draw2DText("LOWER",F_V20, 270, 22, 255, 255, 255, 255);
  con.Draw2DText("SCOPE",F_V20, 270, 102, 255, 255, 255, 255);

  //return to CONN
  gi = g_Sprite.GetSpriteData(38);
  con.RenderGraphicModulate(266, 416, gi, m_red, m_green, m_blue);
  con.Draw2DText("Return",F_V20, 291, 436, 0, 0, 255, 255);
  con.Draw2DText("  to  ",F_V20, 291, 456, 0, 0, 255, 255);
  con.Draw2DText(" CONN",F_V20, 291, 476, 0, 0, 255, 255);

  //player helps
  //******************************************************   
  //rotate left
  if(g_Global.g_mouse.x > 256 && g_Global.g_mouse.x < 315 && pData->m_bScopeUp == false
                              && g_Global.g_mouse.y > 150 && g_Global.g_mouse.y < 421  
                              && ::GetActiveWindow() == g_hWnd){
    gi = g_Sprite.GetSpriteData(18);
    con.RenderGraphicModulate(255, 255, gi, m_red, m_green, m_blue);
    }

  //rotate right
  else if(g_Global.g_mouse.x > 710 && g_Global.g_mouse.x < 777 && pData->m_bScopeUp == false
                                   && g_Global.g_mouse.y > 150 && g_Global.g_mouse.y < 421 
                                   && ::GetActiveWindow() == g_hWnd){  
    gi = g_Sprite.GetSpriteData(19);
    con.RenderGraphicModulate(711, 255, gi, m_red, m_green, m_blue);
  }
  //zoom in
  if(g_Global.g_mouse.x > 389 && g_Global.g_mouse.x < 655 
          && g_Global.g_mouse.y > 431 && g_Global.g_mouse.y < 542 
          && ::GetActiveWindow() == g_hWnd && pData->m_scopeZoom == .5){
    gi = g_Sprite.GetSpriteData(30);
    con.RenderGraphicModulate(486, 414 ,gi, m_red, m_green, m_blue);
  }
  //zoom out
  else if(g_Global.g_mouse.x > 389 && g_Global.g_mouse.x < 675 
          && g_Global.g_mouse.y > 27 && g_Global.g_mouse.y < 119
          && ::GetActiveWindow() == g_hWnd && pData->m_scopeZoom == 2.5){
    gi = g_Sprite.GetSpriteData(29);
    con.RenderGraphicModulate(486, 30 ,gi, m_red, m_green, m_blue);
  }
  
  con.Draw2DText("FIRE!!!",F_V20, 679, 388, 255, 0, 0, 255);
  con.Draw2DTextValue("BRG", (long)( (float)pData->m_scopeRotateAngle/5.689),F_V20, 484, 512, 255, 255, 0);
  con.Draw2DTextValue("Damage", (long)( (float)pData->m_Player.m_damage),F_V20, 474, 532, 255, 0, 0);
      
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 720, 0,255,255);

  //con.Draw2DTextValue("X", (float)pt.x, F_V20, 0, 180, 255, 0, 0);
  //con.Draw2DTextValue("Y", (float)pt.y, F_V20, 0, 200, 255, 0, 0);
  //con.Draw2DTextValue("acctime", (float)pData->m_accTime , F_V20, 0, 220, 255, 0, 0);
  //con.Draw2DTextValue("goaltime", (float)pData->m_goalTime , F_V20, 0, 240, 255, 0, 0);
  //con.Draw2DTextValue("accGoal", (float)pData->m_accGoal , F_V20, 0, 260, 255, 0, 0);
  //con.Draw2DTextValue("goal", (float)pData->m_goal , F_V20, 0, 280, 255, 0, 0);

//  con.Draw2DTextValue("K", (float)(pData->m_Player.m_heading + pData->m_scopeRotateAngle/5.689), F_V20, 0, 260, 255, 0, 0);
  
  con.EndDrawing ();
  con.Present();
}

float cGameStateScope::BearingWidth(float range, float length, float beam, float aob, float mag){
  float sinAngle = 0;
  if(aob == 0 || aob == 4)
    sinAngle = 0.0f;
  else if(aob == 2 || aob == 6)
    sinAngle = 1.0f;
  else
    sinAngle = .707f;

  if(beam < 25)
    beam = 25;
  float bw = (beam + sinAngle * length) / ( (range/1000) * 17.453); //17.453 yards/degree at 1000 yards //52.36);
  bw *= mag;

  return bw;
}


void cGameStateScope::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateScope::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateScope::resume(){
}
void cGameStateScope::pause(){
}
void cGameStateScope::save(){
}

void cGameStateScope::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

bool cGameStateScope::IsInside(int x1, int y1, int x2, int y2, float radius){
  double hyp = 0;

  hyp = (double)sqrt( ((double)x2 - x1)*(x2 - x1)  + (double)(y2 - y1) * (y2 - y1) );
  if(hyp < radius)
    return true;
  else
    return false;

}
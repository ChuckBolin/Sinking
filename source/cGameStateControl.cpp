#include "..\includes\cGameStateControl.h"
#include "..\includes\CGlobal.h"

//objects
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateConn::cGameStateConn(){
}

cGameStateConn::~cGameStateConn()
{
	m_TE.clear();
}

void cGameStateConn::initialize(){
  //gagues
  m_fHeading = 0;
  m_fDepth = 0;
  m_fSpeed = 0;

}

void cGameStateConn::activate(){
   //::ShowCursor(true);
   //::SetCursor(::LoadCursor(NULL, (LPCSTR)IDC_CURSOR3));
}
//***************************************************************
//                         U P D A T E
//***************************************************************
cGameStateObject* cGameStateConn::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;

  //loads mouse position into global variable.
  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y - rct.top;

  //game data
  CGameData *pData = CGameData::Instance();
  pData->Update(timeDifference);

  static bool bDepthDir = true;
  static bool bSpeedDir = true;
  float lastDepthCmd = 0;
  //start ambient sound loop
  CAudioManager *pAudio = CAudioManager::Instance();
  if(pAudio->IsPlaying(SOUND_ANCHORS) == true)
    pAudio->StopSoundClip(SOUND_ANCHORS);
  if(pAudio->IsPlaying(SOUND_AMBIENCE1) == false)
    pAudio->PlaySoundClip(SOUND_AMBIENCE1, 1);

  if(pData->m_bAttack == true){
    if(pAudio->IsPlaying(SOUND_PING) == false)
      pAudio->PlaySoundClip(SOUND_PING,0);
  }
  if(pData->m_bDropped == true && pAudio->IsPlaying(SOUND_DEPTH_CHARGE2) == false)
        pAudio->PlaySoundClip(SOUND_DEPTH_CHARGE2, 0);

  static bool bFirstTimeUpdate = false;

  if(bFirstTimeUpdate == false){
    if(pAudio->IsPlaying(SOUND_GQ) == false)
      pAudio->PlaySoundClip(SOUND_GQ, 0);
    bFirstTimeUpdate = true;
  }

  static double timeDiffTotal = 0;
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > .1){
    timeDiffTotal = 0;

    //update 
    m_fHeading += .001;
    if(m_fHeading >360)
      m_fHeading = 0;

    if(bDepthDir == true){
      m_fDepth += 1;
      if(m_fDepth >= 180)
        bDepthDir = false;
    }
    else{
      m_fDepth -= 1;
      if(m_fDepth <= 0)
        bDepthDir = true;
    }

    if(bSpeedDir == true){
      m_fSpeed += 0.2;
      if(m_fSpeed >= 24)
        bSpeedDir = false;
    }
    else{
      m_fSpeed -= 0.2;
      if(m_fSpeed <= 0)
        bSpeedDir = true;
    }

    //test of shock
    /*
    if(keyDown('G')==true){
      pData->CreateShock(75, 1.8, rand()% 628 * 0.01);// rnd(0.78);
      if(pAudio->IsPlaying(SOUND_DEPTH_CHARGE1) == false)
        pAudio->PlaySoundClip(SOUND_DEPTH_CHARGE1, 0);
    }

    if(keyDown('H') == true){
      pData->CreateShock(25, 2.0, 3.9);
      if(pAudio->IsPlaying(SOUND_DEPTH_CHARGE2) == false)
        pAudio->PlaySoundClip(SOUND_DEPTH_CHARGE2, 0);
    }

    //test losing and winning
    if(::GetActiveWindow() == g_hWnd && keyDown('C'))
       pData->m_missionOver = MISSION_OVER_COLLISION;
    if(::GetActiveWindow() == g_hWnd && keyDown('D'))
       pData->m_missionOver = MISSION_OVER_DEAD;
    if(::GetActiveWindow() == g_hWnd && keyDown('T'))
       pData->m_missionOver = MISSION_OVER_TIMEOVER;
    if(::GetActiveWindow() == g_hWnd && keyDown('W'))
       pData->m_missionOver = MISSION_OVER_WIN;
*/

    //if(keyDown('I') == true)
    //  pData->CreateShock(100, 1.6, 5.3);

    //process short cut buttons
    if(::GetActiveWindow() == g_hWnd && g_Global.g_bLeftMouseDown == true 
      && g_Global.g_mouse.y > 10 && g_Global.g_mouse.y < 130){
        //if(g_Global.g_mouse.x > 10 && g_Global.g_mouse.x < 110)
        //  m_event = EVENT_GO_SONAR;
        //else if(g_Global.g_mouse.x > 120 && g_Global.g_mouse.x < 220)
        //  m_event = EVENT_GO_RADAR;
        if(g_Global.g_mouse.x > 344 && g_Global.g_mouse.x < 390)
          m_event = EVENT_GO_SELECT;
        //else if(g_Global.g_mouse.x > 694 && g_Global.g_mouse.x < 794)
        //  m_event = EVENT_GO_FIRECONTROL;
        //else if(g_Global.g_mouse.x > 804 && g_Global.g_mouse.x < 904)
        //  m_event = EVENT_GO_STATUS;
        else if(g_Global.g_mouse.x > 914 && g_Global.g_mouse.x < 1014)
          m_event = EVENT_GO_CHART;
    }      

    //EVENT_GO_SCOPE
    if(g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd
      && g_Global.g_mouse.x > 417 && g_Global.g_mouse.x < 621
      && g_Global.g_mouse.y > 115 && g_Global.g_mouse.y < 347
      && (pData->m_scopePosition == pData->m_scopeMaxPosition)){
        m_event = EVENT_GO_SCOPE;
      }

    //raise periscope
    if(::GetActiveWindow() == g_hWnd && g_Global.g_bLeftMouseDown == true
      && g_Global.g_mouse.x > 410 && g_Global.g_mouse.x < 626 
      && g_Global.g_mouse.y > 0 && g_Global.g_mouse.y < 452  
      && pData->m_bScopeDown == false && pData->m_bScopeUp == false
      && (pData->m_scopePosition == pData->m_scopeMinPosition)
      && pData->m_Player.m_depth < 90
      && (pData->m_Player.m_bSubmerged == false || (pData->m_Player.m_bSubmerged == true && pData->m_Player.m_speed < 10))){
      pData->m_bScopeUp = true;
      pData->m_bScopeDown = false;
      if(pAudio->IsPlaying(SOUND_PERISCOPE) == false)
        pAudio->PlaySoundClip(SOUND_PERISCOPE, 0);
    }
    //lower periscope
    else if(g_Global.g_mouse.x > 436 && g_Global.g_mouse.x < 601
      && g_Global.g_mouse.y > 384 && g_Global.g_mouse.y < 452
      && (pData->m_scopePosition == pData->m_scopeMaxPosition)
      && pData->m_bScopeDown == false 
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
      pData->m_bScopeUp = false;
      pData->m_bScopeDown = true;
      if(pAudio->IsPlaying(SOUND_PERISCOPE) == false)
        pAudio->PlaySoundClip(SOUND_PERISCOPE, 0);
    }

    //rotate scope left - conn right
    if(::GetActiveWindow() == g_hWnd && g_Global.g_bLeftMouseDown == true 
      && g_Global.g_mouse.x > 131 && g_Global.g_mouse.x < 320 // && pData->m_bScopeUp == true
      && g_Global.g_mouse.y > 338 && g_Global.g_mouse.y < 398  
      && (pData->m_scopePosition == pData->m_scopeMaxPosition)){
        pData->m_bRotateLeft = true;
        pData->m_bRotateRight = false;
        pData->m_scopeControlValue = (g_Global.g_mouse.x - 319);
      //if(pAudio->IsPlaying(SOUND_PERISCOPE) == false)
      //  pAudio->PlaySoundClip(SOUND_PERISCOPE, 0);
    }

    //rotate scope right - conn left
    else if(::GetActiveWindow() == g_hWnd && g_Global.g_bLeftMouseDown == true 
      &&  g_Global.g_mouse.x > 704 && g_Global.g_mouse.x < 903
      && g_Global.g_mouse.y > 338 && g_Global.g_mouse.y < 398 
      && (pData->m_scopePosition == pData->m_scopeMaxPosition)){
        pData->m_bRotateLeft = false;
        pData->m_bRotateRight = true;
        pData->m_scopeControlValue = (g_Global.g_mouse.x - 709);
      //if(pAudio->IsPlaying(SOUND_PERISCOPE) == false)
      //  pAudio->PlaySoundClip(SOUND_PERISCOPE, 0);
    }
    else{
      pData->m_bRotateLeft = false;
      pData->m_bRotateRight = false;
    }

    //automatically lower scope if speed > 10 kts submerged or depth > 90 feet
    if(pData->m_Player.m_bSubmerged == true && (pData->m_Player.m_depth >= 90 || pData->m_Player.m_speed >= 10)){ //less than 90', spd < 10 kts
      pData->m_bScopeUp = false;
      pData->m_bScopeDown = true;
    }

    //speed control
    if(g_Global.g_mouse.x > 77 && g_Global.g_mouse.x < 202 //450, 583
      && g_Global.g_mouse.y > 712 && g_Global.g_mouse.y < 742
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
      if(g_Global.g_mouse.x >= 77 && g_Global.g_mouse.x <= 103)
        pData->m_Player.m_cmdRudder = -25;
      else if(g_Global.g_mouse.x > 103 && g_Global.g_mouse.x <= 127)
        pData->m_Player.m_cmdRudder = -10;
      else if(g_Global.g_mouse.x > 127 && g_Global.g_mouse.x <= 152)
        pData->m_Player.m_cmdRudder = 0;
      else if(g_Global.g_mouse.x > 152 && g_Global.g_mouse.x <= 177)
        pData->m_Player.m_cmdRudder = 10;
      else if(g_Global.g_mouse.x > 177 && g_Global.g_mouse.x <=202)
        pData->m_Player.m_cmdRudder = 25;
    }

    //speed changes on the surface
    else if(g_Global.g_mouse.x > 264 && g_Global.g_mouse.x < 662 //450, 583
      && g_Global.g_mouse.y > 728 && g_Global.g_mouse.y < 746
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd
      && pData->m_Player.m_bSubmerged == false){
      if(g_Global.g_mouse.x >= 264 && g_Global.g_mouse.x <= 308)//back std
        pData->m_Player.m_cmdSpeed = - 8;
      else if(g_Global.g_mouse.x >= 308 && g_Global.g_mouse.x <= 353)//back 2/3
        pData->m_Player.m_cmdSpeed = -6;
      else if(g_Global.g_mouse.x >= 353 && g_Global.g_mouse.x <= 395)//back 1/3
        pData->m_Player.m_cmdSpeed = - 3;
      else if(g_Global.g_mouse.x >= 395 && g_Global.g_mouse.x <= 441)//all stop
        pData->m_Player.m_cmdSpeed = 0;
      else if(g_Global.g_mouse.x >= 441 && g_Global.g_mouse.x <= 486)//ahead  1/3
        pData->m_Player.m_cmdSpeed = 5;
      else if(g_Global.g_mouse.x >= 486 && g_Global.g_mouse.x <= 532)//ahead 2/3
        pData->m_Player.m_cmdSpeed = 10;
      else if(g_Global.g_mouse.x >= 532 && g_Global.g_mouse.x <= 574)//ahead std
        pData->m_Player.m_cmdSpeed = 15;
      else if(g_Global.g_mouse.x >= 574 && g_Global.g_mouse.x <= 619)//ahead full
        pData->m_Player.m_cmdSpeed = 20;
      else if(g_Global.g_mouse.x >= 619 && g_Global.g_mouse.x <= 662)//ahead full
        pData->m_Player.m_cmdSpeed = 22;
      }
    
    //speed changes submerged
    else if(g_Global.g_mouse.x > 264 && g_Global.g_mouse.x < 662 //450, 583
      && g_Global.g_mouse.y > 728 && g_Global.g_mouse.y < 746
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd
      && pData->m_Player.m_bSubmerged == true){
      if(g_Global.g_mouse.x >= 264 && g_Global.g_mouse.x <= 308)//back std
        pData->m_Player.m_cmdSpeed = - 6;
      else if(g_Global.g_mouse.x >= 308 && g_Global.g_mouse.x <= 353)//back 2/3
        pData->m_Player.m_cmdSpeed = -4;
      else if(g_Global.g_mouse.x >= 353 && g_Global.g_mouse.x <= 395)//back 1/3
        pData->m_Player.m_cmdSpeed = - 2;
      else if(g_Global.g_mouse.x >= 395 && g_Global.g_mouse.x <= 441)//all stop
        pData->m_Player.m_cmdSpeed = 0;
      else if(g_Global.g_mouse.x >= 441 && g_Global.g_mouse.x <= 486)//ahead  1/3
        pData->m_Player.m_cmdSpeed = 2;
      else if(g_Global.g_mouse.x >= 486 && g_Global.g_mouse.x <= 532)//ahead 2/3
        pData->m_Player.m_cmdSpeed = 4;
      else if(g_Global.g_mouse.x >= 532 && g_Global.g_mouse.x <= 574)//ahead std
        pData->m_Player.m_cmdSpeed = 6;
      }

    //depth control
    else if(g_Global.g_mouse.x > 962 && g_Global.g_mouse.x < 1005 //450, 583
      && g_Global.g_mouse.y > 478 && g_Global.g_mouse.y < 707
      && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
        lastDepthCmd = pData->m_Player.m_cmdDepth;

        if(g_Global.g_mouse.y >= 478 && g_Global.g_mouse.y <= 510)//surface
          pData->m_Player.m_cmdDepth = 25;
        else if(g_Global.g_mouse.y >= 510 && g_Global.g_mouse.y <= 541)//periscope depth
          pData->m_Player.m_cmdDepth = 60;
        else if(g_Global.g_mouse.y >= 541 && g_Global.g_mouse.y <= 574)//90'
          pData->m_Player.m_cmdDepth = 90;
        else if(g_Global.g_mouse.y >= 574 && g_Global.g_mouse.y <= 606)//120'
          pData->m_Player.m_cmdDepth = 120;
        else if(g_Global.g_mouse.y >= 606 && g_Global.g_mouse.y <= 638)//150'
          pData->m_Player.m_cmdDepth = 150;
        else if(g_Global.g_mouse.y >= 638 && g_Global.g_mouse.y <= 670)//180'
          pData->m_Player.m_cmdDepth = 180;
        else if(g_Global.g_mouse.y >= 670 && g_Global.g_mouse.y <= 707)//oops...random depth greater than 180'
          pData->m_Player.m_cmdDepth = 181 + (rand() % 320) ;

        //diving from the surface
        if(lastDepthCmd == 25 && pData->m_Player.m_cmdDepth > 25){
          if(pAudio->IsPlaying(SOUND_CLEAR_BRIDGE) == false)
            pAudio->PlaySoundClip(SOUND_CLEAR_BRIDGE, 0);
          if(pAudio->IsPlaying(SOUND_DIVING) == false)
            pAudio->PlaySoundClip(SOUND_DIVING, 0);
        }
    }
  }

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

//***************************************************************
//                         R E N D E R 
//***************************************************************
void cGameStateConn::render(Graphics &con){
  // ::SetCursor(::LoadCursor(NULL, (LPCSTR)IDC_CURSOR3));
   ::ShowCursor(true);

  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();

  POINT pt;
  pt.x = g_Global.g_mouse.x;
  pt.y = g_Global.g_mouse.y;

  GRAPHIC_IMAGE gi;
  CGameData *pData = CGameData::Instance();

  //background images...two of them sprites 14 and 17
  gi = g_Sprite.GetSpriteData(14);
  con.RenderGraphicModulate(-pData->m_scopeRotateAngle + pData->m_shockX, 0 + pData->m_shockY,gi, m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(17);
  con.RenderGraphicModulate(-pData->m_scopeRotateAngle + 1024 + pData->m_shockX, pData->m_shockY ,gi, m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(14);
  con.RenderGraphicModulate(-pData->m_scopeRotateAngle + 2048 + pData->m_shockX, pData->m_shockY ,gi, m_red, m_green, m_blue);

  //scope
  gi = g_Sprite.GetSpriteData(16);
  con.RenderGraphicModulate(102 + pData->m_shockX, - pData->m_scopePosition - 100 + pData->m_shockY ,gi, m_red, m_green, m_blue);

  //control panel
  gi = g_Sprite.GetSpriteData(33);
  con.RenderGraphicModulate(0 + pData->m_shockX, 428 + pData->m_shockY, gi, m_red, m_green, m_blue);//con.GetScreenHeight() - gi.height,

  gi = g_Sprite.GetSpriteData(34);
  con.RenderGraphicModulate(670 + pData->m_shockX, 428 + pData->m_shockY, gi, m_red, m_green, m_blue);//con.GetScreenHeight() - gi.height,

  //heading needle
  gi = g_Sprite.GetSpriteData(35);
  gi.rotationAngle = pData->m_Player.m_heading * 3.14/180;
  con.RenderGraphicModulate(139 - gi.width/2 + pData->m_shockX, 552 - gi.height/2 + pData->m_shockY, gi, m_red, m_green, m_blue);

  //speed needle
  gi = g_Sprite.GetSpriteData(35);
  if(pData->m_Player.m_speed >=0)
    gi.rotationAngle = (pData->m_Player.m_speed * 4.71/24) - 2.34;
  else
    gi.rotationAngle = (0 * 4.71/24) - 2.34;//don't move needle below zero
  con.RenderGraphicModulate(408 - gi.width/2 + pData->m_shockX, 553 - gi.height/2 + pData->m_shockY, gi, m_red, m_green, m_blue);

  //depth needle
  gi = g_Sprite.GetSpriteData(35);
  if(pData->m_Player.m_depth < 200)
    gi.rotationAngle = (pData->m_Player.m_depth * 4.71/180) - 2.34;
  else
    gi.rotationAngle = (210 * 4.71/180) - 2.34;
  con.RenderGraphicModulate(814 - gi.width/2 + pData->m_shockX, 563 - gi.height/2 + pData->m_shockY, gi, m_red, m_green, m_blue);

  //display short cuts
  gi = g_Sprite.GetSpriteData(36);//sonar
  con.RenderGraphicModulate(10,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(37);//radar
  con.RenderGraphicModulate(120,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(44);//conn selected
  con.RenderGraphicModulate(230,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(55);//quit mission
  con.RenderGraphicModulate(344,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(39);//fire control
  con.RenderGraphicModulate(694,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(40);//damage control
  con.RenderGraphicModulate(804,10,gi,m_red, m_green, m_blue);
  gi = g_Sprite.GetSpriteData(41);//chart
  con.RenderGraphicModulate(914,10,gi,m_red, m_green, m_blue);

  //scope mount
  //gi = g_Sprite.GetSpriteData(15);
  //con.RenderGraphicModulate(330, 0 ,gi, m_red, m_green, m_blue); //333

  //player helps
  //******************************************************
  //rotate left
  if(g_Global.g_mouse.x > 131 && g_Global.g_mouse.x < 320 
    && g_Global.g_mouse.y > 338 && g_Global.g_mouse.y < 398  
    &&  (pData->m_scopePosition == pData->m_scopeMaxPosition)
    && ::GetActiveWindow() == g_hWnd){
    gi = g_Sprite.GetSpriteData(18);
    con.RenderGraphicModulate(131, 338 ,gi, m_red, m_green, m_blue);
    }

  //rotate right
  else if(g_Global.g_mouse.x > 704 && g_Global.g_mouse.x < 903
    && g_Global.g_mouse.y > 338 && g_Global.g_mouse.y < 398 
    &&  (pData->m_scopePosition == pData->m_scopeMaxPosition)
    && ::GetActiveWindow() == g_hWnd){  
    gi = g_Sprite.GetSpriteData(19);
    con.RenderGraphicModulate(832, 338 ,gi, m_red, m_green, m_blue);
  }
  //lower scope
  else if(g_Global.g_mouse.x > 436 && g_Global.g_mouse.x < 601 
          && g_Global.g_mouse.y > 384 && g_Global.g_mouse.y < 584 
          && pData->m_bScopeUp == false && pData->m_bScopeDown == false 
          &&  (pData->m_scopePosition == pData->m_scopeMaxPosition)          
          && ::GetActiveWindow() == g_hWnd){
    gi = g_Sprite.GetSpriteData(21);
    con.RenderGraphicModulate(486, 348 ,gi, m_red, m_green, m_blue);
  }
  //raise scope
  else if(::GetActiveWindow() == g_hWnd
          && g_Global.g_mouse.x > 410 && g_Global.g_mouse.x < 626 
          && g_Global.g_mouse.y > 26 && g_Global.g_mouse.y < 449
          && (pData->m_scopePosition == pData->m_scopeMinPosition)
          && pData->m_bScopeDown == false
          &! (pData->m_Player.m_bSubmerged == true && (pData->m_Player.m_depth >= 90 || pData->m_Player.m_speed >= 10))
   ){
    gi = g_Sprite.GetSpriteData(20);
    con.RenderGraphicModulate(490, 192 ,gi, m_red, m_green, m_blue);
  }

  //zoom and change state - crosshairs
  else if(g_Global.g_mouse.x > 417 && g_Global.g_mouse.x < 621 //450, 583
     && g_Global.g_mouse.y > 115 && g_Global.g_mouse.y < 347
     &&  (pData->m_scopePosition == pData->m_scopeMaxPosition)
     && ::GetActiveWindow() == g_hWnd){
     gi = g_Sprite.GetSpriteData(24);
     con.RenderGraphicModulate(476, 134,gi, m_red, m_green, m_blue);
  }

  //draw tool tips for short cuts
  if(g_Global.g_mouse.y > 10 && g_Global.g_mouse.y < 130){
    //if(g_Global.g_mouse.x > 10 & g_Global.g_mouse.x < 104)
    //  con.Draw2DText("Sonar", F_V20, 20, 10, 0,255,255);
    //else if(g_Global.g_mouse.x > 120 & g_Global.g_mouse.x < 207)
    //  con.Draw2DText("Radar", F_V20, 130, 10, 0,255,255);
    if(g_Global.g_mouse.x > 344 && g_Global.g_mouse.x < 390){
      con.Draw2DText("Quit", F_V20, 350, 10, 0,255,255);
      con.Draw2DText("Mission", F_V20, 340, 30, 0,255,255);
    }
    //else if(g_Global.g_mouse.x > 694 & g_Global.g_mouse.x < 794)
    //  con.Draw2DText("Torpedos", F_V20, 704, 10, 0,255,255);
    //else if(g_Global.g_mouse.x > 804 & g_Global.g_mouse.x < 904)
    //  con.Draw2DText("Damage", F_V20, 814, 10, 0,255,255);
    else if(g_Global.g_mouse.x > 914 & g_Global.g_mouse.x < 1014)
      con.Draw2DText("Chart", F_V20, 924, 10, 0,255,255);
  }

  //draw selected rudder
  if(pData->m_Player.m_cmdRudder == -25){
    gi = g_Sprite.GetSpriteData(49);
    con.RenderGraphicModulate(75, 689,gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdRudder == -10){
    gi = g_Sprite.GetSpriteData(50);
    con.RenderGraphicModulate(101, 689, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdRudder == 0){
    gi = g_Sprite.GetSpriteData(51);
    con.RenderGraphicModulate(125, 689, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdRudder == 10){
    gi = g_Sprite.GetSpriteData(52);
    con.RenderGraphicModulate(150, 689, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdRudder == 25){
    gi = g_Sprite.GetSpriteData(53);
    con.RenderGraphicModulate(175, 689, gi, m_red, m_green, m_blue);
  }

  //draw selected speed
  if(pData->m_Player.m_bSubmerged == false){ //surface speeds
    if(pData->m_Player.m_cmdSpeed == -8){
      gi = g_Sprite.GetSpriteData(67);
      con.RenderGraphicModulate(261, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == -6){
      gi = g_Sprite.GetSpriteData(68);
      con.RenderGraphicModulate(306, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == -3){
      gi = g_Sprite.GetSpriteData(69);
      con.RenderGraphicModulate(350, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 0){
      gi = g_Sprite.GetSpriteData(70);
      con.RenderGraphicModulate(393, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 5){
      gi = g_Sprite.GetSpriteData(71);
      con.RenderGraphicModulate(439, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 10){
      gi = g_Sprite.GetSpriteData(72);
      con.RenderGraphicModulate(484, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 15){
      gi = g_Sprite.GetSpriteData(73);
      con.RenderGraphicModulate(529, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 20){
      gi = g_Sprite.GetSpriteData(74);
      con.RenderGraphicModulate(573, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 22){
      gi = g_Sprite.GetSpriteData(75);
      con.RenderGraphicModulate(615, 706, gi, m_red, m_green, m_blue);
    }

  }
  else{ //submerged speeds

    if(pData->m_Player.m_cmdSpeed == -6){
      gi = g_Sprite.GetSpriteData(67);
      con.RenderGraphicModulate(261, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == -4){
      gi = g_Sprite.GetSpriteData(68);
      con.RenderGraphicModulate(306, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == -2){
      gi = g_Sprite.GetSpriteData(69);
      con.RenderGraphicModulate(350, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 0){
      gi = g_Sprite.GetSpriteData(70);
      con.RenderGraphicModulate(393, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 2){
      gi = g_Sprite.GetSpriteData(71);
      con.RenderGraphicModulate(439, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 4){
      gi = g_Sprite.GetSpriteData(72);
      con.RenderGraphicModulate(484, 706, gi, m_red, m_green, m_blue);
    }
    else if(pData->m_Player.m_cmdSpeed == 6){
      gi = g_Sprite.GetSpriteData(73);
      con.RenderGraphicModulate(529, 706, gi, m_red, m_green, m_blue);
    }
  }

  //draw selected depth
  if(pData->m_Player.m_cmdDepth == 25){
      gi = g_Sprite.GetSpriteData(76);
      con.RenderGraphicModulate(960, 457, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdDepth == 60){
      gi = g_Sprite.GetSpriteData(77);
      con.RenderGraphicModulate(960, 488, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdDepth == 90){
      gi = g_Sprite.GetSpriteData(78);
      con.RenderGraphicModulate(960, 519, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdDepth == 120){
      gi = g_Sprite.GetSpriteData(79);
      con.RenderGraphicModulate(960, 550, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdDepth == 150){
      gi = g_Sprite.GetSpriteData(80);
      con.RenderGraphicModulate(960, 582, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdDepth == 180){
      gi = g_Sprite.GetSpriteData(81);
      con.RenderGraphicModulate(960, 614, gi, m_red, m_green, m_blue);
  }
  else if(pData->m_Player.m_cmdDepth > 180){
      gi = g_Sprite.GetSpriteData(82);
      con.RenderGraphicModulate(960, 646, gi, m_red, m_green, m_blue);
  }

  //con.Draw2DText("Control state - Press X return.", F_V20, 0, 120, 0,0,0);
  if(pData->m_scopePosition == pData->m_scopeMaxPosition)
    con.Draw2DTextValue("BRG", (long)( (float)pData->m_scopeRotateAngle/5.689),F_V20,484, 262, 255, 255, 0);
  con.Draw2DTextValue("Damage", (long)( (float)pData->m_Player.m_damage),F_V20, 474, 430, 255, 0, 0);


  con.Draw2DTextValue("Time Remaining", (float)(pData->m_goalTime - pData->m_accTime), F_V20, 800, 720, 255, 255, 255);

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 720, 0,255,255);
  con.EndDrawing ();
  con.Present();
}

void cGameStateConn::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateConn::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateConn::resume(){
}
void cGameStateConn::pause(){
}
void cGameStateConn::save(){
}

void cGameStateConn::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

#include "..\includes\cGameStateSelect.h"
#include "..\includes\CGlobal.h"

//objects
//extern Graphics con;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateSelect::cGameStateSelect(){
}

cGameStateSelect::~cGameStateSelect()
{
	m_TE.clear();
}

void cGameStateSelect::initialize(){
  m_selection = 0;
  
  m_red = 255;
  m_green = 255;
  m_blue = 255;

  //loads catalog data from catalog.dat file
  CGameData *pData = CGameData::Instance();
  CLog *pLog = CLog::Instance();
  
  if(pData->LoadCatalogData("catalog.dat")==true){
    pLog->Log("Catalog Load Successful!***********************");
    pLog->Log("Number of catalog entries", pData->m_catalog.size());
    for(int i = 0; i < pData->m_catalog.size();++i)
      pLog->Log(pData->m_catalog[i].vesselID, pData->m_catalog[i].name);
  }
  else{
    pLog->Log("Catalog Load Failure!!!!!!!!!!!!!!!!!!!!1");
  }
  
}

void cGameStateSelect::activate(){
  m_bActivated = false;
  m_enableTimer.initialize();
  m_enableTimer.ResetTimer();
}

cGameStateObject* cGameStateSelect::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  CAudioManager *pAudio = CAudioManager::Instance();
  CLog *pLog = CLog::Instance();
  
  

  if(pAudio->IsPlaying(SOUND_FUNERAL) == true )
      pAudio->StopSoundClip(SOUND_FUNERAL);
  if(pAudio->IsPlaying(SOUND_SINKING) == true )
      pAudio->StopSoundClip(SOUND_SINKING);
  if(pAudio->IsPlaying(SOUND_TAPS) == true )
      pAudio->StopSoundClip(SOUND_TAPS);
  if(pAudio->IsPlaying(SOUND_ANTHEM) == true )
      pAudio->StopSoundClip(SOUND_ANTHEM);
  if(pAudio->IsPlaying(SOUND_ANCHORS) == true )
      pAudio->StopSoundClip(SOUND_ANCHORS);


  if(m_enableTimer.getTimer(.2) == true)
    m_bActivated = true;

  CGameData *pData = CGameData::Instance();
  pData->Update(timeDifference);
/*
  if(keyDown('A') && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_AWARDS;
    ::ShowCursor(false);
  }

  if(keyDown('M') && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_MAIN;
    ::ShowCursor(false);
  }
*/

  //loads mouse position into global variable.
  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y - rct.top;
  std::string fileName = "";
  static double timeDiffTotal = 0;
  static int last_selection = -1;

  timeDiffTotal += timeDifference;
  if(timeDiffTotal > .1){
    timeDiffTotal = 0;

    if(::GetActiveWindow() == g_hWnd){
      if(g_Global.g_mouse.y > 163 && g_Global.g_mouse.y <= 202){//training mission 1
        m_selection = 0;
        fileName = "training1.dat";
      }
      else if(g_Global.g_mouse.y > 202 && g_Global.g_mouse.y <= 236){//training mission 2
        m_selection = 1;
        fileName = "training2.dat";
      }
      else if(g_Global.g_mouse.y > 236 && g_Global.g_mouse.y <= 286){//training mission 3
        m_selection = 2;
        fileName = "training3.dat";
      }
      else if(g_Global.g_mouse.y > 286 && g_Global.g_mouse.y <= 334){//war patrol 10
        m_selection = 3;
        fileName = "patrol1.dat";
      }
      else if(g_Global.g_mouse.y > 334 && g_Global.g_mouse.y <= 368){//war patrol 11
        m_selection = 4;
        fileName = "patrol2.dat";
      }
      else if(g_Global.g_mouse.y > 368 && g_Global.g_mouse.y <= 401){//war patrol 12
        m_selection = 5;
        fileName = "patrol3.dat";
      }
      else if(g_Global.g_mouse.y > 401 && g_Global.g_mouse.y <= 451){//war patrol 13
        m_selection = 6;
        fileName = "patrol4.dat";
      }
      else if(g_Global.g_mouse.y > 451 && g_Global.g_mouse.y <= 528){//
        m_selection = 7;
      }
      else if(g_Global.g_mouse.y > 528){// && g_Global.g_mouse.y <= 560){//return to main
        m_selection = 8;
      }
      if(m_selection != last_selection){
        if(pAudio->IsPlaying(SOUND_BEEP) == false)
          pAudio->PlaySoundClip(SOUND_BEEP, 0);
      }
      last_selection = m_selection;
    }
  }

  if((keyDown(VK_RETURN) || g_Global.g_bLeftMouseDown == true) && m_bActivated == true && ::GetActiveWindow() == g_hWnd){
    if(m_selection >= 0 && m_selection  <= 6) { //training mission or war patrol selected  
      m_event = EVENT_GO_PLAY;
      pLog->Log("***************************************** Select Mission ************");
      CGameData *pData = CGameData::Instance();
    
      //load game data...assume training1.dat is selected
      if(pData->LoadGameData(fileName) == true){
        pLog->Log("Game data load successful!");
        ::ShowCursor(true);
      }
      else{
        pLog->Log("Game data load failed!");  
        m_event = EVENT_GO_NO_WHERE;
      }

    }
    else if(m_selection == 7)
      m_event = EVENT_GO_AWARDS;
    else if(m_selection = 8)
      m_event = EVENT_GO_MAIN;

    ::ShowCursor(false);
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateSelect::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();

  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(22);
  con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  
  gi = g_Sprite.GetSpriteData(23);
  switch(m_selection){
    case 0:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 370 , con.GetScreenCenterY() - 231, gi, m_red, m_green, m_blue);
      break;
    case 1:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 370 , con.GetScreenCenterY() - 197, gi, m_red, m_green, m_blue);
      break;
    case 2:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 370 , con.GetScreenCenterY() - 163, gi, m_red, m_green, m_blue);
      break;
    case 3:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 310 , con.GetScreenCenterY() - 96, gi, m_red, m_green, m_blue);
      break;
    case 4:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 310 , con.GetScreenCenterY() - 61, gi, m_red, m_green, m_blue);
      break;
    case 5:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 310 , con.GetScreenCenterY() - 29, gi, m_red, m_green, m_blue);
      break;
    case 6:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 310 , con.GetScreenCenterY() + 3, gi, m_red, m_green, m_blue);
      break;
    case 7:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 310 , con.GetScreenCenterY() + 67, gi, m_red, m_green, m_blue);
      break;
    case 8:
      con.RenderGraphicModulate(con.GetScreenCenterX() + 310 , con.GetScreenCenterY() + 158, gi, m_red, m_green, m_blue);
      break;
  }

  //con.Draw2DText("Select state", F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() - 30, 0,255,255);
  //con.Draw2DText("Press P to play.", F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() - 10, 0,255,255);
  //con.Draw2DText("Press A to display awards.", F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() + 10, 0,255,255);
  //con.Draw2DText("Press M to return to main menu.", F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() + 30, 0,255,255);

  //display sub data
  CGameData *pData = CGameData::Instance();

  /*
  con.Draw2DTextValue("Heading",pData->m_tHeading,F_V16,400,0,0, 255,255,255);
  con.Draw2DTextValue("Speed",pData->m_tSpeed,F_V16,400,16,0, 255,255,255);
  con.Draw2DTextValue("Depth",pData->m_tDepth,F_V16,400,32,0, 255,255,255);

  //fix this
  std::string sValue;
  sValue = pData->m_tLat.str();
  con.Draw2DTextValue("Latitude",atof(sValue.c_str()), F_V16,500,0,0, 255,255,255);
  sValue = pData->m_tLon.str();
  con.Draw2DTextValue("Longitude",atof(sValue.c_str()),F_V16,500,16,0, 255,255,255);

  con.Draw2DTextValue("Length",pData->m_tLength,F_V16,400,48,0, 255,255,255);
  con.Draw2DTextValue("Beam",pData->m_tBeam,F_V16,400,64,0, 255,255,255);
  con.Draw2DTextValue("Draft",pData->m_tDraft,F_V16,400,80,0, 255,255,255);
  con.Draw2DTextValue("Range",pData->m_tRange,F_V16,400,96,0, 255,255,255);
  con.Draw2DTextValue("Test Depth",pData->m_tTestDepth,F_V16,400,112,0, 255,255,255);
  con.Draw2DTextValue("Max Spd Surf",pData->m_tMaxSpdSurf,F_V16,400,128,0, 255,255,255);
  con.Draw2DTextValue("Max Spd Sub",pData->m_tMaxSpdSub,F_V16,400,144,0, 255,255,255);
  con.Draw2DTextValue("Disp Surf",pData->m_tDispSurf,F_V16,400,160,0, 255,255,255);
  con.Draw2DTextValue("Disp Sub",pData->m_tDispSub,F_V16,400,176,0, 255,255,255);

  con.Draw2DTextValue("Heading",pData->m_tHeading,F_V16,400,0,0, 255,255,255);
  */

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true){
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);
    con.Draw2DTextValue("X", (float)g_Global.g_mouse.x, F_V20, 0, 20, 255, 0, 0);
    con.Draw2DTextValue("Y", (float)g_Global.g_mouse.y, F_V20, 0, 40, 255, 0, 0);
  }

  con.EndDrawing ();
  con.Present();
}

void cGameStateSelect::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateSelect::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateSelect::resume(){
}
void cGameStateSelect::pause(){
}
void cGameStateSelect::save(){
}

void cGameStateSelect::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

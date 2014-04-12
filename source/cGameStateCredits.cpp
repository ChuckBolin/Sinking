#include "..\includes\cgamestatecredits.h"
#include "..\includes\CGlobal.h"

//objects
//extern Graphics con;
//extern CAudioManager g_Audio;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateCredits::cGameStateCredits(void){
}

cGameStateCredits::~cGameStateCredits(void)
{
	m_TE.clear();
}

void cGameStateCredits::initialize(){

  SCROLL_MSG msg;
  //msg.red = 255; msg.green = 255; msg.blue = 255;  //white
  //msg.red = 127; msg.green = 127; msg.blue = 127;  //gray
  msg.red = 0; msg.green = 200; msg.blue = 0;  //green
  msg.msg ="\"http://www.gameinstitute.com\"" ; m_msg.push_back(msg);
  msg.msg ="Game Challenge 6"; m_msg.push_back(msg);
  msg.msg = "\"Sinking of the Rising Sun\"";   m_msg.push_back(msg);
  msg.msg = "March 21 - May 3, 2008"; m_msg.push_back(msg);
  msg.msg ="Created by Chuck Bolin" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  
  msg.red = 255; msg.green = 255; msg.blue = 255;  //white
  msg.msg ="Special thanks to my wife Grace for"; m_msg.push_back(msg);
  msg.msg ="allowing me \'family time\' to work "; m_msg.push_back(msg);
  msg.msg = "on this project." ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  
  
  //msg.red = 0; msg.green = 200; msg.blue = 200; //cyan
  msg.msg ="Art, Music, and Research"; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="Refer to references.txt" ; m_msg.push_back(msg);
  

  msg.msg ="located in the data folder" ; m_msg.push_back(msg);
  msg.msg ="to see all the web sites" ; m_msg.push_back(msg);
  msg.msg ="that were used to develop" ; m_msg.push_back(msg);
  msg.msg ="this game." ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="Thanks to Bursuc for introducing me to Pyro Particle Effects." ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  

  //memorial
  msg.red = 230; msg.green = 0; msg.blue = 0;  //red
  msg.msg ="Dedicated to those on Eternal Patrol." ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);
  msg.msg = "USS SEALION (SS 195) December 10, 1941" ; m_msg.push_back(msg); msg.msg ="" ; m_msg.push_back(msg);
  msg.msg = "USS S 36 (SS 141) January 20, 1942 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS S 26 (SS 131) January 24, 1942 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SHARK (SS 174) February 11, 1942 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS PERCH (SS 176) March 3, 1942 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS S 27 (SS 132) June 19, 1942 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GRUNION (SS 216) July 30, 1942 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS S 39 (SS 144) August 16, 1942" ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS ARGONAUT (SS 166) January 10, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS AMBERJACK (SS 219)February 16, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GRAMPUS (SS 207) March 5, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS TRITON (SS 201) March 15, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS PICKEREL (SS 177) April 3, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GRENADIER (SS 210) April 22, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS RUNNER (SS 275) May 28, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS R 12 (SS 89) June 12, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS POMPANO (SS 181)August 29, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GRAYLING (SS 209) September 9, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS CISCO (SS 290) September 28, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS S 44 (SS 155) October 7, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS WAHOO (SS 238) October 11, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS DORADO (SS 248) October 12, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS CORVINA (SS 226) November 16, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SCULPIN (SS 191) November 19, 1943 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS CAPELIN (SS 289) November 23, 1943" ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SCORPION (SS 278) January 5, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GRAYBACK (SS 208) February 26, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS TROUT (SS 202) February 28, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS TULLIBEE (SS 284) March 26, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS HERRING (SS 233) June 1, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GUDGEON (SS 211) June 7, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GOLET (SS 361) June 14, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS S 28 (SS 133) July 4, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS ROBALO (SS 273) July 26, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS FLIER (SS 250) August 13, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS HARDER (SS 257) August 24, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SEAWOLF (SS 197) October 3, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS ESCOLAR (SS 294) October 17, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS DARTER (SS 227) October 24, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SHARK II (SS 314) October 24, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS TANG (SS 306) October 24, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS ALBACORE (SS 218) November 7, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS GROWLER (SS 215) November 8, 1944 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SCAMP (SS 277) November 9, 1944" ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SWORDFISH (SS 193) January 12, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS BARBEL (SS 316) February 4, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS KETE (SS 369) March 20, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS TRIGGER (SS 237) March 26, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS SNOOK (SS 279) April 8, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS LAGARTO (SS 371) May 3, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS BONEFISH (SS 223) June 18, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="USS BULLHEAD (SS 332) August 6, 1945 " ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);
  msg.msg ="" ; m_msg.push_back(msg);msg.msg ="" ; m_msg.push_back(msg);

  m_timer.initialize(); 
  m_scrollValue = 0;

  //used to show several images in the background
  m_secondTimer.initialize();
  m_secondCount = 0;

  //used for fading
  m_red = 255;//255;
  m_green =255;//255;
  m_blue = 255;//255;
  m_fadeTimer.initialize();
}

void cGameStateCredits::activate(){

}

cGameStateObject* cGameStateCredits::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  static double timeDiffTotal = 0;

  //user presses ESC to quit
  if((keyDown(VK_ESCAPE) || g_Global.g_bLeftMouseDown == true)&& m_secondCount > 1 && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_END;
    ::ShowCursor(true);
  }

  CAudioManager *pAudio = CAudioManager::Instance();
  if(pAudio->IsPlaying(SOUND_HYMN) == false)
    pAudio->PlaySoundClip(SOUND_HYMN, 0);

  //terminate program after scrolling greater than 15 seconds
  if(m_secondCount > 58)
    m_event = EVENT_GO_END;

  //update counter for displaying images
  if(m_secondTimer.secondTimer() == true){
    m_secondCount++;
  }

  //tracks time
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > 0.025){
    timeDiffTotal = 0;
    m_scrollValue += 2;  
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateCredits::render(Graphics &con){
 
  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 0, 0);
  con.BeginDrawing();

  //draw one of three pictures
  GRAPHIC_IMAGE gi;
/*
  if(m_secondCount == 12){
    m_red = 180;
    m_green =180;
    m_blue = 180;
  }
  else if (m_secondCount > 12){*/
  if(m_fadeTimer.getTimer(.2) == true){
   m_blue -= 1;//(5 * m_secondCount);
   m_red -= 2;//(10 * m_secondCount);
   m_green -= 2;//(10 * m_secondCount);
   if(m_red < 0)
     m_red = 0;
   if(m_blue < 0)
     m_blue = 0;
   if(m_green < 0)
     m_green = 0;
  }
  //}

  if(m_secondCount < 6){
    gi = g_Sprite.GetSpriteData(8);
    gi.scale = 0.8;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) + 100 , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }
  else if(m_secondCount < 13){
    gi = g_Sprite.GetSpriteData(9);
    gi.scale = 0.8;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) + 180 , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }
  else if(m_secondCount < 19){
    gi = g_Sprite.GetSpriteData(2);
    gi.scale = 1;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) + 100 , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }

  else if(m_secondCount < 24){
    gi = g_Sprite.GetSpriteData(3);
    gi.scale = 1;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) + 100 , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }
  else if(m_secondCount < 30){
    gi = g_Sprite.GetSpriteData(4);
    gi.scale = 1;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) + 100 , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }
  else if(m_secondCount < 36){
    gi = g_Sprite.GetSpriteData(5);
    gi.scale = 1;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2)  + 100, con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }
  else if(m_secondCount < 42){
    gi = g_Sprite.GetSpriteData(7);
    gi.scale = 1;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) + 100 , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }
  else if(m_secondCount < 46){
    gi = g_Sprite.GetSpriteData(6);
    gi.scale = 1;
    con.RenderGraphicModulate(con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) + 100 , con.GetScreenCenterY() - (gi.height * gi.scale)/2, gi, m_red, m_green, m_blue);
  }


  con.Draw2DText("Left click to quit!", F_V20, con.GetScreenCenterX() - 90, con.GetScreenHeight() - 50, 130, 130, 130);

  //scrolls message
  int posY = 0;

  for(int i = 0; i < m_msg.size(); i++){
    posY = m_scrollValue + ((int)m_msg.size() - i * 20);
    if(posY < con.GetScreenHeight() + 20){
      con.Draw2DText(m_msg[i].msg, F_V20, 20, con.GetScreenHeight() - posY, m_msg[i].red, m_msg[i].green, m_msg[i].blue);
    }
  }

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,200);

  con.EndDrawing ();
  con.Present();
}

void cGameStateCredits::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateCredits::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();
  //g_Audio.KillAll();
}
void cGameStateCredits::resume(){
}
void cGameStateCredits::pause(){
}
void cGameStateCredits::save(){
}

void cGameStateCredits::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

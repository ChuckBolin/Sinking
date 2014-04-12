#include "..\includes\CGameStatePlay.h"
#include "..\includes\keystatus.h"
#include "..\includes\CObject.h"

//extern Graphics g_con;
//extern CAudioManager g_Audio;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern bool g_bResetGame;
extern HWND g_hWnd;

CGameStatePlay::CGameStatePlay(void){
  m_bPaused = false;
  srand(time(0));
}

CGameStatePlay::~CGameStatePlay(void)
{
	m_TE.clear();
}

/*****************************************
 Initializes game state, loads objects to
 be displayed in the game world
*****************************************/
void CGameStatePlay::initialize(){ 
  //m_Game.ResetGame();
  //m_pauseTimer.initialize();

  m_red = 255;
  m_green = 255;
  m_blue = 255;
}

void CGameStatePlay::activate(){
  static bool activateFirst = false;

}

//********************************************
//Loads data corresponding to game level
//********************************************
void CGameStatePlay::LoadLevel(int level){

}

/*************************************  U P D A T E *****************************************/
cGameStateObject* CGameStatePlay::update(double timeDifference){
  m_event = EVENT_GO_NO_WHERE;
  static double timeDiffTotal = 0;
  static double thingTimeDiff = 0;
  //GRAPHIC_IMAGE gi;
  int spriteID;
  int frameMax  = 0;
  int frameCount  = 0;

  //start ambient sound loop
  CAudioManager *pAudio = CAudioManager::Instance();
  if(pAudio->IsPlaying(SOUND_ANCHORS) == false)
    pAudio->PlaySoundClip(SOUND_ANCHORS, 0);

  //loads mouse position into global variable.
  POINT pt;
  POINT *ppt = &pt;
  ::GetCursorPos(ppt);
  RECT rct;
  ::GetWindowRect(g_hWnd, &rct);
  g_Global.g_mouse.x = pt.x - rct.left;
  g_Global.g_mouse.y = pt.y - rct.top;

  //accept...go to control
  if(g_Global.g_mouse.x > 289 && g_Global.g_mouse.x < 407
    && g_Global.g_mouse.y > 683 && g_Global.g_mouse.y < 718
    && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_CONTROL;
  }

  //reject..return to menu  
  if(g_Global.g_mouse.x > 626 && g_Global.g_mouse.x < 743
    && g_Global.g_mouse.y > 683 && g_Global.g_mouse.y < 718
    && g_Global.g_bLeftMouseDown == true && ::GetActiveWindow() == g_hWnd){
    m_event = EVENT_GO_SELECT;
  }

  //game state transition code...very important
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }

  return 0;
}

/******************************************************************************************/
/******************************************************************************************/
/************************************** R E N D E R ***************************************/
/******************************************************************************************/
/******************************************************************************************/
void CGameStatePlay::render(Graphics &con){
  CLog *pLog = CLog::Instance();
  CGameData *pData = CGameData::Instance();
  ::ShowCursor(true);

 // POINT pt;
 // pt.x = g_Global.g_mouse.x;
 // pt.y = g_Global.g_mouse.y;

  int dx = 0; //used to convert object coordinates to screen coordinates
  int dy = 0;
  //HRESULT hr;
  float playerAngle;
  int lastHeight = 0; //used for adding hanging icicles
  int skyOffsetY = 0; //moves sky mountain
  int skyOffsetX = 0;
  std::ostringstream oss;
  static int m_winFrame = 0;//used to animate You Win

  //Clear buffer and draw graphics
  con.ClearBuffer(0, 0, 30, 0);
  con.BeginDrawing();

  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(32);
  con.RenderGraphicModulate(con.GetScreenCenterX() -  gi.width/2 , con.GetScreenCenterY() - gi.height/2, gi, m_red, m_green, m_blue);
  
  //display orders
  for(int i = 0; i < pData->m_orders.size(); ++i)
    con.Draw2DText(pData->m_orders[i],F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() - 180 + (i * 20), 0,0,0);

  //accept...mouse hover outline
  if(g_Global.g_mouse.x > 290 && g_Global.g_mouse.x < 407
    && g_Global.g_mouse.y > 684 && g_Global.g_mouse.y < 718){
      gi = g_Sprite.GetSpriteData(54);
      con.RenderGraphicModulate(278, 653, gi, m_red, m_green, m_blue);    
  }

  //reject..mouse hover outline
  if(g_Global.g_mouse.x > 627 && g_Global.g_mouse.x < 743
    && g_Global.g_mouse.y > 684 && g_Global.g_mouse.y < 718){
      gi = g_Sprite.GetSpriteData(54);
      con.RenderGraphicModulate(615, 653, gi, m_red, m_green, m_blue);    
    
  }

  con.Draw2DText("Left click to continue.", F_V20, con.GetScreenCenterX() - 150, con.GetScreenCenterY() + 30, 0,0,0);



  con.EndDrawing ();
  con.Present();
}

void CGameStatePlay::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}
void CGameStatePlay::deactivate(){

}
void CGameStatePlay::resume(){
}
void CGameStatePlay::pause(){
}
void CGameStatePlay::save(){
}

void CGameStatePlay::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

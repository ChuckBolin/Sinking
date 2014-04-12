/**********************************************************************************
 main.cpp - Written by Chuck Bolin, March 2008
 Starting point for GC6_Chuck
 Remember to link:
   d3d9.lib, d3dx9.lib winmm.lib
**********************************************************************************/

//include files
#include <Windows.h>
#include "..\includes\Graphics.h"
#include <string>
#include <sstream>
#include "..\resource.h"
#include "..\includes\CTimer.h"
#include "..\includes\Keystatus.h"
#include "..\includes\CAudioManager.h"

//game state classes
#include "..\includes\cGameStateObject.h"
#include "..\includes\CGameStatePlay.h"
#include "..\includes\cGameStateIntro.h"
#include "..\includes\cGameStateMain.h"
#include "..\includes\cGameStateHistory.h"
#include "..\includes\cGameStateReload.h"
#include "..\includes\cGameStateCredits.h"
#include "..\includes\cGameStateEnd.h"
#include "..\includes\cGameStateSelect.h"
#include "..\includes\cGameStateAwards.h"
#include "..\includes\cGameStateWin.h"
#include "..\includes\cGameStateLose.h"
#include "..\includes\cGameStateStatus.h"
#include "..\includes\cGameStateChart.h"
#include "..\includes\cGameStateControl.h"
#include "..\includes\cGameStateScope.h"
#include "..\includes\CGameStateSonar.h"
#include "..\includes\CGameStateRadar.h"
#include "..\includes\CGameStateFireControl.h"

//other classes
#include "..\includes\CLog.h"
#include "..\includes\CFileReader.h"
#include "..\includes\CSprite.h"
#include "..\includes\CINIReader.h"
#include "..\includes\CGlobal.h"
#include "..\includes\cgamedata.h"

// Memory leak checker - thanks to Steve Jones
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//objects
CTimer g_Timer; 
CTimer g_LoopTimer;
CTimer g_FPS_Timer;
CSprite g_Sprite;             //stores data for all sprites
CGlobal g_Global;

//variables
HWND g_hWnd;      //Handle to the window the app is running in
BOOL g_bRunning;  //Always set when app is running. App will exit if flag becomes false
POINTS g_mouse; //Mouse g_mouse
bool g_bLeftMouseDown = false;
bool g_bRightMouseDown = false;
int g_SecondCount = 0;
int g_FPS_Counter;
std::string	 g_strFPS  = "";		// SMJ - 11/13/2007 I hate globals!
//const bool	g_WindowedMode				= true;

//manages game states
cGameStateObject* g_pStatePlay = new CGameStatePlay;
cGameStateObject* g_pStateMain = new cGameStateMain;
cGameStateObject* g_pStateIntro = new cGameStateIntro;
cGameStateObject* g_pStateEnd = new cGameStateEnd;
cGameStateObject* g_pStateHistory = new cGameStateHistory;
cGameStateObject* g_pStateReload = new cGameStateReload;
cGameStateObject* g_pStateCredits = new cGameStateCredits;
cGameStateObject* g_pStateSelect = new cGameStateSelect;
cGameStateObject* g_pStateAwards = new cGameStateAwards;
cGameStateObject* g_pStateWin = new cGameStateWin;
cGameStateObject* g_pStateLose = new cGameStateLose;
cGameStateObject* g_pStateStatus = new cGameStateStatus;
cGameStateObject* g_pStateChart = new cGameStateChart;
cGameStateObject* g_pStateControl = new cGameStateConn;
cGameStateObject* g_pStateSonar = new cGameStateSonar;
cGameStateObject* g_pStateScope = new cGameStateScope;
cGameStateObject* g_pStateFireControl = new cGameStateFireControl;
cGameStateObject* g_pStateRadar = new cGameStateRadar;
cGameStateObject* g_pCurrent = 0;
cGameStateObject* g_pNext = 0;
  
// Function Prototypes         
int MakeWindow(HINSTANCE hInstance, bool bWindowedMode, Graphics &g_con);
LRESULT CALLBACK WindowProcedure(HWND hWindow,UINT uMessage,WPARAM wParam,LPARAM lParam);
void Shutdown();//Graphics &g_con);

//The program begins here.
/************************************************************************************************
  WinMain (...)
************************************************************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Always perform a leak check just before app exits.
	int nDbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	nDbgFlags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(nDbgFlags);

  //instantiate log object and log
  CLog *pLog = CLog::Instance();
  pLog->Log("*********************************************************");
  pLog->Log("Program starting..................");
  pLog->SetDelimiter(':');
  pLog->LogDate();
  pLog->LogTime();
  pLog->Log("************************ *********************************");


  pLog->Log ("***********************************************");
  pLog->Log("Game Challenge 6");
  pLog->LogDate();
  pLog->LogTime();
  pLog->Log("***********************************************");

  Graphics g_con;  
  CAudioManager *pAudio = CAudioManager::Instance();

  //***********************************************************************
  //read config.ini file
  // C O N F I G . I N I
  //***********************************************************************
  pLog->Log("************ Config.ini ************");
  CINIReader config;          //reads and stores INI file data
  config.LoadFile("config.ini");
  std::string sValue;
  std::string sParameter;
  int nParameterValue;

  //default values
  g_Global.g_screenWidth = 1024;
  g_Global.g_screenHeight = 768;

  if(config.IsValid() == true){
    pLog->Log("Valid config.ini file");
    pLog->Log("Numbers of lines in config file",config.GetNumberOfLines());

    for(int j=0; j< config.GetNumberOfLines();j++){
      sParameter = config.GetTerm(config.GetLineFromFile(j),1);
      sValue = config.GetTerm(config.GetLineFromFile(j),2);
      nParameterValue =  atoi(sValue.c_str());

      pLog->Log(sParameter, nParameterValue);      
      
      //load g_Global object with config.ini data
      if(sParameter == "WindowedMode" && nParameterValue == 1)
        g_Global.g_WindowedMode = true;
      else if(sParameter == "WindowedMode" && nParameterValue == 0)
        g_Global.g_WindowedMode = false;
      //else if(sParameter == "ScreenWidth" && nParameterValue > 0)
      //  g_Global.g_screenWidth = nParameterValue;
      //else if(sParameter == "ScreenHeight" && nParameterValue > 0)
      //  g_Global.g_screenHeight = nParameterValue;
      //else if(sParameter == "GameLevel" && nParameterValue > 0 && nParameterValue < 6)
      //  g_Global.g_GameLevel = nParameterValue;
      //else if(sParameter == "GameSnow" && nParameterValue == 1)
      //  g_Global.g_bGameSnow = true;
      //else if(sParameter == "GameSnow" && nParameterValue == 0)
      //  g_Global.g_bGameSnow = false;
      else if(sParameter == "GameAudio" && nParameterValue == 1)
        g_Global.g_bGameAudio = true;
      else if(sParameter == "GameAudio" && nParameterValue == 0)
        g_Global.g_bGameAudio = false;
      else if(sParameter == "FrameRate" && nParameterValue == 1)
        g_Global.g_bDisplayFramerate = true;
      else if(sParameter == "FrameRate" && nParameterValue == 0)
        g_Global.g_bDisplayFramerate = false;
    }
  } 
  pLog->Log("Reading Config.ini file is complete!");
	// Break on specific memory allocation number
	//_CrtSetBreakAlloc(175);
  
  MSG msg;    
  g_bRunning = true;//Start program running

  //Make a window and initialize DirectX in windowed mode
  MakeWindow(hInstance, g_Global.g_WindowedMode, g_con);

  //map game state information
  g_pStateIntro->addTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
  g_pStateMain->addTransitionEvent(EVENT_GO_HISTORY, g_pStateHistory);
  g_pStateMain->addTransitionEvent(EVENT_GO_RELOAD, g_pStateReload);
  g_pStateMain->addTransitionEvent(EVENT_GO_CREDITS, g_pStateCredits);
  g_pStateMain->addTransitionEvent(EVENT_GO_SELECT, g_pStateSelect);
  g_pStateHistory->addTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
  g_pStateReload->addTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
  g_pStateCredits->addTransitionEvent(EVENT_GO_END, g_pStateEnd);
  g_pStateSelect->addTransitionEvent(EVENT_GO_AWARDS, g_pStateAwards);
  g_pStateAwards->addTransitionEvent(EVENT_GO_SELECT, g_pStateSelect);  
  g_pStateSelect->addTransitionEvent(EVENT_GO_PLAY, g_pStatePlay);
  g_pStateSelect->addTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
  g_pStatePlay->addTransitionEvent(EVENT_GO_WIN, g_pStateWin);
  g_pStatePlay->addTransitionEvent(EVENT_GO_LOSE, g_pStateLose);
  g_pStateWin->addTransitionEvent(EVENT_GO_SELECT, g_pStateSelect);
  g_pStateLose->addTransitionEvent(EVENT_GO_SELECT, g_pStateSelect);
  g_pStatePlay->addTransitionEvent(EVENT_GO_SELECT, g_pStateSelect);
  g_pStatePlay->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);
  
  //sonar transitions
  g_pStateSonar->addTransitionEvent(EVENT_GO_RADAR, g_pStateRadar);
  g_pStateSonar->addTransitionEvent(EVENT_GO_FIRECONTROL, g_pStateFireControl);
  g_pStateSonar->addTransitionEvent(EVENT_GO_STATUS, g_pStateStatus);
  g_pStateSonar->addTransitionEvent(EVENT_GO_CHART, g_pStateChart);  
  g_pStateSonar->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);

  //radar transitions
  g_pStateRadar->addTransitionEvent(EVENT_GO_SONAR, g_pStateSonar);
  g_pStateRadar->addTransitionEvent(EVENT_GO_FIRECONTROL, g_pStateFireControl);
  g_pStateRadar->addTransitionEvent(EVENT_GO_STATUS, g_pStateStatus);
  g_pStateRadar->addTransitionEvent(EVENT_GO_CHART, g_pStateChart);  
  g_pStateRadar->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);

  //scope transitions
  g_pStateScope->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);
  g_pStateScope->addTransitionEvent(EVENT_GO_WIN, g_pStateWin);
  g_pStateScope->addTransitionEvent(EVENT_GO_LOSE, g_pStateLose);
  
  //conn transitions
  g_pStateControl->addTransitionEvent(EVENT_GO_PLAY, g_pStatePlay);
  g_pStateControl->addTransitionEvent(EVENT_GO_SONAR, g_pStateSonar);
  g_pStateControl->addTransitionEvent(EVENT_GO_RADAR, g_pStateRadar);
  g_pStateControl->addTransitionEvent(EVENT_GO_FIRECONTROL, g_pStateFireControl);
  g_pStateControl->addTransitionEvent(EVENT_GO_STATUS, g_pStateStatus);
  g_pStateControl->addTransitionEvent(EVENT_GO_CHART, g_pStateChart);  
  g_pStateControl->addTransitionEvent(EVENT_GO_SCOPE, g_pStateScope);
  g_pStateControl->addTransitionEvent(EVENT_GO_SELECT, g_pStateSelect);
  g_pStateControl->addTransitionEvent(EVENT_GO_WIN, g_pStateWin);
  g_pStateControl->addTransitionEvent(EVENT_GO_LOSE, g_pStateLose);

  //fire control transitions
  g_pStateFireControl->addTransitionEvent(EVENT_GO_SONAR, g_pStateSonar);
  g_pStateFireControl->addTransitionEvent(EVENT_GO_RADAR, g_pStateRadar);
  g_pStateFireControl->addTransitionEvent(EVENT_GO_STATUS, g_pStateStatus);
  g_pStateFireControl->addTransitionEvent(EVENT_GO_CHART, g_pStateChart);  
  g_pStateFireControl->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);

  //status transitions
  g_pStateStatus->addTransitionEvent(EVENT_GO_SONAR, g_pStateSonar);
  g_pStateStatus->addTransitionEvent(EVENT_GO_RADAR, g_pStateRadar);
  g_pStateStatus->addTransitionEvent(EVENT_GO_FIRECONTROL, g_pStateFireControl);
  g_pStateStatus->addTransitionEvent(EVENT_GO_CHART, g_pStateChart);  
  g_pStateStatus->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);

  //chart transitions
  g_pStateChart->addTransitionEvent(EVENT_GO_SONAR, g_pStateSonar);
  g_pStateChart->addTransitionEvent(EVENT_GO_RADAR, g_pStateRadar);
  g_pStateChart->addTransitionEvent(EVENT_GO_FIRECONTROL, g_pStateFireControl);
  g_pStateChart->addTransitionEvent(EVENT_GO_STATUS, g_pStateStatus);
  g_pStateChart->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);
  g_pStateChart->addTransitionEvent(EVENT_GO_WIN, g_pStateWin);
  g_pStateChart->addTransitionEvent(EVENT_GO_LOSE, g_pStateLose);

  g_Timer.initialize();
  g_LoopTimer.initialize();
  g_FPS_Timer.initialize();
  g_con.InitD3D (g_hWnd, g_Global.g_WindowedMode);

  //***********************************************************************
  //load textures from graphics file
  // G R A P H I C F I L E S . D A T
  //***********************************************************************
  pLog->Log("************ Graphicfiles.dat ************");
  CFileReader* cfr = new CFileReader;
  cfr->LoadFile("data\\graphicfiles.dat");
  int fileNumber;
  std::string fileName;
  if(cfr->IsValid()== true){
    pLog->Log("Numbers of lines in file",cfr->GetNumberOfLines());
    for(int j=0; j< cfr->GetNumberOfLines();j++){
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),1);
      fileNumber = atoi(sValue.c_str());
      sValue = cfr->GetTerm(cfr->GetLineFromFile(j),2);
      fileName = "assets\\artwork\\";
      fileName = fileName + sValue;
      //pLog->Log("Loaded file",fileName);
      pLog->Log(fileName, fileNumber);
      g_con.LoadTexture(fileName, fileNumber);
    }
  } 
  else{
    pLog->Log("ERROR****************** Failure to load textures (graphicfiles.dat)");
    delete cfr;
    g_con.CloseD3D();
    pLog->Log("DirectX closed!");
    Shutdown();
    return 0;
  }

  if(g_Sprite.LoadSprites() == true){
    pLog->Log("Sprites loaded!");
  }
  else{
    pLog->Log("ERROR****************** Failure to sprite data (sprites.dat)");
  }

  //used for framerate display
  std::ostringstream oss;
  std::string sFramerate;
  std::string sText;
  int nDisplayFPSCount = 0;

  //initialize audio manager
  //***************************************************************
  if(g_Global.g_bGameAudio == true){
    pLog->Log("Loading audio clip...");
    if(pAudio->IsValidSound())
      pLog->Log("Audio system is okay!");
    else
      pLog->Log("Audio system failure!");
    
    //load sounds
    pAudio->LoadSample(SOUND_BEEP, "assets\\sounds\\beep-03.wav");
    pAudio->LoadSample(SOUND_REMEMBER, "assets\\sounds\\remember.mp3");
    pAudio->LoadSample(SOUND_HYMN, "assets\\sounds\\navy_hymn.mp3");
    pAudio->LoadSample(SOUND_PERISCOPE, "assets\\sounds\\periscop.wav");
    pAudio->LoadSample(SOUND_BUTTON_CLICK, "assets\\sounds\\button_click.wav");
    pAudio->LoadSample(SOUND_DEPTH_CHARGE1,"assets\\sounds\\dc1.mp3");
    pAudio->LoadSample(SOUND_DEPTH_CHARGE2,"assets\\sounds\\dc2.mp3");
    pAudio->LoadSample(SOUND_TORPEDO1,"assets\\sounds\\torpedo3.mp3");
    pAudio->LoadSample(SOUND_TORPEDO2,"assets\\sounds\\torpedo4.mp3");
    pAudio->LoadSample(SOUND_AMBIENCE1,"assets\\sounds\\ambience1.mp3");
    pAudio->LoadSample(SOUND_CLEAR_BRIDGE,"assets\\sounds\\clear_bridge.wav");
    pAudio->LoadSample(SOUND_DIVING,"assets\\sounds\\diving.mp3");
    pAudio->LoadSample(SOUND_GQ,"assets\\sounds\\general_quarters.wav");
    pAudio->LoadSample(SOUND_ANCHORS,"assets\\sounds\\AnchorsAway.mp3");
    pAudio->LoadSample(SOUND_TAPS,"assets\\sounds\\Taps.mp3");
    pAudio->LoadSample(SOUND_SINKING,"assets\\sounds\\sinking2.mp3");
    pAudio->LoadSample(SOUND_ANTHEM,"assets\\sounds\\USA.mp3");
    pAudio->LoadSample(SOUND_FUNERAL,"assets\\sounds\\fnrlMrch.wav");
    pAudio->LoadSample(SOUND_PING,"assets\\sounds\\sonar1.mp3");
    
    //load stream
    pAudio->LoadStream(SOUND_BIO, "assets\\sounds\\fish.mp3");
    pAudio->AddStreamClip(SOUND_BIO1, SOUND_BIO, 0, 17708);
    pAudio->AddStreamClip(SOUND_BIO2, SOUND_BIO, 17708, 26434);
    pAudio->AddStreamClip(SOUND_BIO3, SOUND_BIO, 26434, 42859);
    pAudio->AddStreamClip(SOUND_BIO4, SOUND_BIO, 42859, 59541);
    pAudio->AddStreamClip(SOUND_BIO5, SOUND_BIO, 59541, 80073);

    for(int i = 0; i < pAudio->Size(); i++){
      pLog->Log(pAudio->GetFilename(i), i); //write filename and file index to log file
    }
  }
  else{
    pLog->Log("Audio disabled");
  }

  pLog->Log("Main game loop entered!");
  ::ShowCursor(false); 
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  g_strFPS = "Game Challenge 6 (Chuck Bolin) - \"Sinking of the Rising Sun\", v0.047 Final Version";
  ::SetWindowText(g_hWnd, g_strFPS.c_str());
  double timeDelta;

  g_con.InitializePyro();

  //Main application loop
  //*******************************************************
  while (g_bRunning)
  {
    //Handle messages
    if (PeekMessage (&msg,NULL,0,0,PM_REMOVE)){
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }        
    else{}
      timeDelta = g_LoopTimer.getTimeDifference();
      g_pNext = g_pCurrent->update(timeDelta);
      if(g_pNext == g_pStateEnd)
        g_bRunning = false;
      else if(NULL != g_pNext)
	    {
        g_pCurrent->deactivate();
        g_pCurrent = g_pNext;
        g_pCurrent->activate();
      }
     
	  // Render our current game object
    g_pCurrent->render(g_con);

    //call each second update FPS
    nDisplayFPSCount++;
    if(g_FPS_Timer.getTimer(1)){
      //g_FPS_Counter++;
      oss.str(""); //clear oss
      oss << (int) nDisplayFPSCount;
      //if(g_FPS_Counter > 60){  //call every 60 seconds
      //  pLog->Log("FPS",oss.str());
      //  g_FPS_Counter == 0;
      //}

  	  // update our FPS string.
	    // This thing can be rendered inside any of the game objects.
      g_strFPS = "FPS " + oss.str();
      g_Global.g_strFPS = "FPS " + oss.str();

      nDisplayFPSCount=0;//reset frame count
    }
  }//while(...
  g_con.ClosePyro();
  g_con.CloseD3D();
  pLog->Log("DirectX closed!");
  Shutdown();

  return 0;//(int)msg.wParam;  //returns exit code to operating system
} 
  
/*************************************************************************
Window Procedure - Process Windows events

*************************************************************************/
LRESULT CALLBACK WindowProcedure (HWND g_hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    CLog *pLog = CLog::Instance();

    //Check message
    switch(uMessage)
    {

    case WM_CREATE:
      g_pStateIntro->initialize();
      g_pStateMain->initialize();
      g_pStatePlay->initialize();    
      g_pStateEnd->initialize();
      g_pStateHistory->initialize();
      g_pStateReload->initialize();
      g_pStateCredits->initialize();
      g_pStateSelect->initialize();
      g_pStateAwards->initialize();
      g_pStateWin->initialize();
      g_pStateLose->initialize();
      g_pStateStatus->initialize();
      g_pStateChart->initialize();
      g_pStateControl->initialize();
      g_pStateSonar->initialize();
      g_pStateEnd->initialize();
      g_pStateScope->initialize();
      g_pStateFireControl->initialize();
      g_pStateRadar->initialize();
      g_pCurrent = g_pStateIntro;
      g_pStateIntro->activate();
      pLog->Log("Game states initialized!");
      pLog->Log("Game State = Intro");
      return 0;

	  case WM_CLOSE:
		  g_bRunning = false;
		  break;

    case WM_DESTROY:
      //Shutdown();
  	  PostQuitMessage(0);
      break;

    case WM_LBUTTONDOWN:
      g_Global.g_bLeftMouseDown = true;
      return 0;

    case WM_LBUTTONUP:
      g_Global.g_bLeftMouseDown = false;
      return 0;

    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
      
    //case WM_MOUSEMOVE:
      return 0;
    
    }
   // return 0;
    return DefWindowProc(g_hWnd,uMessage,wParam,lParam);
}

/*************************************************************************
MakeWindow - Creates window
*************************************************************************/
int MakeWindow (HINSTANCE hInstance, bool bWindowedMode, Graphics &g_con)
{
    CLog *pLog = CLog::Instance();

    WNDCLASSEX wc;
    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1)); 
    wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR3));   //NULL;//(HCURSOR)IDC_CROSS;
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GC6";
    wc.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1)); 
    
    if(!RegisterClassEx(&wc))
        return false;    

    //screen data
    int cxScreen, cyScreen;
	  cxScreen = ::GetSystemMetrics(SM_CXMAXIMIZED);
	  cyScreen = ::GetSystemMetrics(SM_CYMAXIMIZED);
    if(g_Global.g_screenWidth < 1 || g_Global.g_screenWidth > cxScreen)
      g_Global.g_screenWidth = cxScreen;   
    if(g_Global.g_screenHeight < 1 || g_Global.g_screenHeight > cyScreen)
      g_Global.g_screenHeight = cyScreen;    

    //Create window
    g_hWnd = CreateWindowEx (NULL, "GC6", " ",
        g_Global.g_WindowedMode ? WS_BORDER | WS_CAPTION | WS_SYSMENU : WS_EX_TOPMOST,
		0,								// x position
		0,								// y position
		g_Global.g_WindowedMode ? g_Global.g_screenWidth : CW_USEDEFAULT,	// width
		g_Global.g_WindowedMode ? g_Global.g_screenHeight : CW_USEDEFAULT,	// height
    NULL, NULL, hInstance, NULL);
    
    
    if (!g_hWnd){//Make sure window was created properly
      pLog->Log("Window failed to create!");
      return false;
    }
    pLog->Log("Window creation successful!");

  	// Show the window!
    ShowWindow( g_hWnd, SW_SHOW );
    UpdateWindow( g_hWnd );

	  // Read the actual screen size
	  RECT rc;
    ::GetClientRect( g_hWnd, &rc );
	  int ViewWidth, ViewHeight;
    ViewWidth  = rc.right - rc.left;
    ViewHeight = rc.bottom - rc.top;
    g_con.SetScreenWidthHeight(ViewWidth, ViewHeight);//ViewWidth, ViewHeight);
    pLog->Log("WIDTH",g_con.GetScreenWidth());
    pLog->Log("HEIGHT", g_con.GetScreenHeight());
    return true;//Success
}


void Shutdown()
{
  CLog *pLog = CLog::Instance();
  CAudioManager *pAudio = CAudioManager::Instance();
  pLog->Log("************ Shutting Down ************");
	// Properly shut down the current game object first
	g_pCurrent->deactivate();
	delete g_pStatePlay;
  delete g_pStateMain;
  delete g_pStateIntro;
  delete g_pStateHistory;
  delete g_pStateReload;
  delete g_pStateCredits;
  delete g_pStateSelect;
  delete g_pStateAwards;
  delete g_pStateWin;
  delete g_pStateLose;
  delete g_pStateStatus;
  delete g_pStateChart;
  delete g_pStateControl;
  delete g_pStateEnd;
  delete g_pStateScope;
  delete g_pStateSonar;
  delete g_pStateFireControl;
  delete g_pStateRadar;

  pLog->Log("Game state pointers deleted!");
  pAudio->KillAll();
  ::ShowCursor(false);
	//::Sleep(2000);
	//::ShowCursor(true); //turn mouse cursor back on
	pLog->Log("Audio sounds are all killed!");
	pLog->Log("*************************************");
	pLog->Log("Program ended!");
	pLog->Log("*************************************");

	// Kill the main window
	DestroyWindow( g_hWnd );
	g_hWnd = NULL;
}

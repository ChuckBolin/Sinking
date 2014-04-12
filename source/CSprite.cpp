#include "..\includes\CSprite.h"

CSprite::CSprite(){
}

CSprite::~CSprite(){
}

bool CSprite::LoadSprites(){

  //load sprite data
  // S P R I T E S . D A T
  //pLog->Log("************ Sprites.dat ************");
  CFileReader* cfr = new CFileReader;
  std::string sValue;
  cfr->LoadFile("data\\sprites.dat");
  GRAPHIC_IMAGE gi;
  if(cfr->IsValid()== true){
    //pLog->Log("Valid data file for sprites.dat");
    //pLog->Log("Numbers of lines in file",cfr->GetNumberOfLines());
    for(int j=0; j< cfr->GetNumberOfLines();j++){

      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),2);
      gi.fileID = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),3);
      gi.sourceX = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),4);
      gi.sourceY = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),5);
      gi.width = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),6);
      gi.height = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),7);
      gi.rotationX = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),8);
      gi.rotationY = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),9);
      gi.scale = atof(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),10);
      gi.alpha = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),11);
      gi.rotationAngle = atof(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),12);
      gi.name = sValue;
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),13);
      gi.layer = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),14);
      gi.frameMax = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),15);
      gi.updateInterval = (double)atof(sValue.c_str());
      gi.frameCount = 0;
      gi.time = 0;
      gi.enable = true;

      AddSprite(gi);
    }
  }
  else{
   // pLog->Log("ERROR****************** Failure to sprite data (sprites.dat)");
    delete cfr;
   // pLog->Log("Terminating program!");
    return false;
  }

  return true;
}

void CSprite::AddSprite( GRAPHIC_IMAGE &gi ){
  m_image.push_back(gi);
}

GRAPHIC_IMAGE CSprite::GetSpriteData(int spriteID){
  GRAPHIC_IMAGE temp;
  temp = m_image[0]; //default icon...meaning sprite not found

  if(spriteID < 0 || spriteID > m_image.size())
    return temp;

  return m_image[spriteID];
}

int CSprite::Size(){
  return m_image.size();
}


void CSprite::EnableSprite(int spriteID){
  if(spriteID < 0 || spriteID > m_image.size())
    return;
  m_image[spriteID].enable = true;
}

void CSprite::DisableSprite(int spriteID){
  if(spriteID < 0 || spriteID > m_image.size())
    return;
  m_image[spriteID].enable = false;
}

bool CSprite::IsEnabled(int spriteID){
  if(spriteID < 0 || spriteID > m_image.size())
    return false;

  return  m_image[spriteID].enable;
}

//this animates sprites
void CSprite::IncrementSpriteFrameCount(int spriteID, double timeDifference){
  GRAPHIC_IMAGE temp;
  if(spriteID < 0 || spriteID > m_image.size())
    return ;

  if(m_image[spriteID].frameMax == 0)  //don't inc if not animated
    return ;

  //animate if time is correct
  m_image[spriteID].time += timeDifference;
  if(m_image[spriteID].time > m_image[spriteID].updateInterval){
    m_image[spriteID].time = 0;

    if(m_image[spriteID].frameMax > 0){ //clockwise direction
      m_image[spriteID].frameCount++;
      if (m_image[spriteID].frameCount > m_image[spriteID].frameMax - 1)
        m_image[spriteID].frameCount = 0;
    }
    else if(m_image[spriteID].frameMax < 0){ //clockwise direction
      m_image[spriteID].frameCount--;
      if (m_image[spriteID].frameCount < m_image[spriteID].frameMax + 1 )
        m_image[spriteID].frameCount = 0;
    }

  }
}
#include "MyButton.h"


//file CGUIButton.cpp
//普通Gui按钮类实现文件
MyButton::MyButton(int _id,HGE *HgeEngine,HTEXTURE tex,float x,float y,float w,float h,HEFFECT ClickEffect,bool debug)
{
 m_HgeEngine = HgeEngine;
 id = _id;
 m_w = w; m_h = h;
 m_BtnDown = new hgeSprite(tex,0,0,m_w,m_h);
 m_BtnUp = new hgeSprite(*m_BtnDown);
 m_BtnNow = m_BtnUp;
 m_ClickEffect = ClickEffect;
 //设置是否"调位模式"//测试用
 if (!debug)
 {
  m_BtnUp->SetColor(0xFFFFFFFF);
  m_BtnDown->SetColor(0xFFD4C25A);
 }
 else
 {
  m_BtnUp->SetColor(0xFFFF0000);
 }
 bStatic = false; bVisible = true; bEnabled = true;
 //设置按钮的区域
 rect.Set(x,y,x+w,y+h);
}




MyButton::~MyButton()
{
 m_HgeEngine->Effect_Free(m_ClickEffect);
 delete(m_BtnUp);
 delete(m_BtnDown);
}

bool MyButton::MouseLButton(bool bDown)
{
 if (bDown)
 {
  m_HgeEngine->Effect_Play(m_ClickEffect);
  m_BtnNow = m_BtnDown;
 }
 else
  m_BtnNow = m_BtnUp;
 return (bDown)?false:true;
}

void MyButton::Render()
{
 m_BtnNow->Render(rect.x1,rect.y1);
}





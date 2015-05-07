#ifndef MyButton_H_
#define MyButton_H_

#include "hgegui.h"

class MyButton: public hgeGUIObject
{
public:
	MyButton(int id, HGE *HgeEngine, HTEXTURE tex, float x, float y, float w, float h, HEFFECT ClickEffect, bool debug = false);
	~MyButton();

	virtual bool MouseLButton(bool bDown);
	virtual void Render();
private:
	HEFFECT m_ClickEffect; 
	hgeSprite *m_BtnDown;
	hgeSprite *m_BtnUp;
	hgeSprite *m_BtnNow;
	float m_h;
	float m_w;
	HGE *m_HgeEngine; 
};

#endif
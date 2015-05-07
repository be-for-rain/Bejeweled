#include "MyMenu.h"
#include <cmath>
#include <iostream>

using namespace std;


void MyMenu::set_state(MENU_STATE new_state)
{
	state = new_state;
}

int MyMenu::frame_func()
{
	float dt = hge->Timer_GetDelta();
	static float t = 0.0f;
	float tx, ty;
	for (int i = I_PLAY; i <= I_NEWROUND_BACK; i++){
		gui->EnableCtrl(i, false);
		gui->ShowCtrl(i, false);
	}

	switch (state){
	case M_LEAVE:
		return id;
		break;
	case M_WELCOME:
		for (int i = I_PLAY; i <= I_EXIT; i++){
			gui->EnableCtrl(i, true);
			gui->ShowCtrl(i, true);
		}
		break;
	case M_LEVEL:
		for (int i = I_LEVEL_0; i <= I_LEVEL_BACK; i++){
			gui->EnableCtrl(i, true);
			gui->ShowCtrl(i, true);
		}
		break;
	case M_SKIN:
		for (int i = I_SKIN_0; i <= I_SKIN_BACK; i++){
			gui->EnableCtrl(i, true);
			gui->ShowCtrl(i, true);
		}
		break;
	case M_HELP:
		gui->EnableCtrl(I_HELP_BACK, true);
		gui->ShowCtrl(I_HELP_BACK, true);
		break;
	case M_NEWROUND:
		for (int i = I_ADD; i <= I_NEWROUND_BACK; i++){
			gui->EnableCtrl(i, true);
			gui->ShowCtrl(i, true);
		}
		break;
	}

	id = gui->Update(dt);
	switch (id){
	case I_PLAY:
	case I_NEWROUND_PLAY:
	case I_REPLAY:
		state = M_LEAVE;
		break;
	case I_LEVEL:
		state = M_LEVEL;	//enter choosing level menu
		break;
	case I_HELP:
		state = M_HELP;
		break;
	case I_SKIN:
		state = M_SKIN;
		break;
	case I_EXIT:
		state = M_LEAVE;
		break;
	case I_LEVEL_0:
	case I_LEVEL_1:
	case I_LEVEL_2:
	case I_LEVEL_BACK:
		state = M_WELCOME;
		break;
	case I_SKIN_0:
	case I_SKIN_1:
	case I_SKIN_2:
	case I_SKIN_BACK:
		state = M_WELCOME;
		break;
	case I_HELP_BACK:
		state = M_WELCOME;
		break;
	case I_NEWROUND_BACK:
		state = M_WELCOME;
		break;
	}

	// Here we update our background animation
	t+=dt;
	tx=50*cosf(t/60);
	ty=50*sinf(t/60);

	quad.v[0].tx=tx;        quad.v[0].ty=ty;
	quad.v[1].tx=tx+WINDOW_WIDTH/64; quad.v[1].ty=ty;
	quad.v[2].tx=tx+WINDOW_WIDTH/64; quad.v[2].ty=ty+WINDOW_HEIGHT/64;
	quad.v[3].tx=tx;        quad.v[3].ty=ty+WINDOW_HEIGHT/64;

	return id;
	//return false;
}

bool MyMenu::render_func()
{
	hge->Gfx_RenderQuad(&quad);
	switch (state){
	case M_WELCOME:
		text_bejeweled->Render();
		break;
	case M_SKIN:{
		skin0->RenderEx(350, 50, 0, 0.2, 0);
		skin1->RenderEx(350, 200, 0, 0.2, 0);
		skin2->RenderEx(350, 350, 0, 0.2, 0);
		break;
				}
	case M_HELP:
		help->Render(200, 0);
		break;

	}
	gui->Render();
	
	fnt->SetColor(0xFFFFFFFF);
	return false;
}

MyMenu::MyMenu(HGE *hgee)
	:hge(hgee),
	state(M_WELCOME){
		quad.tex=hge->Texture_Load(BG_IMAGE);
		tex=hge->Texture_Load("image/cursor.png");
		snd=hge->Effect_Load("sound/menu.wav");
	
		// Set up the quad we will use for background animation
		quad.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

		for(int i=0;i<4;i++)
		{
			// Set up z-coordinate of vertices
			quad.v[i].z=0.5f;
			// Set up color. The format of DWORD col is 0xAARRGGBB
			quad.v[i].col=0xFFFFFFFF;
		}

		quad.v[0].x=0; quad.v[0].y=0; 
		quad.v[1].x=WINDOW_WIDTH; quad.v[1].y=0; 
		quad.v[2].x=WINDOW_WIDTH; quad.v[2].y=WINDOW_HEIGHT; 
		quad.v[3].x=0; quad.v[3].y=WINDOW_HEIGHT; 


		// Load the font, create the cursor sprite
		fnt=new hgeFont("font/font1.fnt");
		fnt->SetScale(1.2);
		spr=new hgeSprite(tex,0,0,32,32);

		// Create and initialize the GUI
		gui=new hgeGUI();

		//M_WELCOME
		//score_text = new hgeGUIText(20, 30, 300, 100, 100, fnt);
		text_bejeweled = new hgeGUIText(T_WELCOME, 350, 70, 100, 100, fnt);
		text_bejeweled->SetText("Welcome to Bejeweled World~!");
		text_bejeweled->SetMode(HGETEXT_CENTER);
	
		skin0 = new hgeSprite(hge->Texture_Load("image/skin0.jpg"), 0, 0, 800, 600);
		skin1 = new hgeSprite(hge->Texture_Load("image/skin1.jpg"), 0, 0, 800, 600);
		skin2 = new hgeSprite(hge->Texture_Load("image/skin2.jpg"), 0, 0, 800, 600);
	
		help = new hgeSprite(hge->Texture_Load(HELP_IMAGE), 0, 0, 400, 500);
		
		gui->AddCtrl(new hgeGUIMenuItem(I_PLAY,fnt,snd,400,200,0.0f,"Play"));
		gui->AddCtrl(new hgeGUIMenuItem(I_LEVEL,fnt,snd,400,240,0.1f,"Choose Level"));
		gui->AddCtrl(new hgeGUIMenuItem(I_SKIN,fnt,snd,400,280,0.2f,"Choose Skin"));
		gui->AddCtrl(new hgeGUIMenuItem(I_HELP,fnt,snd,400,320,0.3f,"Help"));
		gui->AddCtrl(new hgeGUIMenuItem(I_EXIT,fnt,snd,400,360,0.4f,"Exit"));
	
		//M_LEVEL
		gui->AddCtrl(new hgeGUIMenuItem(I_LEVEL_0,fnt,snd,400,200,0.0f,"Level 0"));
		gui->AddCtrl(new hgeGUIMenuItem(I_LEVEL_1,fnt,snd,400,240,0.1f,"Level 1"));
		gui->AddCtrl(new hgeGUIMenuItem(I_LEVEL_2,fnt,snd,400,280,0.2f,"Level 2"));
		gui->AddCtrl(new hgeGUIMenuItem(I_LEVEL_BACK,fnt,snd,400,320,0.3f,"Back"));

		//M_SKIN
		gui->AddCtrl(new hgeGUIMenuItem(I_SKIN_0,fnt,snd,250,100,0.0f,"Skin 0"));
		gui->AddCtrl(new hgeGUIMenuItem(I_SKIN_1,fnt,snd,250,250,0.1f,"Skin 1"));
		gui->AddCtrl(new hgeGUIMenuItem(I_SKIN_2,fnt,snd,250,400,0.2f,"Skin 2"));
		gui->AddCtrl(new hgeGUIMenuItem(I_SKIN_BACK,fnt,snd,250,550,0.2f,"Back"));
		
		//M_HELP
		gui->AddCtrl(new hgeGUIMenuItem(I_HELP_BACK,fnt,snd,400,550,0.0f,"Back"));
		
		//M_NEWROUND
		gui->AddCtrl(new hgeGUIMenuItem(I_ADD,fnt,snd,400,200,0.0f,"+"));
		gui->AddCtrl(new hgeGUIMenuItem(I_SUBSTRACT,fnt,snd,400,280,0.1f,"-"));
		gui->AddCtrl(new hgeGUIMenuItem(I_NEWROUND_PLAY,fnt,snd,400,320,0.2f,"Play"));
		gui->AddCtrl(new hgeGUIMenuItem(I_REPLAY,fnt,snd,400,360,0.3f,"Replay"));
		gui->AddCtrl(new hgeGUIMenuItem(I_NEWROUND_BACK,fnt,snd,400,400,0.4f,"Back"));

		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetCursor(spr);
		gui->SetFocus(1);

		//gui->Enter();
}


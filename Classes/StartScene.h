#ifndef STARTSCENE_H
#define STARTSCENE_H

#pragma execution_character_set("utf-8")

#include "cocos2d.h"
#include "opencv2/opencv.hpp"

USING_NS_CC;

class StartScene : public cocos2d::Layer, public TextFieldDelegate
{
private:
	//--------------------------------------------------------------------------------
	// �⺻ ����
	Size size_window;												// ���� �������� ������
	TextureCache *cache;											// �ؽ��� ĳ�ð�����
	Sprite *cursor_sprite;											// Ŀ�� ��������Ʈ
	const std::string CURSOR_FILENAME = "images/cursor_arrow.png";	// Ŀ�� ���ϰ��
	//--------------------------------------------------------------------------------
	// Ű �Է�
	bool status_keyUsing;											// Ű ��� ����(�׼����� �� Ű�Է� ����)
	std::vector<EventKeyboard::KeyCode> heldKeys;					// Ű �Է� ���� ����
	//--------------------------------------------------------------------------------
	// ���� ��� opencv ����
	cv::VideoCapture video_capture;									// ������ ĸ�İ�ü
	cv::Mat video_frame;											// ������ 1������
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// ���� ���� ����
	const std::string BGMMAIN_FILENAME = "bgm/intro_Masader.mp3";	// ���� BGM
	const float BGMMAIN_VOLUME = 0.4f;								// ���� BGM ����
	const char SOUND_ELECTIC[30] = "sound/electric.wav";			// ���� ����Ҹ�
	const char SOUND_CHANGELAYER[30] = "sound/init.mp3";			// ��� ���� �Ҹ�
	const char SOUND_CHANGELOGIN[30] = "sound/select.wav";			// �α��� ��Ŀ�� ��ȯ �Ҹ�
	const char SOUND_LOGINFAIL[30] = "sound/tick.wav";				// �α��� ���� �Ҹ�
	const char SOUND_CHANGEMENUSELECT[30] = "sound/select.wav";		// �޴� ���� ��ȯ �Ҹ�
	//--------------------------------------------------------------------------------
	// �� ���� ���� - ���̾� ����
	// START ���� ũ�� 6���� ���̾� ��ȯ�� �ִ�
	// �ΰ��������(0) - �ƹ�Ű�� �����ÿ�(1) - �α���ȭ��(2) - �޴�(4)
	//											��			 ��-- 1. singleplay
	//										 ȸ������ ȭ��	(3)	 |-- 2. multiplay
	//														 |-- 3. editor(5)
	//														 |-- 4. settings(6)
	//														 |-- 5. exit
	int status_layer;
	enum STATUS {
		SHOWLOGO,
		PRESSANYKEY,
		LOGIN,
		SIGNUP,
		MENUSELECT
	};
	Layer *layer_openingLogo;
	Layer *layer_basic;
	Layer *layer_pressAnyKey;
	Layer *layer_login;
	Layer *layer_signup;
	Layer *layer_menuSelect;
	Layer *layer_editor;
	Layer *settings;
	//--------------------------------------------------------------------------------
	/*
		SHOW OPENING ���� ����
		������ �����ϰ� ���� ���� ������ ����
		VideoCapture �� ����� mp4 ������ ���������� �ڸ��� �� �������� schedule�� �Լ��� ȣ����
		��������Ʈ �ؽ��ĸ� ���Ž��� ������ ����Ѵ�

		����� ������ �������Լ� ȣ���� �����ϰ� �ڿ����� �ε�ȭ���� �θ���
	*/

	const std::string OPENING_FILENAME = "video/opening.avi";		// ������ ���� �̸�
	const std::string OPENING_SOUNDFILENAME = "bgm/opening.mp3";	// ������ �������� �̸�
	Texture2D *opening_texture;										// ĸ���� 1�������� ����
	Sprite *opening_sprite;											// ���� �ؽ��ĸ� �ø�
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*
		�⺻���̾� ���� ���� - ��� �������� ����Ѵ�
		�ε�ȭ���� �����ϴ� ���� �������� opencv �� �߶� texture2d �迭�� �����Ѵ�
		iter �� ��ȸ�ϸ� ������ ���ѷ��� ��Ű�� schedule�� ȣ���Ѵ�.

		�⺻ DJTechnician ���� �ΰ� ��������Ʈ�� �⺻ ���̾�� �����Ѵ�.

	*/
	
	const std::string BACKGROUND_FILENAME = "video/background7.mpg";// ������ �����̸� ( 20 �� �̳��� ���ϸ� ���!!)
	const std::string LOGOSPRITE_FILENAME = "images/dj10.png";		// �ΰ� ���� ��������Ʈ �����̸�
	const std::string SUBLOGOSPRITE_FILENAME
		= "images/technician.png";									// ����ΰ� ��������Ʈ �����̸�
	const std::string SUBLOGOPARTICLE_FILENAME
		= "particle/logo_particle.png";								// ����ΰ� ��ƼŬ �����̸�
	Texture2D* background_texture;									// ĸ���� 1�������� ���� texture
	Sprite *background_sprite;										// ĸ���� texture�� ���� ��� sprite
	Sprite *logo_sprite;											// �ΰ� ��������Ʈ
	Sprite *sublogo_sprite;											// ����ΰ� ��������Ʈ
	ParticleGalaxy *sublogo_particle;								// ����ΰ� ��ƼŬ
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// PRESSANYKEY ���� ����
	const std::string PRESSANYKEY_STRING = "PRESS ENTER TO START";	// �� ���
	const std::string PRESSANYKEY_FONTS = "fonts/LuluMonospace.ttf";// ��Ʈ ����
	const int PRESSANYKEY_FONTSIZE = 35;							// ��Ʈ ������
	Label *pressAnyKey_label;										// pressAnyKey ��
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// LOGIN ���� ����
	const std::string LOGINEDITON_FILENAME
		= "images/login_edittext_on.png";					// ����Ʈ �̹���off ����
	const std::string LOGINEDITOFF_FILENAME
		= "images/login_edittext_off.png";					// ����Ʈ �̹���on ����
	const std::string LOGINLOGINOFF_FILENAME
		= "images/login_login_off.png";						// �α��� �̹���off ����
	const std::string LOGINLOGINON_FILENAME
		= "images/login_login_on.png";						// �α��� �̹���on ����
	const std::string LOGINSIGNUPOFF_FILENAME
		= "images/login_signup_off.png";					// ȸ������ �̹���off ����
	const std::string LOGINSIGNUPON_FILENAME
		= "images/login_signup_on.png";						// ȸ������ �̹���on ����
	const std::string LOGINFAIL_FILENAME
		= "images/login_fail.png";							// �α��� ���� â ����
	const std::string LOGIN_FONTS
		= "fonts/LuluMonospace.ttf";						// ��Ʈ ����
	const int LOGIN_FONTSIZE = 20;							// ��Ʈ ������
	int status_login;										// ���� �α��� ȭ�� ���Ž� ����
	enum LOGINSTATUS {
		PREVERR_LOGIN,
		IDINPUT, PWINPUT, LOGINBTN, SIGNUPBTN,
		NEXTERR_LOGIN, LOGINFAIL
	};
	Sprite *login_sprite_id;								// ���̵� �Է� �ؽ�Ʈ��������Ʈ
	TextFieldTTF *login_textfield_id;						// ���̵� �Է� �ؽ�Ʈ�ʵ�
	Sprite *login_sprite_pw;								// �н����� �Է� �ؽ�Ʈ��������Ʈ
	TextFieldTTF *login_textfield_pw;						// �н����� �Է� �ؽ�Ʈ�ʵ�
	Sprite *login_sprite_login;								// �α��� ��ư ��������Ʈ
	Sprite *login_sprite_signup;							// ȸ������ ��ư ��������Ʈ
	Sprite *login_sprite_fail;								// �α��� ���� ��������Ʈ
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// SIGNUP ���� ����

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// MENUSELECT ���� ����
	const std::string MENUSELECT_FONTS
		= "fonts/gnuolane.ttf";								// ��Ʈ ����
	const int MENUSELECT_FONTSIZE = 50;						// ��Ʈ ������
	const int MENUSELECT_DIST = 55;							// �޴��� ����
	const Color4B MENUSELECT_FOCUSEDCOLOR = Color4B::RED;	// ��Ŀ�̵� �޴� ����
	RepeatForever *menuSelect_rep;							// ��¦�̱�
	int status_menuSelect;									// ���� �޴����� ȭ�� ���Ž� ����
	const int MENUSELECT_MENUSIZE = 5;						// �� �޴� ����
	enum MENUSELECTSTATUS {
		SINGLEPLAY, MULTIPLAY, SETTING, EDITOR, EXIT
	};
	const std::string MENUSELECT_STR[5] = {
		"SINGLE PLAY",
		"MULTI  PLAY",
		"SETTINGS",
		"E D I T O R",
		"E  X  I  T"
	};														// �� �� �� ��
	Label *menuSelect_label[5];								// �޴� ��
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// EDITOR ���� ����

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// SETTINGS ���� ����

	//--------------------------------------------------------------------------------


public:

	static cocos2d::Scene* createScene();

	virtual bool init();
	/* ---------------------------- INPUT FUNCTIONS -------------------------------*/
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event *event);
	void onKeyHold(float interval);
	void releaseKeyInput(float interval);
	void onMouseDown(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);
	void onMouseMove(cocos2d::Event* event);
	void onMouseScroll(cocos2d::Event* event);
	/* ----------------------------------------------------------------------------*/

	/* -------------------------- SETTING FUNCTIONS -------------------------------*/
	void showOpening();
	void opening_tick(float interval);
	void setup();
	void enableKeySetting();
	void setLayerBasicLayer();
	void background_tick(float interval);
	void setLayerPressAnyKey();
	void setLayerLogin();
	void setLayerSignUp();
	void setLayerMenuSelect();
	void setLayerEditor();
	void setLayerSettings();
	/* ----------------------------------------------------------------------------*/

	/* -------------------------- SHOW FUNCTIONS ----------------------------------*/
	void showPressAnyKey();
	void startBGM(float interval);
	void enterLogin();
	void changeLoginStatus(int from, int to, bool isEnterKey);
	void returnFailToLoginStatus();
	void enterMenuSelect();
	void changeMenuSelectStatus(int from, int to, bool isEnterKey);
	void enterSignUp();
	/* ----------------------------------------------------------------------------*/

	/* ------------------------ SCENE CHANEGE FUNCTIONS ---------------------------*/
	void goMusicSelectScene();

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
};

#endif
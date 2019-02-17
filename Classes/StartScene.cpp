#include "StartScene.h"
#include "MusicSelectScene.h"
#include "AppDelegate.h"
#include "DBManager.h"
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"

/* Ű ���� �Է� ���� ��ũ�� */
#define _KEYUSING if(status_keyUsing){ return;} status_keyUsing = true
/* Ű ���� �Է� ���� ���� ��ũ�� */
#define _KEYRELEASE(num) this->scheduleOnce(schedule_selector(StartScene::releaseKeyInput), num)

USING_NS_CC;
using namespace experimental;
using namespace CocosDenshion;

//scene ����
Scene* StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

/* Ű ������ ���� �Լ� */
void StartScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()) {
		heldKeys.push_back(keyCode);
	}
}
void StartScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}
void StartScene::onKeyHold(float interval) {

	/*
	
		���Ϳ� ���� Ű�Է��� ���δ� ���� Ű �Է� �� �� �Լ���
		���� Ű �Է��� �ʿ����� ���� ���� status_keyUsing �� true ó�����ְ�
		�Լ��� ������ �������� releaseKeyInput() ���־�� �Ѵ�.
	
	*/

	/* ����Ű UP �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_UP_ARROW) != heldKeys.end()) {

		/* LOGIN ���� ��Ŀ�̵� ���� ����*/
		if (status_layer == STATUS::LOGIN) {
			changeLoginStatus(status_login, status_login - 1, false);
		}

		/* MENUSELECT ���� ��Ŀ�̵� ���� ���� */
		if (status_layer == STATUS::MENUSELECT) {
			changeMenuSelectStatus(status_menuSelect, status_menuSelect - 1, false);
		}

	}

	/* ����Ű DOWN �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_DOWN_ARROW) != heldKeys.end()) {
	
		/* LOGIN ���� ��Ŀ�̵� ���� ����*/
		if (status_layer == STATUS::LOGIN) {
			changeLoginStatus(status_login, status_login + 1, false);
		}

		/* MENUSELECT ���� ��Ŀ�̵� ���� ���� */
		if (status_layer == STATUS::MENUSELECT) {
			changeMenuSelectStatus(status_menuSelect, status_menuSelect + 1, false);
		}

	}

	/* ����Ű RIGHT �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_RIGHT_ARROW) != heldKeys.end()) {

	}

	/* ����Ű LEFT �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_LEFT_ARROW) != heldKeys.end()) {

	}

	/* ENTER Ű �Է� */
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_ENTER) != heldKeys.end()) {
		
		/* PRESSANYKEY -> LOGIN ���� LAYER ��ȯ */
		if (status_layer == STATUS::PRESSANYKEY) {
			enterLogin();
		}

		/* LOGIN ���� ��Ŀ�̵� ���� ��ȯ*/
		if (status_layer == STATUS::LOGIN && status_login != LOGINSTATUS::LOGINFAIL) {
			changeLoginStatus(status_login, status_login + 1, true);
		}

		/* LOGIN ���� ���� -> �α��� ����*/
		if (status_layer == STATUS::LOGIN && status_login == LOGINSTATUS::LOGINFAIL) {
			returnFailToLoginStatus();
		}

		/* MENUSELECT ���� ��Ŀ�̵� ���� ���� */
		if (status_layer == STATUS::MENUSELECT) {
			changeMenuSelectStatus(status_menuSelect, status_menuSelect + 1, true);
		}

	}

	/* ESC Ű �Է� */
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_ESCAPE) != heldKeys.end()) {

	}

	/* BACKSPACE Ű �Է� */
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_BACKSPACE) != heldKeys.end()) {
		
	}
}

void StartScene::releaseKeyInput(float interval) {
	status_keyUsing = false;
}

/*

	���콺 ������ ���� �Լ�
	
*/

/* ���콺 Ŭ�� �� */
void StartScene::onMouseDown(cocos2d::Event* event) {

}

/* ���콺 Ŭ�� �� ������ */
void StartScene::onMouseUp(cocos2d::Event* event) {

}

/* ���콺 �̵� �� - ���콺 ������ ���� */
void StartScene::onMouseMove(cocos2d::Event* event) {
	/*

		���콺 �̵��� �����ؼ� ���콺��������Ʈ�� ��ġ�� ��� �ٲپ���
	
	*/
	auto mouseEvent = static_cast<EventMouse*>(event);
	Vec2 movePoint = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
	cursor_sprite->setPosition(movePoint);
}

/* ���콺 ��ũ�� �� */
void StartScene::onMouseScroll(cocos2d::Event* event) {

}

/* Scene�� ���� init() �Լ� ���� */
bool StartScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

#ifdef _DEBUG
	/* ����� �ÿ��� �ΰ�ȭ�� �ǳʶڴ� */
	setup();
#else
	/* �ε� �� �ΰ�ȭ�� */
	showOpening();
#endif



	return true;
}

void StartScene::setup() {
#ifdef _DEBUG
	/* ������� �ʱ�ȭ */
	size_window = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	cache = Director::getInstance()->getTextureCache();
	status_keyUsing = true;
	status_layer = STATUS::SHOWLOGO;
	heldKeys.clear();
#endif
	// Ű �� ���콺, ��ġ �Է� ����
	enableKeySetting();

	// �� ���̾� ����
	setLayerBasicLayer();
	setLayerPressAnyKey();
	setLayerLogin();
	setLayerSignUp();
	setLayerMenuSelect();
	setLayerEditor();
	setLayerSettings();

	// �����ϸ� ������
	showPressAnyKey();
}

/* �ΰ� ȭ�� �����ֱ� */
void StartScene::showOpening() {

	/* ������� �ʱ�ȭ */
	size_window = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	cache = Director::getInstance()->getTextureCache();
	status_keyUsing = true;
	status_layer = STATUS::SHOWLOGO;
	heldKeys.clear();

	/*
	
			�����ϸ� �����׿����� ������ opencv�� �߶� �ؽ��ĺ��ͷ� ����

	*/

	/* ���̾� �ʱ�ȭ */
	layer_openingLogo = Layer::create();
	layer_openingLogo->setPosition(0, 0);
	layer_openingLogo->setContentSize(Size(size_window.width, size_window.height));
	this->addChild(layer_openingLogo);

	/* ���� ��� */
	AudioEngine::play2d(OPENING_SOUNDFILENAME, false);

	/* OPENCV ���� ĸ�� ���� ���� �� ���� */
	video_capture.open(OPENING_FILENAME);
	if (!video_capture.isOpened()) {
#ifdef _DEBUG
		CCLOG("can't open video background..");
#endif
		return;
	}
	else {

		/* ĸ���� ������ 1������ MAT ������ Texture2D �� ��ȯ */
		video_capture >> video_frame;
		opening_texture = new Texture2D();
		opening_texture->initWithData(video_frame.data,
			video_frame.elemSize() * video_frame.cols * video_frame.rows,
			Texture2D::PixelFormat::RGB888,
			video_frame.cols,
			video_frame.rows,
			Size(video_frame.cols, video_frame.rows));

		/* ������ ��������Ʈ ���� */
		opening_sprite = Sprite::create();
		opening_sprite->initWithTexture(opening_texture);
		opening_sprite->setContentSize(Size(size_window.width, size_window.height));
		opening_sprite->setPosition(size_window.width / 2, size_window.height / 2);

		/* ������ ��������Ʈ ��� ƽ ������ */
		this->schedule(schedule_selector(StartScene::opening_tick), 0.03f);

		layer_openingLogo->addChild(opening_sprite);
	}


}

/* ������ ������ ��� */
void StartScene::opening_tick(float interval) {

	/* ���� �������� ����ĸ�Ŀ��� ������ */
	video_capture >> video_frame;
	if (!video_frame.empty()) {

		/* ���� �������� MAT ���Ϸ� �ؽ��ĸ� ������Ʈ �Ѵ� */
		opening_texture->updateWithData(video_frame.data, 0, 0, video_frame.cols, video_frame.rows);
	}
	else {
		/*

			������ ������ ��� ��. ���� ���̾ �����ϰ�
			setup() ���� �Ѿ.

		*/
		layer_openingLogo->removeChild(opening_sprite);
		this->removeChild(layer_openingLogo);
		this->unschedule(schedule_selector(StartScene::opening_tick));
		setup();
	}

}

/* Ű �Է� �� ���콺 ���� */
void StartScene::enableKeySetting() {

	/* Ű �Է� ���� */
	this->schedule(schedule_selector(StartScene::onKeyHold));
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(StartScene::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(StartScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

	/* Ŀ�� �̹��� ����*/
	cursor_sprite = Sprite::create(CURSOR_FILENAME);
	this->addChild(cursor_sprite, 10);

	/* ���콺 �Է� ���� */
	auto Mouse = EventListenerMouse::create();
	Mouse->onMouseDown = CC_CALLBACK_1(StartScene::onMouseDown, this);
	Mouse->onMouseUp = CC_CALLBACK_1(StartScene::onMouseUp, this);
	Mouse->onMouseMove = CC_CALLBACK_1(StartScene::onMouseMove, this);
	Mouse->onMouseScroll = CC_CALLBACK_1(StartScene::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Mouse, this);

}

/* �⺻ ���̾� ����*/
void StartScene::setLayerBasicLayer() {
	/*

			��� ������ opencv �� �߶� �ؽ��ĺ��ͷ� �޸𸮿� �ε�.
			�ؽ��ĺ��͸� �� �����Ӵ� ��ȸ�ϸ� ��ü�����ν� �������

			10~20�� ������ ���� ������ �� ����� �ξ� ȿ����!
			�׷��� 1�гѾ�� ������ �ѹ��� �̹����ε��ϸ� ��ǻ�Ͱ� �״´�

			������ 10~20�� ���� �������� ����� ��.

	*/

	video_capture.open(BACKGROUND_FILENAME);
	if (!video_capture.isOpened()) {
		CCLOG("can't open video background..");
		return;
	}
	else {
		video_capture >> video_frame;


#ifdef _DEBUG /* ����� �ε� ī��Ʈ ��� ���� */
		int fAllCount = video_capture.get(CV_CAP_PROP_FRAME_COUNT);
		int fCount = 1;
#endif


		while (!video_frame.empty()) {


#ifdef _DEBUG /* ����� �ε� ī��Ʈ ��� */
			CCLOG("load Background Texture2D : %d/%d", fCount++, fAllCount / 2);
#endif

			/* ������ �̸� ĸ���� �����ӵ��� ��� texture ȭ �Ͽ� ���Ϳ� ���� */
			Texture2D *tp_texture = new Texture2D();
			tp_texture->initWithData(video_frame.data,
				video_frame.elemSize() * video_frame.cols * video_frame.rows,
				Texture2D::PixelFormat::RGB888,
				video_frame.cols,
				video_frame.rows,
				Size(video_frame.cols, video_frame.rows));
			background_texture.push_back(tp_texture);
			video_capture >> video_frame;

		}
		
		/* ��� ��������Ʈ �ʱ�ȭ �� ���� */
		background_sprite = Sprite::create();
		background_iter = background_texture.begin();
		background_sprite->initWithTexture(*background_iter);
		background_sprite->setContentSize(size_window);
		background_sprite->setPosition(size_window.width / 2, size_window.height / 2);
		background_sprite->setOpacity(100);
		background_sprite->setVisible(false);

		/* �� ƽ�� ������ �Լ� ȣ���ؼ� ���� texture ���� */
		this->schedule(schedule_selector(StartScene::background_tick));
		
		/* �⺻ ���̾ ��� ��������Ʈ ���*/
		background_sprite->setVisible(false);
		this->addChild(background_sprite);

		/* ���ηΰ� ��������Ʈ ���� ���� - ���� �ִϸ��̼��� ���� ���극�̾� ���(layer_basic) */
		layer_basic = Layer::create();
		layer_basic->setPosition(0, 0);
		layer_basic->setContentSize(size_window);
		layer_basic->setCascadeOpacityEnabled(true);
		layer_basic->setVisible(false);
		this->addChild(layer_basic);
		logo_sprite = Sprite::create(LOGOSPRITE_FILENAME);
		logo_sprite->setPosition(Point(size_window.width / 2, size_window.height / 16 * 11));
		logo_sprite->setCascadeOpacityEnabled(true);
		layer_basic->addChild(logo_sprite, 1);
		auto fo = FadeTo::create(0.2f, 100);
		auto ft = FadeTo::create(0.2f, 150);
		auto dt = DelayTime::create(0.4f);
		auto seq = Sequence::create(fo, ft, dt, nullptr);
		auto rep = RepeatForever::create(seq);
		logo_sprite->runAction(rep);

		/* ����ΰ� ��ƼŬ ���� */
		cache->addImage(SUBLOGOPARTICLE_FILENAME);
		sublogo_particle = ParticleGalaxy::create();
		sublogo_particle->setTexture(cache->getTextureForKey(SUBLOGOPARTICLE_FILENAME));
		sublogo_particle->setPosVar(Point(150, 20));
		sublogo_particle->setStartSize(25);
		sublogo_particle->setEndSize(10);
		sublogo_particle->setOpacity(50);
		sublogo_particle->setDuration(-1);
		sublogo_particle->setStartColor(Color4F::BLUE);
		sublogo_particle->setEndColor(Color4F::RED);
		sublogo_particle->setPosition(size_window.width / 2, size_window.height / 2);
		sublogo_particle->setVisible(false);
		this->addChild(sublogo_particle);

		/* ����ΰ� ��������Ʈ ���� */
		sublogo_sprite = Sprite::create(SUBLOGOSPRITE_FILENAME);
		sublogo_sprite->setPosition(Point(size_window.width / 2, size_window.height / 2));
		sublogo_sprite->setVisible(false);
		this->addChild(sublogo_sprite);

	}
	
	
}

/* ��� ������ ���*/
void StartScene::background_tick(float interval) {
	/* �ؽ��ĸ� ������ ����� ��� �ٽ� ó������ ����*/
	if (background_iter == background_texture.end()) {
		background_iter = background_texture.begin();
	}
	background_sprite->setTexture(*background_iter++);
}

/* Press Any Key ���̾� ����*/
void StartScene::setLayerPressAnyKey() {

	/* ���̾� �ʱ�ȭ */
	layer_pressAnyKey = Layer::create();
	layer_pressAnyKey->setPosition(0, 0);
	layer_pressAnyKey->setContentSize(size_window);
	layer_pressAnyKey->setVisible(false);
	layer_pressAnyKey->setCascadeOpacityEnabled(true);
	this->addChild(layer_pressAnyKey);


	/* �� �ʱ�ȭ �� ���� */
	pressAnyKey_label = Label::createWithTTF(PRESSANYKEY_STRING, PRESSANYKEY_FONTS, PRESSANYKEY_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	pressAnyKey_label->setPosition(Point(size_window.width / 2, size_window.height / 16 * 5));
	pressAnyKey_label->setCascadeOpacityEnabled(true);
	layer_pressAnyKey->addChild(pressAnyKey_label);
	auto fo = FadeOut::create(0.2f);
	auto ft = FadeTo::create(0.2f, 150);
	auto dt = DelayTime::create(0.4f);
	auto seq = Sequence::create(fo, ft, dt, nullptr);
	auto rep = RepeatForever::create(seq);
	pressAnyKey_label->runAction(rep);

}

/* Login ȭ�� ���̾� ���� */
void StartScene::setLayerLogin() {

	/* ���̾� �ʱ�ȭ */
	layer_login = Layer::create();
	layer_login->setPosition(0, 0);
	layer_login->setContentSize(size_window);
	layer_login->setVisible(false);
	layer_login->setCascadeOpacityEnabled(true);
	this->addChild(layer_login);

	/* �α��� ���� �ʱ�ȭ */
	status_login = LOGINSTATUS::IDINPUT;

	/*
	
		��������Ʈ �ʱ�ȭ �� ����
		��������Ʈ�� ĳ�ÿ� �̸� �־���� Ű�� ���� �ҷ��´�
	
	*/

	/* ���̵� �Է� ��������Ʈ ���� - ���� �ʱ� ON ���� */
	cache->addImage(LOGINEDITON_FILENAME);
	cache->addImage(LOGINEDITOFF_FILENAME);
	login_sprite_id = Sprite::createWithTexture(cache->getTextureForKey(LOGINEDITON_FILENAME));
	login_sprite_id->setPosition(size_window.width / 2, size_window.height / 32 * 11 + 30);
	login_sprite_id->setCascadeOpacityEnabled(true);
	layer_login->addChild(login_sprite_id);

	/* ���̵� �Է� �ؽ�Ʈ�ʵ� ���� */
	login_textfield_id = TextFieldTTF::textFieldWithPlaceHolder("I D",
		login_sprite_id->getContentSize(), TextHAlignment::CENTER, LOGIN_FONTS, LOGIN_FONTSIZE);
	login_textfield_id->setPosition(login_sprite_id->getPosition());
	login_textfield_id->setColorSpaceHolder(Color3B::WHITE);
	login_textfield_id->setDelegate(this);
	login_textfield_id->attachWithIME();
	layer_login->addChild(login_textfield_id);

	/* �н����� �Է� ��������Ʈ */
	cache->addImage(LOGINEDITOFF_FILENAME);
	cache->addImage(LOGINEDITON_FILENAME);
	login_sprite_pw = Sprite::createWithTexture(cache->getTextureForKey(LOGINEDITOFF_FILENAME));
	login_sprite_pw->setPosition(size_window.width / 2, size_window.height / 32 * 11 - 30);
	login_sprite_pw->setCascadeOpacityEnabled(true);
	layer_login->addChild(login_sprite_pw);

	/* �н����� �Է� �ؽ�Ʈ�ʵ� ���� */
	login_textfield_pw = TextFieldTTF::textFieldWithPlaceHolder("PASSWORD",
		login_sprite_pw->getContentSize(), TextHAlignment::CENTER, LOGIN_FONTS, LOGIN_FONTSIZE);
	login_textfield_pw->setPosition(login_sprite_pw->getPosition());
	login_textfield_pw->setColorSpaceHolder(Color3B::WHITE);
	login_textfield_pw->setDelegate(this);
	login_textfield_pw->setSecureTextEntry(true);
	layer_login->addChild(login_textfield_pw);

	/* �α��� ��ư ��������Ʈ */
	cache->addImage(LOGINLOGINOFF_FILENAME);
	cache->addImage(LOGINLOGINON_FILENAME);
	login_sprite_login = Sprite::createWithTexture(cache->getTextureForKey(LOGINLOGINOFF_FILENAME));
	login_sprite_login->setPosition(size_window.width / 2 - 75, size_window.height / 32 * 11 - 90);
	login_sprite_login->setCascadeOpacityEnabled(true);
	layer_login->addChild(login_sprite_login);

	/* ȸ������ ��ư ��������Ʈ */
	cache->addImage(LOGINSIGNUPOFF_FILENAME);
	cache->addImage(LOGINSIGNUPON_FILENAME);
	login_sprite_signup = Sprite::createWithTexture(cache->getTextureForKey(LOGINSIGNUPOFF_FILENAME));
	login_sprite_signup->setPosition(size_window.width / 2 + 75, size_window.height / 32 * 11 - 90);
	login_sprite_signup->setCascadeOpacityEnabled(true);
	layer_login->addChild(login_sprite_signup);

	/* �α��� ���� ���� ��������Ʈ */
	cache->addImage(LOGINFAIL_FILENAME);
	login_sprite_fail = Sprite::createWithTexture(cache->getTextureForKey(LOGINFAIL_FILENAME));
	login_sprite_fail->setPosition(size_window.width / 2, size_window.height / 2);
	login_sprite_fail->setOpacity(0);
	layer_login->addChild(login_sprite_fail, 10);

}

/* ���� �α��� ���̾�� ��Ŀ�� ���¸� �����Ѵ� */
void StartScene::changeLoginStatus(int from, int to, bool isEnterKey) {

	/* 
	
		Enter Ű�� �����ߴµ� 3 �̳� 4�� to �� ���´ٸ� �� ����
		���� �α��� �����ϰų� ȸ������ ���̾�� �Ѿ�� �Ѵ�.
		�װ� ����� to �� ���� ������ ���� ���¸� ��ȯ.
	
	*/

	/* Ű �Է� ���� */
	_KEYUSING;

	CCLOG("changeLoginStatus %d - %d", from, to);

	/* �α��� ������ Ȯ���ϰ� ���� �޴� ���� */
	if (isEnterKey && from == LOGINSTATUS::LOGINBTN) {
		std::string id = login_textfield_id->getString();
		std::string pw = login_textfield_pw->getString();
		if (DBManager::getInstance()->checkLoginInfo(id, pw) == DBManager::NOTOKEN) {
			/* �α��� ���� ��ȿ���� ���� */
			SimpleAudioEngine::getInstance()->playEffect(SOUND_LOGINFAIL);
			status_login = LOGINSTATUS::LOGINFAIL;
			login_sprite_fail->setOpacity(255);
			_KEYRELEASE(0.2f);
			return;
		}
		enterMenuSelect();
		return;
	}

	/* ȸ������ ���̾� ���� */
	if (isEnterKey && from == LOGINSTATUS::SIGNUPBTN) {
		enterSignUp();
		_KEYRELEASE(0.2f);
		return;
	}

	/* ��� �ε��� - -1, 4 ó��*/
	if (to == LOGINSTATUS::PREVERR_LOGIN || to == LOGINSTATUS::NEXTERR_LOGIN) {
		_KEYRELEASE(0.2f);
		return;
	}

	/* ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGELOGIN);

	/* status ���� */
	status_login = to;

	/* ��������Ʈ ���� ȿ�� ����*/
	switch (from) {
	/* ���� ��Ŀ�� �ް� �ִ� ��������Ʈ ���� ���� */
	case LOGINSTATUS::IDINPUT:
		login_sprite_id->setTexture(cache->getTextureForKey(LOGINEDITOFF_FILENAME));
		login_textfield_id->detachWithIME();
		break;
	case LOGINSTATUS::PWINPUT:
		login_sprite_pw->setTexture(cache->getTextureForKey(LOGINEDITOFF_FILENAME));
		login_textfield_pw->detachWithIME();
		break;
	case LOGINSTATUS::LOGINBTN:
		login_sprite_login->setTexture(cache->getTextureForKey(LOGINLOGINOFF_FILENAME));
		break;
	case LOGINSTATUS::SIGNUPBTN:
		login_sprite_signup->setTexture(cache->getTextureForKey(LOGINSIGNUPOFF_FILENAME));
		break;
	default:
		break;
	}

	switch (to) {
	/* ���� ��Ŀ�� �ް� �ִ� ��������Ʈ ���� */
	case LOGINSTATUS::IDINPUT:
		login_sprite_id->setTexture(cache->getTextureForKey(LOGINEDITON_FILENAME));
		login_textfield_id->attachWithIME();
		break;
	case LOGINSTATUS::PWINPUT:
		login_sprite_pw->setTexture(cache->getTextureForKey(LOGINEDITON_FILENAME));
		login_textfield_pw->attachWithIME();
		break;
	case LOGINSTATUS::LOGINBTN:
		login_sprite_login->setTexture(cache->getTextureForKey(LOGINLOGINON_FILENAME));
		break;
	case LOGINSTATUS::SIGNUPBTN:
		login_sprite_signup->setTexture(cache->getTextureForKey(LOGINSIGNUPON_FILENAME));
		break;
	default:
		break;
	}

	/* Ű �Է� ���� ����*/
	_KEYRELEASE(0.2f);

}

/* �α��� ���� �˾����� �ٽ� �α��� ���·� ���ư��� */
void StartScene::returnFailToLoginStatus() {
	_KEYUSING;
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGELOGIN);
	status_login = LOGINSTATUS::LOGINBTN;
	login_sprite_fail->setOpacity(0);
	_KEYRELEASE(0.2f);
}

/* ȸ�� ���� ������ ���̾� ���� */
void StartScene::setLayerSignUp() {

}

/* �޴� ���� ���̾� ���� */
void StartScene::setLayerMenuSelect() {

	/*
	
		�޴� ���
		1. �̱��÷���
		2. ��Ƽ�÷���
		3. ����
		4. ������
		5. ������
	
	*/

	/* ���̾� �ʱ�ȭ */
	layer_menuSelect = Layer::create();
	layer_menuSelect->setPosition(0, 0);
	layer_menuSelect->setContentSize(size_window);
	layer_menuSelect->setVisible(false);
	layer_menuSelect->setCascadeOpacityEnabled(true);
	this->addChild(layer_menuSelect);

	/* �޴� ���� ���� �ʱ�ȭ */
	status_menuSelect = MENUSELECTSTATUS::SINGLEPLAY;

	/* 
	
		�� �ʱ�ȭ �� ����
		
	*/

	int i = 0;
	while (i < MENUSELECT_MENUSIZE) {
		menuSelect_label[i] = Label::createWithTTF(MENUSELECT_STR[i], MENUSELECT_FONTS, MENUSELECT_FONTSIZE,
			Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
		menuSelect_label[i]->setPosition(size_window.width / 2, size_window.height / 8 * 3 - (i * MENUSELECT_DIST));
		menuSelect_label[i]->setCascadeOpacityEnabled(true);
		menuSelect_label[i]->enableGlow(Color4B(0, 0, 0, 0));
		menuSelect_label[i]->setOpacity(100);
		layer_menuSelect->addChild(menuSelect_label[i]);
		i++;
	}

	/* ù �̱��÷��� ���õ� ���� �׼� ���� */
	menuSelect_label[MENUSELECTSTATUS::SINGLEPLAY]->enableGlow(MENUSELECT_FOCUSEDCOLOR);
	auto fi = FadeIn::create(0.1f);
	auto ft = FadeTo::create(0.3f, 100);
	auto dt = DelayTime::create(0.4f);
	auto seq = Sequence::create(fi, ft, dt, nullptr);
	auto rep = RepeatForever::create(seq);
	menuSelect_label[MENUSELECTSTATUS::SINGLEPLAY]->runAction(rep);

}

/* ���� �޴����� ���̾�� ��Ŀ�� ���¸� �����Ѵ� */
void StartScene::changeMenuSelectStatus(int from, int to, bool isEnterKey) {

	/*
	
		ENTER Ű�� ���������� from ���� ���� ���·� �����Ѵ�
		�װ��� �ƴ� ��� ���� ��ȯ ����
	
	*/

	/* Ű �Է� ���� */
	_KEYUSING;

	/* ����Ű�� ������ �� �� �����Լ��� �����Ѵ�. */
	if (isEnterKey) {
		switch (from) {
		case MENUSELECTSTATUS::SINGLEPLAY:
			/* �̱��÷��� �� ���� �Լ� ���� */
			goMusicSelectScene();
			break;
		case MENUSELECTSTATUS::MULTIPLAY:
			/* ��Ƽ�÷��� �� ���� �Լ� ���� */
			break;
		case MENUSELECTSTATUS::SETTING:
			/* ���� ���̾� ���� �Լ� ���� */
			break;
		case MENUSELECTSTATUS::EDITOR:
			/* ������ ���̾� ���� �Լ� ���� */
			break;
		case MENUSELECTSTATUS::EXIT:
			/* EXIT �Լ� ����*/
			break;
		default:
			break;
		}
		return;
	}

	/* ����Ű�� �ƴҶ� ���� ��� ��� */
	if (to == MENUSELECTSTATUS::SINGLEPLAY - 1 || to == MENUSELECTSTATUS::EXIT + 1) {
		_KEYRELEASE(0.1f);
		return;
	}

	/* ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGEMENUSELECT);

	/* ���� ���� �ٲ� */
	status_menuSelect = to;

	/* �� ��ȭ from ���� */
	menuSelect_label[from]->enableGlow(Color4B(0, 0, 0, 0));
	menuSelect_label[from]->stopAllActions();
	menuSelect_label[from]->setOpacity(100);

	/* �� ��ȭ to ���� */
	menuSelect_label[to]->enableGlow(MENUSELECT_FOCUSEDCOLOR);
	auto fi = FadeIn::create(0.1f);
	auto ft = FadeTo::create(0.3f, 100);
	auto dt = DelayTime::create(0.4f);
	auto seq = Sequence::create(fi, ft, dt, nullptr);
	auto rep = RepeatForever::create(seq);
	menuSelect_label[to]->runAction(rep);

	/* Ű ���� ���� */
	_KEYRELEASE(0.1f);

}

/* ������ ���̾� ���� */
void StartScene::setLayerEditor() {

}

/* ���� ���̾� ����*/
void StartScene::setLayerSettings() {

}

/* PressAnyKey ����*/
void StartScene::showPressAnyKey() {
	/*
	
		��� setup() ������ �Ϸ�ǰ��� call ��
		Loading Label�� �� ���� ���� showPressAnyKey layer�� ����ش�

		layer_status ����
		Ű �Է¹��� ���� - ���ݺ��� Ű �Է� ����
	
	*/

	/* ���� �� ���̾� ���� ����*/
	status_layer = STATUS::PRESSANYKEY;
	
	/* setVisible = true */
	sublogo_sprite->setVisible(true);
	sublogo_particle->setVisible(false);
	background_sprite->setVisible(true);
	layer_pressAnyKey->setVisible(true);
	layer_basic->setVisible(true);

	/* ����� �Բ� ������� ��� */
	this->scheduleOnce(schedule_selector(StartScene::startBGM), 5.0f);

	/* ��� �ִϸ��̼��� ������ Ű �Է� ���� */
	this->scheduleOnce(schedule_selector(StartScene::releaseKeyInput), 7.5f);

	/* ���� ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_ELECTIC, true);

	/* 
	
		���� �� �ִϸ��̼� ���� �� ��� 
		
	*/

	/* ���� ������ */
	auto fi = FadeIn::create(1.0f);
	auto dt4 = DelayTime::create(0.5f);
	auto ft = FadeTo::create(0.0f, 50);
	auto dt = DelayTime::create(0.6f);
	auto fi2 = FadeIn::create(0.0f);
	auto dt2 = DelayTime::create(0.3f);
	auto ft2 = FadeTo::create(0.0f, 50);
	auto dt7 = DelayTime::create(0.6f);
	auto fi3 = FadeIn::create(0.0f);
	auto seq = Sequence::create(fi, dt4, ft, dt, fi2, dt2, ft2, dt7, fi3, nullptr);
	sublogo_sprite->setOpacity(0);
	sublogo_sprite->runAction(seq);

	dt4 = DelayTime::create(7.5f);
	fi3 = FadeIn::create(2.0f);
	seq = Sequence::create(dt4, fi3, nullptr);
	layer_basic->setOpacity(0);
	layer_basic->runAction(seq);

	auto dt5 = DelayTime::create(6.0f);
	auto fi4 = FadeIn::create(2.0f);
	auto seq2 = Sequence::create(dt5, fi4, nullptr);
	sublogo_sprite->runAction(seq2);
	dt5 = DelayTime::create(6.0f);
	fi4 = FadeIn::create(2.0f);
	seq2 = Sequence::create(dt5, fi4, nullptr);
	background_sprite->setOpacity(0);
	background_sprite->runAction(seq2);

	auto dt6 = DelayTime::create(7.5f);
	fi4 = FadeIn::create(2.0f);
	auto seq3 = Sequence::create(dt6, fi4, nullptr);
	sublogo_particle->setOpacity(0);
	sublogo_particle->runAction(seq3);
	dt6 = DelayTime::create(7.5f);
	fi4 = FadeIn::create(2.0f);
	seq3 = Sequence::create(dt6, fi4, nullptr);
	layer_pressAnyKey->setOpacity(0);
	layer_pressAnyKey->runAction(seq3);

}

/* ���� BGM���*/
void StartScene::startBGM(float interval) {
	SimpleAudioEngine::getInstance()->stopAllEffects();
	AudioEngine::play2d(BGMMAIN_FILENAME, true, BGMMAIN_VOLUME);
}

/* Login ���̾� ���� */
void StartScene::enterLogin() {

	/* Ű �Է� ���� */
	_KEYUSING;

	/* ���� ���̾� ����(PressAnyKey) */
	layer_pressAnyKey->setVisible(false);

	/* ���� �� ���̾� ���� ����*/
	status_layer = STATUS::LOGIN;

	/* setVisible = true */
	layer_login->setVisible(true);

	/* ���� ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGELAYER);

	/* layer_login ���̱� */
	layer_login->setOpacity(0);
	auto fi = FadeIn::create(0.5f);
	layer_login->runAction(fi);

	/* ���� �ִϸ��̼� */




	/* �ִϸ��̼� ������ �ݵ�� Ű �����Ұ�. */
	_KEYRELEASE(0.2f);
}

/* �޴� ���� ���̾� ���� */
void StartScene::enterMenuSelect() {

	/* ���� �α��� ���̾� ���� */
	login_textfield_pw->setString("");
	layer_login->setVisible(false);

	/* ���� �� ���̾� ���� ����*/
	status_layer = STATUS::MENUSELECT;

	/* ���� ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGELAYER);

	/* setVisible = true */
	layer_menuSelect->setVisible(true);

	/* �޴� ���� �ִϸ��̼� */




	/* �ִϸ��̼� ������ �ݵ�� Ű �����Ұ�. */
	_KEYRELEASE(0.2f);

}

/* ���� ����� ���̾� ���� */
void StartScene::enterSignUp() {

}

void StartScene::goMusicSelectScene() {
	
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGELAYER);
	AudioEngine::stopAll();
	auto musicSelectScene = MusicSelectScene::createScene(background_texture);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, musicSelectScene));
}
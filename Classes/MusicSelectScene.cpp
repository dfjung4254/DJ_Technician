#include "MusicSelectScene.h"
#include "AppDelegate.h"
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"

/* Ű ���� �Է� ���� ��ũ�� */
#define _KEYUSING if(status_keyUsing){ return;} status_keyUsing = true
/* Ű ���� �Է� ���� ���� ��ũ�� */
#define _KEYRELEASE(num) this->scheduleOnce(schedule_selector(MusicSelectScene::releaseKeyInput), num)

USING_NS_CC;
using namespace experimental;
using namespace CocosDenshion;

std::vector<Texture2D*> tpbgv;

//scene ����
Scene* MusicSelectScene::createScene(std::vector<Texture2D*> v)
{
	tpbgv.assign(v.begin(), v.end());
	auto scene = Scene::create();
	auto layer = MusicSelectScene::create();
	scene->addChild(layer);
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

/* Ű ������ ���� �Լ� */
void MusicSelectScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()) {
		heldKeys.push_back(keyCode);
	}
}
void MusicSelectScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}
void MusicSelectScene::onKeyHold(float interval) {

	/*

		���Ϳ� ���� Ű�Է��� ���δ� ���� Ű �Է� �� �� �Լ���
		���� Ű �Է��� �ʿ����� ���� ���� status_keyUsing �� true ó�����ְ�
		�Լ��� ������ �������� releaseKeyInput() ���־�� �Ѵ�.

	*/

	/* ����Ű UP �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_UP_ARROW) != heldKeys.end()) {

		if (status_layer == STATUS::MUSICSELECT) {
			/* �� ���� �ε��� ���� ������ŭ ���� */
			changeMusicSelect(status_musicSelect, status_musicSelect - widthCount);
		}

	}

	/* ����Ű DOWN �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_DOWN_ARROW) != heldKeys.end()) {

		if (status_layer == STATUS::MUSICSELECT) {
			/* �� ���� �ε��� ���� ������ŭ ���� */
			changeMusicSelect(status_musicSelect, status_musicSelect + widthCount);
		}

	}

	/* ����Ű RIGHT �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_RIGHT_ARROW) != heldKeys.end()) {

		if (status_layer == STATUS::MUSICSELECT) {
			/* �� ���� �ε��� ���� */
			changeMusicSelect(status_musicSelect, status_musicSelect + 1);
		}

	}

	/* ����Ű LEFT �Է�*/
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_LEFT_ARROW) != heldKeys.end()) {

		if (status_layer == STATUS::MUSICSELECT) {
			/* �� ���� �ε��� ���� */
			changeMusicSelect(status_musicSelect, status_musicSelect - 1);
		}

	}

	/* ENTER Ű �Է� */
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_ENTER) != heldKeys.end()) {

		if (status_layer == STATUS::MUSICSELECT) {
			/* �� �󼼺��� ���̾� ���� */
			showMusicInfo();
		}

		if (status_layer == STATUS::MUSICINFO) {
			/* ���� ������ ���� ���� */
			startGame();
		}

	}

	/* ESC Ű �Է� */
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_ESCAPE) != heldKeys.end()) {

		if (status_layer == STATUS::MUSICINFO) {
			/* �� �󼼺��� ��� */
			hideMusicInfo();
		}

	}

	/* BACKSPACE Ű �Է� */
	if (std::find(heldKeys.begin(), heldKeys.end(), EventKeyboard::KeyCode::KEY_BACKSPACE) != heldKeys.end()) {

		if (status_layer == STATUS::MUSICINFO) {
			/* �� �󼼺��� ��� */
			hideMusicInfo();
		}

	}
}

void MusicSelectScene::releaseKeyInput(float interval) {
	status_keyUsing = false;
}

/*

	���콺 ������ ���� �Լ�

*/

/* ���콺 Ŭ�� �� */
void MusicSelectScene::onMouseDown(cocos2d::Event* event) {

}

/* ���콺 Ŭ�� �� ������ */
void MusicSelectScene::onMouseUp(cocos2d::Event* event) {

}

/* ���콺 �̵� �� - ���콺 ������ ���� */
void MusicSelectScene::onMouseMove(cocos2d::Event* event) {
	/*

		���콺 �̵��� �����ؼ� ���콺��������Ʈ�� ��ġ�� ��� �ٲپ���

	*/
	auto mouseEvent = static_cast<EventMouse*>(event);
	Vec2 movePoint = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
	cursor_sprite->setPosition(movePoint);
}

/* ���콺 ��ũ�� �� */
void MusicSelectScene::onMouseScroll(cocos2d::Event* event) {

}

/* Scene�� ���� init() �Լ� ���� */
bool MusicSelectScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

/* �� ��ȯ �� �ε� ���� */
void MusicSelectScene::onEnterTransitionDidFinish() {

	setup();

}

void MusicSelectScene::setup() {

	/* ������� �ʱ�ȭ */
	size_window = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	cache = Director::getInstance()->getTextureCache();
	status_keyUsing = true;
	heldKeys.clear();
	background_texture.assign(tpbgv.begin(), tpbgv.end());

	/*

		BMS ����Ʈ �о�� �� �����带 ����ϸ� �ε�ȭ���� �����ְ� �ִ´�.
		������ �ݹ��� ���� setBmsList() �� �Ϸ�Ǹ� �ݹ��� ���� �����ϰ�
		�ݹ��Լ����� ������ ������ �����Ѵ�.

	*/

	/* �ε� ȭ�� �����ֱ� */
	showLoading();

	/* BMS ��� ���� ���� �� �ҷ����� */
	setBmsList();

	/* Ű �� ���콺, ��ġ �Է� ���� */
	enableKeySetting();

	/* �� ���̾� ���� */
	setLayerMusicSelect();
	setLayerSubBar();
	setLayerMusicInfo();

	/* �����ϸ� ������ */
	showMusicSelect();

}

/* BMS ������ �о�鿩 ���Ϳ� �����Ѵ�. */
void MusicSelectScene::setBmsList() {

	//auto parser = BMSParser::getInstance();

	/* ������ ����ؼ� �ε� */
	std::thread loading(BMSParser::readAllBmsHeader, &musicHeaders);

	/* �ļ����� ��� ������ ���� ������� �о�� */
	//BMSParser::getInstance()->readAllBmsHeader(&musicHeaders);

	/* ������ �շ��� */
	loading.join();

#ifdef _DEBUG
	/* ���� ���� üũ */
	CCLOG("################### FILE READ CHECK ####################");
	CCLOG("########################################################");
	for (MusicHeader mh : musicHeaders) {
		CCLOG("------------- /bms/%s/%s INFORMATION --------------", mh.getValues(MusicHeader::DIR).c_str(), mh.getValues(MusicHeader::FILENAME).c_str());
		CCLOG("PLAYER    : %s", mh.getValues(MusicHeader::PLAYER).c_str());
		CCLOG("GENRE     : %s", mh.getValues(MusicHeader::GENRE).c_str());
		CCLOG("TITLE     : %s", mh.getValues(MusicHeader::TITLE).c_str());
		CCLOG("ARTIST    : %s", mh.getValues(MusicHeader::ARTIST).c_str());
		CCLOG("BPM       : %s", mh.getValues(MusicHeader::BPM).c_str());
		CCLOG("PLAYLEVEL : %s", mh.getValues(MusicHeader::PLAYLEVEL).c_str());
		CCLOG("RANK      : %s", mh.getValues(MusicHeader::RANK).c_str());
		CCLOG("SUBTITLE  : %s", mh.getValues(MusicHeader::SUBTITLE).c_str());
		CCLOG("TOTAL     : %s", mh.getValues(MusicHeader::TOTAL).c_str());
		CCLOG("STAGEFILE : %s", mh.getValues(MusicHeader::STAGEFILE).c_str());
	}
	CCLOG("################### FILE READ FINISHED##################");
	CCLOG("########################################################");
#endif



}

/* Ű �Է� �� ���콺 ���� */
void MusicSelectScene::enableKeySetting() {

	/* Ű �Է� ���� */
	this->schedule(schedule_selector(MusicSelectScene::onKeyHold));
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(MusicSelectScene::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(MusicSelectScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

	/* Ŀ�� �̹��� ����*/
	cursor_sprite = Sprite::create(CURSOR_FILENAME);
	this->addChild(cursor_sprite, 10);

	/* ���콺 �Է� ���� */
	auto Mouse = EventListenerMouse::create();
	Mouse->onMouseDown = CC_CALLBACK_1(MusicSelectScene::onMouseDown, this);
	Mouse->onMouseUp = CC_CALLBACK_1(MusicSelectScene::onMouseUp, this);
	Mouse->onMouseMove = CC_CALLBACK_1(MusicSelectScene::onMouseMove, this);
	Mouse->onMouseScroll = CC_CALLBACK_1(MusicSelectScene::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Mouse, this);

}

/* �⺻ ���̾� ����*/
void MusicSelectScene::setLayerBasicLayer() {
	/*

			StartScene ���� �ؽ��ĺ��͸� �Ѱܹ޾� �޸𸮿� �ε�.
			�ؽ��ĺ��͸� �� �����Ӵ� ��ȸ�ϸ� ��ü�����ν� �������

			10~20�� ������ ���� ������ �� ����� �ξ� ȿ����!
			�׷��� 1�гѾ�� ������ �ѹ��� �̹����ε��ϸ� ��ǻ�Ͱ� �״´�

			������ 10~20�� ���� �������� ����� ��.

	*/

	/* ��� ��������Ʈ �ʱ�ȭ �� ���� */
	background_sprite = Sprite::create();
	background_iter = background_texture.begin();
	background_sprite->initWithTexture(*background_iter);
	background_sprite->setContentSize(size_window);
	background_sprite->setPosition(size_window.width / 2, size_window.height / 2);
	background_sprite->setVisible(false);

	/* �� ƽ�� ������ �Լ� ȣ���ؼ� ���� texture ���� */
	this->schedule(schedule_selector(MusicSelectScene::background_tick));

	/* �⺻ ���̾ ��� ��������Ʈ ���*/
	this->addChild(background_sprite);

}

/* ��� ������ ���*/
void MusicSelectScene::background_tick(float interval) {
	/* �ؽ��ĸ� ������ ����� ��� �ٽ� ó������ ����*/
	if (background_iter == background_texture.end()) {
		background_iter = background_texture.begin();
	}
	background_sprite->setTexture(*background_iter++);
}

/* �ε� ȭ�� �����ֱ� */
void MusicSelectScene::showLoading() {

	/* ��� ���̾� ���� */
	setLayerBasicLayer();

	/* ��� ��������Ʈ �� */
	background_sprite->setVisible(true);
	background_sprite->setOpacity(100);

	/* �ε� �� ���� */
	loading_label = Label::createWithTTF(LOADING_STR, LOADING_FONT, LOADING_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	loading_label->setPosition(size_window.width / 2, size_window.height / 2);
	this->addChild(loading_label);
	auto fi = FadeIn::create(0.1f);
	auto ft = FadeTo::create(0.3f, 100);
	auto dt = DelayTime::create(0.4f);
	auto seq = Sequence::create(fi, ft, dt, nullptr);
	auto rep = RepeatForever::create(seq);
	loading_label->runAction(rep);

}

/* �� ���� ���̾� ����*/
void MusicSelectScene::setLayerMusicSelect() {

	/* ���̾� �ʱ�ȭ */
	layer_musicSelect = Layer::create();
	layer_musicSelect->setContentSize(size_window);
	layer_musicSelect->setPosition(0, 0);
	layer_musicSelect->setVisible(false);
	this->addChild(layer_musicSelect);

	/* ���ο� �� �� ���� object ���� */
	widthCount = layer_musicSelect->getContentSize().width / MSO_SPRITE_WIDTH;


	/* �� �ʱ�ȭ �� ���� */
	auto iter = musicHeaders.begin();
	int idx = 0;
	while (iter != musicHeaders.end()) {
		/*

			��� ��������� ���� stage ���� �ؽ��ĸ� �ε� �� �����ϰ�
			�� Ÿ��Ʋ�� ��ŷ������ �ʱ�ȭ�Ѵ�.

			�̶� ���� ��������Ʈ�� �ڽĳ��.

		*/

#ifdef _DEBUG
		CCLOG("========= this MusicHeaders (%d) make to SelectObject .. ==========", idx);
#endif

		/* ���� ��������Ʈ�� row �� col �� ��ġ ��� */
		int col = idx % widthCount;
		int row = idx / widthCount;
		int widthPos = (layer_musicSelect->getContentSize().width / widthCount + MSO_SPRITE_WIDTHDIFF)
			* col - MSO_SPRITE_WIDTH / 2;
		int heightPos = -(MSO_SPRITE_HEIGHT - MSO_LABEL_LEVELPOSH - MSO_LABEL_TITLEPOSH) * row + size_window.height;

		/* ���� ��������� ���� Node��ü ���� */
		MusicHeader curHeader = *iter;


		/* ��������Ʈ ��� ���� */
		std::string spritePath;
		spritePath = "bms/" + curHeader.getValues(MusicHeader::DIR)
			+ "/" + curHeader.getValues(MusicHeader::STAGEFILE);

		/* ��������Ʈ ��� ��ȿ�� �˻� */
		struct stat buffer;
		if (curHeader.getValues(MusicHeader::STAGEFILE).compare("") == 0
			|| curHeader.getValues(MusicHeader::STAGEFILE).find(".bmp", 0, 4) != std::string::npos
			|| stat(spritePath.c_str(), &buffer) != 0) {
			/* �������� ������ �������� ���� �� || �������� ������ ��Ʈ�� ������ �� */
			/*

				���� �̹��� ���� �ε��� �� �� �κп��� �ؽ��ĸ� �� �д� ���� �˻��ϰ� ����ó���Ѵ�.

				 1. BMS ���� ���ο� STAGEFILE ����� �������� �ʴ� ���
				 2. STAGEFILE �� Ȯ���ڰ� ��Ʈ���϶� (cocos2dx �� ��Ʈ�� ���� x - ���߿� ��Ʈ�� ��ȯ �����غ� ��)
				 3. STAGEFILE �� ���������� ���� ���� ��ο� �ش��ϴ� ������ �������� �ʴ� ���.

			*/
			spritePath = MSO_NOIMAGEFILE;	// �ӽ�
		}

#ifdef _DEBUG
		CCLOG("stagefilePath : %s", spritePath.c_str());
#endif

		/* Batch Drawing ���� �� */
		SpriteBatchNode *batchNode = SpriteBatchNode::create(spritePath);
		this->addChild(batchNode);
		auto tpSprite = Sprite::createWithTexture(batchNode->getTexture());

		/* �Ϲ� ��������Ʈ ���� �Ҷ� */
		//cache->addImage(spritePath);
		//auto tpSprite = Sprite::createWithTexture(cache->getTextureForKey(spritePath));

		tpSprite->setContentSize(Size(MSO_SPRITE_WIDTH, MSO_SPRITE_HEIGHT));
		tpSprite->setPosition(widthPos, heightPos);
		tpSprite->setOpacity(MSO_OPACITY_UNSELECTED);
		layer_musicSelect->addChild(tpSprite);
		auto tpLabelTitle = Label::createWithTTF("TITLE : " + curHeader.getValues(MusicHeader::TITLE) + curHeader.getValues(MusicHeader::SUBTITLE),
			MSO_FONT, MSO_FONTSIZE, Size(MSO_LABEL_WIDTH, MSO_LABEL_HEIGHT), TextHAlignment::LEFT, TextVAlignment::TOP);
		tpLabelTitle->setPosition(Point(tpSprite->getContentSize().width / 2, MSO_LABEL_TITLEPOSH));
		tpLabelTitle->setOpacity(MSO_OPACITY_UNSELECTED);
		tpSprite->addChild(tpLabelTitle);
		auto tpLabelLevel = Label::createWithTTF("LEVEL : " + curHeader.getValues(MusicHeader::PLAYLEVEL),
			MSO_FONT, MSO_FONTSIZE, Size(MSO_LABEL_WIDTH, MSO_LABEL_HEIGHT), TextHAlignment::LEFT, TextVAlignment::TOP);
		tpLabelLevel->setPosition(Point(tpSprite->getContentSize().width / 2, MSO_LABEL_LEVELPOSH));
		tpLabelLevel->setOpacity(MSO_OPACITY_UNSELECTED);
		tpSprite->addChild(tpLabelLevel);

		/* �� ���� ��ü ��� */
		musicSelectObject.push_back(
			{
				/* stagefile_sprite, label_title, label_playerLevel */
				tpSprite, tpLabelTitle, tpLabelLevel
			}
		);

		/* ���� ������� Ž�� */
		iter++;
		idx++;
	}

}

/* ���� �� �ϴܹ� ���̾� ���� */
void MusicSelectScene::setLayerSubBar() {

	/* ���̾� �ʱ�ȭ */
	layer_subBar = Layer::create();
	layer_subBar->setContentSize(size_window);
	layer_subBar->setPosition(0, 0);
	layer_subBar->setVisible(false);
	this->addChild(layer_subBar);

	/* ��ܹ� ��������Ʈ ���� */
	cache->addImage(SUBBAR_TOPFILE);
	subBarTop_sprite = Sprite::createWithTexture(cache->getTextureForKey(SUBBAR_TOPFILE));
	subBarTop_sprite->setPosition(size_window.width / 2, size_window.height - subBarTop_sprite->getContentSize().height / 2);
	layer_subBar->addChild(subBarTop_sprite);
	auto fi = FadeIn::create(0.1f);
	auto ft = FadeTo::create(0.3f, 100);
	auto dt = DelayTime::create(0.6f);
	auto seq = Sequence::create(fi, ft, dt, nullptr);
	auto rep = RepeatForever::create(seq);
	subBarTop_sprite->runAction(rep);

	/* �ϴܹ� ��������Ʈ ���� */
	cache->addImage(SUBBAR_BTMFILE);
	subBarBtm_sprite = Sprite::createWithTexture(cache->getTextureForKey(SUBBAR_BTMFILE));
	subBarBtm_sprite->setPosition(size_window.width / 2, subBarBtm_sprite->getContentSize().height / 2);
	layer_subBar->addChild(subBarBtm_sprite);
	fi = FadeIn::create(0.1f);
	ft = FadeTo::create(0.3f, 100);
	dt = DelayTime::create(0.6f);
	seq = Sequence::create(fi, ft, dt, nullptr);
	rep = RepeatForever::create(seq);
	subBarBtm_sprite->runAction(rep);

	/* ����� ���� �ε��� �� ���� */
	subBar_label_curIdx = Label::createWithTTF("0", SUBBAR_INDEX_FONT, SUBBAR_INDEX_FONTSIZE,
		Size(100, subBarTop_sprite->getContentSize().height), TextHAlignment::RIGHT, TextVAlignment::CENTER);
	subBar_label_curIdx->setPosition(45, subBarTop_sprite->getContentSize().height / 2);
	subBarTop_sprite->addChild(subBar_label_curIdx);

	/* ����� ��ü �ε��� �� ���� */
	subBar_label_maxIdx = Label::createWithTTF("/ " + std::to_string(musicHeaders.size()),
		SUBBAR_INDEX_FONT, SUBBAR_INDEX_FONTSIZE, Size(100, subBarTop_sprite->getContentSize().height),
		TextHAlignment::LEFT, TextVAlignment::CENTER);
	subBar_label_maxIdx->setPosition(100 + 50, subBarTop_sprite->getContentSize().height / 2);
	subBarTop_sprite->addChild(subBar_label_maxIdx);

	/* ����� Ÿ��Ʋ �� ���� */
	subBar_label_title = Label::create("", SUBBAR_TITLE_FONT, SUBBAR_TITLE_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	subBar_label_title->setPosition(subBarTop_sprite->getContentSize().width / 2, subBarTop_sprite->getContentSize().height / 2);
	subBarTop_sprite->addChild(subBar_label_title);

}

/* �� ������ ���̾� ���� */
void MusicSelectScene::setLayerMusicInfo() {

	/* ���̾� �ʱ�ȭ */
	layer_musicInfo = Layer::create();
	layer_musicInfo->setContentSize(size_window);
	layer_musicInfo->setPosition(0, 0);
	layer_musicInfo->setVisible(false);
	this->addChild(layer_musicInfo);

	/* �麸�� ��������Ʈ ���� */
	cache->addImage(MUSICINFO_BACKBOARDFILE);
	musicInfo_sprite_backBoard = Sprite::createWithTexture(cache->getTextureForKey(MUSICINFO_BACKBOARDFILE));
	musicInfo_sprite_backBoard->setPosition(size_window.width / 4 * 3, size_window.height / 2);
	layer_musicInfo->addChild(musicInfo_sprite_backBoard);
	auto fi = FadeIn::create(0.1f);
	auto ft = FadeTo::create(0.3f, 200);
	auto dt = DelayTime::create(0.6f);
	auto seq = Sequence::create(fi, ft, dt, nullptr);
	auto rep = RepeatForever::create(seq);
	musicInfo_sprite_backBoard->runAction(rep);

#pragma region PLAYER �� ����
	/* �麸�� PLAYER ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::PLAYER] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::PLAYER],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::PLAYER]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_sprite_backBoard->getContentSize().height - 60);
	musicInfo_label_ui[MUSICINFO_LABEL::PLAYER]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::PLAYER]);

	musicInfo_label_info[MUSICINFO_LABEL::PLAYER] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::PLAYER]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::PLAYER]->getPositionY()
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::PLAYER]);
#pragma endregion

#pragma region GENRE ����
	/* �麸�� GENRE ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::GENRE] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::GENRE],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::GENRE]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_label_info[MUSICINFO_LABEL::PLAYER]->getPositionY() - 40
	);
	musicInfo_label_ui[MUSICINFO_LABEL::GENRE]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::GENRE]);

	musicInfo_label_info[MUSICINFO_LABEL::GENRE] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::GENRE]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::GENRE]->getPositionY() - 40
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::GENRE]);
#pragma endregion

#pragma region TITLE ����
	/* �麸�� TITLE ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::TITLE] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::TITLE],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::TITLE]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_label_info[MUSICINFO_LABEL::GENRE]->getPositionY() - 40
	);
	musicInfo_label_ui[MUSICINFO_LABEL::TITLE]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::TITLE]);

	musicInfo_label_info[MUSICINFO_LABEL::TITLE] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::TITLE]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::TITLE]->getPositionY() - 40
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::TITLE]);
#pragma endregion

#pragma region ARTIST ����
	/* �麸�� ARTIST ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::ARTIST] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::ARTIST],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::ARTIST]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_label_info[MUSICINFO_LABEL::TITLE]->getPositionY() - 40
	);
	musicInfo_label_ui[MUSICINFO_LABEL::ARTIST]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::ARTIST]);

	musicInfo_label_info[MUSICINFO_LABEL::ARTIST] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::ARTIST]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::ARTIST]->getPositionY() - 40
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::ARTIST]);
#pragma endregion

#pragma region BPM ����
	/* �麸�� ARTIST ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::BPM] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::BPM],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::BPM]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_label_info[MUSICINFO_LABEL::ARTIST]->getPositionY() - 40
	);
	musicInfo_label_ui[MUSICINFO_LABEL::BPM]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::BPM]);

	musicInfo_label_info[MUSICINFO_LABEL::BPM] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE + 50,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::BPM]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::BPM]->getPositionY() - 40
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::BPM]);
#pragma endregion

#pragma region PLAYLEVEL ����
	/* �麸�� PLAYLEVEL ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::PLAYLEVEL] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::PLAYLEVEL],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::PLAYLEVEL]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_label_info[MUSICINFO_LABEL::BPM]->getPositionY() - 50
	);
	musicInfo_label_ui[MUSICINFO_LABEL::PLAYLEVEL]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::PLAYLEVEL]);

	musicInfo_label_info[MUSICINFO_LABEL::PLAYLEVEL] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::PLAYLEVEL]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::PLAYLEVEL]->getPositionY() - 40
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::PLAYLEVEL]);
#pragma endregion

#pragma region RANK ����
	/* �麸�� RANK ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::RANK] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::RANK],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::RANK]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_label_info[MUSICINFO_LABEL::PLAYLEVEL]->getPositionY() - 40
	);
	musicInfo_label_ui[MUSICINFO_LABEL::RANK]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::RANK]);

	musicInfo_label_info[MUSICINFO_LABEL::RANK] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::RANK]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::RANK]->getPositionY()
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::RANK]);
#pragma endregion

#pragma region TOTAL ����
	/* �麸�� TOTAL ���� */
	musicInfo_label_ui[MUSICINFO_LABEL::TOTAL] = Label::createWithTTF(
		MUSICINFO_LABELSTR[MUSICINFO_LABEL::TOTAL],
		MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(100, 30), TextHAlignment::LEFT, TextVAlignment::CENTER
	);
	musicInfo_label_ui[MUSICINFO_LABEL::TOTAL]->setPosition(
		MUSICINFO_LEFTPADDING,
		musicInfo_label_info[MUSICINFO_LABEL::RANK]->getPositionY() - 40
	);
	musicInfo_label_ui[MUSICINFO_LABEL::TOTAL]->setOpacity(150);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_ui[MUSICINFO_LABEL::TOTAL]);

	musicInfo_label_info[MUSICINFO_LABEL::TOTAL] = Label::createWithTTF(
		"", MUSICINFO_FONT, MUSICINFO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER
	);
	musicInfo_label_info[MUSICINFO_LABEL::TOTAL]->setPosition(
		(musicInfo_sprite_backBoard->getContentSize().width) / 2,
		musicInfo_label_ui[MUSICINFO_LABEL::TOTAL]->getPositionY()
	);
	musicInfo_sprite_backBoard->addChild(musicInfo_label_info[MUSICINFO_LABEL::TOTAL]);
#pragma endregion

}

/* �� ���� ���̾� ����*/
void MusicSelectScene::showMusicSelect() {
	/*

		��� setup() ������ �Ϸ�ǰ��� call ��
		Loading Label�� �� ���� ���� showPressAnyKey layer�� ����ش�

		layer_status ����
		Ű �Է¹��� ���� - ���ݺ��� Ű �Է� ����

	*/

	/* ����� ��� */
	startBGM(0.0f);

	/* ���� �� ���̾� ���� ����*/
	status_layer = STATUS::MUSICSELECT;

	/* �ε� �� ���� */
	loading_label->setVisible(false);

	/* setVisible = true */
	layer_musicSelect->setVisible(true);
	layer_subBar->setVisible(true);

	/* ���� ���õ� ��(0�� �ε���)�� ���� ���̾� ��ġ ���� */
	int col = status_musicSelect % widthCount;
	int row = status_musicSelect / widthCount;
	int widthStatic = size_window.width / 2 + (MSO_SPRITE_WIDTHDIFF + MSO_SPRITE_WIDTH) / 2;
	int widthDiff = (layer_musicSelect->getContentSize().width / widthCount + MSO_SPRITE_WIDTHDIFF);
	int heightStatic = -size_window.height / 2 + MSO_SPRITE_WIDTHDIFF;
	int heightDiff = (MSO_SPRITE_HEIGHT - MSO_LABEL_LEVELPOSH - MSO_LABEL_TITLEPOSH);

	layer_musicSelect->setPosition(Point(widthStatic - (col * widthDiff)
		, heightStatic + (row * heightDiff)));

	musicSelectObject[status_musicSelect].stagefile_sprite->setOpacity(255);
	musicSelectObject[status_musicSelect].title_label->setOpacity(255);
	musicSelectObject[status_musicSelect].rank_label->setOpacity(255);

	/* ��� �ִϸ��̼��� ������ Ű �Է� ���� */
	_KEYRELEASE(0.2f);

	/* ���� ȿ���� ��� */

	/*

		���� �� �ִϸ��̼� ���� �� ���

	*/



}

/* �� ������ ���̾� ���� */
void MusicSelectScene::showMusicInfo() {
	/*

		���� status_musicSelect �� �´� �� ������ �ҷ���
		musicInfo_label_info[] �� ��Ʈ���� �ٲپ��ش�.

	*/

	/* Ű �Է� ��� */
	_KEYUSING;

	/* ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_SELECT);

	/* ���� �� ���̾� ���� ����*/
	status_layer = STATUS::MUSICINFO;

	/* ���� ���� �� ��������Ʈ Ȯ�� �̵� �ִϸ��̼� */
	auto sb = ScaleBy::create(0.15f, 2.0f);
	auto mov = MoveBy::create(0.15f, Point(-size_window.width / 4, 0));
	auto spawn = Spawn::create(sb, mov, nullptr);
	musicSelectObject[status_musicSelect].stagefile_sprite->runAction(spawn);

	auto dt = DelayTime::create(0.15f);
	auto fi = FadeIn::create(0.1f);
	auto cb = CallFunc::create(CC_CALLBACK_0(MusicSelectScene::releaseKeyInput, this, 0.0f));
	auto seq = Sequence::create(dt, fi, cb, nullptr);
	musicInfo_sprite_backBoard->setOpacity(0);
	musicInfo_sprite_backBoard->runAction(seq);

	/* setVisible = true */
	layer_musicInfo->setVisible(true);

	/* �� �������� �� �Է� */
	musicInfo_label_info[MUSICINFO_LABEL::PLAYER]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::PLAYER)
	);
	musicInfo_label_info[MUSICINFO_LABEL::GENRE]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::GENRE)
	);
	musicInfo_label_info[MUSICINFO_LABEL::TITLE]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::TITLE)
	);
	musicInfo_label_info[MUSICINFO_LABEL::ARTIST]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::ARTIST)
	);
	musicInfo_label_info[MUSICINFO_LABEL::BPM]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::BPM)
	);
	musicInfo_label_info[MUSICINFO_LABEL::PLAYLEVEL]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::PLAYLEVEL)
	);
	musicInfo_label_info[MUSICINFO_LABEL::RANK]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::RANK)
	);
	musicInfo_label_info[MUSICINFO_LABEL::TOTAL]->setString(
		musicHeaders[status_musicSelect].getValues(MusicHeader::TOTAL)
	);

	/* ��� ��ȯ �ִϸ��̼��� �����ϰ� �Ŀ� �ݹ����� Ű ���� */
	//_KEYRELEASE(0.2f);

}

/* �� ������ ���̾� ������*/
void MusicSelectScene::hideMusicInfo() {

	/* Ű �Է� ��� */
	_KEYUSING;

	/* ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_BACK);

	/* ���� �� ���̾� ���� ����*/
	status_layer = STATUS::MUSICSELECT;

	/* ���� ���� �� ��������Ʈ ��� ���� �ִϸ��̼� */
	auto sb = ScaleBy::create(0.2f, 0.5f);
	auto mov = MoveBy::create(0.2f, Point(size_window.width / 4, 0));
	auto spawn = Spawn::create(sb, mov, nullptr);
	auto cb = CallFunc::create(CC_CALLBACK_0(MusicSelectScene::releaseKeyInput, this, 0.0f));
	auto seq = Sequence::create(spawn, cb, nullptr);
	musicSelectObject[status_musicSelect].stagefile_sprite->runAction(seq);

	/* setVisible = true */
	layer_musicInfo->setVisible(false);

	/* ��� ��ȯ �ִϸ��̼��� �����ϰ� �Ŀ� �ݹ����� Ű ���� */
	//_KEYRELEASE(0.2f);

}

/* �� ���� ���� */
void MusicSelectScene::changeMusicSelect(int from, int to) {

	/* Ű ���� */
	_KEYUSING;

	/* �ε��� ����°�� ó�� */
	if (to >= musicSelectObject.size() || to < 0) {
		/* ����Ұ� ���� ��� */
		SimpleAudioEngine::getInstance()->playEffect(SOUND_TICK);

		/* Ű ���� ���� */
		_KEYRELEASE(0.1f);
		return;

	}

	/* ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGEMUSICSELECT);

	/* ���� �ε��� ���� */
	status_musicSelect = to;

	/* ����� �ε��� �� Ÿ��Ʋ �� ���� */
	subBar_label_curIdx->setString(std::to_string(status_musicSelect));
	subBar_label_title->setString(musicHeaders[status_musicSelect].getValues(MusicHeader::TITLE));
	subBar_label_title->setOpacity(0);
	auto fi = FadeIn::create(MSO_ACTIONTIME_CHANGE);
	subBar_label_title->runAction(fi);

	/* �ε��� ���濡 ���� ���̾� ��ġ ���� */
	int col = status_musicSelect % widthCount;
	int row = status_musicSelect / widthCount;
	int widthStatic = size_window.width / 2 + (MSO_SPRITE_WIDTHDIFF + MSO_SPRITE_WIDTH) / 2;
	int widthDiff = (layer_musicSelect->getContentSize().width / widthCount + MSO_SPRITE_WIDTHDIFF);
	int heightStatic = -size_window.height / 2 + MSO_SPRITE_WIDTHDIFF;
	int heightDiff = (MSO_SPRITE_HEIGHT - MSO_LABEL_LEVELPOSH - MSO_LABEL_TITLEPOSH);

	auto mt = MoveTo::create(MSO_ACTIONTIME_CHANGE, Point(widthStatic - (col * widthDiff)
		, heightStatic + (row * heightDiff)));
	auto el = EaseElasticOut::create(mt);
	auto cb = CallFunc::create(CC_CALLBACK_0(MusicSelectScene::releaseKeyInput, this, 0.0f));
	auto seq = Sequence::create(el, cb, nullptr);

	layer_musicSelect->runAction(seq);

	/* ���õ� ������Ʈ ���� */
	musicSelectObject[status_musicSelect].stagefile_sprite->setOpacity(255);
	musicSelectObject[status_musicSelect].title_label->setOpacity(255);
	musicSelectObject[status_musicSelect].rank_label->setOpacity(255);

	/* ������ ������Ʈ ���� */
	musicSelectObject[from].stagefile_sprite->setOpacity(MSO_OPACITY_UNSELECTED);
	musicSelectObject[from].title_label->setOpacity(MSO_OPACITY_UNSELECTED);
	musicSelectObject[from].rank_label->setOpacity(MSO_OPACITY_UNSELECTED);

	/* Ű ��� ���� */
}

/* ���� ���� */
void MusicSelectScene::startGame() {

	_KEYUSING;

	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGELAYER);
	AudioEngine::stopAll();
	auto musicSelectScene = GameScene::createScene(
		musicHeaders[status_musicSelect].getValues(MusicHeader::DIR),
		musicHeaders[status_musicSelect].getValues(MusicHeader::FILENAME),
		1.0f,
		true
	);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, musicSelectScene));

}

/* ���� BGM���*/
void MusicSelectScene::startBGM(float interval) {
	AudioEngine::play2d(BGMMAIN_FILENAME, true, BGMMAIN_VOLUME);
}

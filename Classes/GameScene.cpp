#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "AppDelegate.h"

#include <sys/stat.h>
#include <algorithm>
#include <chrono>
#include <fmod.hpp>

#define KEY_USINGSET if (status_keyUsing) return; status_keyUsing = true

USING_NS_CC;
using namespace CocosDenshion;
using namespace experimental;

std::string mn; std::string bn;  float sp; bool ap;

//scene ����
Scene* GameScene::createScene(std::string dirs, std::string bmsName, float speed, bool autoPlay)
{
	mn = dirs; sp = speed; bn = bmsName; ap = autoPlay;
	CCLOG("GameScene::createScene()..");
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	CCLOG("GameScene::createScene()..finished!");
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

//key Listener
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_S:
		CCLOG("keyS : Pressed");
		if (status_autoPlay) { return; }
		operateKeyEffect(KEY::S);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		CCLOG("keyD : Pressed");
		if (status_autoPlay) { return; }
		operateKeyEffect(KEY::D);
		break;
	case EventKeyboard::KeyCode::KEY_F:
		CCLOG("keyF : Pressed");
		if (status_autoPlay) { return; }
		operateKeyEffect(KEY::F);
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		CCLOG("keySpace : Pressed");
		if (status_autoPlay) { return; }
		operateKeyEffect(KEY::SPACE);
		break;
	case EventKeyboard::KeyCode::KEY_J:
		CCLOG("keyJ : Pressed");
		if (status_autoPlay) { return; }
		operateKeyEffect(KEY::J);
		break;
	case EventKeyboard::KeyCode::KEY_K:
		CCLOG("keyK : Pressed");
		if (status_autoPlay) { return; }
		operateKeyEffect(KEY::K);
		break;
	case EventKeyboard::KeyCode::KEY_L:
		CCLOG("keyL : Pressed");
		if (status_autoPlay) { return; }
		operateKeyEffect(KEY::L);
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
		if (status_playing == PLAY_STATUS::NOSTART) {
			CCLOG("game start");
			gameStart();
		}
		else if (status_playing == PLAY_STATUS::PAUSED) {
			operatePauseMenu();
		}
		break;
	case EventKeyboard::KeyCode::KEY_1:
		changeSpeed(false);
		break;
	case EventKeyboard::KeyCode::KEY_2:
		changeSpeed(true);
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		operateESC();
		break;
	default:
		break;
	}

}
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_S:
		endKeyEffect(0.0f, KEY::S);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		endKeyEffect(0.0f, KEY::D);
		break;
	case EventKeyboard::KeyCode::KEY_F:
		endKeyEffect(0.0f, KEY::F);
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		endKeyEffect(0.0f, KEY::SPACE);
		break;
	case EventKeyboard::KeyCode::KEY_J:
		endKeyEffect(0.0f, KEY::J);
		break;
	case EventKeyboard::KeyCode::KEY_K:
		endKeyEffect(0.0f, KEY::K);
		break;
	case EventKeyboard::KeyCode::KEY_L:
		endKeyEffect(0.0f, KEY::L);
		break;
	default:
		break;
	}

}

void GameScene::releaseKeyInput(float dt) {
	status_keyUsing = false;
}

//Scene�� ���� init() �Լ� ����
bool GameScene::init()
{
	CCLOG("GameScene::init()..");
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CCLOG("GameScene::init()..finished!");
	return true;
}

void GameScene::onEnterTransitionDidFinish() {

	// �ε�ȭ�� �����ֱ�
	showLoadingState();

	// �⺻���� ��������
	initData();
}

// �ε�â ����
void GameScene::showLoadingState() {
	CCLOG("GameScene::initData()...");
	//���� �ʱ�ȭ �� �� �⺻ �������� �ҷ�����
	size_window = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	cache = TextureCache::getInstance();
	layer_notes = Layer::create();
	FMOD::System_Create(&sound_system);
	sound_system->setSoftwareChannels(3000);
	sound_system->init(320, FMOD_INIT_NORMAL, NULL);
	status_keyUsing = false;
	status_bmsName = bn;
	CCLOG("########## bmsFile : %s", status_bmsName.c_str());
	status_dirs = mn;
	CCLOG("########## bmsDirs : %s", status_dirs.c_str());
	status_speed = sp;
	CCLOG("########## speed : %lf", status_speed);
	status_autoPlay = ap;
	CCLOG("########## autoPlay : %d", status_autoPlay);
	status_playing = PLAY_STATUS::NOSTART;

	// ȭ�� UI ����
	setUiInfo();

	//Ű ������ ����.
	enableKeySetting();

	// Effect ���� �ε�
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_GO.c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_READY.c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_CHANGESPEED.c_str());

	// ��������Ʈ �ε�
	cache->addImage(UI_SPRITE_NOATEBACKGROUND);
	cache->addImage(UI_SPRITE_SMALLNOTE);
	cache->addImage(UI_SPRITE_LARGENOTE);
	cache->addImage(UI_SPRITE_MIDNOTE);
}

// Ű ����
void GameScene::enableKeySetting() {
	//Ű ������ ����.
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);
}

// ���� ������ �ҷ�����
void GameScene::initData() {
	
	/* ���� ���� ä���� �̸� 1(4/4 ����)�� �ʱ�ȭ �س��´�. */
	std::fill_n(notes_barLength, 1000, 1);

	/* BMS �ε� */
	BMSManager::getInstance()->readBms(status_dirs, status_bmsName, status_bpm,
		str_bpmValue, str_wavFile, str_bmpFile, str_stopValue, &notes, notes_barLength);

	// ��Ʈ ����
	setNotes();

	// ��Ÿ �� ����
	label_time_music = Label::createWithSystemFont("music_time : 0", "consolas", 20);
	label_speed = Label::createWithSystemFont("music_speed : " + std::to_string(status_speed), "consolas", 20);
	label_bpm = Label::createWithSystemFont("music_bpm : " + std::to_string(status_bpm), "consolas", 20);
	label_bar = Label::createWithSystemFont("music_bar : " + std::to_string(bar_iter_latest), "consolas", 20);

	// �� ����
	label_time_music->setPosition(size_window.width / 2, size_window.height / 2);
	this->addChild(label_time_music);

	label_speed->setPosition(size_window.width / 2, size_window.height / 2 - 20);
	this->addChild(label_speed);

	label_bpm->setPosition(size_window.width / 2, size_window.height / 2 - 40);
	this->addChild(label_bpm);

	label_bar->setPosition(size_window.width / 2, size_window.height / 2 - 60);
	this->addChild(label_bar);

	/* ���� ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_READY.c_str());

	CCLOG("GameScene::initData() - finished!");
}

// ��Ʈ����
void GameScene::setNotes() {
	CCLOG("GameScene::setNotes()..");

	/* layer_notes �⺻ ���� */
	layer_notes->setPosition(Point(LAYER_POSITIONX, LAYER_POSITIONY));
	layer_notes->setContentSize(Size(LAYER_WIDTH, LAYER_HEIGHT));

	/* �� ��Ʈ�� Y�� ��ġ �����ϴ� ������ ���� */
	first_bpm = status_bpm;
	offsetPosY = (status_speed * bar_length);
	offsetBar = 1.0;

	/* Batch Drawing */
	SpriteBatchNode *batchSmallNote = SpriteBatchNode::create(UI_SPRITE_SMALLNOTE);
	this->addChild(batchSmallNote);
	SpriteBatchNode *batchLargeNote = SpriteBatchNode::create(UI_SPRITE_LARGENOTE);
	this->addChild(batchLargeNote);
	SpriteBatchNode *batchMidNote = SpriteBatchNode::create(UI_SPRITE_MIDNOTE);
	this->addChild(batchMidNote);
	SpriteBatchNode *batchBar = SpriteBatchNode::create(UI_SPRITE_BAR);
	this->addChild(batchBar);

	/* ���� ��Ʈ iterator �ʱ�ȭ */
	note_iter_latest = notes.begin();

#pragma region ��׶��� �̹��� ���̱�

	/* ��׶��� �� �̹��� ���̱� */
	note_sprite_background = Sprite::createWithTexture(cache->getTextureForKey(UI_SPRITE_NOATEBACKGROUND));
	note_sprite_background->setAnchorPoint(Point(0, 0));
	note_sprite_background->setPosition(Point(0, 0));
	note_sprite_background->setOpacity(OPACITY_NOTE_SPRITE_BACKGROUND);
	layer_notes->addChild(note_sprite_background, ZORDER_NOTEBACKGROUND);

#pragma endregion

#pragma region ������ ��������Ʈ ����

	/* ������ ��������Ʈ ���� */
	int i = 1;
	while (i < 10) {
		cache->addImage(UI_SPRITE_KEYPRESSFILE[i]);
		keyPressed_sprite[i] = Sprite::createWithTexture(cache->getTextureForKey(UI_SPRITE_KEYPRESSFILE[i]));
		keyPressed_sprite[i]->setAnchorPoint(Point(0, 0));
		keyPressed_sprite[i]->setPosition(Point(-2, -LAYER_POSITIONY));
		keyPressed_sprite[i]->setVisible(false);
		note_sprite_background->addChild(keyPressed_sprite[i]);
		i++;
	}

#pragma endregion

#pragma region ��ź ����Ʈ

	/* ��ź ����Ʈ */
	i = 1;
	cache->addImage(UI_SPRITE_BOMB);
	while (i < 10) {
		bomb_sprite[i] = Sprite::createWithTexture(cache->getTextureForKey(UI_SPRITE_BOMB),
			Rect(0, 0, UI_SPRITE_ANIM_BOMB_WIDTH - 20, UI_SPRITE_ANIM_BOMB_HEIGHT));
		bomb_sprite[i]->setPosition(Point(NOTE::NOTE_POS_X[i], -10));

		/* ���� ��� ���ֱ� ���� SCREEN ���� ȿ�� */
		BlendFunc blend = { GL_ONE_MINUS_DST_COLOR, GL_ONE };
		bomb_sprite[i]->setBlendFunc(blend);
		bomb_sprite[i]->setOpacity(0);

		note_sprite_background->addChild(bomb_sprite[i]);
		i++;
	}

#pragma endregion

#pragma region ���������� ����Ʈ

	/* ���������� ����Ʈ */
	i = 0;
	cache->addImage(UI_SPRITE_EQUALIZER);
	equalizer_sprite = Sprite::createWithTexture(cache->getTextureForKey(UI_SPRITE_EQUALIZER));
	equalizer_sprite->setAnchorPoint(Point(0, 0));
	equalizer_sprite->setPosition(Point(0, 0));
	BlendFunc blend = { GL_ONE_MINUS_DST_COLOR, GL_ONE };
	equalizer_sprite->setBlendFunc(blend);
	note_sprite_background->addChild(equalizer_sprite);
	auto fi = FadeIn::create(0.3f);
	auto ft = FadeTo::create(0.3f, 50);
	auto seq = Sequence::create(fi, ft, nullptr);
	auto rep = RepeatForever::create(seq);
	equalizer_sprite->runAction(rep);

#pragma endregion

#pragma region �޺� �� ����

	/* �޺� �� ���� */
	combo_label = Label::createWithTTF("", COMBO_FONT, COMBO_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	combo_label->setPosition(
		note_sprite_background->getContentSize().width / 2,
		note_sprite_background->getContentSize().height / 8 * 5
	);
	combo_label->setOpacity(0);
	combo_label->enableOutline(Color4B::BLACK, 4);
	note_sprite_background->addChild(combo_label);
	auto rgbRed = TintTo::create(0.05f, Color3B(255, 000, 051));
	auto rgbGreen = TintTo::create(0.05f, Color3B(000, 204, 102));
	auto rgbBlue = TintTo::create(0.05f, Color3B(051, 153, 255));
	auto rgbGray = TintTo::create(0.05f, Color3B(204, 204, 204));
	seq = Sequence::create(rgbRed, rgbGreen, rgbBlue, rgbGray, nullptr);
	rep = RepeatForever::create(seq);
	combo_label->runAction(rep);

	combo_label_ui = Label::createWithTTF("COMBO", COMBO_UI_FONT, COMBO_UI_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	combo_label_ui->setPosition(
		note_sprite_background->getContentSize().width / 2,
		(note_sprite_background->getContentSize().height / 4 * 3) + 50
	);
	combo_label_ui->setOpacity(0);
	combo_label_ui->enableOutline(Color4B::BLACK, 4);
	note_sprite_background->addChild(combo_label_ui);

	judge_label = Label::createWithTTF("", JUDGE_FONT, JUDGE_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	judge_label->setPosition(
		note_sprite_background->getContentSize().width / 2,
		note_sprite_background->getContentSize().height / 5
	);
	judge_label->setOpacity(0);
	judge_label->enableOutline(Color4B::BLACK, 4);
	note_sprite_background->addChild(judge_label);
	rgbRed = TintTo::create(0.05f, Color3B(255, 000, 051));
	rgbGreen = TintTo::create(0.05f, Color3B(000, 204, 102));
	rgbBlue = TintTo::create(0.05f, Color3B(051, 153, 255));
	rgbGray = TintTo::create(0.05f, Color3B(204, 204, 204));
	seq = Sequence::create(rgbRed, rgbGreen, rgbBlue, rgbGray, nullptr);
	rep = RepeatForever::create(seq);
	judge_label->runAction(rep);

	judge_time_label = Label::createWithTTF("", JUDGE_FONT, JUDGE_TIME_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	judge_time_label->setPosition(
		note_sprite_background->getContentSize().width / 2,
		note_sprite_background->getContentSize().height / 5 + 50
	);
	judge_time_label->setOpacity(0);
	judge_time_label->enableOutline(Color4B::BLACK, 4);
	note_sprite_background->addChild(judge_time_label);

#pragma endregion
	 
#pragma region ���� �ε�

	/* BMS ���� �������� ��ȿ�� �˻�*/
	std::string sf = str_wavFile[2];
	std::string fp = "bms/" + status_dirs + "/" + sf;
	std::string chStr;
	bool chPathFlag = false;
	struct stat buffer;
	if (stat(fp.c_str(), &buffer) != 0) {
		/* �ش� ���� ������ �������� ���� �� */
		CCLOG("No SoundFile : ");
		/*

			BMS �� ���� ������ .ogg �ε� .wav �� bms �� �Ľ̵Ǿ��ִ� ��찡 �ִ�.
			�̶� ������ �������� ������ Ȯ���ڸ� �ٲپ ���������� �ε��Ѵ�.

		*/
		if (fp.find(".ogg", 0, 4) != std::string::npos) {
			/* ���� Ȯ���ڰ� .ogg �� ��� ���� .wav �� �ٲپ */
			chStr = ".wav";
		}
		else {
			/* ���� Ȯ���ڰ� .wav �� ��� ���� .ogg �� �ٲپ */
			chStr = ".ogg";
		}
		chPathFlag = true;
	}

	// ���� ���� ���� �Ҵ�, ���� �ε�
	v_sound_sound.resize(STR_CHANNELSIZE);
	for (int i = 0; i < v_sound_sound.size(); i++) {
		std::string soundFile = str_wavFile[i];
		std::string filePath = "bms/" + status_dirs + "/" + soundFile;
		if (chPathFlag) {
			/* ���� BMS ��ΰ� ��ȿ���� �����Ƿ� Ȯ���ڸ� �ٲپ� �ε��غ���. */
			filePath.pop_back(); filePath.pop_back();
			filePath.pop_back(); filePath.pop_back();
			filePath += chStr;

#ifdef _DEBUG
			CCLOG("Path Ȯ���� ���� %s", chStr.c_str());
#endif

		}
		FMOD_RESULT ret = sound_system->createSound(filePath.c_str(), FMOD_DEFAULT, 0, &v_sound_sound[i]);
		v_sound_sound[i]->setMode(FMOD_DEFAULT);
		sound_channel[i]->setVolume(1.0f);

#ifdef _DEBUG
		CCLOG("load sound(createSound) - %s, %d", filePath.c_str(), i);
#endif

	}

#pragma endregion

#pragma region ���� �� ��Ʈ ����

	/* ���� ���� ����� ����? */
	i = 0;
	while (i < 1000) {
		if (notes_barLength[i] == 0) {
			notes_barLength[i] = 1;
		}
		CCLOG("notes_barLength[%d] = %lf", i, notes_barLength[i]);
		i++;
	}

	/* ��Ʈ ��������Ʈ �� �ð� ���� */
	sort(notes.begin(), notes.end());

	i = 1;
	double cur_bpm = first_bpm;
	double cur_time = STARTEMPTYTIME;
	double pre_beat = 0.0f;
	double cur_pos = cur_time * cur_bpm * status_speed;
	double bar_stack = cur_pos;
	//double cur_pos = 0;
	int cur_bar = 0;
	int last_bar_pos = 0;
	for (auto it = notes.begin(); it != notes.end(); it++) {

		auto cur_note = it;

		/* ��������Ʈ ���� */
		if (cur_note->note_key == 4) {
			cur_note->sprite->initWithTexture(batchMidNote->getTexture());
		}
		else if (cur_note->note_key % 2 == 0) {
			cur_note->sprite->initWithTexture(batchSmallNote->getTexture());
		}
		else {
			cur_note->sprite->initWithTexture(batchLargeNote->getTexture());
		}

		/* x, y �� ��ġ ���� */
		double notePosX = NOTE::NOTE_POS_X[cur_note->note_key];
		layer_notes->addChild(cur_note->sprite, ZORDER_NOTES);

		cur_note->sprite->setVisible(false);

		/* ��Ʈ Ÿ�� ���� */
		/*
	
			���� ��Ʈ�� �ð����ٰ� ��� ���ذ��� ���
			���� ��Ʈ���� ���̸� ���� ���� ��Ʈ�� �����Ҷ�����
			bpm �� bar_length �� �̿��Ͽ� �ð��� �����ش�.

		*/

		double cur_beat = cur_note->note_beat;

		/*

			���� ��Ʈ�� ���𿡼� ���� ��Ʈ���� �� ���� ����(������̰� �ٸ� �� �����Ƿ�)
			�����Ͽ� �ջ��ϴ� ������� �ð� ���� ���Ѵ�.

		*/

		/* �� ����(����)�� ���ؾ��ϴ� ���� */
		double min, max;

		cur_bar = (int)pre_beat;
		double time_diff_sum = 0.0f;
		double pos_diff_sum = 0.0f;
		while (cur_bar <= cur_beat) {

			/* min ���� */
			min = (cur_bar == (int)pre_beat) ? pre_beat : cur_bar;

			/* max ���� */
			max = (cur_bar == (int)cur_beat) ? cur_beat : cur_bar + 1;

			/* ���� ��Ʈ ���� ���� */
			if (min == cur_bar) {
				notes_barPosition[cur_bar] = bar_stack;
				last_bar_pos = cur_bar;
				CCLOG("BAR : posy : %lf / (beat : %lf)", notes_barPosition[cur_bar], cur_bar);
			}

			/* ���� ���𿡼� ��Ʈ ���� */
			double beat_diff = (double)max - min;

			/* 1��Ʈ�� �ɸ��� �� */
			double secPerBeat = (60.0 / cur_bpm);

			/* ���� ������ ���� - �⺻(1) �϶� 4/4 ���� */
			double cur_barLength = 4.0 * notes_barLength[cur_bar];

			/* ��Ʈ ���̸�ŭ �� �ð����̸� ��� */
			double time_diff = beat_diff * secPerBeat * cur_barLength;
			//CCLOG("time_diff : %lf / beat_diff : %lf / secPerBeat : %lf / cur_barLength : %lf", time_diff, beat_diff, secPerBeat, cur_barLength);

			/* ���� �ð��� �ջ� */
			time_diff_sum += time_diff;
			pos_diff_sum += time_diff * status_speed * cur_bpm;
			bar_stack += time_diff * status_speed * cur_bpm;

			cur_bar += 1;
		}

		/* ���� ��Ʈ�� �ð� �Է� */
		cur_time += time_diff_sum;
		cur_note->note_time = cur_time;

		/* �ð� ��ȭ�� ���� �Ÿ� ���� */
		cur_pos += pos_diff_sum;
		cur_note->note_posy = cur_pos;

		CCLOG("NOTE : posy %s : %lf / (beat : %lf)", NOTE::STR_KEY[cur_note->note_key].c_str(), cur_note->note_posy, cur_note->note_beat);

		/* �Ÿ� ���� */
		cur_note->sprite->setPosition(Point(notePosX, cur_note->note_posy));

		/* ���� ��Ʈ�� BPM Change�� STOP �϶� status ���� */
		switch (cur_note->note_key) {
		case NOTE::KEY_STOP:
			/*

				STOP ���� val / 192 * bar_length �� ���� �ð��� �����־�� ��

			*/
			cur_time += cur_note->note_channel / 192 * str_stopValue[(int)cur_beat];
			break;
		case NOTE::KEY_BPM:
			cur_bpm = cur_note->note_channel;
			break;
		case NOTE::KEY_BPMEXTEND:
			cur_bpm = str_bpmValue[cur_note->note_channel];
			break;
		default:
			break;
		}

		pre_beat = cur_beat;
	}

#ifdef _DEBUG
	i = 1;
	for (NOTE::Note cur_note : notes) {
		CCLOG("%d 's note : %d, %lf, (%lf, %lf)", i++, cur_note.note_key, cur_note.note_time,
			cur_note.sprite->getPosition().x, cur_note.sprite->getPosition().y);
	}
#endif // _DEBUG

	/* ���� �������� �� ���� ���� ���� ��ġ ���� */
	double diff = notes_barPosition[last_bar_pos] - notes_barPosition[last_bar_pos - 1];
	i = last_bar_pos + 1;
	while (i < 1000) {
		notes_barPosition[i] = notes_barPosition[i - 1] + diff;
		i++;
	}

	/* ���� ��������Ʈ ���� */
	i = 0;
	while (i < 1000) {
		/* ��������Ʈ �ʱ�ȭ */
		bars_sprite[i] = Sprite::createWithTexture(batchBar->getTexture());
		//bars_sprite[i]->setPosition(Point(LAYER_WIDTH / 2, size_window.height + 30));
		bars_sprite[i]->setPosition(Point(LAYER_WIDTH / 2, notes_barPosition[i]));
		bars_sprite[i]->setVisible(false);
		layer_notes->addChild(bars_sprite[i], ZORDER_BARS);
		i++;
	}

#pragma endregion

#pragma region BGA �ε�

	loadBGA();

#pragma endregion

	/* ��Ʈ���̾� ���� */
	this->addChild(layer_notes, 1);
	CCLOG("GameScene::setNotes()..finished!");
}

// UI ���� ����
void GameScene::setUiInfo() {
	//��� ��������Ʈ ����
	CCLOG("setting Background ...");
	//auto winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	ui_sprite_background = Sprite::create(UI_SPRITE_BACKGROUND);
	ui_sprite_background->setPosition(Point(size_window.width / 2, size_window.height / 2));
	this->addChild(ui_sprite_background, ZORDER_LAYOUT);

	CCLOG("setting Background ... finished!");
}

/* ���� ���� */
void GameScene::gameStart() {

	/* ������ ���� �������� ���� ���¿����� �۵��ؾ� �Ѵ�. */
	if (status_playing != PLAY_STATUS::NOSTART) {
		return;
	}

	/* ���� ���� ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_GO.c_str());

	/* ������� ���� */
	status_playing = PLAY_STATUS::PLAYING;

	/* ���� �ð� ��ŸƮ */
	time_start = std::chrono::system_clock::now();
	bga_start = std::chrono::system_clock::now();


	/* BGA ��ȭ �ʱ�ȭ */
	status_bgaCh = -1;

	/* ƽ ���۷��̼� �۵� 0.015 �� ���� ȣ�� */
	this->schedule(schedule_selector(GameScene::tickOperate));
	
}

void GameScene::loadTexture() {



}

/* BGA ��� ƽ */
void GameScene::tickOperateBGA() {
	/* ���� BGA ��Ÿ���� */
	/* BGA ä���� �����߳� ���� */
	if (status_bga != status_bgaCh) {

		/* 
		
			�ٲ� ä���� �������� �̹��� �������� �ľ�
		
		*/

		/* BGA ä�� ���� */
		std::string filePath = "bms/" + status_dirs + "/" + str_bmpFile[status_bga];

		/* BGA ��� �ð� ���� */
		bga_start = chrono_time;

		if (status_isVideo[status_bga]) {
			/* ������ */
			/* �� ���� �ε� */
			video_capture.open(filePath);
			if (!video_capture.isOpened()) {
				CCLOG("can't open BGA background.. %s", filePath.c_str());
				/* ��ü �̹��� ��ȯ */
				status_isVideo[status_bga] = false;
				str_bmpFile[status_bga] = "../../images/" + UI_TEXTURE_NOBGA;
			}
			else {
				CCLOG("open BGA background.. %s", filePath.c_str());

				/* ĸ���� ������ 1������ MAT ������ Texture2D �� ��ȯ */
				video_capture >> video_frame;

				/* BGR ���� RGB �÷� ���� */
				cv::cvtColor(video_frame, video_frame, CV_BGR2RGB);

				/* �ؽ��� ��ȯ */
				CCLOG("Texture Setting.. %s", filePath.c_str());
				delete bga_texture;
				bga_texture = new Texture2D();
				bga_texture->initWithData(video_frame.data,
					video_frame.elemSize() * video_frame.cols * video_frame.rows,
					Texture2D::PixelFormat::RGB888,
					video_frame.cols,
					video_frame.rows,
					Size(video_frame.cols, video_frame.rows));
				CCLOG("Texture Setting..finished %s", filePath.c_str());
				CCLOG("Sprite Initting.. %s", filePath.c_str());
				bga_sprite->initWithTexture(bga_texture);
				bga_sprite->setContentSize(Size(size_window.width, size_window.height));
				bga_sprite->setPosition(size_window.width / 2, size_window.height / 2);
				CCLOG("Sprite Initting..finished %s", filePath.c_str());
			}
		}
		else {
			/* �̹��� ���� */
			bga_texture = Director::getInstance()->getTextureCache()->getTextureForKey(filePath);

			/* �̹��� �ִϸ��̼��� ���ư��� �ʴ´ٸ� ���⼭ �� �ڵ� ���־���� 
			
				1. bga_texture�� ������Ʈ ���� �ʾƼ� �������� ��������Ʈ�� ������ �־�� �ϰų�
				2. .bmp Ȯ������ ���.
			
			*/

		}

		status_bgaCh = status_bga;
	}
	else {
		/* BGA ä�� ���� x �״�� ���� */
		/* �̹��� ä���� ��� �ƹ��͵� ���� �ʴ´�. */
		if (status_isVideo[status_bga]) {
			/* �������� ��� ��� */
			/* ���� ���� �������� �ð��� �������� �ʾ����� �����Ѵ� */
			double nextFrameTime = video_capture.get(CV_CAP_PROP_POS_MSEC) / 1000;
			if (nextFrameTime > currentTime_bga) {
				return;
			}

			video_capture >> video_frame;
			if (!video_frame.empty()) {
				/* BGR ���� RGB �÷� ���� */
				cv::cvtColor(video_frame, video_frame, CV_BGR2RGB);

				/* ���� �������� MAT ���Ϸ� �ؽ��ĸ� ������Ʈ �Ѵ� */
				bga_texture->updateWithData(video_frame.data, 0, 0, video_frame.cols, video_frame.rows);
			}
			else {
				/* �뷫 100 �� �̻��� �������̸� �ݺ���� ���� �ʴ´� */
				if (currentTime_bga < 100) {
					/* BGA ��� �ð� ���� */
					bga_start = chrono_time;

					/* �ٽ� ���� - ���ѹݺ� - ���� Ȯ���ϰ� �ݺ� ���ص� �� */
					std::string filePath = "bms/" + status_dirs + "/" + str_bmpFile[status_bga];
					video_capture.open(filePath);
				}
			}
		}

		
	}
}

// ��Ʈ ��� �� �� ƽ�� ����ؾ� �� ��.(������ ����)
void GameScene::tickOperate(float interval) {

	/* Ÿ�̸� ���� �ð� ���� */
	chrono_time = std::chrono::system_clock::now();

	if (status_playing == PLAY_STATUS::PAUSED) {
		pause_time = chrono_time - chrono_pauseTime;
		return;
	}

	/* ä�� ���� */
	sound_system->update();

	/* Ÿ�̸� ���� �ð� ���� */
	time_music = chrono_time - time_start;
	time_music -= pause_time_all;
	time_bga = chrono_time - bga_start;
	time_bga -= pause_time_all;
	currentTime = time_music.count();
	currentTime_bga = time_bga.count();
	label_time_music->setString("music_time : " + std::to_string(currentTime));

	/* ���� ƽ�� �ð��� ���� ƽ�� �ð����� ���� */
	double tick_diff = currentTime - prevTime;
	prevTime = currentTime;

	/* ���� �ð����� ��ũ�� �� �������� ���Ѵ� */
	yPosOffset += tick_diff * status_speed * status_bpm;

	/* ���� BPM ��Ÿ���� */
	label_bpm->setString("music_bpm : " + std::to_string(status_bpm));

	/* ���� Bar ��ȣ ��Ÿ���� */
	label_bar->setString("music_bar : " + std::to_string(bar_iter_latest - 1));

	/* iterator ������ ���� */
	std::vector<NOTE::Note>::iterator cur_note = note_iter_latest;

	/* BGA �ִϸ��̼� �ش� BGA�� fps ���� ȣ���Ѵ�. */
	tickOperateBGA();

	/* ���� ��ȸ�ϸ鼭 ��ġ ���� ���Ѵ�. */
	int cur_bar = bar_iter_latest;
	while (cur_bar < 1000) {

		double bar_posy = notes_barPosition[cur_bar] - yPosOffset;

		if (bar_posy > size_window.height) {
			break;
		}

		bars_sprite[cur_bar]->setPosition(Point(LAYER_WIDTH / 2, bar_posy));
		bars_sprite[cur_bar]->setVisible(true);

		if (bar_posy <= 0) {
			bars_sprite[cur_bar]->setVisible(false);
			bar_iter_latest++;
		}

		cur_bar++;
	}

	/* ��Ʈ�� ��ȸ�ϸ鼭 ��ġ ���� ���Ѵ�. */
	while (cur_note != notes.end()) {

		/* �ð� �� */
		double time_diff_curTime = cur_note->note_time - currentTime;

		/*
		
			���� ��Ʈ�� ��ġ ��
		
		*/

		if (cur_note->note_posy - yPosOffset > size_window.height) {
			break;
		}

		cur_note->sprite->setPosition(Point(cur_note->sprite->getPositionX(), cur_note->note_posy - yPosOffset));
		if (cur_note->note_object == NOTE::OBJ_NOTE && !cur_note->isChecked) {
			cur_note->sprite->setVisible(true);
		}

		if (time_diff_curTime <= 0) {
			if (cur_note->note_object == NOTE::OBJ_EVENT) {
				operateNoteKey(*cur_note);
			}
			else {
				cur_note->sprite->setVisible(false);
				if (status_autoPlay && !cur_note->isChecked) {
					CCLOG("auto Inside : %d", cur_note->note_key);
					operateKeyEffect(cur_note->note_key);
				}
			}
		}

		if (time_diff_curTime <= -JUDGE[JUDGE::BAD]) {
			note_iter_latest++;
			if (!cur_note->isChecked) {
				operateComboEffect(cur_note);
			}
		}
		cur_note++;
	}

}

/* Ű�� Ÿ�Կ� ���� �޸� ���� �Ѵ�. */
void GameScene::operateNoteKey(NOTE::Note &note) {

	if (note.isChecked) {
		return;
	}

	switch (note.note_key) {
	case NOTE::KEY_BPM:
		status_bpm = note.note_channel;
		note.isChecked = true;
		break;
	case NOTE::KEY_BPMEXTEND:
		status_bpm = str_bpmValue[note.note_channel];
		note.isChecked = true;
		break;
	case NOTE::KEY_STOP:
		/* �ƹ����� ������ ���� */
		note.isChecked = true;
		break;
	case NOTE::KEY_BGM:
		sound_system->playSound(v_sound_sound[note.note_channel], NULL, false, &sound_channel[note.note_channel]);
		note.isChecked = true;
		break;
	case NOTE::KEY_BGA:
		status_bga = note.note_channel;
		note.isChecked = true;
		break;
	default:
		break;
	}

#ifdef _DEBUG
	CCLOG("* Operate Note : %d, %lf", note.note_key, note.note_time);
#endif // _DEBUG

}

/* ���� */
void GameScene::changeSpeed(bool isSpeedUp) {
	KEY_USINGSET;
	double before_speed = status_speed;
	if (status_speed > 1 && !isSpeedUp) {
		status_speed -= 1;
	}
	else if (status_speed < 8 && isSpeedUp) {
		status_speed += 1;
	}
	double span = status_speed / before_speed;

	/* ���� ���� ȿ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGESPEED.c_str());

	/*
	
		��ü ��ġ �� ����
	
	*/

	/* ������� ���� ��ũ�� ���� ���� */
	yPosOffset *= span;

	double cur_bpm = first_bpm;
	double cur_time = STARTEMPTYTIME;
	double pre_beat = 0.0f;
	double cur_pos = cur_time * cur_bpm * status_speed;
	//double cur_pos = 0;
	int cur_bar = 0;
	double bar_stack = cur_pos;
	int last_bar_pos;
	for (auto it = notes.begin(); it != notes.end(); it++) {

		auto cur_note = it;

		double cur_beat = cur_note->note_beat;

		double min, max;

		cur_bar = (int)pre_beat;
		double time_diff_sum = 0.0f;
		double pos_diff_sum = 0.0f;
		while (cur_bar <= cur_beat) {

			/* min ���� */
			min = (cur_bar == (int)pre_beat) ? pre_beat : cur_bar;

			/* max ���� */
			max = (cur_bar == (int)cur_beat) ? cur_beat : cur_bar + 1;

			/* ���� ��Ʈ ���� ���� */
			if (min == cur_bar) {
				notes_barPosition[cur_bar] = bar_stack;
				last_bar_pos = cur_bar;
			}

			/* ���� ���𿡼� ��Ʈ ���� */
			double beat_diff = max - min;

			/* 1��Ʈ�� �ɸ��� �� */
			double secPerBeat = (60.0 / cur_bpm);

			/* ���� ������ ���� - �⺻(1) �϶� 4/4 ���� */
			double cur_barLength = 4 * notes_barLength[cur_bar];

			/* ��Ʈ ���̸�ŭ �� �ð����̸� ��� */
			double time_diff = beat_diff * secPerBeat * cur_barLength;

			/* ���� �ð��� �ջ� */
			time_diff_sum += time_diff;
			pos_diff_sum += time_diff * status_speed * cur_bpm;
			bar_stack += time_diff * status_speed * cur_bpm;

			cur_bar += 1;
		}

		/* ���� ��Ʈ�� �ð� �Է� */
		cur_time += time_diff_sum;
		cur_note->note_time = cur_time;

		/* �ð� ��ȭ�� ���� �Ÿ� ���� */
		cur_pos += pos_diff_sum;
		cur_note->note_posy = cur_pos;

		/* ��ġ �������� ���� ��� ��带 ���������� setVisible = false ���ش�. */
		cur_note->sprite->setPosition(Point(cur_note->sprite->getPositionX(), cur_note->note_posy - yPosOffset));

		/* ���� ��Ʈ�� BPM Change�� STOP �϶� status ���� */
		switch (cur_note->note_key) {
		case NOTE::KEY_STOP:
			/*

				STOP ���� val / 192 * bar_length �� ���� �ð��� �����־�� ��

			*/
			cur_time += cur_note->note_channel / 192 * str_stopValue[(int)cur_beat];
			break;
		case NOTE::KEY_BPM:
			cur_bpm = cur_note->note_channel;
			break;
		case NOTE::KEY_BPMEXTEND:
			cur_bpm = str_bpmValue[cur_note->note_channel];
			break;
		default:
			break;
		}

		pre_beat = cur_beat;
	}

	/* ���� �������� �� ���� ���� ���� ��ġ ���� */
	double diff = notes_barPosition[last_bar_pos] - notes_barPosition[last_bar_pos - 1];
	int i = last_bar_pos + 1;
	while (i < 1000) {
		notes_barPosition[i] = notes_barPosition[i - 1] + diff;
		i++;
	}

	/* ���� ��ġ ������ */
	i = 0;
	while (i < 1000) {
		bars_sprite[i]->setPosition(Point(LAYER_WIDTH / 2, notes_barPosition[i] - yPosOffset));
		i++;
	}

	CCLOG("Changed speed : %lf", status_speed);
	label_speed->setString("music_speed : " + std::to_string(status_speed));
	this->scheduleOnce(schedule_selector(GameScene::releaseKeyInput), 0.015f);
}

/* Ű ���� �Է� ó�� */
void GameScene::operateKeyEffect(int keyNo) {

	//CCLOG("keyNo : %d", keyNo);

	/*

		S D F SPACE J K L
		Ű�� �ԷµǾ��� �� ���� �� ����Ʈ ������ �Ѵ�.
		�ش� �Լ������� Ű���� ����Ʈ�� ���� ����� �ϰ�
		�������δ� operateComboEffect ���� �����Ѵ�.

	*/

	/* ���� ���� ������ ��� ���� */
	if (status_playing == PLAY_STATUS::NOSTART) {
		return;
	}

	/* ���� ������ �ִ� ������ ��� ���� - �ߺ��Է� ���� */
	//if (status_keyPressing[keyNo]) {
	//	return;
	//}

	///* Ű ���� ���� ON */
	//status_keyPressing[keyNo] = true;

	/* ���� ���� Ű�� ��ȿ�� ��Ʈ�� ã�´�. */
	auto cur_note = note_iter_latest;

	/* ��Ʈ Ž�� */
	while (cur_note != notes.end()) {
			if (cur_note->note_key == keyNo && !cur_note->isChecked) {
				/*

					���� ���� Ű���� ����
					���� ����� üũ���� ���� ��Ʈ�� ã�´�.
					
					���� ����� ��Ʈ�� ã���� �ش� ��Ʈ�� ��ȿ���� ���� �ִ��� Ȯ���Ѵ�
					operateComboEffect ���� noteTime�� �������� �����Ѵ�.

				*/
				operateComboEffect(cur_note);
				CCLOG("find right Note : %d", cur_note->note_key);
				/* ���� ä�� */
				thisKeyChannel = cur_note->note_channel;
				//CCLOG("%d channel sound pressed", thisKeyChannel);
				break;
			}
		cur_note++;
	}

	/* ��Ʈ ����Ʈ */
	keyPressed_sprite[keyNo]->setVisible(true);
	if (status_autoPlay) {
		keyPressed_sprite[keyNo]->stopAllActions();
		auto dt = DelayTime::create(0.15f);
		auto cb = CallFunc::create(CC_CALLBACK_0(GameScene::endKeyEffect, this, 0.0f, keyNo));
		auto seq = Sequence::create(dt, cb, nullptr);
		keyPressed_sprite[keyNo]->runAction(seq);
	}

	/* �� Ű�� ���� ������ ������ �Է¹��� Ű�� ���� ��� */
	sound_system->playSound(v_sound_sound[thisKeyChannel], NULL, false, &sound_channel[thisKeyChannel]);
	
}

/* Ű ���� �Է����� ó�� */
void GameScene::endKeyEffect(float interval, int keyNo) {

	/* Ű �����Է� ���� */
	status_keyPressing[keyNo] = false;

	/* ��Ʈ ����Ʈ ���� */
	keyPressed_sprite[keyNo]->setVisible(false);

}

/* ���� ó�� */
void GameScene::operateComboEffect(std::vector<NOTE::Note>::iterator cur_note) {

	//CCLOG("noteTime : %lf", cur_note->note_time);

	/*

		���� ��Ʈ�� �����ð��� ����
		�� ������ ���� �޺��� ��������� �ϰ�
		�ش� ����Ʈ�� ����.

	*/

	/* ���� ��Ʈ���� �м� */
	int keyNo = cur_note->note_key;
	double judgeTime = cur_note->note_time - currentTime;
	CCLOG("comboEffect enter : keyNo : %d", keyNo);

	/* 6�� ��ũ��ġ ����� �޺��� �ݿ����� �ʴ´�. */
	if (keyNo == 6) {
		cur_note->isChecked = true;
		cur_note->sprite->setVisible(false);
		return;
	}

	/*

		���� ���� �˻�

	*/
	int judgeNo = 0;

	if (judgeTime > JUDGE[JUDGE::BAD]) {
		/* �̸� �������� ������ */
		return;
	}

	if (judgeTime >= -JUDGE[JUDGE::PERFECT] && judgeTime <= JUDGE[JUDGE::PERFECT]) {
		/* ����Ʈ +- 0.15f �̳� */
		cur_note->isChecked = true;
		CCLOG("Perfect Checked : %d, %d", cur_note->isChecked, cur_note->note_key);
		cur_note->sprite->setVisible(false);
		judgeNo = JUDGE::PERFECT;
		if (currentCombo == maxCombo) {
			maxCombo = ++currentCombo;
		}
		else {
			currentCombo++;
		}
	}
	else if (judgeTime >= -JUDGE[JUDGE::GOOD] && judgeTime <= JUDGE[JUDGE::GOOD]) {
		/* �� +- 0.3f �̳� */
		cur_note->isChecked = true;
		cur_note->sprite->setVisible(false);
		judgeNo = JUDGE::GOOD;
		if (currentCombo == maxCombo) {
			maxCombo = ++currentCombo;
		}
		else {
			currentCombo++;
		}
	}
	else if (judgeTime >= -JUDGE[JUDGE::BAD] && judgeTime <= JUDGE[JUDGE::BAD]) {
		/* ��� +- 0.4f �̳� */
		cur_note->isChecked = true;
		cur_note->sprite->setVisible(false);
		judgeNo = JUDGE::BAD;
		if (currentCombo == maxCombo) {
			maxCombo = ++currentCombo;
		}
		else {
			currentCombo++;
		}
	}
	else if (judgeTime < -JUDGE[JUDGE::BAD]) {
		/* �̽� - 0.4f �ʰ� */
		cur_note->isChecked = true;
		judgeNo = JUDGE::MISS;
		currentCombo = 0;
	}

	/*

		�޺� �ִϸ��̼� ����

	*/

	/* ���� ����Ʈ ���̱� */
	judge_label->setString(JUDGE_STR[judgeNo]);
	judge_label->setPosition(
		note_sprite_background->getContentSize().width / 2,
		note_sprite_background->getContentSize().height / 5
	);
	auto mov = MoveBy::create(0.0f, Point(0, -10));
	auto mov1 = MoveBy::create(COMBO_ACTIONTIME, Point(0, 10));
	auto ft = FadeTo::create(COMBO_ACTIONTIME, 150);
	auto spawn = Spawn::create(mov1, ft, nullptr);
	auto dt = DelayTime::create(COMBO_ACTIONDELAYTIME);
	auto fo = FadeOut::create(COMBO_ACTIONTIME);
	auto seq = Sequence::create(mov, spawn, dt, fo, nullptr);
	seq->setTag(101);
	judge_label->stopActionByTag(101);
	judge_label->runAction(seq);

	if (judgeTime >= 0) {
		judge_time_label->setString("+" + std::to_string(judgeTime));
		judge_time_label->setTextColor(Color4B::BLUE);
	}
	else {
		judge_time_label->setString(std::to_string(judgeTime));
		judge_time_label->setTextColor(Color4B::RED);
	}
	if (judgeTime >= -JUDGE[JUDGE::PERFECT] && judgeTime <= JUDGE[JUDGE::PERFECT]) {
		judge_time_label->setTextColor(Color4B::WHITE);
	}
	judge_time_label->setOpacity(0);
	ft = FadeTo::create(COMBO_ACTIONTIME, 150);
	dt = DelayTime::create(COMBO_ACTIONDELAYTIME);
	fo = FadeOut::create(COMBO_ACTIONTIME);
	seq = Sequence::create(ft, dt, fo, nullptr);
	judge_time_label->stopAllActions();
	judge_time_label->runAction(seq);

	/* �̽��� �� ���� ���� �����ִ� ����Ʈ */
	if (judgeNo != JUDGE::MISS) {

		/* �ǹ� ����Ʈ ���̱� */
		auto fi = FadeIn::create(0.0f);
		auto foo = FadeOut::create(0.0f);
		auto anim = Animation::create();
		anim->setDelayPerUnit(0.01f);
		for (int i = 0; i < 16; i++) {
			anim->addSpriteFrameWithTexture(cache->getTextureForKey(UI_SPRITE_BOMB),
				Rect(UI_SPRITE_ANIM_BOMB_WIDTH*i, 0, UI_SPRITE_ANIM_BOMB_WIDTH - 20, UI_SPRITE_ANIM_BOMB_HEIGHT));
		}
		auto animate = Animate::create(anim);
		auto seqan = Sequence::create(fi, animate, foo, nullptr);
		bomb_sprite[keyNo]->stopAllActions();
		bomb_sprite[keyNo]->runAction(seqan);

		/* ���� ����Ʈ ���̱� */
		combo_label->setString(std::to_string(currentCombo));
		combo_label->setPosition(LAYER_WIDTH / 2, note_sprite_background->getContentSize().height / 8 * 5);
		mov = MoveBy::create(0.0f, Point(0, -10));
		mov1 = MoveBy::create(COMBO_ACTIONTIME, Point(0, 10));
		ft = FadeTo::create(COMBO_ACTIONTIME, 150);
		spawn = Spawn::create(mov1, ft, nullptr);
		dt = DelayTime::create(COMBO_ACTIONDELAYTIME);
		fo = FadeOut::create(COMBO_ACTIONTIME);
		seq = Sequence::create(mov, spawn, dt, fo, nullptr);
		seq->setTag(100);
		combo_label->stopActionByTag(100);
		combo_label->runAction(seq);

		combo_label_ui->setOpacity(0);
		ft = FadeTo::create(COMBO_ACTIONTIME, 150);
		dt = DelayTime::create(COMBO_ACTIONDELAYTIME);
		fo = FadeOut::create(COMBO_ACTIONTIME);
		seq = Sequence::create(ft, dt, fo, nullptr);
		combo_label_ui->stopAllActions();
		combo_label_ui->runAction(seq);

	}
	else {
		/* �̽� ���� �� ���� �Ⱥ��̰� */
		combo_label_ui->setOpacity(0);
		combo_label->setOpacity(0);
	}

}

/* BGA ���� ���� �ε� */
void GameScene::loadBGA() {

	/*

		�� BGA ä���� ��ȸ�ϸ鼭
		���� BGA �� ������ ��������, �ƴϸ� �Ϲ� �̹��� ��������
		�˻���� �Ѵ�.

	*/

	int i = 0;
	while (i < STR_CHANNELSIZE) {
		std::string imgf = str_bmpFile[i];
		std::string fp = "bms/" + status_dirs + "/" + imgf;
		std::string chStr = "";
		bool chPathFlag = false;
		if (imgf.compare("") == 0) {
			/* ���� - �� ä���� ��� */
			i++;
			continue;
		}
		if (fp.find(".mpeg", 0, 5) != std::string::npos
			|| fp.find(".mpg", 0, 4) != std::string::npos
			|| fp.find(".wmv", 0, 4) != std::string::npos
			|| fp.find(".avi", 0, 4) != std::string::npos) {

			/* ���� ä���� ������ ������ �� */
			status_isVideo[i] = true;

		}
		else {

			/* Ȯ���� ��ȿ�� �˻� - �����ϴ� Ȯ���� ã�Ƽ� ���� ������ No image */
			struct stat buffer;
			if (stat(fp.c_str(), &buffer) != 0) {
				/* ���� ������ ��ȿ���� ���� �� - jpg, png, bmp ���� Ȯ�� �� �� */
				fp.pop_back(); fp.pop_back();
				fp.pop_back(); fp.pop_back();
				str_bmpFile[i].pop_back();
				str_bmpFile[i].pop_back();
				str_bmpFile[i].pop_back();
				str_bmpFile[i].pop_back();
				if (stat((fp + ".jpg").c_str(), &buffer) == 0) {
					/* jpg Ȯ���� ���� �� */
					str_bmpFile[i] += ".jpg";
				}
				else if (stat((fp + ".png").c_str(), &buffer) == 0) {
					/* jpg Ȯ���� ���� �� */
					str_bmpFile[i] += ".png";
				}
				else if (stat((fp + ".bmp").c_str(), &buffer) == 0) {
					/* jpg Ȯ���� ���� �� */
					str_bmpFile[i] += ".bmp";
				}
				else {
					/* Ȯ���� ���� ���� */
					str_bmpFile[i] = "../../images/" + UI_TEXTURE_NOBGA;
				}
			}
#ifdef _DEBUG
			CCLOG("str_bmpFile[%d] = %s", i, str_bmpFile[i].c_str());
#endif // _DEBUG

			/* ���� ä���� ���� ���� �� �� */
			status_isVideo[i] = false;

		}
		i++;
	}

	/* ������ ��������Ʈ ���� */
	bga_texture = new Texture2D();
	auto img = new Image();
	img->initWithImageFile(UI_TEXTURE_NOBGA);
	bga_texture->initWithImage(img, Texture2D::PixelFormat::RGB888);

	bga_sprite = Sprite::create();
	bga_sprite->initWithTexture(bga_texture);
	bga_sprite->setContentSize(Size(size_window.width, size_window.height));
	bga_sprite->setPosition(size_window.width / 2, size_window.height / 2);

	this->addChild(bga_sprite);

}

/* ESC Ű ������ �� */
void GameScene::operateESC() {

	/*
	
		���� �������� ���� ���ָ� �ߴ��ϰ� ������ �ɼ� â�� ���.
		���� �ߴ���(�ɼ�â)�� ���� �ɼ�â�� ���ְ� ���ָ� �簳 ��
	
	*/

	/* ȿ���� ���� ��� */
	SimpleAudioEngine::getInstance()->playEffect(SOUND_PAUSE.c_str());

	if (status_playing == PLAY_STATUS::PLAYING) {
		/* �ߴ��ؾ� �� */
		chrono_pauseTime = chrono_time;
		status_playing = PLAY_STATUS::PAUSED;
		for (FMOD::Channel *channel : sound_channel) {
			channel->setPaused(true);
		}
	}
	else if(status_playing == PLAY_STATUS::PAUSED){
		/* �簳�ؾ� �� */
		pause_time_all += pause_time;
		status_playing = PLAY_STATUS::PLAYING;
		for (FMOD::Channel *channel : sound_channel) {
			channel->setPaused(false);
		}
	}

	/* �ɼ�â�� ���������� �����ְ� �����ְ� ������ �� */

}

/* Pause �޴��� �� ��Ȳ�� �´� ���� */
void GameScene::operatePauseMenu() {
	
	/* �ϴ��� �޴��������� �ǵ��ư��� �ۿ� ���� */
	goBackMusicSelectScene();

}

/* �ٽ� ���� ���� ������ ���ư� */
void GameScene::goBackMusicSelectScene() {
	SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGESPEED);
	AudioEngine::stopAll();
	auto musicSelectScene = MusicSelectScene::createScene(background_texture);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, musicSelectScene));
}
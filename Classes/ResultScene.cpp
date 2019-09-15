#include "ResultScene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace Judge;

Score _tpScore;
MusicHeader _info;

//scene ����
Scene* ResultScene::createScene(MusicHeader header, Score score)
{
	_info = header;
	_tpScore = score;
	auto scene = Scene::create();
	auto layer = ResultScene::create();
	scene->addChild(layer);
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

//key Listener
void ResultScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ENTER:
		/* select scene���� �Ѿ�� */
		enterOperate();
		break;
	default:
		break;
	}

}
void ResultScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {

}

//Scene�� ���� init() �Լ� ����
bool ResultScene::init()
{
	CCLOG("ResultScene::init()..");
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	CCLOG("ResultScene::init()..finished!");
	return true;
}

void ResultScene::onEnterTransitionDidFinish() {

	initData();
	setUiInfo();

	enableKeySetting();

}

/* ������ �ε� */
void ResultScene::initData() {

	/* ���� �ʱ�ȭ */
	size_window = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	cache = Director::getInstance()->getTextureCache();

	/* ��, ���ھ� ���� �޾ƿ��� */
	score = _tpScore;
	music_header = _info;

	/* ���ھ� ��� */
	calculateScore();

}

/* ���ھ� ���� ��� */
void ResultScene::calculateScore() {

	/* ���⼭ total_score �� �����Ѵ� */
	total_score = score.getCurrentScore();

}

/* UI ������ �ε� */
void ResultScene::setUiInfo() {

	setLayerBasicLayer();

	/* ���� ���� ���̾� ���� */
	cache->addImage(RECTBACKGROUND_FILENAME);
	rectBackground_sprite = Sprite::createWithTexture(cache->getTextureForKey(RECTBACKGROUND_FILENAME));
	rectBackground_sprite->setAnchorPoint(Point(0, 0));
	rectBackground_sprite->setPosition(0,0);
	rectBackground_sprite->setOpacity(RECTBACKGROUND_OPACITY);
	this->addChild(rectBackground_sprite);

	/* Ÿ��Ʋ �� ���� */
	title_label = Label::createWithTTF(music_header.getValues(MusicHeader::TITLE), UI_LABEL_TITLE_FONT, UI_LABEL_TITLE_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	title_label->setPosition(size_window.width / 2, size_window.height - 100);
	rectBackground_sprite->addChild(title_label);

	/* DjLevel �� ���� */
	djLevel_label = Label::createWithTTF(Judge::DJLEVEL_STR[score.getDjLevel()], UI_LABEL_DJLEVEL_FONT, UI_LABEL_DJLEVEL_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	djLevel_label->setPosition(size_window.width / 4, size_window.height / 16 * 11);
	djLevel_label->setColor(Judge::DJLEVEL_COLOR[score.getDjLevel()]);
	rectBackground_sprite->addChild(djLevel_label);

	/* �� stagefile ��������Ʈ ���� */
	//std::string path = "bms/" + music_header.getValues(MusicHeader::DIR) + "/" + music_header.getValues(MusicHeader::FILENAME);
	//cache->addImage(path);
	//music_sprite = Sprite::createWithTexture(cache->getTextureForKey(path));
	//music_sprite->setPosition(size_window.width / 4 * 3, size_window.height / 16 * 9);
	//music_sprite->setContentSize(Size(256, 256));
	//rectBackground_sprite->addChild(music_sprite);

	/* score �� ���� */
	score_label = Label::createWithTTF(std::to_string(total_score), UI_LABEL_SYSTEM_FONT, UI_LABEL_SYSTEMSCORE_FONTSIZE,
		Size(), TextHAlignment::CENTER, TextVAlignment::CENTER);
	score_label->setPosition(size_window.width / 4, size_window.height / 16*11 - 70);
	rectBackground_sprite->addChild(score_label);

	/* info �� ���� */
	info_label = Label::createWithTTF("SCORE INFORMATION", UI_LABEL_SYSTEM_FONT, UI_LABEL_SYSTEM_FONTSIZE + 10,
		Size(), TextHAlignment::LEFT, TextVAlignment::CENTER);
	info_label->setPosition(size_window.width / 4, size_window.height / 16 * 11 - 90);
	rectBackground_sprite->addChild(info_label);

	/* max �޺� ui �� ���� */
	maxCombo_label_ui = Label::createWithTTF("MAX", UI_LABEL_SYSTEM_FONT, UI_LABEL_SYSTEM_FONTSIZE,
		Size(150, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
	maxCombo_label_ui->setPosition(size_window.width / 4, size_window.height / 2 - 90);
	rectBackground_sprite->addChild(maxCombo_label_ui);

	/* max �޺� �� ���� */
	maxCombo_label = Label::createWithTTF(":   " + std::to_string(score.getMaxCombo()), UI_LABEL_SYSTEM_FONT, UI_LABEL_SYSTEM_FONTSIZE,
		Size(150, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
	maxCombo_label->setPosition(size_window.width / 4 + 150, size_window.height / 2 - 80);
	rectBackground_sprite->addChild(maxCombo_label);

	/* judgeCount �� ���� */
	int i = 0;
	while (i < 5) {
		
		judgeCount_label_ui[i] = Label::createWithTTF(Judge::JUDGE_STR[i], UI_LABEL_SYSTEM_FONT, UI_LABEL_SYSTEM_FONTSIZE,
			Size(150,50), TextHAlignment::LEFT, TextVAlignment::CENTER);
		judgeCount_label_ui[i]->setPosition(size_window.width / 4, maxCombo_label->getPositionY() - ((i + 1) * 30));
		rectBackground_sprite->addChild(judgeCount_label_ui[i]);

		judgeCount_label[i] = Label::createWithTTF(":   " + std::to_string(score.getjudgeCount(i)), UI_LABEL_SYSTEM_FONT, UI_LABEL_SYSTEM_FONTSIZE,
			Size(150, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
		judgeCount_label[i]->setPosition(size_window.width / 4 + 150, maxCombo_label->getPositionY() - ((i + 1) * 30));
		rectBackground_sprite->addChild(judgeCount_label[i]);

		i++;
	}
	
}

/* �⺻ ���̾� ����*/
void ResultScene::setLayerBasicLayer() {
	/*

			�ؽ��ĺ��͸� �� �����Ӵ� ��ȸ�ϸ� ��ü�����ν� �������

	*/

	video_capture.open(BACKGROUND_FILENAME);
	if (!video_capture.isOpened()) {
		CCLOG("can't open video background..");
		return;
	}
	else {
		video_capture >> video_frame;

		/* BGR ���� RGB �÷� ���� */
		cv::cvtColor(video_frame, video_frame, CV_BGR2RGB);

		/* ������ �̸� ĸ���� �����ӵ��� ��� texture ȭ �Ͽ� ���Ϳ� ���� */
		background_texture = new Texture2D();
		background_texture->initWithData(video_frame.data,
			video_frame.elemSize() * video_frame.cols * video_frame.rows,
			Texture2D::PixelFormat::RGB888,
			video_frame.cols,
			video_frame.rows,
			Size(video_frame.cols, video_frame.rows));
		video_capture >> video_frame;

		/* ��� ��������Ʈ �ʱ�ȭ �� ���� */
		background_sprite = Sprite::create();
		background_sprite->initWithTexture(background_texture);
		background_sprite->setContentSize(size_window);
		background_sprite->setPosition(size_window.width / 2, size_window.height / 2);
		background_sprite->setOpacity(100);

		/* �� ƽ�� ������ �Լ� ȣ���ؼ� ���� texture ���� */
		this->schedule(schedule_selector(ResultScene::background_tick), 1.0 / 30);

		/* �⺻ ���̾ ��� ��������Ʈ ���*/
		this->addChild(background_sprite);

	}


}

/* ��� ������ ���*/
void ResultScene::background_tick(float interval) {
	/* ���� �������� ����ĸ�Ŀ��� ������ */
	video_capture >> video_frame;
	if (!video_frame.empty()) {

		/* BGR ���� RGB �÷� ���� */
		cv::cvtColor(video_frame, video_frame, CV_BGR2RGB);

		/* ���� �������� MAT ���Ϸ� �ؽ��ĸ� ������Ʈ �Ѵ� */
		background_texture->updateWithData(video_frame.data, 0, 0, video_frame.cols, video_frame.rows);
	}
	else {
		/*

			��浿���� ���� ����

		*/
		video_capture.open(BACKGROUND_FILENAME);
	}
}

// Ű ����
void ResultScene::enableKeySetting() {
	//Ű ������ ����.
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(ResultScene::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(ResultScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);
}

/* Enter Ű ������ �� ���� */
void ResultScene::enterOperate() {

	/* �����, �� ���� ������ ������ �� */
	
	/* �ӽ� - ���ư��� */
	auto musicSelectScene = MusicSelectScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, musicSelectScene));

}
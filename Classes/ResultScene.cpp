#include "ResultScene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace Judge;

Score _tpScore;

//scene ����
Scene* ResultScene::createScene(Score score)
{
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
		/* ���� ���� ���۷���Ʈ */

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

	/* ���ھ� ���� �޾ƿ��� */
	score = _tpScore;

}

/* UI ������ �ε� */
void ResultScene::setUiInfo() {

	setLayerBasicLayer();


	
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
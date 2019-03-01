#include "PauseScene.h"

USING_NS_CC;
using namespace CocosDenshion;

//scene ����
Scene* PauseScene::createScene()
{
	auto scene = Scene::create();
	auto layer = PauseScene::create();
	scene->addChild(layer);\
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

//key Listener
void PauseScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ENTER:
		/* ���� ���� ���۷���Ʈ */
		
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		/* �ٽ� ���� �簳 - �ǵ��ư��� */
		Director::getInstance()->popScene();
		break;
	default:
		break;
	}

}
void PauseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {

}

//Scene�� ���� init() �Լ� ����
bool PauseScene::init()
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

void PauseScene::onEnterTransitionDidFinish() {

	// �ε�ȭ�� �����ֱ�
	enableKeySetting();
}


// Ű ����
void PauseScene::enableKeySetting() {
	//Ű ������ ����.
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(PauseScene::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(PauseScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);
}
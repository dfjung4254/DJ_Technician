#ifndef RESULTSCENE_H
#define RESULTSCENE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Score.h"
#include "opencv2/opencv.hpp"

USING_NS_CC;

class ResultScene : public cocos2d::Layer
{
private:
	//--------------------------------------------------------------------------------
	// �⺻ ����
	Size size_window;												// ���� �������� ������
	TextureCache *cache;											// �ؽ��� ĳ�ð�����
	Sprite *cursor_sprite;											// Ŀ�� ��������Ʈ
	const std::string CURSOR_FILENAME = "images/cursor_arrow.png";	// Ŀ�� ���ϰ��
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		���� ��� opencv ���� �� �⺻ ���̾� ����
		- ����� ����.
		- ������ StartScene���� �Ѿ�� ���� vector<Texture2D> �� �״�� �Ѱܹ޾� ����Ѵ�.
		- StartScene �� �����ϰ� schedule�� ���� �� ƽ�� iter�� ���� background_sprite�� ������Ʈ ���ش�.
		- �߰��� ���� �簢�� Ʋ�� ����� �������.

	*/

	const std::string BACKGROUND_FILENAME = "video/background8.mpg";// ������ �����̸� ( 20 �� �̳��� ���ϸ� ���!!)
	Texture2D* background_texture;									// ĸ���� 1�������� ���� texture
	Sprite *background_sprite;										// ĸ���� texture�� ���� ��� sprite
	cv::VideoCapture video_capture;									// ������ ĸ�İ�ü
	cv::Mat video_frame;											// ������ 1������
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		���� ���� ����

	*/
	Score score;													// �⺻ ���ھ� - GameScene���� �Ѱܹ޴´�.
	//--------------------------------------------------------------------------------

public:

	static cocos2d::Scene* createScene(Score score);

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event *event);
	void enableKeySetting();

	/* setting */
	void initData();
	void setUiInfo();
	void setLayerBasicLayer();
	void background_tick(float interval);

	// implement the "static create()" method manually
	CREATE_FUNC(ResultScene);
};

#endif
#ifndef RESULTSCENE_H
#define RESULTSCENE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Score.h"
#include "opencv2/opencv.hpp"
#include "MusicHeader.h"
#include "MusicSelectScene.h"

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

		����, �� ���� ����

	*/
	MusicHeader music_header;										// �� ��� ����
	int total_score;												// ��Ż ���ھ� ����
	Score score;													// �⺻ ���ھ� - GameScene���� �Ѱܹ޴´�.
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		UI ���� ����

	*/

	const std::string UI_LABEL_TITLE_FONT
		= "fonts/LuluMonospace.ttf";								// UI Ÿ��Ʋ ��Ʈ
	const int UI_LABEL_TITLE_FONTSIZE = 60;							// UI Ÿ��Ʋ ��Ʈ ������
	const std::string UI_LABEL_DJLEVEL_FONT
		= "fonts/swagger.ttf";										// UI djLevel ��Ʈ
	const int UI_LABEL_DJLEVEL_FONTSIZE = 250;						// UI djLevel ��Ʈ ������
	const std::string UI_LABEL_SYSTEM_FONT
		= "fonts/nanumLight.ttf";									// UI �Ϲݽý��� ��Ʈ
	const int UI_LABEL_SYSTEM_FONTSIZE = 20;						// UI �Ϲݽý��� ��Ʈ ������
	const int UI_LABEL_SYSTEMSCORE_FONTSIZE = 40;					// UI �Ϲݽý��� ���ھ� ��Ʈ ������

	const std::string RECTBACKGROUND_FILENAME
		= "images/result_background.png";							// ���� ��� ��������Ʈ ����
	Sprite *rectBackground_sprite;									// ���� ��� ��������Ʈ
	const int RECTBACKGROUND_OPACITY = 200;							// ���� ��� ����

	Sprite *music_sprite;											// �� ��������Ʈ
	Label *title_label;												// �� ���� ��
	Label *djLevel_label;											// djLevel ��
	Label *info_label;												// �� ���� ��
	Label *score_label;												// ���� ��
	Label *maxCombo_label;											// �ƽ��޺� ��
	Label *maxCombo_label_ui;										// �ƽ��޺� ui ��
	Label *judgeCount_label[5];										// ����ī��Ʈ ��
	Label *judgeCount_label_ui[5];									// ����ī��Ʈ ui ��

	//--------------------------------------------------------------------------------

public:

	static cocos2d::Scene* createScene(MusicHeader header, Score score);

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

	/* function */
	void calculateScore();
	void enterOperate();

	// implement the "static create()" method manually
	CREATE_FUNC(ResultScene);
};

#endif
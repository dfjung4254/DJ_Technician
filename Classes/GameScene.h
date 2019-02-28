#ifndef	GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
#include "opencv2/opencv.hpp"
#include "BMSParser.h"
#include <chrono>
#include <fmod.hpp>

USING_NS_CC;


class GameScene : public cocos2d::Layer
{
private:
	//--------------------------------------------------------------------------------
	// ���� ������ ������
	Size size_window;									// ���� �������� ������
	TextureCache *cache;								// �ؽ��� ĳ�ð�����
	int status_screenMode;								// ���� ȭ�� ���
	enum SCREEN {
		WINDOW, FULLSCREEN
	};
	//--------------------------------------------------------------------------------
	/* ���� ���� ���� */
	const std::string SOUND_CHANGESPEED
		= "sound/change2.wav";							// ��� ��ȯ ����
	const std::string SOUND_READY
		= "sound/robot_ready.wav";						// �κ� �غ� ����
	const std::string SOUND_GO
		= "sound/robot_go.wav";							// �κ� ���� ����
	const std::string SOUND_PAUSE
		= "sound/pause.wav";							// �ߴ� ����
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		�����Է��� ���Ϳ� ��� �����ؼ� �޴� ���� �ƴϴ�.
		GameScene ���� �����Է����� � �̵������ ���� �� �ʿ䰡 ����.

		�ٸ� �� ��Ʈ �ν��� ���� Flag �� �ּ� onKeyPress�� ������ onKeyRelease�� �ߵ��ϱ�
		������ Flag �� True�� �д�.

	*/
	bool status_keyUsing;								// Ű ��� ����(�׼����� �� Ű�Է� ����)
	bool status_keyPressing[50];						// Ű ���� ���� ����(1 - 7 Ű ����)
	enum KEY {
		S = 1, D, F, SPACE, J, K = 8, L = 9, ALT
	};
	//--------------------------------------------------------------------------------
	// ���� �� ���� (���� MusicSelectScene ���� �ѱ��)
	std::string status_bmsName;							// ���� bms ���� �̸�
	std::string status_dirs;							// ���� bms ���͸� �̸�
	float status_speed;									// ���� �� ���(���氡��)
	double status_bpm;									// ���� �� BPM
	double first_bpm;									// ���� �� ���� BPM
	int status_bpm_tick;								// ������� BPM �︰ Ƚ��
	double status_next_tick_time;						// ���� ƽ�� �Ǿ�� �� �ð�

	std::chrono::system_clock::time_point time_start;	// �� ���� �ð�
	std::chrono::system_clock::time_point bga_start;	// BGA ���� �ð�
	std::chrono::duration<double> time_music;			// �� ���� �ð�
	std::chrono::duration<double> time_bga;				// BGA ���� �ð�

	int score_noteSize;									// ��ü ��Ʈ ����(Max �޺� ��)

	std::vector<NOTE::Note>::iterator note_iter_latest;	// ���� �ֽ� ��Ʈ
	int bar_iter_latest;								// ���� �ֽ� �����ȣ
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		���� ���� ���� ����

	*/
	std::chrono::time_point
		<std::chrono::system_clock> chrono_time;		// ƽ�� ���� ���� �ð� ����
	std::chrono::duration<double> pause_time;			// �Ͻ� ���� �ð�
	std::chrono::duration<double> pause_time_all;		// �� �Ͻ� ���� ���� �ð�
	std::chrono::time_point
		<std::chrono::system_clock> chrono_pauseTime;	// ���� �ߴ� ���� �ð� ����
	double currentTime;									// ���� ���� ���� �ð�
	double currentTime_bga;								// ����ä�� bga ������ ���� �ð�
	double yPosOffset;									// ��ũ�� �Ÿ� ����
	double prevTime;									// ���� ƽ�� �ð�
	int currentBarIdx;									// ���� ���� ���� ����
	double offsetPosY;									// ��Ʈ ��ġ ��� offset
	double offsetBar;									// ���� ���� offset
	double offsetBpm;									// BPM �� ��� offset
	int thisKeyChannel;									// ���� ���� Ű�� ���� ä�ΰ�
	bool status_autoPlay;								// �ڵ� ���� ���
	int status_playing;									// ���� ���� ����
	enum PLAY_STATUS {
		NOSTART, PLAYING, PAUSED
	};
	const double STARTEMPTYTIME = 2.0f;					// �����ϰ� �� ��Ʈ �ð�
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		���� ���� ����

	*/
	const float JUDGE[5] = {
		0.021f, 0.084f, 0.147f, 0.21f, 0.273f
	};													// ���� ����
	const std::string JUDGE_STR[5] = {
		"PERFECT", "GREAT",  "GOOD", "BAD", "MISS"
	};													// ���� STRING ��
	enum JUDGE {
		PERFECT, GREAT, GOOD, BAD, MISS
	};
	const std::string DJLEVEL_STR[9] = {
		"F", "E", "D", "C", "B", "A", "AA", "AAA", "S"
	};
	const Color3B DJLEVEL_COLOR[9] = {
		Color3B(50,50,50), Color3B(100, 100, 100), Color3B(150,150,150),
		Color3B(200,200,200), Color3B(200,200,0), Color3B(255,150,0),
		Color3B(255,100,0), Color3B(255, 50, 0), Color3B(255, 0, 0)
	};
	enum DJLEVEL {
		RANK_F, RANK_E, RANK_D, RANK_C, RANK_B, RANK_A, RANK_AA, RANK_AAA, RANK_S
	};
	Label *combo_label;									// ���� �޺� ��
	Label *combo_label_ui;								// ���� �޺� UI ��
	Label *judge_label;									// ���� ����
	Label *judge_time_label;							// ���� �����ð� ��
	
	Label *score_currentScore_label;					// ���� ���� ��
	const int UI_LABEL_CURRENTSCORE_X = 117;			// ���� ���� �� X
	const int UI_LABEL_CURRENTSCORE_Y = 45;				// ���� ���� �� Y
	const std::string UI_LABEL_CURRENTSCORE_FONT
		= "fonts/Teko-Bold.ttf";						// ���� �� BPM ��Ʈ
	const int UI_LABEL_CURRENTSCORE_FONTSIZE = 20;		// ���� �� BPM ��Ʈ������

	Label *score_currentCombo_label;					// ���� �޺� ��

	Label *score_maxCombo_label;						// ���� �ƽ��޺� ��
	const int UI_LABEL_MAXCOMBO_X = 1085;				// �ƽ� �޺� �� X
	const int UI_LABEL_MAXCOMBO_Y = 45;					// �ƽ� �޺� �� Y
	const std::string UI_LABEL_MAXCOMBO_FONT
		= "fonts/Teko-Bold.ttf";						// ���� �� BPM ��Ʈ
	const int UI_LABEL_MAXCOMBO_FONTSIZE = 20;			// ���� �� BPM ��Ʈ������

	Label *score_count_label[5];						// ���� ����
	const int UI_LABEL_COUNT_X = 578;					// ���� ���� X
	const int UI_LABEL_COUNT_Y = 295;					// ���� ���� Y

	Label *score_countUi_label[5];
	const int UI_LABEL_COUNTUI_X = 408;					// ���� ���� UI X	
	const int UI_LABEL_COUNTUI_Y = 295;					// ���� ���� UI Y

	Label *score_djLevel_label;							// DJ Level ��
	const int UI_LABEL_DJLEVEL_X = 470;					// DJ Level X
	const int UI_LABEL_DJLEVEL_Y = 118;					// DJ Level Y
	const std::string UI_LABEL_DJLEVEL_FONT
		= "fonts/Teko-Bold.ttf";						// ���� �� BPM ��Ʈ
	const int UI_LABEL_DJLEVEL_FONTSIZE = 60;			// ���� �� BPM ��Ʈ������

	const std::string COMBO_FONT
		= "fonts/Teko-Light.ttf";						// �޺� ��Ʈ
	const int COMBO_FONTSIZE = 200;						// �޺� ��Ʈ������
	const std::string COMBO_UI_FONT
		= "fonts/Teko-Light.ttf";						// �޺� ui ��Ʈ
	const int COMBO_UI_FONTSIZE = 50;					// �޺� ui ��Ʈ������
	const std::string JUDGE_FONT
		= "fonts/Teko-Light.ttf";						// ���� ��Ʈ
	const int JUDGE_FONTSIZE = 100;						// ���� ��Ʈ������
	const int JUDGE_TIME_FONTSIZE = 25;					// ���� Ÿ�� ��Ʈ������
	const std::string SYSTEMS_FONT
		= "fonts/Teko-Bold.ttf";						// �ý��� ��Ʈ
	const int SYSTEMS_FONTSIZE = 22;					// �ý��� ��Ʈ������
	const float COMBO_ACTIONTIME = 0.02f;				// �޺� �׼� �ð�
	const float COMBO_ACTIONDELAYTIME = 1.0f;			// �޺� ���� �ð�
	int score_currentScore;								// ���� ����
	int score_currentCombo;								// ���� �޺�
	int score_maxCombo;									// �ִ� �޺�
	int score_judgeCount[5];							// ���� ����
	int score_currentSize;								// ���� ���� ��Ʈ ��
	int score_djLevel;									// ���� DJ Level
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		���� ��� ���� ����

	*/
	const std::string UI_TEXTURE_NOBGA
		= "images/noImage2.png";						// BGA ������ ��ü �ؽ���
	bool status_isVideo[1296];							// ���� ä���� �������� �̹������� ����
	Sprite *bga_sprite;									// BGA ���� ��������Ʈ
	Texture2D *bga_texture;								// BGA ���� �ؽ���
	cv::VideoCapture video_capture;						// ������ ĸ�İ�ü
	cv::Mat video_frame;								// ������ 1������
	int status_bga;										// ���� ä�� �ε���
	int status_bgaCh;									// ���� ä�� ���� ���� �ε���

	//--------------------------------------------------------------------------------
	// ���� ���� ����(FMOD)
	FMOD::System *sound_system;							// fmod ���� �ý���
	FMOD::Channel *sound_channel[1296];					// ä�� ����(�ϴ��� �� ����key���� ���� ä�� �����غ���)
	std::vector<FMOD::Sound*> v_sound_sound;			// ���� ����(��Ʈ�� wav �� �������� ���带 �Ҵ��Ѵ�)
	//--------------------------------------------------------------------------------
	// �� UI ����
	const std::string UI_SPRITE_KEYPRESSFILE[10] = {		// Ű Ŭ�� ��������Ʈ 
		"",
		"images/game_note_background_key1.png",
		"images/game_note_background_key2.png",
		"images/game_note_background_key3.png",
		"images/game_note_background_key4.png",
		"images/game_note_background_key5.png",
		"images/game_note_background_key0.png",
		"images/game_note_background_key0.png",
		"images/game_note_background_key6.png",
		"images/game_note_background_key7.png",
	};
	const std::string UI_SPRITE_BACKGROUND
		= "images/game_background_hd.png";				// ��ü ��� ��������Ʈ ����
	const std::string UI_SPRITE_NOATEBACKGROUND
		= "images/game_note_background.png";			// ��Ʈ ��� ��������Ʈ ����
	const std::string UI_SPRITE_SMALLNOTE
		= "images/game_note_small.png";					// ���� ��Ʈ ��������Ʈ ����
	const std::string UI_SPRITE_LARGENOTE
		= "images/game_note_large.png";					// ū ��Ʈ ��������Ʈ ����
	const std::string UI_SPRITE_MIDNOTE
		= "images/game_note_mid.png";					// �߰� ��Ʈ ��������Ʈ ����
	const std::string UI_SPRITE_BOMB
		= "images/bomb2.png";							// ��Ʈ ����Ʈ
	const int UI_SPRITE_ANIM_BOMB_WIDTH = 181;			// ����Ʈ ���� ������
	const int UI_SPRITE_ANIM_BOMB_HEIGHT = 192;			// ����Ʈ ���� ������
	const std::string UI_SPRITE_EQUALIZER
		= "images/equalizer.png";						// ���������� ����Ʈ
	const std::string UI_SPRITE_BAR
		= "images/bars.png";							// ���� ��������Ʈ ����
	const std::string UI_SPRITE_JUDGEBAR
		= "images/judgeBar.png";						// ������ ��������Ʈ ����
	const std::string UI_SPRITE_TURNTABLE
		= "images/turntable.png";						// �����̺� ��������Ʈ ����
	const int UI_SPRITE_TURNTABLE_X = 87;				// �����̺� ��������Ʈ ������ġ
	const int UI_SPRITE_TURNTABLE_Y = 107;				// �����̺� ��������Ʈ ������ġ
	const std::string UI_SPRITE_TURNBAR
		= "images/turnbar.png";							// �� �� ��������Ʈ ����
	const int UI_SPRITE_TURNBAR_X = 443;				// �� �� ��������Ʈ ������ġ
	const int UI_SPRITE_TURNBAR_Y = 370;				// �� �� ��������Ʈ ������ġ
	const int OPACITY_NOTE_SPRITE_BACKGROUND = 225;		// ��Ʈ ��� ���� - BGA ������ �ڿ� �󸶳� ������ ���ΰ�
	const int OPACITY_JUDGEBAR = 255;					// ������ ����
	const int JUDGE_HEIGHT = 5;							// ���� ���� ����

	Label *label_time_music;							// �� ���� �ð� ǥ�� ��
	const int UI_LABEL_TIMEMUSIC_X = 688;				// �� ���� �ð� ǥ�� X
	const int UI_LABEL_TIMEMUSIC_Y = 27;				// �� ���� �ð� ǥ�� Y
	const std::string UI_LABEL_TIMEMUSIC_FONT
		= "fonts/Teko-Bold.ttf";						// ���� �� BPM ��Ʈ
	const int UI_LABEL_TIMEMUSIC_FONTSIZE = 20;			// ���� �� BPM ��Ʈ������

	Label *label_speed;									// ���� �� ��� ��
	const int UI_LABEL_SPEED_X = 338;					// ���� �� ��� X
	const int UI_LABEL_SPEED_Y = 27;					// ���� �� ��� Y
	const std::string UI_LABEL_SPEED_FONT
		= "fonts/Teko-Bold.ttf";						// ���� �� BPM ��Ʈ
	const int UI_LABEL_SPEED_FONTSIZE = 20;				// ���� �� BPM ��Ʈ������

	Label *label_bpm;									// ���� �� BPM ��
	const int UI_LABEL_BPM_X = 453;						// ���� �� BPM X
	const int UI_LABEL_BPM_Y = 19;						// ���� �� BPM Y
	const std::string UI_LABEL_BPM_FONT
		= "fonts/Teko-Bold.ttf";						// ���� �� BPM ��Ʈ
	const int UI_LABEL_BPM_FONTSIZE = 20;				// ���� �� BPM ��Ʈ������

	Label *label_bar;									// ���� �� Bar ��
	Sprite *ui_sprite_background;						// ��ü ��� ��������Ʈ
	Sprite *note_sprite_background;						// ��Ʈ ��� ��������Ʈ
	Sprite *keyPressed_sprite[10];						// �� �������� ��������Ʈ
	Sprite *bomb_sprite[10];							// ����� ������ �� ��ź ��������Ʈ
	Sprite *equalizer_sprite;							// ���������� ��������Ʈ
	Sprite *bars_sprite[1000];							// ���� ��������Ʈ
	Sprite *judgeBar_sprite;							// ������ ��������Ʈ
	Sprite *turntable_sprite;							// �����̺� ��������Ʈ
	Sprite *turnbar_sprite;								// �� �� ��������Ʈ
	//--------------------------------------------------------------------------------
	// ��Ʈ ����
	std::vector<NOTE::Note> notes;						// ��Ʈ ����(Űä�� 11 ~ 19)
	double notes_barLength[1000];						// ���� ���� ����(�̺�Ʈä�� 02)
	double notes_barPosition[1000];						// �� ����� ��ġ ����

	const int ZORDER_NOTES = 4;							// ��Ʈ�� z order
	const int ZORDER_LAYOUT = 1;						// ���̾ƿ��� z order
	const int ZORDER_NOTEBACKGROUND = 2;				// ��Ʈ ����� z order
	const int ZORDER_BARS = 3;							// ��Ʈ ������ z order
	const int ZORDER_LABEL = 5;							// ���� z order

	const int KEY_SIZE = 7;								// Ű ������ (7 Ű�� �����Ѵ�)

	Layer *layer_notes;									// ��Ʈ ���̾�(��Ʈ ��������Ʈ ���ϰ�)
	const int LAYER_POSITIONX = 127;					// ��Ʈ ���̾� �ʱ� x ��ġ
	const int LAYER_POSITIONY = 135;					// ��Ʈ ���̾� �ʱ� y ��ġ
	const int LAYER_WIDTH = 259;						// ��Ʈ ���̾� ���� ������
	const int LAYER_HEIGHT = 584;						// ��Ʈ ���̾� ���� ������

	const int NOTE_LARGEWIDTH = 37;						// �� ū��Ʈ ���� ������
	const int NOTE_SMALLWIDTH = 29;						// �� ������Ʈ ���� ������
	const int NOTE_HEIGHT = 10;							// �� ��Ʈ ���� ������

	int bar_length = 250;								// �� ���� ����(�ӽ� : 300 - ��Ӱ� bpm �� ���� �����ؾ���)
	const int BAR_MAXSIZE = 10000;						// �ִ� �����

	enum {
		CHANNEL_NOTE_KEYS1 = 11,						// Ű ��ȣ ä�� (1 - 7)
		CHANNEL_NOTE_KEYS2 = 12,
		CHANNEL_NOTE_KEYS3 = 13,
		CHANNEL_NOTE_KEYS4 = 14,
		CHANNEL_NOTE_KEYS5 = 15,
		CHANNEL_NOTE_KEYS6 = 18,
		CHANNEL_NOTE_KEYS7 = 19,
		CHANNEL_TRANS_KEYS1 = 31,						// ���� Ű ��ȣ ä�� (1 - 7)
		CHANNEL_TRANS_KEYS2 = 32,
		CHANNEL_TRANS_KEYS3 = 33,
		CHANNEL_TRANS_KEYS4 = 34,
		CHANNEL_TRANS_KEYS5 = 35,
		CHANNEL_TRANS_KEYS6 = 38,
		CHANNEL_TRANS_KEYS7 = 39,
		CHANNEL_EVENT_BGM = 1,							// BGM ��� �̺�Ʈä��
		CHANNEL_EVENT_SHORTENBAR = 2,					// ���� ���� �̺�Ʈä��
		CHANNEL_EVENT_CHANGEBPM = 3,					// BPM ��ȭ �̺�Ʈä��
		CHANNEL_EVENT_BGA = 4,							// BGA �����ֱ� �̺�Ʈä��
		CHANNEL_EVENT_POORBGA = 6,						// MISS �� BGA �����ֱ� �̺�Ʈä��
		CHANNEL_EVENT_STOPSEQUENCE = 9					// ������Ʈ ���� �̺�Ʈä��
	};



	//--------------------------------------------------------------------------------
	// BMS �Ľ� ����
	const int STR_CHANNELSIZE = 1296;					// ä�� ������(36���� 2�ڸ�)
	double str_bpmValue[1296];							// BPM Ȯ�� ä�� ����
	int str_stopValue[1296];							// STOP Ȯ�� ä�� ����
	std::string str_wavFile[1296];						// WAV ���� ����(36���� 2�ڸ� 36*36)
	std::string str_bmpFile[1296];						// BMP ���� ����(36���� 2�ڸ�)

	//--------------------------------------------------------------------------------

public:

	static cocos2d::Scene* createScene(std::string musicName, std::string bmsName, float speed, bool autoPlay);

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event *event);
	void releaseKeyInput(float dt);
	void showLoadingState();
	void enableKeySetting();
	void initData();
	void setNotes();
	void parseBMS();
	void setUiInfo();
	void gameStart();
	void tickOperate(float interval);
	void operateNoteKey(NOTE::Note &note);
	void changeSpeed(bool isSpeedUp);
	void operateKeyEffect(int keyNo);
	void endKeyEffect(float interval, int keyNo);
	void operateComboEffect(std::vector<NOTE::Note>::iterator cur_note);
	int calculateCombo(double judgeTime);
	void tickOperateBGA();
	void loadBGA();
	void loadTexture();
	void operateESC();
	void operatePauseMenu();
	void goBackMusicSelectScene();
	void releaseData();
	void changeScreenMode();
	void setFullscreen();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif
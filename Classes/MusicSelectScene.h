#pragma execution_character_set("utf-8")

#include "cocos2d.h"
#include "opencv2/opencv.hpp"
#include "GameScene.h"

USING_NS_CC;

/*

	�� ����ȭ�� �� ���� �����ִµ� ����� ��������
	����ü�� �����Ѵ�.

*/
struct MusicSelectObject {
	Sprite *stagefile_sprite;			// Ÿ��Ʋ ���� ������ ��.
	Label *title_label;					// �� ���� ������ ��(title + subtitle)
	Label *rank_label;					// �� ���̵� ������ �� 
};

class MusicSelectScene : public cocos2d::Layer, public TextFieldDelegate
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
	// ���� ���� ����
	const std::string BGMMAIN_FILENAME
		= "bgm/musicSelect_Bossfight.mp3";							// ���� BGM
	const float BGMMAIN_VOLUME = 0.2f;								// ���� BGM ����
	const char SOUND_CHANGELAYER[30] = "sound/init.mp3";			// ��� ���� �Ҹ�
	const char SOUND_CHANGELOGIN[30] = "sound/select.wav";			// �α��� ��Ŀ�� ��ȯ �Ҹ�
	const char SOUND_TICK[30] = "sound/tick.wav";					// ��ȯ �Ұ� �Ҹ�
	const char SOUND_CHANGEMUSICSELECT[30] = "sound/change.wav";	// �� ���� ��ȯ �Ҹ�
	const char SOUND_SELECT[30] = "sound/select2.wav";				// ������ ��ȯ �Ҹ�
	const char SOUND_BACK[30] = "sound/back.wav";					// ������ �ڷΰ��� �Ҹ�
	//--------------------------------------------------------------------------------
	/*

		BMS �Ľ� ���� ����

	*/
	std::vector<MusicHeader> musicHeaders;							// ��� ���� ��� ���� ����
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*
		
		Layer ���� ����.
		�� ���� ���� �帧�� ������ ����.
		
		1. �ε�ȭ�� �����ֱ�
		-> (Thread�� ����� bms ������ �а� Callback �� ���� �ε��� �����ؾ� �Ѵ�.)
		-> (Main Thread�� �ε� ���̾� UI �ִϸ��̼��� �����ֵ��� �Ѵ�.)
		-> (bms/ ���������� ��� bms, bme, bml ������ ����� �а� ���� ������ vector<MusicHeader>�� �����Ѵ�.)
		-> (�� �� STAGEFILE �̹����� cache�� �̸� �ε��� ���´�.)
		-> (�� �ļ��� 7Key �� �����ϹǷ� 5Ű�� 14Ű ���� bms������ ������ ���� ���� �����Ѵ�.)
		-> �Ľ��� vector<MusicHeader> �� �������� �⺻ ��������Ʈ�� �� �� ���̾� ������ setup.
		
		2. �ε��� ������ �ε� ���̾ ����� �޴� ���̾ �����ش�.
		-> 240x160 ������ ������ ������(���δ� �� Ÿ��Ʋ2�ٱ��� 160+a ) (1280 ���� �� ���� 5�� ��)
		-> ���� ���õ� ������ ���Ϳ� ����.

		3. �̵��ؼ� ���� �����ϸ� � ���� ������ ���̾ �����ش�.


	*/
	int status_layer;												// ���̾� ����
	enum STATUS {
		MUSICSELECT,
		MUSICINFO
	};
	Layer *layer_musicSelect;										// �� ���� ���̾� 
	Layer *layer_subBar;											// ���� �� �ϴܹ�
	Layer *layer_musicInfo;											// �� ������ ���̾�
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		���� ��� opencv ���� �� �⺻ ���̾� ����
		- ����� ����.
		- ������ StartScene���� �Ѿ�� ���� vector<Texture2D> �� �״�� �Ѱܹ޾� ����Ѵ�.
		- StartScene �� �����ϰ� schedule�� ���� �� ƽ�� iter�� ���� background_sprite�� ������Ʈ ���ش�.
		- �߰��� ���� �簢�� Ʋ�� ����� �������.

	*/
	std::vector<Texture2D*> background_texture;						// ĸ���� 1�������� ���� texture
	std::vector<Texture2D*>::iterator background_iter;				// ��� vector iterator
	Sprite *background_sprite;										// ĸ���� texture�� ���� ��� sprite
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// �ε� �� ����
	const std::string LOADING_STR = "LOADING..";					// �ε� �ؽ�Ʈ
	const std::string LOADING_FONT = "fonts/LuluMonospace.ttf";		// �ε� ��Ʈ
	const int LOADING_FONTSIZE = 50;								// �ε� ��Ʈ ������
	Label *loading_label;											// �ε� ��
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*
	
		�� ���� �޴� ���̾�
		��������Ʈ ĳ�� �ε�� std::vector<MusicHeaders> musicHeaders �� BMS ��� ������
		push�� �Ϸ�� �� musicHeaders ���� musicHeaders.getValue(STAGEFILE) �� �����´�.

		��������Ʈ �ؿ� ���� child�� �ٿ��ش�
	
	*/
	int status_musicSelect;											// ���� ���õ� �� �ε���
	const float MSO_ACTIONTIME_CHANGE = 0.15f;						// ��ȯ �ִϸ��̼� �ð�

	int widthCount;													// �� ���� ���� ����
	const int MSO_OPACITY_UNSELECTED = 50;							// ���õ��� ���� ������Ʈ�� ����
	const int MSO_SPRITE_WIDTH = 240;								// �� ���� ��������Ʈ ���� ������
	const int MSO_SPRITE_HEIGHT = 160;								// �� ���� ��������Ʈ ���� ������
	const int MSO_SPRITE_WIDTHDIFF = 30;							// �� ���� ��������Ʈ ���� ����
	const std::string MSO_NOIMAGEFILE = "images/noImage.png";		// �� ���� ��������Ʈ NOIMAGE 
	const int MSO_LABEL_WIDTH = 240;								// �� ���� �� ���� ������
	const int MSO_LABEL_HEIGHT = 100;								// �� ���� �� ���� ������
	const int MSO_LABEL_TITLEPOSH = -60;							// �� ���� �� ���� ��ġ(��������Ʈ�� �����)
	const int MSO_LABEL_LEVELPOSH = -100;							// �� ���� �� ���� ��ġ(��������Ʈ�� �����)
	const std::string MSO_FONT = "fonts/LuluMonospace.ttf";			// �� ���� ��Ʈ
	const int MSO_FONTSIZE = 15;									// �� ���� ��Ʈ ������
	std::vector<MusicSelectObject> musicSelectObject;				// �� ���� ����
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		�� ���� �� �ϴ� ������ ���̾�

		ȭ�� ��/�ϴܿ� ���� �ٸ� �׸���
		���������� ǥ���Ѵ�.

		+ ��������
		 1. ���� �ε��� / ��ü �� ����
		 2. ���� ���� + �������� ��
		 3. ���� ���õ� ����
		 4. ȯ�� ����
		 5. ����

		 #�ϴ� ���� �⺻���� 1,2 �� ����� ���Ŀ� �߰�.

	*/
	const std::string SUBBAR_TOPFILE = "images/subBar_top.png";		// ���� ��ܹ� �����̸�
	const std::string SUBBAR_BTMFILE
		= "images/subBar_bottom.png";								// ���� �ϴܹ� �����̸�
	Sprite *subBarTop_sprite;										// ���� ��ܹ� ��������Ʈ
	Sprite *subBarBtm_sprite;										// ���� �ϴܹ� ��������Ʈ
	const std::string SUBBAR_INDEX_FONT = "fonts/swagger.ttf";		// �ε��� ��Ʈ
	const int SUBBAR_INDEX_FONTSIZE = 20;							// �ε��� ��Ʈ ������
	Label *subBar_label_curIdx;										// ����� ���� �ε���
	Label *subBar_label_maxIdx;										// ����� ��ü �ε���
	const std::string SUBBAR_TITLE_FONT = "fonts/LuluMonospace.ttf";// Ÿ��Ʋ ��Ʈ
	const int SUBBAR_TITLE_FONTSIZE = 30;							// Ÿ��Ʋ ��Ʈ ������
	Label *subBar_label_title;										// ����� ����� ����
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	/*

		�� ������ ���̾�
		���� ������ ���� 8���� ������ �����ش�.
		PLAYER, GENRE, TITLE, ARTIST, BPM, PLAYLEVEL, RANK, TOTAL

	*/
	const std::string MUSICINFO_BACKBOARDFILE
		= "images/music_info.png";									// �� ���� ���� �����̸�
	Sprite *musicInfo_sprite_backBoard;								// �� ���� ���� ��������Ʈ
	Label *musicInfo_label_ui[8];									// �� ���� UI��
	Label *musicInfo_label_info[8];									// �� ���� Info ��
	const int MUSICINFO_LEFTPADDING = 100;							// �� ���� UI ���� �е� ��
	const std::string MUSICINFO_LABELSTR[8] = {
		"PLAYER", "GENRE", "TITLE", "ARTIST", "BPM", "PLAYLEVEL", "RANK", "TOTAL"
	};
	enum MUSICINFO_LABEL {
		PLAYER, GENRE, TITLE, ARTIST, BPM, PLAYLEVEL, RANK, TOTAL
	};
	const std::string MUSICINFO_FONT
		= "fonts/LuluMonospace.ttf";								// �� ���� ��Ʈ
	const int MUSICINFO_FONTSIZE = 15;								// �� ���� ��Ʈ ������


	//--------------------------------------------------------------------------------


public:

	static cocos2d::Scene* createScene(std::vector<Texture2D*> v);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();

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
	void setup();
	void showLoading();
	void setBmsList();
	void enableKeySetting();
	void setLayerBasicLayer();
	void background_tick(float interval);
	void setLayerMusicSelect();
	void setLayerSubBar();
	void setLayerMusicInfo();
	/* ----------------------------------------------------------------------------*/

	/* --------------------------- SHOW FUNCTIONS ---------------------------------*/
	void startBGM(float interval);
	void showMusicSelect();
	void showMusicInfo();
	void hideMusicInfo();
	void changeMusicSelect(int from, int to);
	void startGame();
	/* ----------------------------------------------------------------------------*/
	// implement the "static create()" method manually
	CREATE_FUNC(MusicSelectScene);
};

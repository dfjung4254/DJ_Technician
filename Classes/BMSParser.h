#pragma execution_character_set("utf-8")

#include "cocos2d.h"
#include "Note.h"
#include "MusicHeader.h"

/*

	BMS �Ľ� Ŭ���� .
	��� BMS ���� �а� �ε� �ϴ� �۾���
	�� Ŭ�������� �Ѵ�

	�̱��� �������� getInstance�� ���� ȣ���ؼ� ���.

*/

class BMSManager
{
private:
	/* �̱��� ���� */
	BMSManager() {}
	static bool instanceFlag;
	static BMSManager* instance;

	/* �Ľ� ���� */
	/* �Ľ� ���� ���� */
	const int BPMEXTEND_PUNC = 36;							// BPM Ȯ�� ä�� ����(36���� �����) 0 - Z
	const int BPM_PUNC = 16;								// BPM ���� (16���� �����)0 - F
	const int STOP_FUNC = 36;								// STOP ä�� ����(36���� �����) 0 - Z
	const int BMP_PUNC = 36;								// BMP ����(36���� �����) 0 - Z
	const int BAR_PUNC = 10;								// ���� ����(10 ����)
	const int CHANNEL_PUNC = 16;							// ä�� ����(16 ����)
	const int WAV_PUNC = 36;								// WAV ����(36���� �����) 0 - Z

public:
	/* �̱��� ���� */
	static BMSManager* getInstance();
	virtual ~BMSManager() { instanceFlag = false; };

	/* BMS �������� ��� �а� �����ϱ� */
	static void readAllBmsHeader(std::vector<MusicHeader> *musicHeaders);
	static void readBms(std::string dirs, std::string fileName, double &status_bpm,
		double str_bpmValue[], std::string str_wavFile[], std::string str_bmpFile[], int str_stopValue[],
		std::vector<NOTE::Note> *notes, double notes_barLength[]);

	/* �Ľ̿� �Լ� */
	std::string readLineFromIndex(char ch[], int idx);
	std::vector<int> readDataLineFromIndex(char ch[], int idx, int punc);
	bool isDataSection(char ch[]);
	int changeCharToInteger(char ch[], int idx);
	int getBarNumber(char ch[]);
	int getChannelNumber(char ch[], int idx, int punc);
	void setNoteFromTmpVector(std::vector<int> st, int bar_no, std::vector<NOTE::Note> *notes,
		char obj, int key);

};
#ifndef MUSICHEADER_H
#define MUSICHEADER_H

#pragma execution_character_set("utf-8")

#include "cocos2d.h"

/*

	BMS �� ��������� �о �����ϴ� Ŭ����
	MusicHeader �� �ϳ��� .bms ���Ͽ� ���� ��������� ������.

*/

class MusicHeader {
private:

	/* ��� ���� */
	std::string values[12];			/* values�� �� �����Ѵ�. */

public:
	/* ������ */
	MusicHeader(){}

	/* �Ҹ��� */
	~MusicHeader(){}

	/* ��� ���� */
	const int valueSize = 12;		/* ��ü ������� ���� */
	enum {
		DIR, FILENAME, PLAYER, GENRE, TITLE, ARTIST, BPM, PLAYLEVEL,
		RANK, SUBTITLE, TOTAL, STAGEFILE
	};

	/* ��� ���� setter & getter */
	std::string getValues(int valueIdx);
	void setValues(int valueIdx, std::string val);

};

#endif
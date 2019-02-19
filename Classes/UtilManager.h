#ifndef UTILMANAGER_H
#define UTILMANAGER_H

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

class UtilManager
{
private:
	/* �̱��� ���� */
	UtilManager() {}
	static bool instanceFlag;
	static UtilManager* instance;


public:
	/* �̱��� ���� */
	static UtilManager* getInstance();
	virtual ~UtilManager() { instanceFlag = false; };

	

};

#endif
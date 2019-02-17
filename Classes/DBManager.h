#pragma execution_character_set("utf-8")

#include "cocos2d.h"

/* 

	DB ���� ���� �Ŵ���.
	��� ȸ������ �� �α��� ������
	�� Ŭ�������� �Ѵ�

	�̱��� �������� getInstance�� ���� ȣ���ؼ� ���.

*/

class DBManager  
{
private:
	/* �̱��� ���� */
	DBManager() {}
	static bool instanceFlag;
	static DBManager* instance;
	
public:
	/* �̱��� ���� */
	static DBManager* getInstance(); 
	virtual ~DBManager(){ instanceFlag = false; };

	const static int NOTOKEN = -1;
	int checkLoginInfo(std::string id, std::string pw);


	
};

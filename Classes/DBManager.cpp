#include "DBManager.h"

/* �̱��� ���� */
bool DBManager::instanceFlag = false;
DBManager* DBManager::instance = nullptr;
DBManager* DBManager::getInstance() {
	if (!instance) {
		instance = new DBManager();
		instanceFlag = true;
	}
	return instance;
}

/* �α��� ���� Ȯ�� */
int DBManager::checkLoginInfo(std::string id, std::string pw) {
	/* 
	
		�Ѿ�� id �� pw ������ �̿��� �� ���� DB�� ��ȸ�ϰ�
		�ش� ������ ���� ��ū���� �Ѱ��ش�.
	
	*/

	


	/* ���� �ؾ��� */



	return 1;
}
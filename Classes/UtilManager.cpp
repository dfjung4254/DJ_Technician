#include <io.h>
#include "UtilManager.h"

/* �̱��� ���� */
bool UtilManager::instanceFlag = false;
UtilManager* UtilManager::instance = nullptr;
UtilManager* UtilManager::getInstance() {
	if (!instance) {
		instance = new UtilManager();
		instanceFlag = true;
	}
	return instance;
}


#include "Score.h"

using namespace Judge;

/* ��Ʈ ���� ���� */
void Score::plusNoteSize() {
	noteSize++;
	updateOffset();
}

void Score::updateOffset() {
	double b = 100000.0 / noteSize;
	double v = 50000.0 / (10 * noteSize - 55);
}

/* �־��� �����ð��� ���� ������ �����ϰ� ������ �����Ѵ� */
int Score::calculateCombo(double judgeTime) {

	/* ���� �� */
	int judgeNo;

	/* ���� ������ ���� */
	double plusScore = 0;

	if (judgeTime >= -JUDGE[JUDGE::PERFECT] && judgeTime <= JUDGE[JUDGE::PERFECT]) {
		/* ����Ʈ +- 0.15f �̳� */
		judgeNo = JUDGE::PERFECT;
		currentCombo = (currentCombo == maxCombo)
			? maxCombo = currentCombo + 1 : currentCombo + 1;

		/* ���� ���� */
		int offset = (currentCombo <= 10) ? currentCombo - 1 : 10;
		plusScore = 1.5 * b + offset * v;

	}
	else if (judgeTime >= -JUDGE[JUDGE::GREAT] && judgeTime <= JUDGE[JUDGE::GREAT]) {
		/* �׷���Ʈ */
		judgeNo = JUDGE::GREAT;
		currentCombo = (currentCombo == maxCombo)
			? maxCombo = currentCombo + 1 : currentCombo + 1;

		/* ���� ���� */
		int offset = (currentCombo <= 10) ? currentCombo - 1 : 10;
		plusScore = 1.0 * b + offset * v;
	}
	else if (judgeTime >= -JUDGE[JUDGE::GOOD] && judgeTime <= JUDGE[JUDGE::GOOD]) {
		/* �� +- 0.3f �̳� */
		judgeNo = JUDGE::GOOD;
		currentCombo = (currentCombo == maxCombo)
			? maxCombo = currentCombo + 1 : currentCombo + 1;

		/* ���� ���� */
		int offset = (currentCombo <= 10) ? currentCombo - 1 : 10;
		plusScore = 0.2 * b + offset * v;
	}
	else if (judgeTime >= -JUDGE[JUDGE::BAD] && judgeTime <= JUDGE[JUDGE::BAD]) {
		/* ��� +- 0.4f �̳� */
		judgeNo = JUDGE::BAD;
		currentCombo = 0;
	}
	else if (judgeTime < -JUDGE[JUDGE::BAD]) {
		/* �̽� - 0.4f �ʰ� */
		judgeNo = JUDGE::MISS;
		currentCombo = 0;
	}

	judgeCount[judgeNo]++;
	currentScore += plusScore;
	currentSize++;

	int ex_score = judgeCount[JUDGE::PERFECT] * 2 + judgeCount[JUDGE::GREAT];
	int max_score = currentSize * 2;
	double level_score = (double)ex_score / max_score;
	if (level_score >= 8.5 / 9) {
		djLevel = 8;
	}
	else {
		djLevel = level_score * 9;
	}

	return judgeNo;
}
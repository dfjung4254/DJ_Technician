#ifndef SCORE_H
#define SCORE_H

#pragma execution_character_set("utf-8")

#include "cocos2d.h"

USING_NS_CC;

/*

	���ӿ��� ����Ǵ� ���� ������ �����ϴ� Ŭ����

*/

namespace Judge {
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
}

class Score {
private:

	/* ��� ���� */
	int currentScore;								// ���� ����
	int currentCombo;								// ���� �޺�
	int maxCombo;									// �ִ� �޺�
	int judgeCount[5];								// ���� ����
	int currentSize;								// ���� ���� ��Ʈ ��
	int noteSize;									// ��ü ��Ʈ ����(Max �޺� ��)
	int djLevel;									// ���� DJ Level	

	/* ���� ���� offset */
	double b;
	double v;

public:
	/* ������ */
	Score() {}

	/* �Ҹ��� */
	~Score() {}

	/* func */
	void plusNoteSize();							// ��ü ��Ʈ ���� �������� �ϳ��� ���Ѵ�.
	int calculateCombo(double judgeTime);			// �־��� �����ð��� �������� ���������� �����ϰ� �������� �����Ѵ�
	void updateOffset();

	/* getter */
	int getCurrentScore() {
		return currentScore;
	}
	int getCurrentCombo() {
		return currentCombo;
	}
	int getMaxCombo() {
		return maxCombo;
	}
	int getjudgeCount(int idx) {
		return (idx >= 5 || idx < 0) ? -1:judgeCount[idx];
	}
	int getCurrentSize() {
		return currentSize;
	}
	int getDjLevel() {
		return djLevel;
	}
	int getNoteSize() {
		return noteSize;
	}
	double getB() {
		return b;
	}
	double getV() {
		return v;
	}

	/* setter */
	void setCurrentScore(int currentScore) {
		this->currentScore = currentScore;
	}
	void setCurrentCombo(int currentCombo) {
		this->currentCombo = currentCombo;
	}
	void setMaxCombo(int maxCombo) {
		this->maxCombo = maxCombo;
	}
	void setJudgeCount(int idx, int judgeCount) {
		if (idx >= 5 || idx < 0) {
			return;
		}
		this->judgeCount[idx] = judgeCount;
	}
	void setCurrentSize(int currentSize) {
		this->currentSize = currentSize;
	}
	void setDjLevel(int djLevel) {
		this->djLevel = djLevel;
	}
	void setNoteSize(int noteSize) {
		this->noteSize = noteSize;
	}
	void setB(double b) {
		this->b = b;
	}
	void setV(double v) {
		this->v = v;
	}

};


#endif
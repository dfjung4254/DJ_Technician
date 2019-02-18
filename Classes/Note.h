#pragma execution_character_set("utf-8")

#include "cocos2d.h"

//USING_NS_CC;

/*

	BMS �� ������ ���� ������ �о �����ϴ� Ŭ����
	Note �� Ű��

*/

namespace NOTE {

	/*

		��Ʈ Ŭ������ ���õ� ���� ���� ��

	*/

	/* note_object */
	const char OBJ_EVENT = '0';
	const char OBJ_NOTE = '1';

	/* note_key */
	enum {
		KEY_BGM = 0,
		KEY_1 = 1,
		KEY_2 = 2,
		KEY_3 = 3,
		KEY_4 = 4,
		KEY_5 = 5,
		KEY_SCRATCH = 6,
		KEY_PAD = 7, /* no-use */
		KEY_6 = 8,
		KEY_7 = 9,
		KEY_STOP = 10,
		KEY_TRANS1 = 11,
		KEY_TRANS2 = 12,
		KEY_TRANS3 = 13,
		KEY_TRANS4 = 14,
		KEY_TRANS5 = 15,
		KEY_TRANSSCRATCH = 16,
		KEY_TRANSPAD = 17,
		KEY_TRANS6 = 18,
		KEY_TRANS7 = 19,
		KEY_BPM = 20,
		KEY_BPMEXTEND = 21,
		KEY_BGA = 22
	};

	const std::string STR_KEY[23] = {
		"KEY_BGM",
		"KEY_1",
		"KEY_2",
		"KEY_3",
		"KEY_4",
		"KEY_5",
		"KEY_SCRATCH",
		"KEY_PAD",
		"KEY_6",
		"KEY_7",
		"KEY_STOP",
		"KEY_TRANS1",
		"KEY_TRANS2",
		"KEY_TRANS3",
		"KEY_TRANS4",
		"KEY_TRANS5",
		"KEY_TRANSSCRATCH",
		"KEY_TRANSPAD",
		"KEY_TRANS6",
		"KEY_TRANS7",
		"KEY_BPM",
		"KEY_BPMEXTEND",
		"KEY_BGA"
	};

	const double NOTE_POS_X[23] = {
		282,											// BGM Ű x �� ��ǥ
		19,												// 1�� Ű x �� ��ǥ
		55,												// 2�� Ű x �� ��ǥ
		92,												// 3�� Ű x �� ��ǥ
		131,											// 4�� Ű x �� ��ǥ
		167,											// 5�� Ű x �� ��ǥ
		-80,											// ��ũ��ġ Ű x �� ��ǥ
		-100,											// �̻��
		205,											// 6�� Ű x �� ��ǥ
		242,											// 7�� Ű x �� ��ǥ
		362,											// STOP Ű x �� ��ǥ
		410,											// 1�� ���� Ű x �� ��ǥ
		410,											// 2�� ���� Ű x �� ��ǥ
		410,											// 3�� ���� Ű x �� ��ǥ
		410,											// 4�� ���� Ű x �� ��ǥ
		410,											// 5�� ���� Ű x �� ��ǥ
		410,											// ��ũ��ġ ���� Ű x �� ��ǥ
		420,											// �̻��
		410,											// 6�� ���� Ű x �� ��ǥ
		410,											// 7�� ���� Ű x �� ��ǥ
		322,											// BPM Ű x �� ��ǥ
		322,											// BPM Ȯ�� Ű x �� ��ǥ
		322												// BGA ���� Ű x �� ��ǥ
	};

	class Note {
	private:

		/* ��� ���� */

	public:
		/* ������ */
		Note() {}

		/* �Ҹ��� */
		~Note() {}

		/* ��� ���� */

		/*

			object ���� EVENT �̸� note_key �� ä�� �Ӽ��� ��Ÿ����
			 1 : bgm
			 2 : �������           - ���� ������ ��� Note �� �ƴ϶� ���� GameScene ���� �迭�� �����Ѵ�.
			 3 : bpm ����
			 4 : bga
			   ...
			 8 : bpm extend ����
			 9 : stop

			object ���� NOTE �̸� note_key �� Ű ��ȣ�� ��Ÿ����.
			 1 - 5 : 1 - 5�� Ű
			 6 : ��ũ��ġ
			 7 : ���(���x)
			 8, 9 : 8, 9�� Ű

		*/
		char note_object;
		int note_key;

		/*

			��Ʈ�� ������ �ִ� ���� ä�ΰ��̴�.
			note_channel�� �밳 �̸� ������ �迭�� �ε����� ��Ÿ�� ���� �ְ� ���� ���� ���� ���� ����

			 ���� ��Ʈ��

			   1. bgm ä�� �϶�(01)
				- note_channel : str_wav[] �迭�� �ε����� ��Ÿ�� (36���� 2�ڸ��� ����ϹǷ� �ִ� �ε����� 36*36 = 1296)
				- str_wav[] �� BMSParser �� ���ϰ� �ִ� ������ #WAVxx ���� �о���� ���������� ���ϸ��� ������ ����
				- str_wav[] �� �ε����� GameScene���� FMOD Sound[] ���� �ε��� ���������� �ε����͵� ��ġ��.
				- �� BGM �� �����Ҷ� FMOD_sound[note_channel] �� ���带 ����ϸ� ��.

			   2. bpm ���� ä�� �϶� (03)
				- note_channel : �ٲپ���� BPM ���� ���� (16���� 2�ڸ��� ����ϹǷ� 1 - 255 BPM ����)

			   3. bga ä�� �϶� (04)
				-

			   4. bpm extend ä�� �϶� (08)
				- note_channel : str_bpm[] �迭�� �ε����� ��Ÿ�� ���� str_bpm[note_channel] �� �ٲٿ����� BPM ���� ����.

		*/
		int note_channel;

		/*

			�ش� ��Ʈ�� ��Ʈ���̴�.
			��Ʈ�� ���� 4.5�̸� 4��° ���� ���������� ��Ʈ�� ��ġ���� ��Ÿ����.

		*/

		double note_beat;
		double note_time;
		double note_posy;
		cocos2d::Sprite *sprite;
		bool isChecked;

		/* Operator ������ */
		bool operator < (const Note &n) {
			return note_beat < n.note_beat;
		}

	};
}
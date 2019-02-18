#include <io.h>
#include "BMSParser.h"

/* �̱��� ���� */
bool BMSParser::instanceFlag = false;
BMSParser* BMSParser::instance = nullptr;
BMSParser* BMSParser::getInstance() {
	if (!instance) {
		instance = new BMSParser();
		instanceFlag = true;
	}
	return instance;
}

/* BMS ��� ���� �б� - ���� �� ��ü �� ���� ��ĵ */
void BMSParser::readAllBmsHeader(std::vector<MusicHeader> *musicHeaders) {
	/* 
	
		Bms ���� ������ �ִ� ��� ���͸��� �����Ͽ� bms, bme, bml ���� ����� �а�
		������ MusicHeader�� ���·� �����Ͽ� ���Ϳ� �������ش�.

		�� �Լ��� MusicSelectScene���� �� ���� ȭ���� �����ֱ� ���� �̸� ������ �ִ�
		��� ���� ��� ������ �� �ε���Ű�µ� ���ȴ�.
	
	*/

	/* �Ѿ�� ���͸� �ʱ�ȭ */
	musicHeaders->clear();

	/* �ӽ� ���͸� ��� ������ ���� */
	std::vector<std::string> bmsDirs;
	std::vector<std::pair<std::string, std::string >> bmsFiles;

#pragma region BMS ���� ���͸� ��ĵ �� ���͸� �� bmsDirs ����

#ifdef _DEBUG
	//CCLOG("BMSParser : scanning bmsDirs...");
#endif

	/* bms ���� ���͸��� ��� �������� �� ã�� bmsDirs�� �ϴ� �ִ´�. */
	_finddata_t fd;
	long handle;
	int result = 1;
	/* BMS ������ ���� bms������ �� ã�� */
	handle = _findfirst("bms/*", &fd);

	if (handle == -1)
	{
		CCLOG("file read failed");
		return;
	}

	while (result != -1)
	{
		bmsDirs.push_back(fd.name);
		//CCLOG("%s", fd.name);
		result = _findnext(handle, &fd);
	}

	_findclose(handle);

	/* . �� .. ������ ���� */
	bmsDirs.erase(bmsDirs.begin());
	bmsDirs.erase(bmsDirs.begin());

#ifdef _DEBUG
	//CCLOG("BMSParser : scanning bmsDirs... - finished");
#endif

#pragma endregion

#pragma region �� ���͸��� �湮�ϸ鼭 bms, bml, bme ������ ã�´�.

	auto iter = bmsDirs.begin();
	while (iter != bmsDirs.end()) {
		/* bms/curDirectory/*.* �� �� �а� bms, bml, bme Ȯ���ڸ� �ɷ�����. */
		std::string curDirectory = *iter;
		std::string path = "bms/" + curDirectory + "/*.*";
		iter++;
		_finddata_t fdd;
		result = 1;
		/* bms ������ curDirectory ���� ���ϵ� �� ã�� */
		handle = _findfirst(path.c_str(), &fdd);

		if (handle == -1)
		{
			CCLOG("file read failed");
			return;
		}

		while (result != -1)
		{

			std::string tpFileName = fdd.name;
			if (tpFileName.find(".bms", 0, 4) != std::string::npos
				|| tpFileName.find(".bme", 0, 4) != std::string::npos
				|| tpFileName.find(".bml", 0, 4) != std::string::npos) {
				/* bms ������ */
				bmsFiles.push_back(make_pair(curDirectory, tpFileName));
				//CCLOG("%s", tpFileName.c_str());
			}
			result = _findnext(handle, &fdd);
		}
		_findclose(handle);

	}

#pragma endregion

#pragma region �� bms, bml, bme ������ ���� vector<MusicHeader> �� �����Ѵ�.

	auto iter2 = bmsFiles.begin();
	while (iter2 != bmsFiles.end()) {
		/* ���� ������ ��ο� �����̸� */
		std::string curDir = iter2->first;
		std::string curFile = iter2->second;
		std::string path = "bms/" + curDir + "/" + curFile;
		iter2++;

		/* ���� ������ �����Ͽ� �д´�. */
#ifdef _DEBUG
		//CCLOG("*********** %s file open start ********", path.c_str());
#endif
		MusicHeader curHeader;

		/* ���� �̸��� ���� �̸� ���� ���� */
		curHeader.setValues(MusicHeader::DIR, curDir);
		curHeader.setValues(MusicHeader::FILENAME, curFile);

		FILE *fp = fopen(path.c_str(), "r");
		int inHeaderSection = 0;
		char ch[1024];
		while (!feof(fp)) {
			fgets(ch, sizeof(ch), fp);
			if (ch[0] != '#') {
				if (ch[0] == '*') {
					/* ��� ������ ���� ���� �б� �ߴ� */
					if (++inHeaderSection >= 2) {
						break;
					}
				}
				else {
					continue;
				}
			}
			std::string readValue;
			int idx = -1;
			int valIdx = 0;
			if (strncmp(ch, "#WAV", 4) == 0) {
				/* ������ǿ��� WAV ������ ���� �ʴ´�. */
				continue;
			}
			else if (strncmp(ch, "#BMP", 4) == 0) {
				/* ������ǿ��� BMP ������ ���� �ʴ´�. */
				continue;
			}
			else if (strncmp(ch, "#PLAYER", 7) == 0) {
				idx = 8;
				valIdx = MusicHeader::PLAYER;
			}
			else if (strncmp(ch, "#GENRE", 6) == 0) {
				idx = 7;
				valIdx = MusicHeader::GENRE;
			}
			else if (strncmp(ch, "#TITLE", 6) == 0) {
				idx = 7;
				valIdx = MusicHeader::TITLE;
			}
			else if (strncmp(ch, "#ARTIST", 7) == 0) {
				idx = 8;
				valIdx = MusicHeader::ARTIST;
			}
			else if (strncmp(ch, "#BPM", 4) == 0) {
				idx = 5;
				valIdx = MusicHeader::BPM;
			}
			else if (strncmp(ch, "#PLAYLEVEL", 10) == 0) {
				idx = 11;
				valIdx = MusicHeader::PLAYLEVEL;
			}
			else if (strncmp(ch, "#RANK", 5) == 0) {
				idx = 6;
				valIdx = MusicHeader::RANK;
			}
			else if (strncmp(ch, "#TOTAL", 6) == 0) {
				idx = 7;
				valIdx = MusicHeader::TOTAL;
			}
			else if (strncmp(ch, "#STAGEFILE", 10) == 0) {
				idx = 11;
				valIdx = MusicHeader::STAGEFILE;
			}
			else if (strncmp(ch, "#SUBTITLE", 9) == 0) {
				idx = 10;
				valIdx = MusicHeader::SUBTITLE;
			}
			else {
				//CCLOG("music Parse Failed %s", ch);
			}
			if (idx != -1) {
				while (ch[idx] != '\n') {
					readValue += ch[idx++];
				}
				/* �� �ش��ϴ� idx �� ���� ���� �����Ѵ�. */
				curHeader.setValues(valIdx, readValue);
#ifdef _DEBUG
				//CCLOG("valIdx %d : %s", valIdx, curHeader.getValues(valIdx).c_str());
#endif
			}
		}
		fclose(fp);

		/* �� �а� ������ ���� �� MusicHeader�� vector�� �����Ѵ�. */
		musicHeaders->push_back(curHeader);

#ifdef _DEBUG
		//CCLOG("*********** %s file open finished!! ********", path.c_str());
#endif

	}

#pragma endregion

}

/* '\n' ������ �б� */
std::string BMSParser::readLineFromIndex(char ch[], int idx) {

	/*
	
		ch �� �־��� �Է� ������
		idx�� ���� ������ �о �����Ѵ�.
	
	*/

	std::string ret;

	while (ch[idx] != '\n') {
		ret += ch[idx++];
	}

	return ret;
}

/* '\n' ������ ������ ���� �б� */
std::vector<int> BMSParser::readDataLineFromIndex(char ch[], int idx, int punc) {

	/*
	
		������ ������ �� ��Ʈ������ �а� ä�ΰ��� ���Ϳ� �ӽ������Ͽ� ��ȯ�Ѵ�.
	
	*/

	std::vector<int> st;

	while (ch[idx] != '\n') {
		int channel = getChannelNumber(ch, idx, punc);
		idx += 2;
		// wav ���� ���־����
		st.push_back(channel);
	}

	return st;
}

/* ���� ������ ������ ��Ʈ�����ͷ� ����� */
void BMSParser::setNoteFromTmpVector(std::vector<int> st, int bar_no,
	std::vector<NOTE::Note> *notes, char obj, int key) {

	/* �� ä���� ��Ʈ�� = �����ȣ + �����ε��� / ��ü������ */
	int stackSize = st.size();
	int inIdx = 0;

	/* �ٽ� ������ ��ȸ�ϸ� ���� ��Ʈ ������ �Է��Ѵ�. */
	for (int channel : st) {
		/* 00 �� ä�ΰ��� ���� ������ �����Ѵ�. ��ü ��Ʈ�� �����ϱ� ���� �뵵�θ� ���� */
		if (channel != 0) {

			/* ���ο� ��Ʈ ������ �Է��ϰ� ���Ϳ� ���� */
			NOTE::Note note = NOTE::Note();
			note.note_object = obj;
			note.note_key = key;
			note.note_channel = channel;
			note.note_beat = bar_no + ((double)inIdx / stackSize);
			note.isChecked = false;
			note.note_time = 0;
			note.note_posy = 0;
			note.sprite = cocos2d::Sprite::create();

			notes->push_back(note);

#ifdef _DEBUG
			NOTE::Note tpNote = notes->back();
			CCLOG(" * read Note Info : \n obj:%c / key:%s / ch:%d / beat:%lf", tpNote.note_object, NOTE::STR_KEY[tpNote.note_key].c_str(), tpNote.note_channel, tpNote.note_beat);
#endif // _DEBUG

		}
		inIdx++;
	}

}

/* �о���� ������ ������ �������� �Ǻ� */
bool BMSParser::isDataSection(char ch[]) {
	/*
	
		�о���� ������ 1��° �ε����� �����̸� HeaderSection
						   �ε����� 0-9 ���� �����̸� DataSection
	
	*/
	return (ch[1] >= '0' && ch[1] <= '9');
}

/* char - ���� ���� ��ȯ */
int BMSParser::changeCharToInteger(char ch[], int idx) {

	/*
	
		���� char ������ ���ڰ����� �ٲپ��ش�.
		char ����� �����̸� - '0' ���ְ�
		char ����� ����(10���� �̻��� ��) �̸� -'7' ���־�
		�ش� char ������ ���� ���� ���� ���Ѵ�.
	
	*/

	return (ch[idx] > '9') ? ch[idx] - '7' : ch[idx] - '0';

}

/* ���� �� ���� ��ȣ ���� */
int BMSParser::getBarNumber(char ch[]) {

	/*
	
		�Է� ���� 1,2,3��°�� 10������ �ִ� 3�ڸ� ���� ǥ���Ǿ� �ִ�.
		���ϰ��� �ϴ� ���� #XXX.. �� ��ȯ
	
	*/

	int ch1tp = changeCharToInteger(ch, 1);
	int ch2tp = changeCharToInteger(ch, 2);
	int ch3tp = changeCharToInteger(ch, 3);

	return ch1tp * (BAR_PUNC*BAR_PUNC) + ch2tp * BAR_PUNC + ch3tp;
}

/* ���� �� ä�� ��ȣ ���� */
int BMSParser::getChannelNumber(char ch[], int idx, int punc) {

	/*
	
		���ϰ��� �ϴ� ä�ΰ� #...XX ���� �־��� ������ ��ȯ�ؼ� ��ȯ ��.
	
	*/

	return changeCharToInteger(ch, idx) * punc + changeCharToInteger(ch, idx + 1);
}

/* BMS ��ü ���� �б� */
void BMSParser::readBms(std::string dirs, std::string fileName, double &status_bpm,
	double str_bpmValue[], std::string str_wavFile[], std::string str_bmpFile[], int str_stopValue[],
	std::vector<NOTE::Note> *notes, double notes_barLength[]) {

	/*
	
		���õ� ���� BMS ������ �а� ������ �Է��Ѵ�.
	
	*/

	/* filename Ȯ���ڸ� ã���� ��� ������ filePath���� read */
	std::string filePath = "bms/" + dirs + "/" + fileName;

#ifdef _DEBUG
	CCLOG("*********** %s file open start ********", filePath.c_str());
#endif

	FILE *fp = fopen(filePath.c_str(), "r");
	char ch[1024];
	while (!feof(fp)) {

		/* �� �� (�ִ� 1024) �о�� */
		fgets(ch, sizeof(ch), fp);

		/* bms ������ #���� �����ϴ� �͸� �д´�. */
		if (ch[0] != '#') {
			continue;
		}

		/* �Ľ̿� �ε��� ���� */
		int idx = -1;

		if (!getInstance()->isDataSection(ch)) {

#pragma region :: ��� �Ľ� ����

			/*

				��� ���� �Ľ�

			*/

#pragma region :: BPM �Ľ� ����
			if (strncmp(ch, "#BPM ", 5) == 0) {
				idx = 5;
				status_bpm = std::stod(getInstance()->readLineFromIndex(ch, idx));

#ifdef _DEBUG
				CCLOG("status_bpm = %lf", status_bpm);
#endif

			}
#pragma endregion
#pragma region :: BPM Ȯ�� ä�� �Ľ� ����
			/* #BPM 179 �� �ƴ϶� # BPMxx 222 ���� ä�ΰ��� ��� �ִ� ������ ��� */
			else if (strncmp(ch, "#BPM", 4) == 0) {
				idx = 7;
				int bpm_channel = getInstance()->getChannelNumber(ch, 4, getInstance()->BPMEXTEND_PUNC);
				str_bpmValue[bpm_channel] = std::stod(getInstance()->readLineFromIndex(ch, idx));

#ifdef _DEBUG
				CCLOG("str_bpmValue[%d] = %lf", bpm_channel, str_bpmValue[bpm_channel]);
#endif

			}
			// ########################################
#pragma endregion
#pragma region :: WAV �Ľ� ����
			else if (strncmp(ch, "#WAV", 4) == 0) {
				idx = 7;
				int wav_channel = getInstance()->getChannelNumber(ch, 4, getInstance()->WAV_PUNC);
				str_wavFile[wav_channel] = getInstance()->readLineFromIndex(ch, idx);

#ifdef _DEBUG
				CCLOG("str_wavFile[%d] = %s", wav_channel, str_wavFile[wav_channel].c_str());
#endif

			}
#pragma endregion
#pragma region :: BMP �̹��� �Ľ� ����
			else if (strncmp(ch, "#BMP", 4) == 0) {
				idx = 7;
				int bmp_channel = getInstance()->getChannelNumber(ch, 4, getInstance()->BMP_PUNC);
				str_bmpFile[bmp_channel] = getInstance()->readLineFromIndex(ch, idx);

#ifdef _DEBUG
				CCLOG("str_bmpFile[%d] = %s", bmp_channel, str_bmpFile[bmp_channel].c_str());
#endif

			}
#pragma endregion
#pragma region :: STOP �Ľ� ����
			else if (strncmp(ch, "#STOP", 5) == 0) {
				idx = 8;
				int stop_channel = getInstance()->getChannelNumber(ch, 4, getInstance()->STOP_FUNC);		
				str_stopValue[stop_channel] = std::stoi(getInstance()->readLineFromIndex(ch, idx));

#ifdef _DEBUG
				CCLOG("str_stopFile[%d] = %d", stop_channel, str_stopValue[stop_channel]);
#endif

			}

#pragma endregion
#pragma region :: �ļ��� ���� ��� ������ ��
			else {

#ifdef _DEBUG
				CCLOG("music Parse Failed %s", ch);
#endif

			}
#pragma endregion

#pragma endregion

		}
		else {

#pragma region :: ������ �Ľ� ����
			/*

				������ ���� �Ľ�
				�ϴ� �ش� ������ ���� ������ ������ �ӽ� ���ÿ� �ְ�
				�ٽ� ������ ���鼭 �� ������ �ε��� / ������ ������ �� ���� ��Ʈ �Ҽ��� ������ ���Ѵ�.
				�̷����ϸ� ���� 16, 32��Ʈ ������ �ִ� ���� �ƴ϶� �ƹ��� ��Ʈ�� �ɰ��
				�� ������ �����ϴ�.

			*/

			/*

				TODO : ������ ������ ������ ¦������ üũ�ϴ� �Լ� ���� üũ���־�� ��

			*/

			/* ���� ���� ���� ��ȣ ���� */
			int bar_no = getInstance()->getBarNumber(ch);

			/* �����͸� �ӽ� ������ ���� ���� */
			std::vector<int> st;
			
			/* 7��° �ε������� �����Ϳ��� ���� */
			idx = 7;

			/*

				ä�� �Ӽ� ����
				���� ���� 4�� �ε����� 0,1,3 �� ����
				
			*/
			switch (ch[4]) {
			case '0':
#pragma region :: �̺�Ʈ ä���� ���

				if (ch[5] == '1') {
#pragma region :: BGM ��� ä�� "01"

					/*

						BGM ä���� �Ϲ� ��Ʈ�� ���� ����ؼ� bpm �̳� ������̿� ������ �޴´�.(��Ȯ�� Ȯ�� �ʿ�)
						���� �Ϲ� notes ���Ϳ��� ���� ��ũ�� �ϰ� time �� ����ϵ� �ڵ����� ó�����ش�.

					*/

					/* ������ä���� 2���� �Ľ��Ͽ� �� ��Ʈ�� ä�ΰ��� ���Ϳ� �����Ѵ�. */
					st = getInstance()->readDataLineFromIndex(ch, idx, getInstance()->WAV_PUNC);

					/* ���� ������ ������� ��Ʈ ������ ���� �� ���� �Ѵ�. */
					getInstance()->setNoteFromTmpVector(st, bar_no, notes, NOTE::OBJ_EVENT, NOTE::KEY_BGM);

#pragma endregion
				}
				if (ch[5] == '2') {
#pragma region :: ���� ���� ä�� "02"

					/*

						���� ���� ä�� �� ���
						��������� ��Ʈ ������ �ƴ϶� �迭�� ���� ������ ���´�.

					*/

					notes_barLength[bar_no] = std::stod(getInstance()->readLineFromIndex(ch, idx));

#pragma endregion					
				}
				if (ch[5] == '3') {
#pragma region :: BPM ���� ä�� "03"

					/*

						BPM ä���� �Ϲ� ��Ʈ�� ���� ����ؼ� bpm �̳� ������̿� ������ �޴´�.
						���� �Ϲ� notes ���Ϳ��� ���� ��ũ�� �ϰ� time �� ����ϰ� BPM �� �������ش�.

					*/

					st = getInstance()->readDataLineFromIndex(ch, idx, getInstance()->BPM_PUNC);
					getInstance()->setNoteFromTmpVector(st, bar_no, notes, NOTE::OBJ_EVENT, NOTE::KEY_BPM);

#pragma endregion
				}
				if (ch[5] == '8') {
#pragma region :: BPM ���� Ȯ�� ä�� "08"

					/*

						BPM Ȯ�� ä���� �Ϲ� ��Ʈ�� ���� ����ؼ� bpm �̳� ������̿� ������ �޴´�.
						���� �Ϲ� notes ���Ϳ��� ���� ��ũ�� �ϰ� time �� ����ϰ� BPM �� �������ش�.
						�� �̶�, ������ ��Ʈ�� ä�ΰ��� BPM �� ����Ű�°� �ƴ϶� BPM Ȯ�� ä�� �迭�� �ε�����
						��Ÿ����. ���� "03" �������� �ٷ� ä�ΰ����� BPM ���� ������� ������ "08"�� ���
						�ش� str_bpm[notes_channel] �� ������ BPM ���� �������־�� �Ѵ�.

					*/

					st = getInstance()->readDataLineFromIndex(ch, idx, getInstance()->BPMEXTEND_PUNC);
					getInstance()->setNoteFromTmpVector(st, bar_no, notes, NOTE::OBJ_EVENT, NOTE::KEY_BPMEXTEND);

#pragma endregion
				}
				if (ch[5] == '8') {
#pragma region :: ������ STOP ä�� "09"

					/*

						������ ���� ä���� ���� �Ϲ� ��Ʈ�� ���� ���.
						�� �ش� ��Ʈ�� ��� str_stop[] ä���� ����ŭ ������Ų��.

					*/

					st = getInstance()->readDataLineFromIndex(ch, idx, getInstance()->STOP_FUNC);
					getInstance()->setNoteFromTmpVector(st, bar_no, notes, NOTE::OBJ_EVENT, NOTE::KEY_STOP);

#pragma endregion
				}
				if (ch[5] == '4') {
#pragma region BGA ���� ä�� "04"

					/*

						BGA ���� ä���� �Ϲ� ��Ʈ�� ���� ����ؼ� bpm �̳� ������̿� ������ �޴´�.
						���� �Ϲ� notes ���Ϳ��� ���� ��ũ�� �ϰ� time �� ����ϰ� BGA �� �������ش�.
						�� �̶�, ������ ��Ʈ�� ä�ΰ��� BGA ���ϸ��� ����Ű�°� �ƴ϶� BGM Ȯ�� ä�� �迭�� �ε�����
						��Ÿ����. ���� "08"�� ���� ����
						�ش� str_bmp[notes_channel] �� ������ v_bga_bga[notes_channel] BGA �ؽ��� �Ǵ� ��������
						�ε����ְ� v_bga_bga[notes_channel] �� �ش� �ؽ��ĳ� ������ �����Ѵ�.

					*/

					st = getInstance()->readDataLineFromIndex(ch, idx, getInstance()->BMP_PUNC);
					getInstance()->setNoteFromTmpVector(st, bar_no, notes, NOTE::OBJ_EVENT, NOTE::KEY_BGA);

#pragma endregion

				}
				break;
#pragma endregion
			case '1':
#pragma region :: ��Ʈ Ű ä���� ���
				
				/*
				
					���� ����Ǵ� ��Ʈ���� ����.

					1Ű�� notes_key = 1
					2Ű�� notes_key = 2
					3Ű�� notes_key = 3
					4Ű�� notes_key = 4
					5Ű�� notes_key = 5
					��ũ��ġŰ�� notes_key = 6
					6Ű�� notes_key = 8
					7Ű�� notes_key = 9

					�� ����Ǿ�� ��
				
				*/

				st = getInstance()->readDataLineFromIndex(ch, idx, getInstance()->WAV_PUNC);

				/* Ű���� ch[5] �ε����� int �� ��ȯ�Ͽ� Ű���� �ִ´�. */
				getInstance()->setNoteFromTmpVector(st, bar_no, notes, NOTE::OBJ_NOTE, getInstance()->changeCharToInteger(ch, 5));
				
				break;
#pragma endregion 
			case '3':
#pragma region :: ���� ��Ʈ Ű ä���� ���
				
				/*

					���� ����Ǵ� ��Ʈ���� ����.

					1Ű�� notes_key = 11
					2Ű�� notes_key = 12
					3Ű�� notes_key = 13
					4Ű�� notes_key = 14
					5Ű�� notes_key = 15
					��ũ��ġŰ�� notes_key = 16
					6Ű�� notes_key = 18
					7Ű�� notes_key = 19

					�� ����Ǿ�� ��

				*/

				st = getInstance()->readDataLineFromIndex(ch, idx, getInstance()->WAV_PUNC);

				/* Ű���� ch[5] �ε����� int �� ��ȯ�ϰ� 10�� ���Ͽ� Ű���� �ִ´�. */
				getInstance()->setNoteFromTmpVector(st, bar_no, notes, NOTE::OBJ_NOTE, getInstance()->changeCharToInteger(ch, 5) + 10);

				break;

#pragma endregion
			default:
				break;
			}

#pragma endregion

		}


	}

#ifdef _DEBUG
	CCLOG("*********** %s file read finished! ********", filePath.c_str());
#endif

}
#include	"pch.h"

#include	"exe_angou.h"

// ****************************************************************
// �Í��̑}���ʒu��T��
// ================================================================

BYTE exe_angou_find_code[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_CODE_SIZE] =
{
	TNM_EXE_ANGOU_FIND_CODE00,
	TNM_EXE_ANGOU_FIND_CODE01,
	TNM_EXE_ANGOU_FIND_CODE02,
	TNM_EXE_ANGOU_FIND_CODE03,
	TNM_EXE_ANGOU_FIND_CODE04,
	TNM_EXE_ANGOU_FIND_CODE05,
	TNM_EXE_ANGOU_FIND_CODE06,
	TNM_EXE_ANGOU_FIND_CODE07,
};

const int TNM_EXE_ANGOU_ERRCODE_OK						= 0;		// �n�j
const int TNM_EXE_ANGOU_ERRCODE_CANCEL					= 1;		// �L�����Z��
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_NOT_FIND		= 2;		// �Í���}������ꏊ��������܂���
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_FIND_OVER	= 3;		// �Í���}������ꏊ��������߂��ł�
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_SHORT			= 4;		// �Í��̕����񂪒Z�߂��܂�
const int TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_LONG			= 5;		// �Í��̕����񂪒��߂��܂�
const int TNM_EXE_ANGOU_ERRCODE_FILENONE				= 6;		// �d�w�d�t�@�C�������݂��܂���
const int TNM_EXE_ANGOU_ERRCODE_FILEOPEN				= 7;		// �d�w�d�t�@�C�����������݃��[�h�ŊJ���܂���ł���
const int TNM_EXE_ANGOU_ERRCODE_FILEREAD				= 8;		// �d�w�d�t�@�C�����ǂݍ��߂܂���ł���
const int TNM_EXE_ANGOU_ERRCODE_FILESIZE				= 9;		// �d�w�d�t�@�C���̃T�C�Y������܂���
const int TNM_EXE_ANGOU_ERRCODE_FILESAVE				= 10;		// �d�w�d�t�@�C�����ۑ��ł��܂���ł���

// �e�t�m�b
int C_tnms_exe_angou::find_angou_insert_address_func(BYTE *exe_p, int exe_size, int *ret_find_code_count, BYTE *ap[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_MAX_CNT])
{
	int ap_cnt[TNM_EXE_ANGOU_FIND_CODE_CNT];
//	int find_code_count = (exe_size >= (1024 * 1024 * 9)) ? 2 : 1;		// �X�l�o�C�g�ȏ�̓f�o�b�O�ł̂d�w�d�ƔF���i�f�o�b�O�łɂ͂Q�ӏ�����j
	int find_code_count = 1;											// �f�o�b�O�ł̂d�w�d���P�ӏ�����������Ȃ��c�Ȃ����낤�H
	for (int i = 0; i < TNM_EXE_ANGOU_FIND_CODE_CNT; i++)	{
		for (int j = 0; j < TNM_EXE_ANGOU_FIND_MAX_CNT; j++)	{			// TNM_EXE_ANGOU_FIND_MAX_CNT(10)���̃o�b�t�@��p�ӂ��Ă��邪�Q�ڂŃG���[�i�f�o�b�O�ł͂R�ڂŃG���[�j
			ap[i][j] = 0;
		}
		ap_cnt[i] = find_angou_insert_address_funcfunc(exe_p, exe_size, find_code_count, exe_angou_find_code[i], ap[i]);
		if (ap_cnt[i] <= 0)	{
			return TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_NOT_FIND;		// �Í���}������ꏊ��������܂���
		}
		else if (ap_cnt[i] != find_code_count)	{
			return TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_FIND_OVER;		// �Í���}������ꏊ��������߂��ł�
		}
	}
	if (ret_find_code_count)	{	*ret_find_code_count = find_code_count;	}
	return TNM_EXE_ANGOU_ERRCODE_OK;
}

// �e�t�m�b�e�t�m�b
int C_tnms_exe_angou::find_angou_insert_address_funcfunc(BYTE *exe_p, int exe_size, int find_code_count, BYTE *find_code, BYTE **ap)
{
	int end_size = exe_size - TNM_EXE_ANGOU_FIND_CODE_SIZE;
	BYTE *wp = exe_p;
	int ap_cnt = 0;
	while (1)	{
		if (*wp == *find_code)	{
			BYTE *sp = find_code;
			BYTE *dp = wp;
			int i;
			for (i = 0; i < TNM_EXE_ANGOU_FIND_CODE_SIZE; i++)	{
				if (*sp != *dp)	{	break;	}	sp++;	dp++;
			}
			// �R�[�h����������
			if (i == TNM_EXE_ANGOU_FIND_CODE_SIZE)	{
				*ap++ = wp;		// �I�t�Z�b�g���L��
				ap_cnt++;
				if (ap_cnt > find_code_count)	{	break;	}		// �P�O���̃o�b�t�@��p�ӂ��Ă��邪�Q�ڂŃG���[�i�f�o�b�O�ł͂R�ځj
			}
		}
		wp++;
		end_size--;
		if (end_size < 0)	{	break;	}	// ��������
	}
	return ap_cnt;
}

// ****************************************************************
// �d�w�d���Í�������Ă��邩���肷��
// ================================================================
bool C_tnms_exe_angou::check(CTSTR& exe_path)
{
	// �ǂݍ���
	BUFFER exe_data;
	if (!C_file::read_full_data(exe_path, exe_data))	{
		return false;
	}

	// �Í��̑}���ʒu��T��
	BYTE *ap[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_MAX_CNT];		// �Í��}���ʒu�̃I�t�Z�b�g���擾����o�b�t�@
	int err_code = find_angou_insert_address_func(exe_data.get(), (int)exe_data.size(), NULL, ap);
	if (err_code != TNM_EXE_ANGOU_ERRCODE_OK)	{
		return false;
	}

	return true;
}

// ****************************************************************
// �d�w�d���Í�������
// ================================================================

// �{��
bool C_tnms_exe_angou::angou(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str)
{
	int err_code = angou_func(h_wnd, exe_path, angou_str);
	err_code_msgbox(h_wnd, exe_path, angou_str, err_code);
	return (err_code == TNM_EXE_ANGOU_ERRCODE_OK) ? true : false;
}

// �e�t�m�b
int C_tnms_exe_angou::angou_func(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str)
{
	MBSTR mb_angou_str = TSTR_to_MBSTR(angou_str);
	int angou_str_size = (int)mb_angou_str.size();
	if (angou_str_size < TNM_EXE_ANGOU_STR_MIN_LEN)			{	return TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_SHORT;	}	// �Í��̕����񂪒Z�߂��܂�
//	else if (angou_str_size > TNM_EXE_ANGOU_STR_MAX_LEN)	{	return TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_LONG;	}	// �Í��̕����񂪒��߂��܂�

	if (file_get_type(exe_path) != 1)	{	return TNM_EXE_ANGOU_ERRCODE_FILENONE;	}	// �d�w�d�t�@�C�������݂��܂���

	// �������݃��[�h�ŊJ���邩�e�X�g����
	C_file file;
	if (!file.open(exe_path, _T("rb")))	{	return TNM_EXE_ANGOU_ERRCODE_FILEOPEN;	}	// �d�w�d�t�@�C�����������݃��[�h�ŊJ���܂���ł���
	file.close();

	// �ǂݍ���
	BUFFER exe_data;
	if (!C_file::read_full_data(exe_path, exe_data))	{	return TNM_EXE_ANGOU_ERRCODE_FILEREAD;	}		// �d�w�d�t�@�C�����ǂݍ��߂܂���ł���
	if (exe_data.size() <= 0)	{	return TNM_EXE_ANGOU_ERRCODE_FILESIZE;	}	// �d�w�d�t�@�C���̃T�C�Y������܂���

	// �Í��̑}���ʒu��T��
	int find_code_count;
	BYTE *ap[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_MAX_CNT];		// �Í��}���ʒu�̃A�h���X���擾����o�b�t�@
	int err_code = find_angou_insert_address_func(exe_data.get(), (int)exe_data.size(), &find_code_count, ap);
	if (err_code != TNM_EXE_ANGOU_ERRCODE_OK)	{	return err_code;	}

	// �ŏI�m�F
	TSTR mess =
		_T("�Í����̃G���[�`�F�b�N���������܂����B\n\n")
		_T("�d�w�d���Í����ł��܂��B\n\n")
		_T("���s���Ă���낵���ł����H\n\n")
		_T("�@") + exe_path + _T("�@�@\n");
	int res = MessageBox(h_wnd, mess.c_str(), _T("�m�F"), MB_OKCANCEL | MB_ICONINFORMATION);
	if (res == IDCANCEL)	{	return TNM_EXE_ANGOU_ERRCODE_CANCEL;	}

	// �Í��v�f�쐬
	BYTE angou_element[TNM_EXE_ANGOU_ELEMENT_CNT];
	make_angou_element((BYTE *)mb_angou_str.c_str(), (int)mb_angou_str.size(), angou_element);

	// ���������������ă[���N���A����
	BYTE insert_code[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_CODE_SIZE];
	for (int i = 0; i < TNM_EXE_ANGOU_FIND_CODE_CNT; i++)	{
		for (int j = 0; j < TNM_EXE_ANGOU_FIND_CODE_SIZE; j++)	{
			insert_code[i][j] = ((rand() % 5) != 0) ? 0 : (rand() % 0xff);
		}
	}

	// �Í��`
	insert_code[0][TNM_EXE_ANGOU_OFFSET00A] = angou_element[TNM_EXE_ANGOU_ELEMENT00A];
	insert_code[1][TNM_EXE_ANGOU_OFFSET01A] = angou_element[TNM_EXE_ANGOU_ELEMENT01A];
	insert_code[2][TNM_EXE_ANGOU_OFFSET02A] = angou_element[TNM_EXE_ANGOU_ELEMENT02A];
	insert_code[3][TNM_EXE_ANGOU_OFFSET03A] = angou_element[TNM_EXE_ANGOU_ELEMENT03A];
	insert_code[4][TNM_EXE_ANGOU_OFFSET04A] = angou_element[TNM_EXE_ANGOU_ELEMENT04A];
	insert_code[5][TNM_EXE_ANGOU_OFFSET05A] = angou_element[TNM_EXE_ANGOU_ELEMENT05A];
	insert_code[6][TNM_EXE_ANGOU_OFFSET06A] = angou_element[TNM_EXE_ANGOU_ELEMENT06A];
	insert_code[7][TNM_EXE_ANGOU_OFFSET07A] = angou_element[TNM_EXE_ANGOU_ELEMENT07A];

	// �Í��a
	insert_code[0][TNM_EXE_ANGOU_OFFSET00B] = angou_element[TNM_EXE_ANGOU_ELEMENT00B];
	insert_code[1][TNM_EXE_ANGOU_OFFSET01B] = angou_element[TNM_EXE_ANGOU_ELEMENT01B];
	insert_code[2][TNM_EXE_ANGOU_OFFSET02B] = angou_element[TNM_EXE_ANGOU_ELEMENT02B];
	insert_code[3][TNM_EXE_ANGOU_OFFSET03B] = angou_element[TNM_EXE_ANGOU_ELEMENT03B];
	insert_code[4][TNM_EXE_ANGOU_OFFSET04B] = angou_element[TNM_EXE_ANGOU_ELEMENT04B];
	insert_code[5][TNM_EXE_ANGOU_OFFSET05B] = angou_element[TNM_EXE_ANGOU_ELEMENT05B];
	insert_code[6][TNM_EXE_ANGOU_OFFSET06B] = angou_element[TNM_EXE_ANGOU_ELEMENT06B];
	insert_code[7][TNM_EXE_ANGOU_OFFSET07B] = angou_element[TNM_EXE_ANGOU_ELEMENT07B];

	// �Í��}��
	for (int i = 0; i < TNM_EXE_ANGOU_FIND_CODE_CNT; i++)	{
		for (int j = 0; j < find_code_count; j++)	{
			CopyMemory(ap[i][j], insert_code[i], TNM_EXE_ANGOU_FIND_CODE_SIZE);
		}
	}

	// �ۑ�
	if (!C_file::write_full_data(exe_path, exe_data))	{
		err_code = TNM_EXE_ANGOU_ERRCODE_FILESAVE;		// �d�w�d�t�@�C�����ۑ��ł��܂���ł���
	}

	return TNM_EXE_ANGOU_ERRCODE_OK;
}

// ****************************************************************
// �G���[�R�[�h���b�Z�[�W�{�b�N�X
// ================================================================
void C_tnms_exe_angou::err_code_msgbox(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str, int err_code)
{
	if (err_code == TNM_EXE_ANGOU_ERRCODE_OK)	{
		MessageBox(h_wnd, _T("�d�w�d���Í������܂����B\n"), _T("�m�F"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (err_code == TNM_EXE_ANGOU_ERRCODE_CANCEL)	{
		MessageBox(h_wnd, _T("�L�����Z�����܂����B\n"), _T("�m�F"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	// ���̑��G���[
	TSTR mess;
	bool add_angou_str_err = false;
	bool add_exe_path_err = false;
	bool add_exe_lock_err = false;
	switch (err_code)	{
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_NOT_FIND:		// �Í���}������ꏊ��������܂���
			mess =
				_T("�Í���}������ꏊ��������܂���B\n\n")
				_T("�����̂d�w�d�͊��ɈÍ�������Ă���\��������܂��B\n\n")
				_T("���P�x�Í������ꂽ�d�w�d���ĈÍ������鎖�͂ł��܂���B\n\n")
				_T("�@�Í������ɖ߂������ꍇ �� �Ⴄ�Í��ɂ������ꍇ�́A\n")
				_T("�@�Í�������Ă��Ȃ����̂d�w�d�������Ă��ăR�s�[���ĉ������B\n");
			break;
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_CODE_FIND_OVER:	// �Í���}������ꏊ��������߂��ł�
			mess =
				_T("�Í���}������ꏊ��������߂��ł��B\n\n")
				_T("�@���m�̃G���[���������܂����B\n\n")
				_T("�@�����萔�ł����A�r�W���A���A�[�c�̃v���O�����S���ɂ��₢���킹�������B\n");
			break;
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_SHORT:			// �Í��̕����񂪒Z�߂��܂�
			mess = _T("�Í��̕����񂪒Z�߂��܂��B\n\n");
			add_angou_str_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_ANGOU_STR_LONG:			// �Í��̕����񂪒��߂��܂�
			mess = _T("�Í��̕����񂪒��߂��܂��B\n\n");
			add_angou_str_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILENONE:				// �d�w�d�t�@�C�������݂��܂���
			mess = _T("�d�w�d�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n\n");
			if (exe_path.size() == 0)	{
				mess += _T("�u���s�t�@�C���v �ɂd�w�d���w�肵�ĉ������B\n");
			}
			else	{
				add_exe_path_err = true;
			}
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILEOPEN:				// �d�w�d�t�@�C�����������݃��[�h�ŊJ���܂���ł���
			mess = _T("�d�w�d�t�@�C�����������݃��[�h�ŊJ���܂���ł����B\n\n");
			add_exe_lock_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILEREAD:				// �d�w�d�t�@�C�����ǂݍ��߂܂���ł���
			mess = _T("�d�w�d�t�@�C�����ǂݍ��߂܂���ł����B\n\n");
			add_exe_lock_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILESIZE:				// �d�w�d�t�@�C���̃T�C�Y������܂���
			mess = _T("�d�w�d�t�@�C���̃T�C�Y������܂���B\n\n");
			add_exe_path_err = true;
			break;
		case TNM_EXE_ANGOU_ERRCODE_FILESAVE:				// �d�w�d�t�@�C�����ۑ��ł��܂���ł���
			mess = _T("�d�w�d�t�@�C�����ۑ��ł��܂���ł����B\n\n");
			add_exe_lock_err = true;
			break;
	}

	if (add_angou_str_err)	{
		mess +=
			_T("�@��Í��̕������\n\n")
			_T("�@�@") + angou_str + _T("\n\n")
			_T("�@���p�����Ȃ�W�����ȏ�U�S�����ȉ�����͂��ĉ������B\n\n")
			_T("�@���S�p�����͔��p�����̂Q�������ł��B\n");
	}

	if (add_exe_path_err)	{
		mess +=
			_T("�u���s�t�@�C���v �Ɏw�肳��Ă���d�w�d�̃p�X���m�F���ĉ������B\n\n")
			_T("�@��d�w�d�̃p�X��\n\n")
			_T("�@�@") + exe_path + _T("\n");
	}

	if (add_exe_lock_err)	{
		mess +=
			_T("���d�w�d���N�����Ă���ꍇ�́A�I�����ĉ������B\n\n")
			_T("�@�I�����Ă���ꍇ�́A�o�b�N�^�X�N�œ����Ă��Ȃ����^�X�N�}�l�[�W���[�Ŋm�F���ĉ������B\n\n")
			_T("�@�������� Windows ���ċN�����ĉ������B\n\n")
			_T("���ǂݎ���p�̑����ɂȂ��Ă��܂��񂩁H\n\n")
			_T("�@�ǂݎ���p�̑������O���ĉ������B\n\n")
			_T("���d�w�d�t�@�C�������̃v���Z�X�ɂ���ă��b�N����Ă���\��������܂��B\n\n")
			_T("�@�d�w�d�t�@�C�������b�N���Ă���\��������c�[�����I�����ĉ������B\n\n")
			_T("�@�������� Windows ���ċN�����ĉ������B\n");
	}

	MessageBox(h_wnd, mess.c_str(), _T("�G���["), MB_OK | MB_ICONWARNING);
}

// ****************************************************************
// �Í��v�f�쐬
// ================================================================

BYTE exe_angou_original_element[TNM_EXE_ANGOU_ELEMENT_CNT] = {
	0x36, 0x59, 0xc9, 0x73, 0x2e, 0xb5, 0x09, 0xba, 0xe4, 0x4c, 0xf2, 0x6a, 0xa2, 0x34, 0xec, 0x7c,
};

// �o�C�g�����񂩂�쐬
void C_tnms_exe_angou::make_angou_element(BYTE *angou_str, int angou_str_size, BYTE *ret_angou_element)
{
	CopyMemory(ret_angou_element, exe_angou_original_element, TNM_EXE_ANGOU_ELEMENT_CNT);

	if (angou_str_size <= 0)	{	return;	}

	int cnt = (angou_str_size < TNM_EXE_ANGOU_ELEMENT_CNT) ? TNM_EXE_ANGOU_ELEMENT_CNT : angou_str_size;

	int aaa = 0;
	int bbb = 0;
	for (int i = 0; i < cnt; i++)	{
		*(ret_angou_element + bbb) ^= *(angou_str + aaa);
		aaa++;	if (aaa == angou_str_size)			{	aaa = 0;	}
		bbb++;	if (bbb == TNM_EXE_ANGOU_ELEMENT_CNT)	{	bbb = 0;	}
	}
}

// TSTR�^�����񂩂�쐬
void C_tnms_exe_angou::make_angou_element(CTSTR& angou_str, BYTE *ret_angou_element)
{
	MBSTR mb_angou_str = TSTR_to_MBSTR(angou_str);
	make_angou_element((BYTE *)mb_angou_str.c_str(), (int)mb_angou_str.size(), ret_angou_element);
}

#include	"pch.h"

#include	"utility/util_dummy_file.h"

// ****************************************************************
// �_�~�[�t�@�C���`�F�b�N
//		version: 1=�o�[�W�����P�A2=�o�[�W�����Q�ȏ�
// ================================================================
bool C_dummy_file_checker::check_dummy_file(int version, CTSTR& dummy_dir, CTSTR& dummy_file_name, int key, CTSTR& code)
{
	// �p�����[�^������
	TSTR dummy_file_path[16];
	int code_len = 8, dummy_file_cnt = 0;
	MBSTR mb_code = TSTR_to_MBSTR(code);
	BYTE script_code[256] = {0};	// �X�N���v�g�ɖ��ߍ��܂�Ă����R�[�h
	BYTE file_code[256] = {0};		// �t�@�C���ɖ��ߍ��܂�Ă����R�[�h

	// �_�~�[�t�@�C����T���B�t�@�C���͍ő�16��
	for (int i = 0; i < 16; i ++)	{
		dummy_file_path[i] = dummy_dir + _T("\\") + dummy_file_name + str_format(_T(".%03d"), i + 1);
		if (file_get_type(dummy_file_path[i]) == 1)
			dummy_file_cnt ++;
		else
			break;
	}

	// �_�~�[�t�@�C�����P�ȏ㌩�����ꍇ
	if (dummy_file_cnt > 0)	{

		// �S�Ẵ_�~�[�t�@�C���̃R�[�h����������
		for (int dummy_file_no = 0; dummy_file_no < dummy_file_cnt; dummy_file_no++)	{

			// �t�@�C�����J��
			C_file file;
			if (!file.open(dummy_file_path[dummy_file_no], _T("rb")))
				return false;

			// �o�[�W�����Q�ȏ�̏ꍇ�̓o�[�W������ǂݍ���
			if (version >= 2)
				file.read(&version, sizeof(int));

			// �o�[�W�����ɂ���ăR�[�h�̒����͈Ⴄ
			if (version >= 2)
				code_len = 16;

			// �R�[�h�̖��ߍ��݈ʒu���擾
			int code_pos = 0;
			file.read(&code_pos, sizeof(int));
			BYTE* p = (BYTE *)&code_pos;
			for (int i = 0; i < sizeof(int); i++)
				*p++ ^= key;			// �P�o�C�g������

			// �R�[�h���擾
			file.seek(code_pos, SEEK_SET);
			for (int i = dummy_file_no; i < code_len; i += dummy_file_cnt)	{
				file.read(&file_code[i], 1);
				file_code[i] ^= key;	// �P�o�C�g������
			}
		}

		// �X�N���v�g�Ŏw�肳�ꂽ�R�[�h���擾
		memcpy(script_code, mb_code.c_str(), mb_code.size());

		// �o�[�W�����Q�ȏ�̏ꍇ�̓X�N���v�g�R�[�h��MD5�ϊ�
		if (version >= 2)	{
			MD5_STRUCT md5st;
			MD5init(&md5st, code_len);
			MD5funk(&md5st, script_code, code_len);
			memcpy(script_code, md5st.code, 16);	// MD5�ϊ��ŏo���オ�����R�[�h��16����
			code_len = 16;							// �R�[�h�̒�����ϊ���̃R�[�h�ɕ␳
		}

		// �R�[�h�`�F�b�N
		if (memcmp(script_code, file_code, code_len) == 0)
			return true;
	}

	return false;
}




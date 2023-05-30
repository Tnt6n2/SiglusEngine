#pragma		once

#include	"data/tnm_save_00.h"

// ****************************************************************
// �Z�[�u�X�g���[���F�Z�[�u�F�o�n�c�z��
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::save(C_tnm_pod_array<TYPE1, TYPE2>& data)
{
	// �T�C�Y����������
	save<int>((int)data.size());

	// �Z�[�u����
	save((const void *)data.get(), (int)data.size() * sizeof(TYPE1));
}

// ****************************************************************
// �Z�[�u�X�g���[���F�Z�[�u�F�x�[�X�z��
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::save(C_tnm_base_array<TYPE1, TYPE2>& data)
{
	// �T�C�Y����������
	save<int>((int)data.size());

	// �Z�[�u����
	for (C_tnm_base_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		save(*itr);
}

// ****************************************************************
// �Z�[�u�X�g���[���F�Z�[�u�F�N���X�z��
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::save(C_tnm_class_array<TYPE1, TYPE2>& data)
{
	// �T�C�Y����������
	save<int>((int)data.size());

	// �Z�[�u����
	for (C_tnm_class_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		itr->save(*this);
}

// ****************************************************************
// �Z�[�u�X�g���[���F���[�h�F�o�n�c�z��
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::load(C_tnm_pod_array<TYPE1, TYPE2>& data)
{
	// �T�C�Y��ǂݍ���
	int cnt = load_ret<int>();

	// �T�C�Y��ύX����
	data.resize(cnt);

	// �S���ǂݍ���
	pop((void *)data.get(), (int)data.size() * sizeof(TYPE1));
}

// ****************************************************************
// �Z�[�u�X�g���[���F���[�h�F�x�[�X�z��
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::load(C_tnm_base_array<TYPE1, TYPE2>& data)
{
	// �T�C�Y��ǂݍ���
	int cnt = load_ret<int>();

	// �T�C�Y��ύX����
	data.resize(cnt);

	// �S���ǂݍ���
	for (C_tnm_base_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		load(*itr);
}

// ****************************************************************
// �Z�[�u�X�g���[���F���[�h�F�N���X�z��
// ================================================================
template <typename TYPE1, typename TYPE2> inline void C_tnm_save_stream::load(C_tnm_class_array<TYPE1, TYPE2>& data)
{
	// �T�C�Y��ǂݍ���
	int cnt = load_ret<int>();

	// �T�C�Y��ύX����
	data.resize(cnt);

	// �f�[�^��ǂݍ���
	for (C_tnm_class_array<TYPE1, TYPE2>::iterator itr = data.begin(); itr != data.end(); ++itr)
		itr->load(*this);
}

// ****************************************************************
// �Z�[�u�X�g���[���F�Z�[�u�F�z��i�T�C�Y�Œ�j
// ================================================================
template <typename TYPE1> inline void C_tnm_save_stream::save_fixed_array_begin(ARRAY<TYPE1>& data, int* jump)
{
	// �J�b�g���邽�߂̃W�����v����L������ʒu��\�񂵂Ă���
	*jump = get_seek();
	save<int>(0);

	// �T�C�Y�`�F�b�N���s�����߂ɃT�C�Y����������
	save<int>((int)data.size());
}

inline void C_tnm_save_stream::save_fixed_array_end(int* jump)
{
	// �J�b�g���邽�߂̃W�����v�����������
	*(int *)get(*jump) = get_seek();
}

// ****************************************************************
// �Z�[�u�X�g���[���F���[�h�F�z��i�T�C�Y�Œ�j
// ================================================================
template <typename TYPE1> inline int C_tnm_save_stream::load_fixed_array_begin(ARRAY<TYPE1>& data, CTSTR& name, int* jump)
{
	// �J�b�g���邽�߂̃W�����v���ǂݍ���
	*jump = load_ret<int>();

	// �T�C�Y�`�F�b�N���s�����߂̃T�C�Y��ǂݍ���
	int cnt = load_ret<int>();

	// ����
	if (cnt == (int)data.size())	{}

	// �f�[�^��������I
	else if (cnt > (int)data.size())	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, str_format(_T("�O��̃f�[�^��� %s �������Ă��܂��B\n�ݒ�t�@�C��������������ꂽ�\��������܂��B\n%d �̃f�[�^�͓ǂݍ��܂��ɐ؂�̂Ă܂��B"), name.c_str(), cnt - data.size()));

		// �ǂݍ��ނׂ��T�C�Y���C���I
		cnt = (int)data.size();
	}

	// �f�[�^�����Ȃ�����I
	else if (cnt < (int)data.size())	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, str_format(_T("�O��̃f�[�^��� %s �������Ă��܂��B\n�ݒ�t�@�C��������������ꂽ�\��������܂��B\n%d �̃f�[�^���\�z���܂��B"), name.c_str(), data.size() - cnt));
	}

	// �ǂݍ��߂����Ԃ�
	return cnt;
}

inline void C_tnm_save_stream::load_fixed_array_end(int* jump)
{
	// �c��f�[�^���J�b�g����
	set_seek(*jump);
}

// ****************************************************************
// �Z�[�u�X�g���[���F�X�L�b�v�F�z��i�T�C�Y�Œ�j
// ================================================================
inline void C_tnm_save_stream::skip_fixed_array()
{
	// �J�b�g���邽�߂̃W�����v���ǂݍ���
	int jump = load_ret<int>();

	// �f�[�^���J�b�g����
	set_seek(jump);
}

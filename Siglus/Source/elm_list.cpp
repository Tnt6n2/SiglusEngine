#include	"pch.h"

#include	"tnm_form_code.h"
#include	"element/elm_list.h"
#include	"data/tnm_save_00.h"

// ****************************************************************
// �������X�g�F������
// ================================================================
void C_elm_int_list::init(S_element element, CTSTR& name, int size, bool extend_enable)
{
	// ���N���X�̏�����
	C_elm_list<int>::init(element, FM_INTLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// �������X�g�F�r�b�g��ݒ�
// ================================================================
void C_elm_int_list::set_bit_1(int index, int value)
{
	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b1[") + tostr(index) + _T("] ���Q�Ƃ��܂����B"));
		return;
	}

	int* dst = &m_sub[index / 32];	// �^�[�Q�b�g�̃t���O
	DWORD mask = 0x00000001 << index;		// �^�[�Q�b�g�̃r�b�g�̂� 1 �̃}�X�N�����
	value  = (value << index) & mask;		// �l���^�[�Q�b�g�̈ʒu�Ɉړ����ă}�X�N��������
	*dst = (*dst & ~mask) | value;			// �^�[�Q�b�g�� 0 �N���A���Ă���l����
}

void C_elm_int_list::set_bit_2(int index, int value)
{
	index *= 2;		// 2�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b2[") + tostr(index / 2) + _T("] ���Q�Ƃ��܂����B"));
		return;
	}

	int* dst = &m_sub[index / 32];	// �^�[�Q�b�g
	DWORD mask = 0x00000003 << index;		// �^�[�Q�b�g�̃r�b�g�̂� 1 �̃}�X�N�����
	value  = (value << index) & mask;		// �l���^�[�Q�b�g�̈ʒu�Ɉړ����ă}�X�N��������
	*dst = (*dst & ~mask) | value;			// �^�[�Q�b�g�� 0 �N���A���Ă���l����
}

void C_elm_int_list::set_bit_4(int index, int value)
{
	index *= 4;		// 4�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b4[") + tostr(index / 4) + _T("] ���Q�Ƃ��܂����B"));
		return;
	}

	int* dst = &m_sub[index / 32];	// �^�[�Q�b�g�̃t���O
	DWORD mask = 0x0000000F << index;		// �^�[�Q�b�g�̃r�b�g�̂� 1 �̃}�X�N�����
	value  = (value << index) & mask;		// �l���^�[�Q�b�g�̈ʒu�Ɉړ����ă}�X�N��������
	*dst = (*dst & ~mask) | value;			// �^�[�Q�b�g�� 0 �N���A���Ă���l����
}

void C_elm_int_list::set_bit_8(int index, int value)
{
	index *= 8;		// 8�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b8[") + tostr(index / 8) + _T("] ���Q�Ƃ��܂����B"));
		return;
	}

	int* dst = &m_sub[index / 32];	// �^�[�Q�b�g
	DWORD mask = 0x000000FF << index;		// �^�[�Q�b�g�̃r�b�g�̂� 1 �̃}�X�N�����
	value  = (value << index) & mask;		// �l���^�[�Q�b�g�̈ʒu�Ɉړ����ă}�X�N��������
	*dst = (*dst & ~mask) | value;			// �^�[�Q�b�g�� 0 �N���A���Ă���l����
}

void C_elm_int_list::set_bit_16(int index, int value)
{
	index *= 16;		// 16�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b16[") + tostr(index / 16) + _T("] ���Q�Ƃ��܂����B"));
		return;
	}

	int* dst = &m_sub[index / 32];	// �^�[�Q�b�g
	DWORD mask = 0x0000FFFF << index;		// �^�[�Q�b�g�̃r�b�g�̂� 1 �̃}�X�N�����
	value  = (value << index) & mask;		// �l���^�[�Q�b�g�̈ʒu�Ɉړ����ă}�X�N��������
	*dst = (*dst & ~mask) | value;			// �^�[�Q�b�g�� 0 �N���A���Ă���l����
}

// ****************************************************************
// �������X�g�F�r�b�g���擾
// ================================================================
int C_elm_int_list::get_bit_1(int index)
{
	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b1[") + tostr(index) + _T("] ���Q�Ƃ��܂����B"));
		return 0;
	}

	int value = m_sub[index / 32];	// �^�[�Q�b�g�̃t���O
	return (value >> index) & 0x00000001;	// �^�[�Q�b�g�̃r�b�g��Ԃ�
}

int C_elm_int_list::get_bit_2(int index)
{
	index *= 2;		// 2�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b2[") + tostr(index / 2) + _T("] ���Q�Ƃ��܂����B"));
		return 0;
	}

	int value = m_sub[index / 32];	// �^�[�Q�b�g�̃t���O
	return (value >> index) & 0x00000003;	// �^�[�Q�b�g�̃r�b�g��Ԃ�
}

int C_elm_int_list::get_bit_4(int index)
{
	index *= 4;		// 4�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b4[") + tostr(index / 4) + _T("] ���Q�Ƃ��܂����B"));
		return 0;
	}

	int value = m_sub[index / 32];	// �^�[�Q�b�g�̃t���O
	return (value >> index) & 0x0000000F;	// �^�[�Q�b�g�̃r�b�g��Ԃ�
}

int C_elm_int_list::get_bit_8(int index)
{
	index *= 8;		// 8�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b8[") + tostr(index / 8) + _T("] ���Q�Ƃ��܂����B"));
		return 0;
	}

	int value = m_sub[index / 32];	// �^�[�Q�b�g�̃t���O
	return (value >> index) & 0x000000FF;	// �^�[�Q�b�g�̃r�b�g��Ԃ�
}

int C_elm_int_list::get_bit_16(int index)
{
	index *= 16;	// 16�{���Ă����Ə������ȒP

	// �͈̓`�F�b�N
	if (index < 0 || (int)m_sub.size() <= index / 32)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("�͈͊O�� ") + get_element_name() + _T(".b16[") + tostr(index / 16) + _T("] ���Q�Ƃ��܂����B"));
		return 0;
	}

	int value = m_sub[index / 32];	// �^�[�Q�b�g�̃t���O
	return (value >> index) & 0x0000FFFF;	// �^�[�Q�b�g�̃r�b�g��Ԃ�
}

// ****************************************************************
// �������X�g�F�T�u�̏�����
// ================================================================
void C_elm_int_list::_init(int begin, int end)
{
	ZeroMemory(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// �������X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_int_list::_reinit(int begin, int end)
{
	ZeroMemory(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// �������X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_int_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	stream.save(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// �������X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_int_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	stream.load(m_sub.get() + begin, (end - begin) * sizeof(int));
}

// ****************************************************************
// �����񃊃X�g�F������
// ================================================================
void C_elm_str_list::init(S_element element, CTSTR& name, int size, bool extend_enable)
{
	// ���N���X�̏�����
	C_elm_list<TSTR>::init(element, FM_STRLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// �����񃊃X�g�F�T�u�̏�����
// ================================================================
void C_elm_str_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].clear();
}

// ****************************************************************
// �����񃊃X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_str_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].clear();
}

// ****************************************************************
// �����񃊃X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_str_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		stream.save(m_sub[i]);
}

// ****************************************************************
// �����񃊃X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_str_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		stream.load(m_sub[i]);
}


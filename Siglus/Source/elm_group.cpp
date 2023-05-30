#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_save_00.h"

#include	"element/elm_object.h"
#include	"element/elm_group.h"

#include	"engine/eng_input.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// �O���[�v�F������
// ================================================================
void C_elm_group::init(S_element element, CTSTR& name, int group_no, S_element target_object)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_GROUP, name, NULL);

	// �Œ�p�����[�^��ݒ�
	m_def.group_no = group_no;
	m_def.target_object = target_object;

	// �ď�����
	reinit();
}

// ****************************************************************
// �O���[�v�F�ď�����
// ================================================================
void C_elm_group::reinit()
{
	m_param.sorter = S_tnm_sorter(0, 0);
	m_param.cancel_priority = 0;
	m_param.cancel_se_no = -1;
	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;

	m_param.result = TNM_GROUP_RESULT_NONE;
	m_param.result_button_no = 0;

	m_param.doing = false;
	m_param.pause_flag = false;
	m_param.wait_flag = false;
	m_param.cancel_flag = false;
	m_param.target_object = m_def.target_object;

	m_work.hit_button_no = -1;
	m_work.pushed_button_no = -1;
	m_work.last_hit_button_no = -1;
	m_work.last_pushed_button_no = -1;
}

// ****************************************************************
// �O���[�v�F�I����������
// ================================================================
void C_elm_group::init_sel()
{
	m_param.cancel_priority = 0;
	m_param.cancel_se_no = -1;
	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;

	m_param.result = TNM_GROUP_RESULT_NONE;
	m_param.result_button_no = 0;

	m_param.doing = false;
	m_param.pause_flag = false;
	m_param.wait_flag = false;
	m_param.cancel_flag = false;

	m_work.hit_button_no = -1;
	m_work.pushed_button_no = -1;
	m_work.last_hit_button_no = -1;
	m_work.last_pushed_button_no = -1;
}

// ****************************************************************
// �O���[�v�F�Z�[�u
// ================================================================
void C_elm_group::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
}

// ****************************************************************
// �O���[�v�F���[�h
// ================================================================
void C_elm_group::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
}

// ****************************************************************
// �O���[�v�F�R�s�[
// ================================================================
void C_elm_group::copy(C_elm_group* src)
{
	m_param = src->m_param;
}

// ****************************************************************
// �O���[�v�F�{�^���t���[������������
// ================================================================
void C_elm_group::button_frame_init()
{
	m_work.hit_button_no = m_work.last_hit_button_no;
	m_work.pushed_button_no = m_work.last_pushed_button_no;
}

// ****************************************************************
// �O���[�v�F�L�����Z������
// ================================================================
void C_elm_group::cancel_check()
{
	// �L�����Z���\�ȏꍇ
	if (m_param.doing && m_param.cancel_flag)	{

		// �E�N���b�N���ꂽ
		if (tnm_input_use_key_down_up(VK_EX_CANCEL))	{

			// �L�����Z��
			cancel();
		}
	}
}

// ****************************************************************
// �O���[�v�F���肳�ꂽ
// ================================================================
void C_elm_group::decide(int button_no)
{
	if (m_param.doing)	{

		m_param.doing = false;

		// result v1
		m_param.decided_button_no = button_no;
		// result v2
		m_param.result = TNM_GROUP_RESULT_DECIDED;
		m_param.result_button_no = button_no;

		// ���������������
		m_work.hit_button_no = -1;
		m_work.pushed_button_no = -1;
		m_work.last_hit_button_no = -1;
		m_work.last_pushed_button_no = -1;

		// ���ʉ��̓I�u�W�F�N�g���炷

		// �҂���Ԃ������ꍇ
		if (get_wait_flag())	{
			tnm_stack_push_int(button_no);			// ���ʂ��X�^�b�N�ɐς�
			tnm_pop_proc();							// ���̃v���Z�X��
		}
	}
}

// ****************************************************************
// �O���[�v�F�L�����Z�����ꂽ
// ================================================================
void C_elm_group::cancel()
{
	if (m_param.doing)	{

		m_param.doing = false;

		// result v1
		m_param.decided_button_no = TNM_GROUP_CANCELED;

		// result v2
		m_param.result = TNM_GROUP_RESULT_CANCELLED;
		m_param.result_button_no = m_work.hit_button_no;

		// ���������������
		m_work.hit_button_no = -1;
		m_work.pushed_button_no = -1;
		m_work.last_hit_button_no = -1;
		m_work.last_pushed_button_no = -1;

		// �L�����Z�����̓O���[�v���炷
		tnm_play_se(m_param.cancel_se_no);

		// �҂���Ԃ������ꍇ
		if (get_wait_flag())	{
			tnm_stack_push_int(TNM_GROUP_CANCELED);	// ���ʂ��X�^�b�N�ɐς�
			tnm_pop_proc();							// ���̃v���Z�X��
		}
	}
}

// ****************************************************************
// �O���[�v���X�g�F������
// ================================================================
void C_elm_group_list::init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target_object)
{
	// �Œ�p�����[�^
	m_target_object = target_object;

	// ���̏�����
	C_elm_list<C_elm_group>::init(element, FM_GROUPLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// �O���[�v���X�g�F�T�u�̏�����
// ================================================================
void C_elm_group_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"), i, m_target_object);
}

// ****************************************************************
// �O���[�v���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_group_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �O���[�v���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_group_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �O���[�v���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_group_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}











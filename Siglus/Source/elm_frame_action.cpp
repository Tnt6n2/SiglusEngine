#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_save_00.h"
#include	"data/tnm_proc.h"
#include	"data/tnm_lexer.h"

#include	"element/elm_call.h"
#include	"element/elm_frame_action.h"

#include	"engine/flow_script.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"

// ****************************************************************
// ������
// ================================================================
void C_elm_frame_action::init(S_element element, CTSTR& name, S_element target)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_FRAMEACTION, name, NULL);

	// �Œ�p�����[�^
	m_target = target;
	m_counter.init(element + S_element(ELM_FRAMEACTION_COUNTER), _T("counter"));

	// �ʏ�p�����[�^
	m_scn_no = -1;
	m_cmd_no = -1;
	m_end_time = 0;
	m_scn_name.clear();
	m_cmd_name.clear();
	m_arg_list.clear();
	m_end_action_flag = false;
}

// ****************************************************************
// �ď�����
// ================================================================
void C_elm_frame_action::reinit(bool finish_flag)
{
	// �I��
	if (finish_flag)
		finish();

	// ������
	m_scn_no = -1;
	m_cmd_no = -1;
	m_scn_name.clear();
	m_cmd_name.clear();
	m_arg_list.clear();
	m_counter.reinit();
	m_end_action_flag = false;
}

// ****************************************************************
// �I��
// ================================================================
void C_elm_frame_action::finish()
{
	// �t���[���A�N�V�����̏����L��
	int scn_no = m_scn_no;
	int cmd_no = m_cmd_no;
	int end_time = m_end_time;

	// �t���[���A�N�V�������������i���̂P�j
	// �G���h�A�N�V�����̒��Ńt���[���A�N�V����������������ƁA����ɃG���h�A�N�V�������Ă΂�āc�Ƃ����������[�v��h�����߁A��Ƀp�����[�^�����������Ă����܂��B
	m_scn_no = -1;
	m_cmd_no = -1;
	m_scn_name.clear();
	m_cmd_name.clear();
	m_end_action_flag = true;	// �G���h�A�N�V�����t���O�𗧂Ă�

	// �G���h�A�N�V�������N����
	if (scn_no >= 0 && cmd_no >= 0)	{
		m_counter.set_count(end_time == -1 ? 0 : end_time);
		do_action(scn_no, cmd_no);
	}

	// �t���[���A�N�V�������������i���̂Q�j
	m_end_action_flag = false;	// �G���h�A�N�V�����t���O���~�낷
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_frame_action::save(C_tnm_save_stream& stream)
{
	stream.save(m_end_time);		// �I������
	stream.save(m_scn_name);		// �V�[����
	stream.save(m_cmd_name);		// �R�}���h��
	m_arg_list.save(stream);		// ����
	m_counter.save(stream);			// �J�E���^
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_frame_action::load(C_tnm_save_stream& stream)
{
	// ���������ς݂�O��Ƃ��Ă��܂��B

	stream.load(m_end_time);		// �I������
	stream.load(m_scn_name);		// �V�[����
	stream.load(m_cmd_name);		// �R�}���h��
	m_arg_list.load(stream);		// ����
	m_counter.load(stream);			// �J�E���^

	// �č\�z
	restruct();
}

// ****************************************************************
// �R�s�[
// ================================================================
void C_elm_frame_action::copy(const C_elm_frame_action* src, bool finish_flag)
{
	if (finish_flag)
		finish();

	m_end_time = src->m_end_time;		// �I������
	m_scn_name = src->m_scn_name;		// �V�[����
	m_cmd_name = src->m_cmd_name;		// �R�}���h��
	m_arg_list = src->m_arg_list;		// ����
	m_counter.copy(&src->m_counter);	// �J�E���^

	// �č\�z
	restruct();
}

// ****************************************************************
// �č\�z
// ================================================================
void C_elm_frame_action::restruct()
{
	m_scn_no = Gp_lexer->get_scn_no(m_scn_name);
	m_cmd_no = Gp_lexer->get_user_cmd_no(m_scn_no, m_cmd_name);
}

// ****************************************************************
// �p�����[�^��ݒ肷��
// ================================================================
void C_elm_frame_action::set_param(int end_time, int real_flag, CTSTR& scn_name, CTSTR& cmd_name, C_tnm_prop_list& arg_list)
{
	// ������
	reinit(true);

	// �p�����[�^��ݒ�
	m_end_time = end_time;
	m_scn_name = scn_name;
	m_cmd_name = cmd_name;
	m_arg_list = arg_list;

	// �J�E���^���X�^�[�g����
	if (real_flag)	{
		m_counter.start_real();
	}
	else	{
		m_counter.start();
	}

	// �č\�z
	restruct();
}

// ****************************************************************
// �A�N�V���������s����
// ================================================================
void C_elm_frame_action::do_action()
{
	do_action(m_scn_no, m_cmd_no);
}

void C_elm_frame_action::do_action(int scn_no, int cmd_no)
{
	if (scn_no < 0 || cmd_no < 0)
		return;

	// ���g�̃G�������g�������Ƃ��Đς�
	tnm_stack_push_element(get_element());

	// �^�[�Q�b�g�̃G�������g�������Ƃ��Đς�
	if (m_target.code_cnt > 0)
		tnm_stack_push_element(m_target);

	// �^�`�F�b�N�����Ȃ���������X�^�b�N�ɐς�
	for (C_tnm_prop* arg = m_arg_list.get_ptr(); arg < m_arg_list.get_end_ptr(); arg++)	{
		if (false);
		else if (arg->form == FM_INT)	{
			tnm_stack_push_int(arg->Int);
		}
		else if (arg->form == FM_STR)	{
			tnm_stack_push_str(arg->str);
		}
	}

	// ���݂̃R�[�������擾
	int cur_call_cnt = tnm_get_cur_call_cnt();

	// �R�}���h���R�[��
	tnm_scene_proc_call_user_cmd(scn_no, cmd_no, FM_VOID, false, true);

	// �R�[���̎�ނ�ݒ肷��
	C_elm_call* p_cur_call = tnm_get_cur_call();
	p_cur_call->arg_cnt = m_arg_list.get_size() + 1 + (m_target.code_cnt > 0 ? 1 : 0);		// �����̌�
	p_cur_call->scn_no = scn_no;	// ���[�U�R�}���h�̃V�[���ԍ�
	p_cur_call->cmd_no = cmd_no;	// ���[�U�R�}���h�̃R�}���h�ԍ�

	// �ēx�X�N���v�g�����s����
	tnm_proc_script();

	// �����R�[�������ς���Ă�����Areturn �ł͂Ȃ� disp �ȂǂŔ������\��
	// return ���s��
	//int new_call_cnt = tnm_get_cur_call_cnt();
	//if (new_call_cnt != cur_call_cnt)	{
	//	tnm_return();
	//}
}

// ****************************************************************
// ���Ԃ��X�V
// ================================================================
void C_elm_frame_action::update_time(int past_game_time, int past_real_time)
{
	// �t���[���A�N�V�����J�E���^
	m_counter.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// �t���[������
// ================================================================
void C_elm_frame_action::frame()
{
	// �t���[���A�N�V�����I��
	if (m_scn_no >= 0 && m_cmd_no >= 0 && m_end_time >= 0)	{
		if (m_counter.get_count() >= m_end_time)	{
			reinit(true);
		}
	}
}

// ****************************************************************
// �t���[���A�N�V�������X�g�F������
// ================================================================
void C_elm_frame_action_list::init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target)
{
	m_target = target;

	// ���̏�����
	C_elm_list_ex<C_elm_frame_action>::init(element, FM_FRAMEACTIONLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// �t���[���A�N�V�������X�g�F�ď�����
// ================================================================
void C_elm_frame_action_list::reinit(bool finish_flag, bool restruct_flag)
{
	// �I������
	if (finish_flag)
		finish();

	// ���̍ď�����
	C_elm_list_ex<C_elm_frame_action>::reinit(restruct_flag);
}

// ****************************************************************
// �t���[���A�N�V�������X�g�F�I��
// ================================================================
void C_elm_frame_action_list::finish()
{
	int fa_cnt = (int)m_sub.size();
	for (int i = 0; i < fa_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// �t���[���A�N�V�������X�g�F�T�u�̏�����
// ================================================================
void C_elm_frame_action_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"), m_target);
}

// ****************************************************************
// �t���[���A�N�V�������X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_frame_action_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(true);				// ����� finish �t���O
}

// ****************************************************************
// �t���[���A�N�V�������X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_frame_action_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �t���[���A�N�V�������X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_frame_action_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}



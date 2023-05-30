#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_cmd_line.h"
#include	"data/tnm_input.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_rmenu.h"
#include	"data/tnm_timer.h"
#include	"element/elm_call.h"
#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_init.h"
#include	"engine/eng_save.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_dialog.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_error.h"
#include	"localize/localize.h"

// ****************************************************************
// ���݂̃v���Z�X���擾
// ================================================================
C_tnm_proc* tnm_get_cur_proc()
{
	return &Gp_local->cur_proc;
}

// ****************************************************************
// ���݂̃R�[�����擾�i�C�����C���ɂ���ׂ��H�j
// ================================================================
C_elm_call* tnm_get_cur_call()
{
	return Gp_call_list->get_sub(Gp_call_list->get_call_cnt() - 1, true);
}

// ****************************************************************
// ���݂̃R�[�������擾�i�C�����C���ɂ���ׂ��H�j
// ================================================================
int tnm_get_cur_call_cnt()
{
	return Gp_call_list->get_call_cnt();
}

// ****************************************************************
// �V�[�������FScene.pck ��ǂݒ���
// ================================================================
bool tnm_reload_scene_pck()
{
	// �f�B���N�g�����ς�����I�i�����V�X�e���j
	if (Gp_local_save->append_dir != Gp_dir->append_dir)
	{
		// �f�B���N�g����ݒ�
		Gp_dir->append_dir = Gp_local_save->append_dir;
		Gp_dir->append_name = Gp_local_save->append_name;
		// �V�[��������������
		Gp_lexer->init();
		// �V�[����ǂݍ���
		if (!Gp_lexer->load_scn(LCL_SCENE_PCK))
		{
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�V�[���f�[�^�i") LCL_SCENE_PCK _T("�j���J���܂���ł����B"));
		}
	}

	return true;
}

// ****************************************************************
// �V�[�������F�W�����v
// ================================================================
bool tnm_scene_proc_jump(int scn_no, int z_no)
{
	// �V�[���ɃW�����v
	Gp_lexer->jump_to_scn(scn_no, z_no);

	return true;
}

bool tnm_scene_proc_jump(CTSTR& scn_name, int z_no)
{
	// �V�[���ԍ����擾
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss ��������܂���ł����B"));
	}

	// �V�[���ɃW�����v
	if (!tnm_scene_proc_jump(scn_no, z_no))
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F���x���R�[��
// ================================================================
bool tnm_scene_proc_gosub(int label_no, int ret_form_code)
{
	int src_scn_no = Gp_lexer->get_cur_scn_no();

	// �R�[�����̏���ݒ�
	C_elm_call* p_src_call = tnm_get_cur_call();
	p_src_call->ret_form_code = ret_form_code;

	tnm_save_call();						// �R�[�������Z�[�u
	Gp_lexer->jump_to_label(label_no);		// �R�[����ɃW�����v

	// �R�[����̏���ݒ�
	C_elm_call* p_dst_call = tnm_get_cur_call();
	p_dst_call->call_type = E_tnm_call_type_gosub;

	// �f�o�b�O���
	p_dst_call->src_scn_no = src_scn_no;
	p_dst_call->dst_scn_no = src_scn_no;
	p_dst_call->dst_label_no = label_no;

	return true;
}

// ****************************************************************
// �V�[�������F�V�[���R�[��
// ================================================================
bool tnm_scene_proc_farcall(int scn_no, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	int src_scn_no = Gp_lexer->get_cur_scn_no();

	// �R�[�����̏���ݒ�
	C_elm_call* p_src_call = tnm_get_cur_call();
	p_src_call->ret_form_code = ret_form_code;

	tnm_save_call();						// �R�[�������Z�[�u
	Gp_lexer->jump_to_scn(scn_no, z_no);	// �R�[����ɃW�����v

	// �R�[����̏���ݒ�
	C_elm_call* p_dst_call = tnm_get_cur_call();
	p_dst_call->call_type = E_tnm_call_type_farcall;
	p_dst_call->excall_flag = ex_call_flag;
	p_dst_call->frame_action_flag = frame_action_flag;
	if (ex_call_flag)	{

		// �d�w�R�[�����t���O�𗧂Ă�
		Gp_global->ex_call_flag = true;

		// �X�N���v�g�ɐi��
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}
	else if (frame_action_flag)	{

		// �X�N���v�g�ɐi��
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}

	// �f�o�b�O���
	p_dst_call->src_scn_no = src_scn_no;
	p_dst_call->dst_scn_no = scn_no;
	p_dst_call->dst_label_no = z_no;

	return true;
}

bool tnm_scene_proc_farcall(CTSTR& scn_name, int z_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	// �V�[���ԍ����擾
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss ��������܂���ł����B"));
	}

	// �R�[��
	if (!tnm_scene_proc_farcall(scn_no, z_no, ret_form_code, ex_call_flag, frame_action_flag))
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F���[�U�R�}���h�R�[��
// ================================================================
bool tnm_scene_proc_call_user_cmd(int scn_no, int user_cmd_no, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	int src_scn_no = Gp_lexer->get_cur_scn_no();

	// �R�[�����̏���ݒ�
	C_elm_call* p_src_call = tnm_get_cur_call();
	p_src_call->ret_form_code = ret_form_code;

	tnm_save_call();									// �R�[�������Z�[�u
	Gp_lexer->jump_to_user_cmd(scn_no, user_cmd_no);	// �R�[����ɃW�����v

	// �R�[����̏���ݒ�
	C_elm_call* p_dst_call = tnm_get_cur_call();
	p_dst_call->call_type = E_tnm_call_type_user_cmd;
	p_dst_call->excall_flag = ex_call_flag;
	p_dst_call->frame_action_flag = frame_action_flag;
	if (ex_call_flag)	{

		// �d�w�R�[�����t���O�𗧂Ă�
		Gp_global->ex_call_flag = true;

		// �X�N���v�g�ɐi��
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}
	else if (frame_action_flag)	{

		// �X�N���v�g�ɐi��
		tnm_push_proc(TNM_PROC_TYPE_SCRIPT);
	}

	// �f�o�b�O���
	p_dst_call->src_scn_no = src_scn_no;
	p_dst_call->dst_scn_no = scn_no;
	p_dst_call->dst_label_no = user_cmd_no;
			
	return true;
}

bool tnm_scene_proc_call_user_cmd(int scn_no, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	// ���[�U�R�}���h�ԍ����擾
	int user_cmd_no = Gp_lexer->get_user_cmd_no(scn_no, user_cmd_name);
	if (user_cmd_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("���[�U�R�}���h ") + user_cmd_name + _T(" ��������܂���ł����B"));
	}

	// �R�[��
	if (!tnm_scene_proc_call_user_cmd(scn_no, user_cmd_no, ret_form_code, ex_call_flag, frame_action_flag))
		return false;

	return true;
}

bool tnm_scene_proc_call_user_cmd(CTSTR& scn_name, CTSTR& user_cmd_name, int ret_form_code, bool ex_call_flag, bool frame_action_flag)
{
	// �V�[���ԍ����擾
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no == -1)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�V�[�� ") + scn_name + _T(" ��������܂���ł����B"));
	}

	if (!tnm_scene_proc_call_user_cmd(scn_no, user_cmd_name, ret_form_code, ex_call_flag, frame_action_flag))
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F���^�[��
// ================================================================
bool tnm_scene_proc_return()
{
	// �R�[���̎�ނɂ�鏈��
	C_elm_call* p_cur_call = tnm_get_cur_call();
	if (p_cur_call->excall_flag == 1)	{

		// �d�w�R�[���t���O���~�낷
		Gp_global->ex_call_flag = false;

		// �v���Z�X�����ɖ߂�
		tnm_pop_proc();

		// ���L�[�N���A
		Gp_input->clear();
	}
	else if (p_cur_call->frame_action_flag == 1)	{

		// �v���Z�X�����ɖ߂�
		tnm_pop_proc();
	}

	// ���^�[��
	if (!tnm_load_call())	// �R�[���������[�h
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F�R�[���X�^�b�N��������
// ================================================================
void tnm_scene_init_call_stack()
{
	Gp_call_list->set_call_cnt(1);	// �����l�͂P
}

// ****************************************************************
// �V�[�������F�R�[���X�^�b�N���폜
// ================================================================
void tnm_scene_del_call_stack(int del_cnt)
{
	if (del_cnt <= 0)
		return;

	int cur_cnt = Gp_call_list->get_call_cnt();
	int dst_cnt = std::max(cur_cnt - del_cnt, 1);	// �P�ȉ��ɂ͌��点�Ȃ�
	Gp_call_list->set_call_cnt(dst_cnt);
}

// ****************************************************************
// �V�[�������F�R�[���X�^�b�N�̌���ݒ�
// ================================================================
void tnm_scene_set_call_stack_cnt(int dst_cnt)
{
	if (dst_cnt >= 1)	{	// �P�ȉ��ɂ͌��点�Ȃ�

		// ���炷�����݂̂ɐݒ�ł���
		int cur_cnt = Gp_call_list->get_call_cnt();
		if (dst_cnt < cur_cnt)	{
			Gp_call_list->set_call_cnt(dst_cnt);
		}
	}
}

// ****************************************************************
// �V�[�������F�R�[���X�^�b�N�̌����擾
// ================================================================
int tnm_scene_get_call_stack_cnt()
{
	return Gp_call_list->get_call_cnt();
}

// ****************************************************************
// �V�[�������F���X�^�[�g
// ================================================================
bool tnm_scene_proc_restart_func(int scn_no, int z_no)
{
	// �V�[���ɃW�����v
	Gp_lexer->jump_to_scn(scn_no, z_no);

	// �X�N���v�g�J�n
	tnm_push_proc(TNM_PROC_TYPE_SCRIPT);

	return true;
}

// ****************************************************************
// �V�[�������F�X�^�[�g�V�[������X�^�[�g
// ================================================================
bool tnm_scene_proc_start_from_start_scene()
{
	// ���[�J��������
	tnm_init_local();

	// �V�[���ԍ����擾
	int scn_no = Gp_lexer->get_scn_no(Gp_global->start_scene_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);	// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, Gp_global->start_scene_name + _T(".ss ��������܂���ł����B"));
	}

	// ����\��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�X�^�[�g�V�[������J�n���܂��B"));

	// �V�[������n�߂�
	tnm_scene_proc_restart_func(scn_no, Gp_global->start_z_no);

	// �����荞�݌x�������I
	C_tnm_proc warning_proc;
	warning_proc.type = TNM_PROC_TYPE_START_WARNING;
	warning_proc.option = 0;
	tnm_push_proc(warning_proc);

	return true;
}

// ****************************************************************
// �V�[�������F�X�^�[�g�V�[�����烊�X�^�[�g
// ================================================================
bool tnm_scene_proc_restart_from_start_scene()
{
	// ���[�J���I������
	tnm_finish_local();

	// ���[�J���ď�����
	tnm_reinit_local(true);

	// �V�[���ԍ����擾
	int scn_no = Gp_lexer->get_scn_no(Gp_global->start_scene_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);	// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, Gp_global->start_scene_name + _T(".ss ��������܂���ł����B"));
	}

	// ����\��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�X�^�[�g�V�[������ĊJ���܂��B"));

	// �V�[������n�߂�
	tnm_scene_proc_restart_func(scn_no, Gp_global->start_z_no);

	// �����荞�݌x�������I
	C_tnm_proc warning_proc;
	warning_proc.type = TNM_PROC_TYPE_START_WARNING;
	warning_proc.option = 0;
	tnm_push_proc(warning_proc);

	return true;
}

// ****************************************************************
// �V�[�������F���j���[�V�[�����烊�X�^�[�g
// ================================================================
bool tnm_scene_proc_restart_from_menu_scene()
{
	// ���[�J���I��
	tnm_finish_local();

	// ���[�J���ď�����
	tnm_reinit_local(true);

	// �A�y���h������������
	if (Gp_cmd_line->select_scene_pck_set)
	{
		Gp_local_save->append_dir = Gp_cmd_line->select_scene_pck;
		Gp_local_save->append_name = _T("[dummy]");

		for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			if (Gp_select_ini->m_append_list[i].m_dir == Gp_local_save->append_dir)
			{
				Gp_local_save->append_name = Gp_select_ini->m_append_list[i].m_name;
			}
		}
	}
	else
	{
		Gp_local_save->append_dir = Gp_select_ini->first_dir();	// �擪�̃f�B���N�g����T��
		Gp_local_save->append_name = Gp_select_ini->first_name();
	}

	// Scene.pck ��ǂݒ���
	if (!tnm_reload_scene_pck())
		return false;

	// �V�[���ԍ����擾
	int scn_no = Gp_lexer->get_scn_no(Gp_ini->menu_scene_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);	// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, Gp_ini->menu_scene_name + _T(".ss ��������܂���ł����B"));
	}

	// ����\��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���j���[�V�[�����烊�X�^�[�g���܂��B"));

	// �V�[������n�߂�
	return tnm_scene_proc_restart_func(scn_no, Gp_ini->menu_scene_z_no);
}

// ****************************************************************
// �V�[�������F�w��̃V�[�����烊�X�^�[�g�i�V�[���ԍ����w��j
// ================================================================
bool tnm_scene_proc_restart(int scn_no, int z_no)
{
	// ���[�J���I��
	tnm_finish_local();

	// ���[�J���ď�����
	tnm_reinit_local(true);

	// �V�[������n�߂�
	return tnm_scene_proc_restart_func(scn_no, z_no);
}

// ****************************************************************
// �V�[�������F�w��̃V�[�����烊�X�^�[�g�i�V�[�������w��j
// ================================================================
bool tnm_scene_proc_restart(CTSTR& scn_name, int z_no)
{
	// ���[�J���I��
	tnm_finish_local();

	// ���[�J���ď�����
	tnm_reinit_local(true);

	// �V�[���ԍ����擾
	int scn_no = Gp_lexer->get_scn_no(scn_name);
	if (scn_no < 0)	{
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss ��������܂���ł����B"));
	}

	// ����\��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�V�[�� ") + scn_name + _T(" ���烊�X�^�[�g���܂��B"));

	// �V�[������n�߂�
	return tnm_scene_proc_restart_func(scn_no, z_no);
}

// ****************************************************************
// �V�[�������F�V�X�e���R�}���h�V�[�����Ă�
// ================================================================
bool tnm_scene_proc_call_syscom_scene()
{
	// �R�[��
	if (!tnm_scene_proc_farcall(Gp_ini->cancel_scene_name, Gp_ini->cancel_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F�t���b�N�V�[�����Ă�
// ================================================================
bool tnm_scene_proc_call_flick_scene(int no)
{
	// �R�[��
	if (!tnm_scene_proc_farcall(Gp_ini->flick_scene_name[no], Gp_ini->flick_scene_z_no[no], FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F�Z�[�u�V�[�����Ă�
// ================================================================
bool tnm_scene_proc_call_save_scene()
{
	// �R�[��
	if (!tnm_scene_proc_farcall(Gp_ini->save_scene_name, Gp_ini->save_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F���[�h�V�[�����Ă�
// ================================================================
bool tnm_scene_proc_call_load_scene()
{
	// �R�[��
	if (!tnm_scene_proc_farcall(Gp_ini->load_scene_name, Gp_ini->load_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// �V�[�������F���ݒ�V�[�����Ă�
// ================================================================
bool tnm_scene_proc_call_config_scene()
{
	// �R�[��
	if (!tnm_scene_proc_farcall(Gp_ini->config_scene_name, Gp_ini->config_scene_z_no, FM_VOID, true, false))
		return false;

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�Z�[�u
// ================================================================
bool tnm_saveload_proc_save(int save_no)
{
	// �Z�[�u�\���ǂ����`�F�b�N
	if (!tnm_is_save_enable(save_no))
		return false;

	// ���[�J���Z�[�u
	tnm_save_local_on_file(save_no);

	// �O���[�o���������ɃZ�[�u����i���[�J���Z�[�u�Ɠ�������邽�߁B�K�{�I�j
	tnm_save_global_on_file();

	// ����\��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(save_no) + _T(" �ԂɃZ�[�u���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�N�C�b�N�Z�[�u
// ================================================================
bool tnm_saveload_proc_quick_save(int quick_save_no)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// �Z�[�u�\���ǂ����`�F�b�N
	if (!tnm_is_save_enable(save_no))
		return false;

	// ���[�J���Z�[�u
	tnm_save_local_on_file(save_no);

	// �O���[�o���������ɃZ�[�u����i���[�J���Z�[�u�Ɠ�������邽�߁B�K�{�I�j
	tnm_save_global_on_file();

	// ����\��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(quick_save_no) + _T(" �ԂɃN�C�b�N�Z�[�u���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�G���h�Z�[�u
// ================================================================
bool tnm_saveload_proc_end_save()
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// �Z�[�u�\���ǂ����`�F�b�N
	if (!tnm_is_save_enable(save_no))
		return false;

	// ���[�J���Z�[�u
	tnm_save_local_on_file(save_no);

	// �O���[�o���������ɃZ�[�u����i���[�J���Z�[�u�Ɠ�������邽�߁B�K�{�I�j
	tnm_save_global_on_file();

	// ����\��
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�G���h�Z�[�u���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F���[�h
// ================================================================
bool tnm_saveload_proc_load(int save_no)
{
	// ���[�h�\���ǂ����`�F�b�N
	if (!tnm_is_load_enable(save_no))
		return false;

	// ���[�J���I��
	tnm_finish_local();

	// ���[�J���ď�����
	tnm_reinit_local(false);		// ���č\�z���Ȃ�

	// �t�@�C�����烍�[�h����
	if (!tnm_load_local_on_file(save_no))
		return false;

	// Scene.pck ��ǂݒ���
	if (!tnm_reload_scene_pck())
		return false;

	// �t�@�C�����烍�[�J���f�[�^�ǂݍ���
	tnm_load_local();				// �������ōč\�z���s����

	// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
	tnm_remove_backlog_save();

	// ���
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(save_no) + _T(" �Ԃ����[�h���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�N�C�b�N���[�h
// ================================================================
bool tnm_saveload_proc_quick_load(int quick_save_no)
{
	int save_no = Gp_ini->save_cnt + quick_save_no;

	// ���[�h�\���ǂ����`�F�b�N
	if (!tnm_is_load_enable(save_no))
		return false;

	// ���[�J���I��
	tnm_finish_local();

	// ���[�J���ď�����
	tnm_reinit_local(false);		// ���č\�z���Ȃ�

	// �t�@�C�����烍�[�h����
	if (!tnm_load_local_on_file(save_no))
		return false;

	// Scene.pck ��ǂݒ���
	if (!tnm_reload_scene_pck())
		return false;

	// �t�@�C�����烍�[�J���f�[�^�ǂݍ���
	tnm_load_local();				// �������ōč\�z���s����

	// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
	tnm_remove_backlog_save();

	// ���
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, tostr(quick_save_no) + _T(" �Ԃ��N�C�b�N���[�h���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�G���h���[�h
// ================================================================
bool tnm_saveload_proc_end_load()
{
	int save_no = Gp_ini->save_cnt + Gp_ini->quick_save_cnt;

	// ���[�h�\���ǂ����`�F�b�N
	if (!tnm_is_load_enable(save_no))
		return false;

	// ���[�J���I��
	tnm_finish_local();

	// ���[�J���ď�����
	tnm_reinit_local(false);		// ���č\�z���Ȃ�

	// �t�@�C�����烍�[�h����
	if (!tnm_load_local_on_file(save_no))
		return false;

	// Scene.pck ��ǂݒ���
	if (!tnm_reload_scene_pck())
		return false;

	// �t�@�C�����烍�[�J���f�[�^�ǂݍ���
	tnm_load_local();				// �������ōč\�z���s����

	// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
	tnm_remove_backlog_save();

	// ���
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�G���h���[�h���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�O�̑I�����ɖ߂�
// ================================================================
bool tnm_saveload_proc_return_to_sel()
{
	if (!tnm_is_exist_sel_save())
		return false;

	S_tnm_local_save temp_save;

	// ���[�J���I��
	tnm_finish_local();

	// �I�����Z�[�u���e���|�����ɕۑ�
	tnm_get_sel_save(&temp_save);

	// ���[�J���ď�����
	tnm_reinit_local(false);		// ���č\�z���Ȃ�

	// ���[�J���Z�[�u�f�[�^�Ƀe���|�������R�s�[
	tnm_set_local_save(&temp_save);

	// ���[�J���Z�[�u�f�[�^����f�[�^�����[�h����
	tnm_load_local();				// �������ōč\�z���s����

	// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
	tnm_remove_backlog_save();

	// �G���[��ݒ�
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�I�����ɖ߂�܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�C���i�[�Z�[�u���쐬����
// ================================================================
bool tnm_saveload_proc_create_inner_save(int inner_save_no)
{
	// ���[�J���Z�[�u���C���i�[�Z�[�u�ɃR�s�[����
	tnm_savedata_system_copy_local_save_to_inner_save(inner_save_no);

	// �G���[��ݒ�
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("%d �ԂɃC���i�[�Z�[�u�i%d�j�B"), inner_save_no, tnm_savedata_system_get_inner_save_size(inner_save_no)));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�C���i�[�Z�[�u�����[�h����
// ================================================================
bool tnm_saveload_proc_load_inner_save(int inner_save_no)
{
	if (!tnm_savedata_system_check_inner_save(inner_save_no))
		return false;

	S_tnm_local_save temp_save;

	// ���[�J���I��
	tnm_finish_local();

	// �C���i�[�Z�[�u���e���|�����Ɏ擾
	tnm_savedata_system_get_inner_save(inner_save_no, &temp_save);

	// ���[�J���ď�����
	tnm_reinit_local(false);		// ���č\�z���Ȃ�

	// ���[�J���Z�[�u�f�[�^�Ƀe���|�������R�s�[
	tnm_set_local_save(&temp_save);

	// ���[�J���Z�[�u�f�[�^����f�[�^�����[�h����
	tnm_load_local();				// �������ōč\�z���s����

	// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
	tnm_remove_backlog_save();

	// �G���[��ݒ�
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�C���i�[�Z�[�u�����[�h���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�C���i�[�Z�[�u���N���A����
// ================================================================
bool tnm_saveload_proc_clear_inner_save(int inner_save_no)
{
	// �C���i�[�Z�[�u���N���A����
	tnm_savedata_system_clear_inner_save(inner_save_no);

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�C���i�[�Z�[�u���R�s�[����
// ================================================================
bool tnm_saveload_proc_copy_inner_save(int from, int to)
{
	// �C���i�[�Z�[�u���R�s�[����
	tnm_savedata_system_copy_inner_save(from, to);

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�C���i�[�Z�[�u�����邩�𔻒肷��
// ================================================================
bool tnm_saveload_proc_check_inner_save(int inner_save_no)
{
	// �C���i�[�Z�[�u�����邩�𔻒肷��
	return tnm_savedata_system_check_inner_save(inner_save_no);
}

// ****************************************************************
// �Z�[�u���[�h�����F�o�b�N���O���[�h
// ================================================================
bool tnm_saveload_proc_backlog_load(S_tid save_id)
{
	// ���[�h�ł��Ȃ��Ȃ玸�s
	if (!tnm_check_backlog_save(save_id))
		return false;

	S_tnm_local_save temp_save;

	// ���[�J���I��
	tnm_finish_local();

	// �o�b�N���O�Z�[�u���e���|�����Ɏ擾
	temp_save = *Gp_backlog_save->map[save_id];

	// ���[�J���ď�����
	tnm_reinit_local(false);		// ���č\�z���Ȃ�

	// ���[�J���Z�[�u�f�[�^�Ƀe���|�������R�s�[
	tnm_set_local_save(&temp_save);

	// ���[�J���Z�[�u�f�[�^����f�[�^�����[�h����
	tnm_load_local();				// �������ōč\�z���s����

	// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
	tnm_remove_backlog_save();

	// �G���[��ݒ�
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���b�Z�[�W�o�b�N�Z�[�u�����[�h���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u���[�h�����F�Z�[�u�|�C���g���������[�h����
// ================================================================
bool tnm_saveload_proc_load_save_history(S_tnm_local_save* p_local_save)
{
	if (p_local_save->save_stream.empty())
		return false;

	// ���[�J���I��
	tnm_finish_local();

	// �Z�[�u�|�C���g�������e���|�����ɕۑ�
	S_tnm_local_save temp_save = *p_local_save;

	// ���[�J���ď�����
	tnm_reinit_local(false);		// ���č\�z���Ȃ�

	// ���[�J���Z�[�u�f�[�^�Ƀe���|�������R�s�[
	tnm_set_local_save(&temp_save);

	// ���[�J���Z�[�u�f�[�^���烍�[�h����
	tnm_load_local();				// �������ōč\�z���s����

	// ���b�Z�[�W�o�b�N�Z�[�u�𐮗�����
	tnm_remove_backlog_save();

	// �G���[��ݒ�
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Z�[�u�|�C���g�������烍�[�h���܂����B"));

	return true;
}

// ****************************************************************
// �Z�[�u�|�C���g�𗧂Ă�
// ================================================================
void tnm_set_save_point()
{
	// ���[�J���Z�[�u
	tnm_save_local();
}

// ****************************************************************
// �Z�[�u�|�C���g���N���A����
// ================================================================
void tnm_clear_save_point()
{
	// ���[�J���Z�[�u�f�[�^��������
	tnm_init_local_save();
}

// ****************************************************************
// �Z�[�u�|�C���g�𔻒肷��
// ================================================================
bool tnm_check_save_point()
{
	// ���[�J���Z�[�u�f�[�^�����邩�ǂ�����Ԃ�
	return tnm_is_exist_local_save();
}

// ****************************************************************
// �I�����|�C���g�𗧂Ă�
// ================================================================
void tnm_set_sel_point()
{
	// ���[�J���Z�[�u�f�[�^�ɑI�����Z�[�u�f�[�^���R�s�[
	tnm_copy_local_save_to_sel_save();
}

// ****************************************************************
// �I�����|�C���g���N���A����
// ================================================================
void tnm_clear_sel_point()
{
	// �I�����Z�[�u�f�[�^��������
	tnm_init_sel_save();
}

// ****************************************************************
// �I�����|�C���g�𔻒肷��
// ================================================================
bool tnm_check_sel_point()
{
	// �I�����Z�[�u�f�[�^�����邩�ǂ�����Ԃ�
	return tnm_is_exist_sel_save();
}

// ****************************************************************
// �I�����|�C���g���X�g�b�N����
// ================================================================
void tnm_stack_sel_point()
{
	tnm_copy_local_save_to_sel_save_stock();
}

// ****************************************************************
// �I�����|�C���g���~�낷
// ================================================================
void tnm_drop_sel_point()
{
	tnm_copy_sel_save_stock_to_sel_save();
	tnm_init_sel_save_stock();
}

// ****************************************************************
// �V�[�����G�f�B�^�ŊJ��
// ================================================================
bool tnm_open_by_editor(bool open_dialog)
{
	// ss �t�H���_��G�f�B�^�p�X���ݒ肳��Ă��Ȃ��ꍇ
	if (Gp_config->ss_path.empty() || Gp_config->editor_path.empty())	{

		// �ݒ�_�C�A���O���J��
		if (open_dialog)	{
			tnm_dlg_open_debug_setting_dialog();
		}
	}
	// ���b�Z�[�W�o�b�N��
	else if (tnm_msg_back_is_open())	{
		int scn_no = tnm_msg_back_get_scn_no();
		int line_no = tnm_msg_back_get_line_no();
		if (scn_no >= 0 && line_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			TSTR ss_path = Gp_config->ss_path + _T("\\") + scn_name + _T(".ss");
			open_file_by_editor(ss_path, line_no, Gp_config->editor_path);
		}
	}
	// ���̑�
	else	{
		// �G�f�B�^�� ss ���J��
		int scn_no = Gp_lexer->get_cur_scn_no();
		int line_no = Gp_lexer->get_cur_line_no();
		if (scn_no >= 0 && line_no >= 0)	{
			TSTR scn_name = Gp_lexer->get_scn_name(scn_no);
			TSTR ss_path = Gp_config->ss_path + _T("\\") + scn_name + _T(".ss");
			open_file_by_editor(ss_path, line_no, Gp_config->editor_path);
		}
	}

	return true;
}

// ****************************************************************
// �����֘A�t���ŊJ��
// ================================================================
bool tnm_open_koe(bool open_dialog)
{
	// ���p�X���ݒ肳��Ă��Ȃ��ꍇ
	if (Gp_config->koe_path.empty())	{

		// �ݒ�_�C�A���O���J��
		if (open_dialog)	{
			tnm_dlg_open_debug_setting_dialog();
		}
	}
	// ���b�Z�[�W�o�b�N��
	else if (tnm_msg_back_is_open())	{
		int koe_no = tnm_msg_back_get_koe_no();
		if (koe_no >= 0)	{
			int dir_no = koe_no / 100000;
			
			TSTR path = Gp_config->koe_path + str_format(_T("\\%04d\\Z%09d.wav"), dir_no, koe_no);
			if (!shell_execute_open(path))	{
				tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("���t�@�C�����J���܂���ł����B\n") + get_last_error_msg());
			}
		}
	}
	// ���̑�
	else	{
		// �G�f�B�^�� wav ���J��
		int koe_no = Gp_local->pod.cur_koe_no;
		if (koe_no >= 0)	{
			int dir_no = koe_no / 100000;
			
			TSTR path = Gp_config->koe_path + str_format(_T("\\%04d\\Z%09d.wav"), dir_no, koe_no);

			// ���{�c�B�d�w�d���w�肵�ĊJ���͖̂������ۂ��c�B�i�N���I�v�V���������W���[���ɂ���ĈႤ�j
			//if (!Gp_config->koe_tool_path.empty())	{
			//	if (!shell_execute_open_by_module(path, Gp_config->koe_tool_path))	{
			//		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("���t�@�C�����J���܂���ł����B\n") + get_last_error_msg());
			//	}
			//}
			//else	{

				// ���̗p�B�f�t�H���g�̊֘A�t���ŊJ���B
				if (!shell_execute_open(path))	{
					tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("���t�@�C�����J���܂���ł����B\n") + get_last_error_msg());
				}
			//}
		}
	}

	return true;
}


















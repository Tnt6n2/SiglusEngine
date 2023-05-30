#include	"pch.h"

#include	"tnm_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_timer.h"

#include	"engine/eng_etc.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// �Z�[�u�i���[�J���j
// ================================================================
bool C_tnm_eng::save_local()
{
	// �Z�[�u�f�[�^���N���A����
	m_local_save_history_index = -1;
	m_local_save.save_id = S_tid();
	m_local_save.append_dir = Gp_dir->append_dir;
	m_local_save.append_name = Gp_dir->append_name;
	m_local_save.save_scene_title = tnm_get_scene_title();
	m_local_save.save_msg.clear();
	m_local_save.save_full_msg = Gp_local->cur_full_message;
	m_local_save.save_stream.clear();
	m_local_save.save_stream_ex.clear();

	// �Z�[�u�f�[�^ ID
	{
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		m_local_save.save_id = sys_time;
	}

	// �o�b�t�@���m�ۂ���
	m_local_save.save_stream.get_buffer().reserve(2000000);		// �Ƃ肠���� 2MB �\��

	// ���N�T�[
	m_local_save.save_stream.save(m_lexer.get_scn_name(m_lexer.get_cur_scn_no()));
	m_local_save.save_stream.save(m_lexer.get_cur_line_no());
	m_local_save.save_stream.save(m_lexer.get_prg_cntr());

	// �V�[��
	m_local.cur_proc.save(m_local_save.save_stream);
	m_local_save.save_stream.save(m_local.proc_stack);
	m_local_save.save_stream.save(m_local.cur_mwnd);
	m_local_save.save_stream.save(m_local.cur_sel_mwnd);
	m_local_save.save_stream.save(m_local.last_mwnd);
	m_local_save.save_stream.save(m_local.scene_title);
	m_local_save.save_stream.save(m_local.cur_full_message);
	m_local_save.save_stream.save(m_local.mwnd_btn_disable.get(), (int)m_local.mwnd_btn_disable.size());
	m_local_save.save_stream.save(m_local.font_name);

	// �V�[���ipod�j
	m_local_save.save_stream.save(m_local.pod);

	// �X�^�b�N
	m_local_save.save_stream.save(m_stack.int_now_cnt);									// int ��
	m_local_save.save_stream.save(m_stack.int_p, m_stack.int_now_cnt * sizeof(int));	// int �X�^�b�N

	// ������X�^�b�N
	m_local_save.save_stream.save((int)m_stack.str_stack.size());
	for (int i = 0; i < (int)m_stack.str_stack.size(); i++)
		m_local_save.save_stream.save(m_stack.str_stack[i]);

	// �G�������g�|�C���g
	m_local_save.save_stream.save((int)m_stack.stack_point_list.size());
	m_local_save.save_stream.save(m_stack.stack_point_list.get(), (int)m_stack.stack_point_list.size() * sizeof(int));

	// �^�C�}�[
	m_local_save.save_stream.save(m_timer.local_real_time);
	m_local_save.save_stream.save(m_timer.local_game_time);
	m_local_save.save_stream.save(m_timer.local_wipe_time);

	// �V�X�e���R�}���h���j���[
	m_local_save.save_stream.save(m_syscom.syscommenu);

	// �t�H�O
	m_fog.save_local(m_local_save.save_stream);

	// �t���O
	m_flag.save_local(m_local_save.save_stream);

	// ���[�U�C���N�v���p�e�B
	m_user_inc_prop.save(m_local_save.save_stream);

	// ���[�U�V�[���v���p�e�B���X�g
	{
		// �V�[���ԍ� �� �V�[�����ɒ����Ȃ���ۑ�
		int scn_cnt = m_user_scn_prop_list.get_size();
		m_local_save.save_stream.save(scn_cnt);
		for (int i = 0; i < scn_cnt; i++)	{
			TSTR scn_name = m_lexer.get_scn_name(i);
			m_local_save.save_stream.save(scn_name);
			m_user_scn_prop_list[i].prop_list.save(m_local_save.save_stream);
		}
	}

	// �J�E���^
	m_counter_list.save(m_local_save.save_stream);

	// �t���[���A�N�V����
	m_frame_action.save(m_local_save.save_stream);

	// �t���[���A�N�V�����`�����l��
	m_frame_action_ch_list.save(m_local_save.save_stream);

	// �f�O�O�o�b�t�@
	m_g00_buf_list.save(m_local_save.save_stream);

	// �}�X�N
	m_mask_list.save(m_local_save.save_stream);

	// �X�e�[�W
	m_stage_list[TNM_STAGE_BACK].save(m_local_save.save_stream);
	m_stage_list[TNM_STAGE_FRONT].save(m_local_save.save_stream);

	// �X�N���[��
	m_screen.save(m_local_save.save_stream);

	// �T�E���h
	m_sound.save(m_local_save.save_stream);

	// ���ʉ��C�x���g
	m_pcm_event_list.save(m_local_save.save_stream);

	// �G�f�B�b�g�{�b�N�X
	m_editbox_list.save(m_local_save.save_stream);

	// �R�[��
	m_call_list.save(m_local_save.save_stream);

	// ���b�Z�[�W�o�b�N
	m_msg_back.save(m_local_save.save_stream);

	// �Z�[�u�|�C���g�X�^�b�N
	m_sel_save_stock.save_stream.save(m_local_save.save_stream);

	// �C���i�[�Z�[�u
	m_local_save.save_stream.save((int)m_inner_save_list.size());
	for (int i = 0; i < (int)m_inner_save_list.size(); i++)
		m_inner_save_list[i].save_stream.save(m_local_save.save_stream);

	// �I�����Z�[�u���X�g
	m_local_save.save_stream.save(m_sel_save.list.size());
	for (int i = 0; i < (int)m_sel_save.list.size(); i++)
		m_local_save.save_stream.save(m_sel_save.list[i]);

	// ���[�J���ݒ���Z�[�u����
	save_local_ex();

	// �Z�[�u�����ɒǉ�
	if (m_global.debug_flag && Gp_ini->save_history_cnt > 0)
	{
		DWORD now_time = ::timeGetTime();
		int past_time = now_time - m_save_history.update_time;
		if (past_time >= 3000)
		{
			m_save_history.update_time = now_time;
			int index = (m_save_history.top + m_save_history.cnt) % Gp_ini->save_history_cnt;
			m_save_history.item[index].flag = 0;
			GetLocalTime(&Gp_eng->m_save_history.item[index].systemtime);
			m_save_history.item[index].local_save = m_local_save;
			if (m_save_history.cnt < Gp_ini->save_history_cnt)
			{
				m_save_history.cnt++;
			}
			else
			{
				m_save_history.top++;

				if (m_save_history.top >= Gp_ini->save_history_cnt)
				{
					m_save_history.top = 0;
				}
			}
			m_save_history.update_flag = true;
			m_local_save_history_index = index;
		}
	}

	return true;
}

// ****************************************************************
// ���[�h�i���[�J���j
// ================================================================
bool C_tnm_eng::load_local()
{
	if (m_local_save.save_stream.empty())
		return false;

	// �V�[�N�Z�b�g
	m_local_save.save_stream.set_seek();

	// �Z�[�u�X�g���[�����珇�Ƀf�[�^�����[�h

	// ���N�T�[
	{
		// ����
		TSTR scn_name = m_local_save.save_stream.load_ret<TSTR>();
		int line_no = m_local_save.save_stream.load_ret<int>();
		int prg_cntr = m_local_save.save_stream.load_ret<int>();

		// �V�[���ԍ��ɕϊ�
		int scn_no = m_lexer.get_scn_no(scn_name);
		if (scn_no == -1)
		{
			tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
			return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss ��������܂���ł����B"));
		}
		// ���[�h�����p�����[�^��ݒ�
		m_lexer.load_local(scn_no, line_no, prg_cntr);
	}

	// �V�[��
	m_local.cur_proc.load(m_local_save.save_stream);
	m_local_save.save_stream.load(m_local.proc_stack);
	m_local_save.save_stream.load(m_local.cur_mwnd);
	m_local_save.save_stream.load(m_local.cur_sel_mwnd);
	m_local_save.save_stream.load(m_local.last_mwnd);
	m_local_save.save_stream.load(m_local.scene_title);
	m_local_save.save_stream.load(m_local.cur_full_message);
	m_local_save.save_stream.load(m_local.mwnd_btn_disable.get(), (int)m_local.mwnd_btn_disable.size());
	m_local_save.save_stream.load(m_local.font_name);

	// �V�[���ipod�j
	m_local_save.save_stream.load(m_local.pod);

	// �X�^�b�N
	m_local_save.save_stream.load(m_stack.int_now_cnt);									// �������[�h
	tnm_stack_realloc(m_stack.int_now_cnt);												// �����Ċm�ۂ��s��
	m_local_save.save_stream.load(m_stack.int_p, m_stack.int_now_cnt * sizeof(int));	// �f�[�^�����[�h

	// ������X�^�b�N
	m_stack.str_stack.resize(m_local_save.save_stream.load_ret<int>());
	for (int i = 0; i < (int)m_stack.str_stack.size(); i++)
		m_local_save.save_stream.load(m_stack.str_stack[i]);

	// �G�������g�|�C���g
	m_stack.stack_point_list.resize(m_local_save.save_stream.load_ret<int>());
	m_local_save.save_stream.load(m_stack.stack_point_list.get(), (int)m_stack.stack_point_list.size() * sizeof(int));

	// �^�C�}�[
	m_local_save.save_stream.load(m_timer.local_real_time);
	m_local_save.save_stream.load(m_timer.local_game_time);
	m_local_save.save_stream.load(m_timer.local_wipe_time);

	// �V�X�e���R�}���h���j���[
	m_local_save.save_stream.load(m_syscom.syscommenu);

	// �t�H�O
	m_fog.load_local(m_local_save.save_stream);

	// �t���O
	m_flag.load_local(m_local_save.save_stream);

	// ���[�U�C���N�v���p�e�B
	m_user_inc_prop.load(m_local_save.save_stream);

	// ���[�U�V�[���v���p�e�B���X�g
	{
		// �V�[���� �� �V�[���ԍ��ɒ����Ȃ���ǂݍ���
		int scn_cnt = m_local_save.save_stream.load_ret<int>();
		for (int i = 0; i < scn_cnt; i++)	{
			TSTR scn_name = m_local_save.save_stream.load_ret<TSTR>();

			// �V�[���ԍ��ɕϊ�
			int scn_no = Gp_lexer->get_scn_no(scn_name);
			if (scn_no == -1)	{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss ��������܂���ł����B"));
				// �V�[�����Ȃ������̂Ńf�[�^���X�L�b�v
				m_local_save.save_stream.skip_fixed_array();
			}
			else	{
				// �V�[�����������̂ł��̃V�[���ɓǂݍ���
				m_user_scn_prop_list[scn_no].prop_list.load(m_local_save.save_stream);
			}
		}
	}

	// �J�E���^
	m_counter_list.load(m_local_save.save_stream);

	// �t���[���A�N�V����
	m_frame_action.load(m_local_save.save_stream);

	// �t���[���A�N�V�����`�����l��
	m_frame_action_ch_list.load(m_local_save.save_stream);

	// �f�O�O�o�b�t�@
	m_g00_buf_list.load(m_local_save.save_stream);

	// �}�X�N
	m_mask_list.load(m_local_save.save_stream);

	// �X�e�[�W
	m_stage_list[TNM_STAGE_BACK].load(m_local_save.save_stream);
	m_stage_list[TNM_STAGE_FRONT].load(m_local_save.save_stream);

	// �X�N���[��
	m_screen.load(m_local_save.save_stream);

	// �T�E���h
	m_sound.load(m_local_save.save_stream);

	// ���ʉ��C�x���g
	m_pcm_event_list.load(m_local_save.save_stream);

	// �G�f�B�b�g�{�b�N�X
	m_editbox_list.load(m_local_save.save_stream);

	// �R�[��
	m_call_list.load(m_local_save.save_stream);

	// ���b�Z�[�W�o�b�N
	m_msg_back.load(m_local_save.save_stream);

	// �Z�[�u�|�C���g�X�^�b�N
	m_sel_save_stock.save_stream.load(m_local_save.save_stream);

	// �C���i�[�Z�[�u
	m_inner_save_list.resize(m_local_save.save_stream.load_ret<int>());
	for (int i = 0; i < (int)m_inner_save_list.size(); i++)
		m_inner_save_list[i].save_stream.load(m_local_save.save_stream);

	// �I�����Z�[�u���X�g
	m_sel_save.list.resize(m_local_save.save_stream.load_ret<int>());
	for (int i = 0; i < (int)m_sel_save.list.size(); i++)
		m_local_save.save_stream.load(m_sel_save.list[i]);

	// �I�����Z�[�u���X�g�𒲐�����
	{
		// �O�̂��߃��X�g���\�[�g����
		std::sort(m_sel_save.list.begin(), m_sel_save.list.end());

		// �}�[�W�̗v�̂ŁA�����ɑ��݂�����̂������c���Ă���
		ARRAY<S_tid>::iterator itr_id = m_sel_save.list.begin();
		std::map<S_tid, BSP<S_tnm_local_save>>::iterator itr_map = m_sel_save.map.begin();
		while (1)	{

			if (itr_id == m_sel_save.list.end())	{
				m_sel_save.map.erase(itr_map, m_sel_save.map.end());
				break;
			}
			else if (itr_map == m_sel_save.map.end())	{
				m_sel_save.list.erase(itr_id, m_sel_save.list.end());
				break;
			}
			else if (*itr_id < itr_map->first)	{
				itr_id = m_sel_save.list.erase(itr_id);
			}
			else if (itr_map->first < *itr_id)	{
				itr_map = m_sel_save.map.erase(itr_map);
			}
			else	{
				++itr_id;
				++itr_map;
			}
		}
	}

	// ���[�J���ݒ�����[�h����
	load_local_ex();

	// �Z�[�u���b�Z�[�W�̓N���A����
	// �Z�[�u���ꂽ���́A�Z�[�u���b�Z�[�W�͋�̂͂�������ł���B
	// �Z�[�u��ɒǉ����ꂽ���b�Z�[�W�́A���[�h����ǉ������͂��B
	m_local_save.save_msg.clear();

	// �������R�ŁA�t�����b�Z�[�W�̓��[�J������R�s�[���܂��B
	m_local_save.save_full_msg = m_local.cur_full_message;

	return true;
}

// ****************************************************************
// �Z�[�u�i���[�J���̌��݂̕��́j
// ================================================================
bool C_tnm_eng::save_local_msg(CTSTR& msg)
{
	if (msg.empty())
		return false;

	m_local_save.save_msg += msg;
	if ((int)m_local_save.save_msg.size() > TNM_SAVE_MESSAGE_MAX_LEN)
		m_local_save.save_msg.resize(TNM_SAVE_MESSAGE_MAX_LEN);

	m_local_save.save_full_msg += msg;
	if ((int)m_local_save.save_full_msg.size() > TNM_SAVE_FULL_MESSAGE_MAX_LEN)
		m_local_save.save_full_msg.resize(TNM_SAVE_FULL_MESSAGE_MAX_LEN);

	// �Z�[�u�����ɔ��f
	if (Gp_global->debug_flag)
	{
		if (m_local_save_history_index >= 0)
		{
			m_save_history.item[m_local_save_history_index].local_save.save_msg = m_local_save.save_msg;
			m_save_history.item[m_local_save_history_index].local_save.save_full_msg = m_local_save.save_full_msg;
			m_save_history.update_flag = true;
		}
	}

	return true;
}

// ****************************************************************
// �Z�[�u�i���[�J���ݒ�j
// ================================================================
bool C_tnm_eng::save_local_ex()
{
	// �X�g���[�����N���A����
	m_local_save.save_stream_ex.clear();
	
	// ���[�J���ݒ���Z�[�u����
	m_local_save.save_stream_ex.save(Gp_syscom->syscommenu.local_extra_switch, sizeof(Gp_syscom->syscommenu.local_extra_switch));
	m_local_save.save_stream_ex.save(Gp_syscom->syscommenu.local_extra_mode, sizeof(Gp_syscom->syscommenu.local_extra_mode));

	return true;
}

// ****************************************************************
// ���[�h�i���[�J���ݒ�j
// ================================================================
bool C_tnm_eng::load_local_ex()
{
	if (m_local_save.save_stream_ex.empty())
		return false;

	// �V�[�N�Z�b�g
	m_local_save.save_stream_ex.set_seek();
	
	// ���[�J���ݒ�����[�h����
	m_local_save.save_stream_ex.load(Gp_syscom->syscommenu.local_extra_switch, sizeof(Gp_syscom->syscommenu.local_extra_switch));
	m_local_save.save_stream_ex.load(Gp_syscom->syscommenu.local_extra_mode, sizeof(Gp_syscom->syscommenu.local_extra_mode));

	return true;
}

// ****************************************************************
// �Z�[�u�i�R�[���f�[�^�j
// ================================================================
bool C_tnm_eng::save_call()
{
	C_elm_call* p_cur_call = &m_call_list[m_call_list.get_call_cnt() - 1];

	// �f�[�^�����ɃZ�[�u

	// ���N�T�[
	p_cur_call->m_call_save.lexer_scn_name = m_lexer.get_scn_name(m_lexer.get_cur_scn_no());
	p_cur_call->m_call_save.lexer_line_no = m_lexer.get_cur_line_no();
	p_cur_call->m_call_save.lexer_prg_cntr = m_lexer.get_prg_cntr();

	// �R�[�����v�b�V��
	m_call_list.add_call();

	return true;
}

// ****************************************************************
// ���[�h�i�R�[���f�[�^�j
// ================================================================
bool C_tnm_eng::load_call()
{
	// �R�[���X�^�b�N����Ȃ�I��
	if (m_call_list.get_call_cnt() == 1)	{	// ������Ԃł͂P�A�������Ƃ���
		tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
		return m_error.set_error(TNM_ERROR_TYPE_FATAL, _T("�R�[���X�^�b�N����ł��B"));
	}

	// �R�[���f�[�^���|�b�v
	m_call_list.sub_call();

	// ���݂̃R�[�����擾
	C_elm_call* p_cur_call = &m_call_list[m_call_list.get_call_cnt() - 1];

	// �f�[�^�����Ƀ��[�h

	// ���N�T�[
	{
		TSTR scn_name = p_cur_call->m_call_save.lexer_scn_name;
		int line_no = p_cur_call->m_call_save.lexer_line_no;
		int prg_cntr = p_cur_call->m_call_save.lexer_prg_cntr;

		// �V�[���ԍ��ɕϊ�
		int scn_no = Gp_lexer->get_scn_no(scn_name);
		if (scn_no == -1)	{
			tnm_push_proc(TNM_PROC_TYPE_NONE);		// �X�N���v�g�������~�߂�
			return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, scn_name + _T(".ss ��������܂���ł����B"));
		}
		// ���[�h�����p�����[�^��ݒ�
		m_lexer.load_local(scn_no, line_no, prg_cntr);
	}

	return true;
}

